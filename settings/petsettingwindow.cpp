#include "petsettingwindow.h"
#include "ui_petsettingwindow.h"
#include "CustomWidget/live2dactionlistwidget.h"

PetSettingWindow::PetSettingWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PetSettingWindow)
{
    ui->setupUi(this);

    // 初始化表情列表
    m_expressionList = ui->widget_expressionList;
    m_expressionList->addItem("眨眼");
    m_expressionList->addItem("微笑");
    m_expressionList->addItem("惊讶");

    // 初始化动作列表
    m_actionList = ui->widget_actionList;
    m_actionList->addItem("挥手");
    m_actionList->addItem("点头");
    m_actionList->addItem("跳跃");
}

PetSettingWindow::~PetSettingWindow()
{
    delete ui;
}
