#include "llamacontrol.h"
#include "qdebug.h"

#include <QRegularExpression>
#include <chat.h>






LlamaControl::LlamaControl()
{
    p_model = nullptr;
}

LlamaControl::~LlamaControl()
{

    model_release();
}

void LlamaControl::model_init(QString model_path)
{

    // 然后传给函数
    llama_params.model.path = model_path.toStdString();
    llama_params.default_template_kwargs["enable_thinking"]="false";
    llama_params.default_template_kwargs["add_generation_prompt"]="true";
    llama_params.n_predict = 128;
    llama_params.n_parallel = 1;
    llama_params.sampling.temp = 0.7f;
    // llama_params.sampling.penalty_freq = 0.5f;
    //llama_params.sampling.penalty_repeat = 1.45;
    llama_params.sampling.top_p = 0.8f;
    llama_params.sampling.top_k = 20;
    llama_params.sampling.min_p = 0;

    llama_params.sampling.penalty_present = 1.5;
    //llama_params.sampling.n_prev = 128;

    common_init();
    //初始化LLM
    llama_backend_init();
    llama_numa_init(llama_params.numa);
    //初始化模型
    llama_model_params model_params = common_model_params_to_llama(llama_params);
    p_model = llama_model_load_from_file(llama_params.model.path.c_str(), model_params);
    if (p_model == NULL) {
        LOG_ERR("%s: error: unable to load model\n" , __func__);
    }
    //词表初始化
    vocab = llama_model_get_vocab(p_model);
    //GBNF语法初始化
    GBNF_init();
    Ctx_init();
    qDebug()<<"model init success";
    predict_system();
    //predict_content(QString::fromStdString(g_chat_ctx.GetSystem()));

}


void LlamaControl::model_release()
{
    llama_sampler_free(content_smpl);
    llama_sampler_free(emotion_smpl);
    llama_backend_free();
    llama_model_free(p_model);
    llama_free(p_ctx);
}



void LlamaControl::GBNF_init()
{
    QStringList e = {"angry","happy","sad","shocked","neutral"};
    QString expressionRule;
    int count = 0;
    for (QString key:e) {
        if (count > 0) expressionRule += " | ";
        expressionRule += QString("\"%1\"").arg(key);
        count++;
    }
    if (expressionRule.isEmpty()) expressionRule = "\"\"";

    json_gbnf = R"(
        root ::= expression "|" text
        expression ::=)" + expressionRule + R"(
        text ::= [\u0020-\u007E\u4E00-\u9FFF\uFF00-\uFFEF]{1,120}
    )";

    // emotion_gbnf = R"(
    //     root ::=)" ;
}

void LlamaControl::Ctx_init()
{
    llama_context_params ctx_params = common_context_params_to_llama(llama_params);
    ctx_params.n_batch = 2048;
    //上下文长度
    //ctx_params.n_ctx   = n_kv_req;
    ctx_params.n_ctx   = 65536;
    //批次大小，一次处理多少token，可提高进行提速
    p_ctx = llama_init_from_model(p_model, ctx_params);

    //检查上下文
    if (p_ctx == NULL) {
        LOG_ERR("%s: error: failed to create the llama_context\n" , __func__);
    }

}


llama_sampler* LlamaControl::GetContentSmpl(){
    //采样器初始化
    auto sparams = llama_sampler_chain_default_params();
    sparams.no_perf = false;
    content_smpl = llama_sampler_chain_init(sparams);
    //语法加入采样链路，强制约束输出
    llama_sampler_chain_add(content_smpl, llama_sampler_init_grammar(vocab,json_gbnf.toUtf8(),"root"));
    llama_sampler_chain_add(content_smpl,llama_sampler_init_top_k(llama_params.sampling.top_k));
    llama_sampler_chain_add(content_smpl,llama_sampler_init_top_p(llama_params.sampling.top_p, llama_params.sampling.min_keep));
    llama_sampler_chain_add(content_smpl,llama_sampler_init_temp(llama_params.sampling.temp));
    llama_sampler_chain_add(content_smpl,llama_sampler_init_dist(llama_params.sampling.seed));
    llama_sampler_reset(content_smpl);
    return content_smpl;
}


llama_sampler *LlamaControl::GetEmotionSmpl()
{
    //采样器初始化
    auto sparams = llama_sampler_chain_default_params();
    sparams.no_perf = false;
    emotion_smpl = llama_sampler_chain_init(sparams);
    //语法加入采样链路，强制约束输出
    llama_sampler_chain_add(emotion_smpl, llama_sampler_init_grammar(vocab,emotion_gbnf.toUtf8(),"root"));
    llama_sampler_chain_add(emotion_smpl,llama_sampler_init_top_k(llama_params.sampling.top_k));
    llama_sampler_chain_add(emotion_smpl,llama_sampler_init_top_p(llama_params.sampling.top_p, llama_params.sampling.min_keep));
    llama_sampler_chain_add(emotion_smpl,llama_sampler_init_temp(llama_params.sampling.temp));
    llama_sampler_chain_add(emotion_smpl,llama_sampler_init_dist(llama_params.sampling.seed));
    llama_sampler_reset(emotion_smpl);
    return emotion_smpl;
}




void LlamaControl::predict_content(QString prompt){
    g_chat_ctx.add_msg(true, prompt.toStdString());
    llama_params.prompt = g_chat_ctx.GetEndUserText();
    GetContentSmpl();

    QString text = predict(content_smpl);
    qDebug() << "Raw output:" << text;

    AiReplyResult reply = parseReply(text);

    if (reply.isValid) {
        g_chat_ctx.add_msg(false, text.toStdString());
        emit DecodeInfo(reply.content);
        emit replyReady(reply.expression, reply.content);
        emit expressionChanged(reply.expression);
        emit contentReady(reply.content);
    } else {
        g_chat_ctx.pop_last_msg();
        qDebug() << "输出有问题,重新推理";
        predict_content(prompt);
    }
}

void LlamaControl::predict_system()
{
    llama_params.prompt = g_chat_ctx.GetSystem();
    GetContentSmpl();
    QString text = predict(content_smpl);
    qDebug()<<text;

}





QString LlamaControl::predict(llama_sampler* smpl){
    //prompt进行分词
    std::vector<llama_token> tokens_list = common_tokenize(vocab,llama_params.prompt, true,true);
    //并行解码批次
    int n_parallel = llama_params.n_parallel;
    //需要生成多少字（预定好的）
    int n_predict = llama_params.n_predict;
    const int n_kv_req = tokens_list.size() + (n_predict - tokens_list.size())*n_parallel;
    const int n_ctx = llama_n_ctx(p_ctx);
    //检查空间够不够
    if (n_kv_req > n_ctx) {
        LOG_ERR("the required KV cache size is not big enough\n");
        return "";
    }

    //打印提示词
    for (auto id : tokens_list) {
        LOG("%s", common_token_to_piece(p_ctx, id).c_str());
    }
    LOG("\n");
    //创建batch，批量提交器

    llama_batch batch = llama_batch_init(2048,
                                         0, n_parallel);

    //序列id
    std::vector<llama_seq_id> seq_ids(n_parallel, 0);
    for (int32_t i = 0; i < n_parallel; ++i) {
        seq_ids[i] = i;
    }

    //将prompt转化的token塞进batch
    llama_memory_t mem = llama_get_memory(p_ctx);
    llama_pos last_pos = llama_memory_seq_pos_max(mem, 0)+1;
    qDebug()<<last_pos;
    for (size_t i = last_pos; i < tokens_list.size()+last_pos; ++i) {
        common_batch_add(batch, tokens_list[i-last_pos], i, seq_ids, false);
    }

    GGML_ASSERT(batch.n_tokens == (int) tokens_list.size());

    //如果有decoder模型
    if (llama_model_has_encoder(p_model)) {
        if (llama_encode(p_ctx, batch)) {
            LOG_ERR("%s : failed to eval\n", __func__);
        }

        llama_token decoder_start_token_id = llama_model_decoder_start_token(p_model);
        if (decoder_start_token_id == LLAMA_TOKEN_NULL) {
            decoder_start_token_id = llama_vocab_bos(vocab);
        }
        common_batch_clear(batch);
        common_batch_add(batch, decoder_start_token_id, 0, seq_ids, false);
    }

    // llama_decode will output logits only for the last token of the prompt
    batch.logits[batch.n_tokens - 1] = true;



    //这一步计算，读完token准备输出回答
    if (llama_decode(p_ctx, batch) != 0) {
        LOG_ERR("%s: llama_decode() failed\n", __func__);
    }

    //qDebug()<<batch.n_tokens;
    //多线程情况下多条序列生成
    if (n_parallel > 1) {
        LOG("\n\n%s: generating %d sequences ...\n", __func__, n_parallel);
    }


    //主循环，解析输出
    //数据流，保存每条线程的数据
    std::vector<std::string> streams(n_parallel);
    //位置记录
    std::vector<int32_t> i_batch(n_parallel, batch.n_tokens - 1);
    int n_cur = batch.n_tokens;
    int n_decode = 0;
    const auto t_main_start = llama_time_us();

    //解析
    std::string result;
    bool running = true;
    // mem = llama_get_memory(p_ctx);
    // last_pos = llama_memory_seq_pos_max(mem, 0)+1;
    // qDebug()<<last_pos;

    while (running) {
        // prepare the next batch
        common_batch_clear(batch);
        // sample the next token for each parallel sequence / stream
        for (int32_t i = 0; i < n_parallel; ++i) {
            if (i_batch[i] < 0) {
                //该流解析完毕
                continue;
            }
            //预测下一个字
            const llama_token new_token_id = llama_sampler_sample(smpl, p_ctx, i_batch[i]);
            //遇到结束符（eog）或者到达最大长度，结束
            if (llama_vocab_is_eog(vocab, new_token_id)) {
                i_batch[i] = -1;
                LOG("\n");
                if (n_parallel > 1) {
                    LOG_INF("%s: stream %d finished at n_cur = %d", __func__, i, n_cur);
                }
                running = false;
                continue;
            }

            // if there is only one stream, we print immediately to stdout
            if (n_parallel == 1) {
                //单线程输出，直接打印
                result += common_token_to_piece(p_ctx, new_token_id);
                qDebug()<< common_token_to_piece(p_ctx, new_token_id);
                //LOG("%s", common_token_to_piece(ctx, new_token_id).c_str());
            }

            streams[i] += common_token_to_piece(p_ctx, new_token_id);
            i_batch[i] = batch.n_tokens;
            //将token加入下一批
            common_batch_add(batch, new_token_id, n_cur+last_pos, { i }, true);
            n_decode += 1;
        }

        if (batch.n_tokens == 0) {
            break;
        }

        n_cur += 1;
        //继续推理
        if (llama_decode(p_ctx, batch)) {
            LOG_ERR("%s : failed to eval, return code %d\n", __func__, 1);
        }

    }

    //多线程拼接结果
    if (n_parallel > 1) {
        LOG("\n");
        for (int32_t i = 0; i < n_parallel; ++i) {
            result += streams[i];
            //LOG("sequence %d:\n\n%s%s\n\n", i, llama_params.prompt.c_str(), streams[i].c_str());
        }
    }

    //打印速度
    const auto t_main_end = ggml_time_us();
    LOG_INF("%s: decoded %d tokens in %.2f s, speed: %.2f t/s\n",
            __func__, n_decode, (t_main_end - t_main_start) / 1000000.0f,
            n_decode / ((t_main_end - t_main_start) / 1000000.0f));
    LOG("\n");



    llama_perf_sampler_print(content_smpl);
    llama_perf_context_print(p_ctx);
    // fprintf(stderr, "\n");
    //释放资源
    llama_batch_free(batch);

    QString text = QString::fromStdString(result);
    return text;
}





void LlamaControl::predict_emotion(){
    //获取prompt进行推理
    g_chat_ctx.add_msg(true,"根据上一轮回答输出一个表情");
    llama_params.prompt = g_chat_ctx.GetEndUserText();
    qDebug()<<llama_params.prompt;
    //重置表情采样器
    GetEmotionSmpl();
    QString text = predict(emotion_smpl);
    qDebug()<<text;
    g_chat_ctx.pop_last_msg();
}

bool LlamaControl::GetResult(QString &result)
{
    QRegularExpression reg(R"(<\|im_start\|>assistant|\n(.*?)<\|im_end\|>)",
                           QRegularExpression::DotMatchesEverythingOption);
    QStringList blockRuleList = {
        R"(<\|im_start\|>)",
        R"(<\|im_start\|>assistant)",
        R"(<\|im_start\|>user)",
        R"(<\|im_start\|>system)",
        R"(<\|im_end\|>)",
        R"(<think>)",
        R"(</think>)",
        R"(<think>(.*?)</think>)"
    };

    QString blockPattern = blockRuleList.join("|");
    QRegularExpression blockWordRegex(blockPattern, QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatchIterator iter = reg.globalMatch(result);
    QString res;
    bool success = false;
    while (iter.hasNext())
    {
        QRegularExpressionMatch match = iter.next();
        QString content = match.captured(1).trimmed();
        if(content == "") continue;
        QString blockedContent = content;
        blockedContent.replace(blockWordRegex, "");
        res += blockedContent;
        success = true;
    }

    QString fullShowText;
    QRegularExpression main(R"([\s\S]*?([。！？.!?”"]))", QRegularExpression::DotMatchesEverythingOption);
    iter = main.globalMatch(res);

    int lastValidPos = 0;
    while (iter.hasNext())
    {
        auto m = iter.next();
        lastValidPos = m.capturedEnd();
    }
    if (lastValidPos > 0)
    {
        fullShowText += res.left(lastValidPos);
        res = res.mid(lastValidPos);
    }
    result = fullShowText;
    return success;
}



AiReplyResult LlamaControl::parseReply(const QString &rawOutput)
{
    AiReplyResult result;
    result.isValid = false;
    result.expression = "neutral";
    result.content = "";

    QString cleaned = rawOutput.trimmed();

    int pipeIdx = cleaned.indexOf('|');
    if (pipeIdx < 0) {
        result.content = cleaned;
        result.isValid = !cleaned.isEmpty();
        return result;
    }

    QString expr = cleaned.left(pipeIdx).trimmed().toLower();
    QString text = cleaned.mid(pipeIdx + 1).trimmed();

    QStringList validExprs = {"angry", "happy", "sad", "shocked", "neutral"};
    if (validExprs.contains(expr)) {
        result.expression = expr;
    } else {
        result.expression = "neutral";
    }

    if (text.isEmpty()) {
        text = cleaned;
    }

    result.content = text;
    result.isValid = !text.isEmpty();
    return result;
}










// void LlamaControl::test_demo()
// {
//     common_params params;
//     params.model.path = R"(E:\gguffiles\GLM-OCR-f16.gguf)";
//     params.prompt = "what is beef?";
//     params.n_predict = 128;

//     common_init();

//     // batched数量
//     int n_parallel = params.n_parallel;

//     // prompt的长度
//     int n_predict = params.n_predict;

//     //初始化LLM
//     llama_backend_init();
//     llama_numa_init(params.numa);

//     //初始化模型
//     llama_model_params model_params = common_model_params_to_llama(params);
//     llama_model *model = llama_model_load_from_file(params.model.path.c_str(), model_params);

//     if (model == NULL) {
//         LOG_ERR("%s: error: unable to load model\n" , __func__);
//     }



//     //token初始化
//     const llama_vocab * vocab = llama_model_get_vocab(model);
//     std::vector<llama_token> tokens_list;
//     tokens_list = common_tokenize(vocab, params.prompt, true);
//     const int n_kv_req = tokens_list.size() + (n_predict - tokens_list.size())*n_parallel;
//     //上下文初始化
//     llama_context_params ctx_params = common_context_params_to_llama(params);
//     ctx_params.n_ctx   = n_kv_req;
//     ctx_params.n_batch = std::max(n_predict, n_parallel);
//     llama_context * ctx = llama_init_from_model(model, ctx_params);
//     auto sparams = llama_sampler_chain_default_params();
//     sparams.no_perf = false;
//     llama_sampler * smpl = llama_sampler_chain_init(sparams);



//     const char* json_gbnf =
//     R"(
//     root ::= emoji "|" text
//     emoji ::= "a" | "b" | "c" | "d" | "e" | "f" | "g"
//     text ::= [a-zA-Z0-9\u4e00-\u9fa5 ]+
//     )";


//     //语法加入采样链路，强制约束输出
//     llama_sampler_chain_add(smpl, llama_sampler_init_grammar(vocab,json_gbnf,"root"));
//     llama_sampler_chain_add(smpl,
//                             llama_sampler_init_top_k(params.sampling.top_k));
//     llama_sampler_chain_add(smpl,
//                             llama_sampler_init_top_p(params.sampling.top_p, params.sampling.min_keep));
//     llama_sampler_chain_add(smpl,
//                             llama_sampler_init_temp (params.sampling.temp));
//     llama_sampler_chain_add(smpl,
//                             llama_sampler_init_dist (params.sampling.seed));

//     if (ctx == NULL) {
//         LOG_ERR("%s: error: failed to create the llama_context\n" , __func__);
//         //return 1;
//     }

//     const int n_ctx = llama_n_ctx(ctx);
//     LOG_INF("\n%s: n_predict = %d, \
//             n_ctx = %d, \
//                    n_batch = %u, \
//                           n_parallel = %d, \
//                                  n_kv_req = %d\n",
//                                    __func__, n_predict, n_ctx, ctx_params.n_batch, n_parallel, n_kv_req);

//     // make sure the KV cache is big enough to hold all the prompt and generated tokens
//     if (n_kv_req > n_ctx) {
//         LOG_ERR("the required KV cache size is not big enough\n");
//         //return 1;
//     }

//     // print the prompt token-by-token
//     LOG("\n");
//     for (auto id : tokens_list) {
//         LOG("%s", common_token_to_piece(ctx, id).c_str());
//     }

//     // create a llama_batch
//     // we use this object to submit token data for decoding
//     llama_batch batch = llama_batch_init(std::max(tokens_list.size(),
//                                                   (size_t) n_parallel),
//                                          0, n_parallel);

//     std::vector<llama_seq_id> seq_ids(n_parallel, 0);
//     for (int32_t i = 0; i < n_parallel; ++i) {
//         seq_ids[i] = i;
//     }

//     // evaluate the initial prompt
//     for (size_t i = 0; i < tokens_list.size(); ++i) {
//         common_batch_add(batch, tokens_list[i], i, seq_ids, false);
//     }
//     GGML_ASSERT(batch.n_tokens == (int) tokens_list.size());

//     if (llama_model_has_encoder(model)) {
//         if (llama_encode(ctx, batch)) {
//             LOG_ERR("%s : failed to eval\n", __func__);
//             //return 1;
//         }

//         llama_token decoder_start_token_id = llama_model_decoder_start_token(model);
//         if (decoder_start_token_id == LLAMA_TOKEN_NULL) {
//             decoder_start_token_id = llama_vocab_bos(vocab);
//         }
//         common_batch_clear(batch);
//         common_batch_add(batch, decoder_start_token_id, 0, seq_ids, false);
//     }

//     // llama_decode will output logits only for the last token of the prompt
//     batch.logits[batch.n_tokens - 1] = true;

//     if (llama_decode(ctx, batch) != 0) {
//         LOG_ERR("%s: llama_decode() failed\n", __func__);
//         //return 1;
//     }

//     if (n_parallel > 1) {
//         LOG("\n\n%s: generating %d sequences ...\n", __func__, n_parallel);
//     }

//     // main loop
//     std::vector<std::string> streams(n_parallel);
//     std::vector<int32_t> i_batch(n_parallel, batch.n_tokens - 1);
//     int n_cur = batch.n_tokens;
//     int n_decode = 0;
//     const auto t_main_start = llama_time_us();

//     while (n_cur <= n_predict) {
//         // prepare the next batch
//         common_batch_clear(batch);

//         // sample the next token for each parallel sequence / stream
//         for (int32_t i = 0; i < n_parallel; ++i) {
//             if (i_batch[i] < 0) {
//                 // the stream has already finished
//                 continue;
//             }

//             const llama_token new_token_id = llama_sampler_sample(smpl, ctx, i_batch[i]);

//             if (llama_vocab_is_eog(vocab, new_token_id) || n_cur == n_predict) {
//                 i_batch[i] = -1;
//                 LOG("\n");
//                 if (n_parallel > 1) {
//                     LOG_INF("%s: stream %d finished at n_cur = %d", __func__, i, n_cur);
//                 }
//                 continue;
//             }

//             // if there is only one stream, we print immediately to stdout
//             if (n_parallel == 1) {
//                 LOG("%s", common_token_to_piece(ctx, new_token_id).c_str());
//             }

//             streams[i] += common_token_to_piece(ctx, new_token_id);
//             i_batch[i] = batch.n_tokens;
//             common_batch_add(batch, new_token_id, n_cur, { i }, true);
//             n_decode += 1;
//         }

//         if (batch.n_tokens == 0) {
//             break;
//         }

//         n_cur += 1;
//         if (llama_decode(ctx, batch)) {
//             LOG_ERR("%s : failed to eval, return code %d\n", __func__, 1);
//             //return 1;
//         }
//     }

//     if (n_parallel > 1) {
//         LOG("\n");
//         for (int32_t i = 0; i < n_parallel; ++i) {
//             LOG("sequence %d:\n\n%s%s\n\n", i, params.prompt.c_str(), streams[i].c_str());
//         }
//     }

//     const auto t_main_end = ggml_time_us();
//     // LOG_INF("%s: decoded %d tokens in %.2f s, speed: %.2f t/s\n",
//     //         __func__, n_decode, (t_main_end - t_main_start) / 1000000.0f,
//     //         n_decode / ((t_main_end - t_main_start) / 1000000.0f));

//     LOG("\n");
//     llama_perf_sampler_print(smpl);
//     llama_perf_context_print(ctx);

//     fprintf(stderr, "\n");
//     llama_batch_free(batch);
//     llama_sampler_free(smpl);
//     llama_free(ctx);
//     llama_model_free(model);
//     llama_backend_free();
// }









LlamaWorkerThread::LlamaWorkerThread(QObject *parent)
    : QThread(parent), m_stopFlag(false), m_modelLoaded(false)
{llamaControl=new LlamaControl();}

LlamaWorkerThread::~LlamaWorkerThread() {
    stop();
    wait();
    delete llamaControl;
}

void LlamaWorkerThread::run() {
    forever {
        QMutexLocker locker(&m_mutex);
        // 等待任务
        while (!m_stopFlag && m_taskType == TaskType::None) {
            m_cond.wait(&m_mutex);
        }

        // 检查停止标志
        if (m_stopFlag) {
            break;
        }

        // 处理任务
        TaskType currentTask = m_taskType;
        QString currentPrompt = m_prompt;
        QString currentModelPath = m_modelPath;
        m_taskType = TaskType::None; // 重置任务类型
        locker.unlock();

        try {
            switch (currentTask) {
            case TaskType::LoadModel:
                llamaControl->model_init(currentModelPath);
                m_modelLoaded = true;
                emit modelLoaded(true);
                qDebug() << "Model loaded successfully in thread:" << QThread::currentThreadId();
                break;

            case TaskType::PredictContent:
                if (m_modelLoaded) {
                    qDebug()<<currentPrompt;
                    llamaControl->predict_content(currentPrompt);
                    //emit predictResult(llamaControl.getLastResult());
                } else {
                    emit errorOccurred("Model not loaded!");
                }
                break;

            case TaskType::PredictEmotion:
                if (m_modelLoaded) {
                    llamaControl->predict_emotion();
                    //emit predictResult(llamaControl.getLastEmotionResult());
                } else {
                    emit errorOccurred("Model not loaded!");
                }
                break;

            case TaskType::ReleaseModel:
                llamaControl->model_release();
                m_modelLoaded = false;
                emit modelLoaded(false);
                break;
            default:
                break;
            }
        } catch (const std::exception& e) {
            emit errorOccurred(QString("Error: %1").arg(e.what()));
        }
    }

    // 线程退出前释放模型
    if (m_modelLoaded) {
        llamaControl->model_release();
    }
}
