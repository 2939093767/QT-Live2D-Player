#include "aisettingwindow.h"
#include "ui_aisettingwindow.h"
#include "LLamaAIControl/llamacontrol.h"
#include "CustomWidget/chatwidget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QVBoxLayout>

AISettingWindow::AISettingWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AISettingWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_browseModel, &QPushButton::clicked, this, &AISettingWindow::onBrowseModelClicked);
    connect(ui->pushButton_loadModel, &QPushButton::clicked, this, &AISettingWindow::onLoadModelClicked);
    connect(ui->pushButton_unloadModel, &QPushButton::clicked, this, &AISettingWindow::onUnloadModelClicked);
    connect(ui->pushButton_testAI, &QPushButton::clicked, this, &AISettingWindow::onTestAIClicked);
    connect(ui->pushButton_clearHistory, &QPushButton::clicked, this, &AISettingWindow::onClearHistoryClicked);

    initWorkerThread();
    updateUIState(false);
}

AISettingWindow::~AISettingWindow()
{
    if (m_worker) {
        m_worker->stop();
        m_worker->wait();
        delete m_worker;
        m_worker = nullptr;
    }
    delete ui;
}

void AISettingWindow::initWorkerThread()
{
    m_worker = new LlamaWorkerThread(this);
    m_worker->start();

    connect(m_worker, &LlamaWorkerThread::modelLoaded, this, &AISettingWindow::onModelLoaded);
    connect(m_worker, &LlamaWorkerThread::predictResult, this, &AISettingWindow::onPredictResult);
    connect(m_worker, &LlamaWorkerThread::errorOccurred, this, &AISettingWindow::onErrorOccurred);
}

void AISettingWindow::initChatWidget()
{
    if (m_chatWidget)
        return;

    m_chatWidget = new ChatWidget(nullptr);
    m_chatWidget->setWindowTitle("AI 对话");
    m_chatWidget->resize(500, 600);
    m_chatWidget->setInputPlaceholder("输入消息，按 Enter 发送...");

    connect(m_chatWidget, &ChatWidget::sendMessageRequested, this, &AISettingWindow::onSendMessage);
}

void AISettingWindow::onBrowseModelClicked()
{
    QString path = QFileDialog::getOpenFileName(this, "选择模型文件", "", "GGUF 模型文件 (*.gguf);;所有文件 (*.*)");
    if (!path.isEmpty()) {
        ui->lineEdit_llamaModel->setText(path);
    }
}

void AISettingWindow::onLoadModelClicked()
{
    QString path = ui->lineEdit_llamaModel->text().trimmed();
    if (path.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选择模型文件");
        return;
    }

    ui->pushButton_loadModel->setEnabled(false);
    ui->pushButton_unloadModel->setEnabled(false);
    ui->plainTextEdit_modelInfo->appendPlainText(QString("[%1] 开始加载模型: %2").arg(
        QDateTime::currentDateTime().toString("hh:mm:ss"), path));

    m_worker->loadModel(path);
}

void AISettingWindow::onUnloadModelClicked()
{
    ui->pushButton_loadModel->setEnabled(false);
    ui->pushButton_unloadModel->setEnabled(false);
    ui->plainTextEdit_modelInfo->appendPlainText(QString("[%1] 正在卸载模型...").arg(
        QDateTime::currentDateTime().toString("hh:mm:ss")));

    m_worker->releaseModel();
}

void AISettingWindow::onTestAIClicked()
{
    if (!m_modelLoaded) {
        QMessageBox::warning(this, "提示", "请先加载模型");
        return;
    }

    initChatWidget();
    m_chatWidget->show();
    m_chatWidget->activateWindow();
    m_chatWidget->raise();

    m_chatShown = true;
}

void AISettingWindow::onClearHistoryClicked()
{
    ui->plainTextEdit_modelInfo->clear();
}

void AISettingWindow::onSendMessage(const QString &text)
{
    if (!m_modelLoaded || !m_chatWidget)
        return;

    m_chatWidget->addUserMessage(text);
    m_worker->predictContent(text);
}

void AISettingWindow::onModelLoaded(bool success)
{
    m_modelLoaded = success;
    updateUIState(success);
    if (success) {
        ui->plainTextEdit_modelInfo->appendPlainText(QString("[%1] 模型加载成功！").arg(
            QDateTime::currentDateTime().toString("hh:mm:ss")));
    } else {
        ui->plainTextEdit_modelInfo->appendPlainText(QString("[%1] 模型加载失败！").arg(
            QDateTime::currentDateTime().toString("hh:mm:ss")));
    }
}

void AISettingWindow::onPredictResult(const QString &result)
{
    ui->plainTextEdit_modelInfo->appendPlainText(QString("[%1] AI回复: %2").arg(
        QDateTime::currentDateTime().toString("hh:mm:ss"), result));

    if (m_chatWidget && m_chatShown) {
        m_chatWidget->addAssistantMessage(result);
    }
}

void AISettingWindow::onExpressionChanged(const QString &expression)
{
    Q_UNUSED(expression);
}

void AISettingWindow::onErrorOccurred(const QString &error)
{
    ui->plainTextEdit_modelInfo->appendPlainText(QString("[%1] 错误: %2").arg(
        QDateTime::currentDateTime().toString("hh:mm:ss"), error));
    updateUIState(m_modelLoaded);
}

void AISettingWindow::updateUIState(bool loaded)
{
    ui->pushButton_loadModel->setEnabled(!loaded);
    ui->pushButton_unloadModel->setEnabled(loaded);
    ui->pushButton_testAI->setEnabled(loaded);
    ui->lineEdit_llamaModel->setEnabled(!loaded);
    ui->pushButton_browseModel->setEnabled(!loaded);
    ui->groupBox_sample->setEnabled(!loaded);
}
