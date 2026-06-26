#ifndef PETSETTINGWINDOW_H
#define PETSETTINGWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class PetSettingWindow; }
QT_END_NAMESPACE

class Live2dActionListWidget;

class PetSettingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PetSettingWindow(QWidget *parent = nullptr);
    ~PetSettingWindow();

private:
    Ui::PetSettingWindow *ui;
    Live2dActionListWidget *m_expressionList = nullptr;
    Live2dActionListWidget *m_actionList = nullptr;
};

#endif // PETSETTINGWINDOW_H
