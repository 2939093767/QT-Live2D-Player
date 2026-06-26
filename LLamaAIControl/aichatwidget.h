#ifndef AICHATWIDGET_H
#define AICHATWIDGET_H

#include <QVBoxLayout>
#include <QWidget>
#include <LLamaAIControl/llamacontrol.h>

namespace Ui {
class AIChatWidget;
}

class AIChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AIChatWidget(QWidget *parent = nullptr);
    ~AIChatWidget();
    void AddChat(QString info,bool isself);
protected:
    void showEvent(QShowEvent* event)override;

public slots:
    void SendInfo();
    void GetLLMresult(QString info);
    void LoadModel();
    void ChooseGGUFs();

private:
    Ui::AIChatWidget *ui;
    QVBoxLayout* layoutContent;
    LlamaWorkerThread* thread;
};

#endif // AICHATWIDGET_H
