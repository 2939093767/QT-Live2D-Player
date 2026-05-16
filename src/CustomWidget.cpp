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
    Qt::Key key = (Qt::Key)event->key();
    lineEdit()->setText(QKeySequence(key).toString());
}




