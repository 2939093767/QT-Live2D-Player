#ifndef AISETTINGWINDOW_H
#define AISETTINGWINDOW_H

#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class AISettingWindow; }
QT_END_NAMESPACE

class LlamaWorkerThread;
class ChatWidget;

class AISettingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AISettingWindow(QWidget *parent = nullptr);
    ~AISettingWindow();

signals:
    void loadModelRequested(const QString &modelPath);
    void unloadModelRequested();
    void testChatRequested(const QString &prompt);

public slots:
    void onModelLoaded(bool success);
    void onPredictResult(const QString &result);
    void onExpressionChanged(const QString &expression);
    void onErrorOccurred(const QString &error);

private slots:
    void onBrowseModelClicked();
    void onLoadModelClicked();
    void onUnloadModelClicked();
    void onTestAIClicked();
    void onClearHistoryClicked();
    void onSendMessage(const QString &text);

private:
    void updateUIState(bool loaded);
    void initWorkerThread();
    void initChatWidget();

private:
    Ui::AISettingWindow *ui;
    LlamaWorkerThread *m_worker = nullptr;
    ChatWidget *m_chatWidget = nullptr;
    bool m_modelLoaded = false;
    bool m_chatShown = false;
};

#endif // AISETTINGWINDOW_H
