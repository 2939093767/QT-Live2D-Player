#ifndef LLAMACONTROL_H
#define LLAMACONTROL_H

#include <QObject>
#include "Common/common.h"
#include "Common/arg.h"
#include "Common/log.h"
#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>
#include "llama.h"



class LlamaControl
{
public:
    static LlamaControl& instance(){
        static LlamaControl m_instance;
        return m_instance;
    }
    // 禁止拷贝构造
    LlamaControl(const LlamaControl&) = delete;
    LlamaControl& operator=(const LlamaControl&) = delete;


    void model_init(QString model_path);
    void predict(QString prompt);
    static void test_demo();
private:
    LlamaControl();
    ~LlamaControl();


    QString model_path;
    common_params llama_params;
    llama_model* p_model;

};

#endif // LLAMACONTROL_H
