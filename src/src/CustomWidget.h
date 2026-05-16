#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QComboBox>
#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QObject>

class CheckComboBox:public QComboBox
{
    Q_OBJECT
public:
    explicit CheckComboBox(QWidget *parent = nullptr);
    ~CheckComboBox();
protected:
    void keyPressEvent(QKeyEvent *event)override;



};

#endif // CUSTOMWIDGET_H
