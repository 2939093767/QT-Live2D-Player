#ifndef CONFIG_WIDGET_H
#define CONFIG_WIDGET_H

#include <QWidget>
#include <QLayout>
#include <QRadioButton>
#include <qlineedit.h>
#include <QFileDialog>
#include <QDir>
#include <QSettings>
#include <QApplication>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>



#include "config_setting.h"
#include "CustomWidget.h"

#include "LAppModel.hpp"
#include "LAppDelegate.hpp"
#include "LAppLive2DManager.hpp"

#include <QProcess>

namespace Ui {
    class AppWidget;
    class ModelWidget;
    class config_widget;

}


class config_card;




/***************************************************************
*  @FileName:   config_widget.h
*  @Brief:      整体设置窗口
*  @Author:     LH
*  @Date:       2026-05-10
*  @note:
****************************************************************/
class config_widget : public QWidget
{
    Q_OBJECT

public:
    explicit config_widget (QWidget *parent = nullptr);
    ~config_widget ();


    void ui_init();
    static void findAllWidgets(QWidget *parent);

public slots:
    void config_save();
    void return_default();
    void software_restart();

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
private:
    Ui::config_widget  *ui;

};



/***************************************************************
*  @FileName:   config_widget.h
*  @Brief:      抽象基类
*  @Author:     LH
*  @Date:       2026-05-11
*  @note:
****************************************************************/
class config_card : public QWidget
{
    Q_OBJECT

public:
    explicit config_card(QWidget *parent = nullptr);
    ~config_card();
    virtual void change_ui() = 0;
    virtual void save_config() = 0;
    virtual void return_default() = 0;
    QHash<QString,QWidget*> p_config_widgets;//(继承变量，用于保存“配置”->控件的映射)
};



/***************************************************************
*  @FileName:   config_widget.h
*  @Brief:      APP设置
*  @Author:     LH
*  @Date:       2026-05-11
*  @note:
****************************************************************/
class AppWidget:public config_card
{
    Q_OBJECT
public:
    explicit AppWidget(QWidget *parent = nullptr);
    ~AppWidget();
    void change_ui()override;
    void save_config()override;
    void return_default()override;
    void image_choose(QString name,QLineEdit* ui);
private:
    Ui::AppWidget  *ui;

};



/***************************************************************
*  @FileName:   config_widget.h
*  @Brief:      模型设置
*  @Author:     LH
*  @Date:       2026-05-11
*  @note:
****************************************************************/
class ModelWidget : public config_card
{
    Q_OBJECT
public:
    explicit ModelWidget(QWidget *parent = nullptr);
    ~ModelWidget();
    void change_ui()override;
    void save_config()override;
    void return_default()override;

    void image_choose(QString name,QLineEdit* ui);

    void add_expression(QString text);
    void add_motions(QString text);


    const void RegisterglobalhotKey(int id, int MOD_KEY, int key = 0);


// protected:
//     bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;


public slots:
    void folder_choose();
    void QuickkeyUpdate(bool clicked);
    void expression_israndom(bool clicked);
    void motion_israndom(bool clicked);



private:
    //GlobalKeyboard* m_hotkey_manager;
    Ui::ModelWidget* ui;
    bool israndom_ex;
    bool israndom_mo;
};












#endif // CONFIG_WIDGET_H
