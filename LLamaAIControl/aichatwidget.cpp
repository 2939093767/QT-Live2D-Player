#include "aichatwidget.h"
#include "ui_aichatwidget.h"

#include <QFileDialog>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QScrollBar>
#include <QTimer>

AIChatWidget::AIChatWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AIChatWidget)
{
    ui->setupUi(this);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 关闭横拉条
    layoutContent = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layoutContent->setSpacing(16);
    layoutContent->addStretch(1);   // 消息从顶部开始堆
    thread = new LlamaWorkerThread(this);
    thread->start();



}


AIChatWidget::~AIChatWidget()
{
    delete ui;
}


void AIChatWidget::AddChat(QString info, bool isself)
{
    QWidget *bubble = new QWidget();
    QHBoxLayout *layoutBubble = new QHBoxLayout(bubble);
    layoutBubble->setContentsMargins(8,8,8,8);
    layoutBubble->setSpacing(0);

    QLabel *labelText = new QLabel(info);
    //labelText->setWordWrap(true);
    labelText->setWordWrap(true);   // 自动换行
    labelText->setMaximumWidth(bubble->width());// 最大宽度，防止太宽
    //labelText->setMinimumWidth(80);
    labelText->setStyleSheet(R"(
        QLabel{
            padding:10px 14px;
            border-radius:16px;
            font-size:15px;
            color:#fff;
            background-color:#5ba6f6; /* 用户气泡蓝色 */
        }
    )");

    if (!isself) {
        // AI 气泡：左侧、绿色
        labelText->setStyleSheet(R"(
            QLabel{
                padding:10px 14px;
                border-radius:16px;
                font-size:15px;
                color:#333;
                background-color:#e5e5ea; /* AI灰色 */
            }
        )");
        layoutBubble->addWidget(labelText);
        layoutBubble->addStretch(1);   // 靠左
    } else {
        // 用户气泡：靠右、蓝色
        layoutBubble->addStretch(1);
        layoutBubble->addWidget(labelText);
    }

    layoutContent->insertWidget(layoutContent->count()-1,bubble);
    QTimer::singleShot(10, this, [this]() {
        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum());
    });
}

void AIChatWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

}

void AIChatWidget::SendInfo()
{
    QString inputText = ui->textEdit->toPlainText();
    // 去掉首尾空格、换行（防止发送空消息）
    inputText = inputText.trimmed();
    if(inputText != ""){
        AddChat(inputText,true);
        thread->predictContent(inputText);
        // QFuture<void> future = QtConcurrent::run([=](){
        //     LlamaControl::instance().predict_content(inputText);
        // });
    }
}

void AIChatWidget::GetLLMresult(QString info)
{
    AddChat(info,false);

}


void AIChatWidget::LoadModel()
{
    if(!QFile::exists(ui->lineEdit->text())){
        qDebug()<<ui->lineEdit->text();
        QMessageBox::warning(this, "提示", "模型文件不存在！");
        return;
    }
    thread->loadModel(ui->lineEdit->text());
    // QFuture<void> future = QtConcurrent::run([=](){
    //     LlamaControl::instance().model_init(ui->lineEdit->text());
    // });


}



void AIChatWidget::ChooseGGUFs()
{

    QString filePath = QFileDialog::getOpenFileName(
        this,
        "选择 GGUF 模型文件",
        QCoreApplication::applicationDirPath(),
        "GGUF Files (*.gguf);;All Files (*.*)"  // 只显示 gguf + 可选所有文件
    );
    if(filePath != ""){
        ui->lineEdit->setText(filePath);
    }

}




