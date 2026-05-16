#include "CustomWidget.h"



CheckComboBox::CheckComboBox(QWidget *parent)
    : QComboBox(parent)
{
    setEditable(true);
}

CheckComboBox::~CheckComboBox()
{

}

void CheckComboBox::keyPressEvent(QKeyEvent *event)
{

    bool isCtrl  = event->modifiers() & Qt::ControlModifier;
    bool isAlt   = event->modifiers() & Qt::AltModifier;
    bool isShift = event->modifiers() & Qt::ShiftModifier;

    // 没有修饰键，正常输入，不拦截
    if (!isCtrl && !isAlt && !isShift)
    {
        QComboBox::keyPressEvent(event);
    }
    qDebug()<<"过滤事件";
    Qt::Key key = (Qt::Key)event->key();

    QString hotkey;
    if (isCtrl)  hotkey += "Ctrl+";
    if (isAlt)   hotkey += "Alt+";
    if (isShift) hotkey += "Shift+";

    // 按键名
    QString keyName = QKeySequence(key).toString();
    hotkey += keyName;
    qDebug()<<hotkey;
    // 插入到输入框
    lineEdit()->setText(hotkey);

}




