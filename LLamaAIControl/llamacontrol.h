#ifndef LLAMACONTROL_H
#define LLAMACONTROL_H

#include <QObject>
#include "common.h"
#include "arg.h"
#include "log.h"
#include "llama.h"
#include "sampling.h"
#include "json-schema-to-grammar.h"

#include <nlohmann/json.hpp>
#include <ConfigManager/qrc_manager.h>
#include <QMutexLocker>
#include <QThread>
#include <QWaitCondition>
#include <QString>



// #include "common/json-schema-to-grammar.h"
#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>
#include "llama.h"



// 单条对话消息
struct ChatMessage {
    bool is_user;       // true 用户，false AI助手
    std::string text;   // 对话文本（图片占位符也放这里）
};

// AI回复结果（表情+正文）
struct AiReplyResult {
    QString expression;  // 表情: angry/happy/sad/shocked/neutral
    QString content;     // 聊天正文
    bool isValid;        // 解析是否成功
};

// 对话管理器
struct ChatMLBuilder {
    const std::string IM_START = "<|im_start|>";
    const std::string IM_END   = "<|im_end|>";

    // 系统提示词，Live2D虚拟主播人设，规范输出格式
    std::string system_prompt = R"(你是一个Live2D虚拟主播，负责回应用户的对话。
回复格式必须严格遵循：表情|正文
表情只能选择以下五种之一：angry, happy, sad, shocked, neutral
根据对话内容选择合适的表情，然后是竖线分隔符，最后是回复正文。
例如：happy|你好呀，很高兴见到你！
例如：sad|听到这个消息我很难过。
例如：neutral|好的，我明白了。
正文用中文回复，简洁自然，符合对话情境。不知道的知识直接回答不清楚即可。)";

    std::vector<ChatMessage> chat_history;
    // 添加一轮对话记录
    void add_msg(bool is_user, const std::string& content)
    {chat_history.push_back({is_user, content});}

    std::string GetSystem(){
        std::stringstream ss;
        // 1. 写入 system 块
        ss << IM_START << "system\n";
        ss << system_prompt << "\n";
        ss << IM_END << "\n";
        ss << "<|im_start|>assistant\n";

        return ss.str();
    }
    std::string GetEndUserText(){
        std::stringstream ss;
        ss << IM_START << "user\n";
        ss << chat_history[chat_history.size()-1].text << "\n";
        ss << IM_END << "\n";
        ss << "<|im_start|>assistant\n";
        return ss.str();
    }

    // 替换自定义 system 提示词
    void set_system(const std::string& new_sys)
    {
        system_prompt = new_sys;
    }

    // 清空所有对话历史
    void clear_history()
    {
        chat_history.clear();
    }

    //回退一个对话
    void pop_last_msg()
    {
        if (!chat_history.empty())
        {
            chat_history.pop_back();
        }
    }
};


class LlamaControl;
// 定义工作线程类
class LlamaWorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit LlamaWorkerThread(QObject *parent = nullptr);
    ~LlamaWorkerThread() override;

    // 线程安全的停止方法
    void stop() {
        QMutexLocker locker(&m_mutex);
        m_stopFlag = true;

    }

    // 加载模型（线程安全）
    void loadModel(const QString& modelPath) {
        QMutexLocker locker(&m_mutex);
        m_modelPath = modelPath;
        m_taskType = TaskType::LoadModel;
        m_cond.wakeAll();
    }

    // 推理内容（线程安全）
    void predictContent(const QString& prompt) {
        QMutexLocker locker(&m_mutex);
        m_prompt = prompt;
        m_taskType = TaskType::PredictContent;
        m_cond.wakeAll();
    }

    // 推理表情（线程安全）
    void predictEmotion() {
        QMutexLocker locker(&m_mutex);
        m_taskType = TaskType::PredictEmotion;
        m_cond.wakeAll();
    }

    // 释放模型（线程安全）
    void releaseModel() {
        QMutexLocker locker(&m_mutex);
        m_taskType = TaskType::ReleaseModel;
        m_cond.wakeAll();
    }

signals:
    // 模型加载完成信号
    void modelLoaded(bool success);
    // 推理结果信号
    void predictResult(const QString& result);
    // 错误信息信号
    void errorOccurred(const QString& error);

protected:
    void run() override;

private:
    enum class TaskType {
        None,
        LoadModel,
        PredictContent,
        PredictEmotion,
        ReleaseModel
    };

    LlamaControl* llamaControl;
    QMutex m_mutex;
    QWaitCondition m_cond;
    bool m_stopFlag;
    bool m_modelLoaded;
    TaskType m_taskType = TaskType::None;
    QString m_modelPath;
    QString m_prompt;
};





class LlamaControl:public QObject
{
    Q_OBJECT
public:
    LlamaControl();
    ~LlamaControl();
    // // 禁止拷贝构造
    // LlamaControl(const LlamaControl&) = delete;
    // LlamaControl& operator=(const LlamaControl&) = delete;

    void model_init(QString model_path);
    void model_release();
    void LoadConfig();
    void GBNF_init();
    void Ctx_init();



    //static void test_demo();
    llama_sampler *GetEmotionSmpl();
    llama_sampler *GetContentSmpl();
    void predict_content(QString prompt);
    void predict_system();
    void predict_emotion();


signals:
    void DecodeInfo(QString s);
    void replyReady(const QString &expression, const QString &content);
    void expressionChanged(const QString &expression);
    void contentReady(const QString &content);

private:

    QString predict(llama_sampler *smpl);
    bool GetResult(QString &result);
    AiReplyResult parseReply(const QString &rawOutput);

    QString model_path;
    common_params llama_params;
    llama_model* p_model;
    llama_sampler * content_smpl;
    llama_sampler * emotion_smpl;
    const llama_vocab * vocab;
    llama_context * p_ctx;

    QString json_gbnf;
    QString emotion_gbnf;
    ChatMLBuilder g_chat_ctx;
};

#endif // LLAMACONTROL_H
