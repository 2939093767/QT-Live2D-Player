#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qsystemtrayicon.h>
#include <config_widget.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")







QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE





class WatchdogTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initTray();
    void initConfig();
    void initSlots();
    void SetAppMode(APP_MODE mode);

public slots:
    void ontime();
protected:

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void closeEvent(QCloseEvent* event) override;

private:
    QPoint m_lastPos;
    bool m_isDrag = false;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    //菜单业务功能
    QAction *restoreAct;
    QAction *windowmodeAct;
    QAction *configAct;
    QAction *quitAct;

    Ui::MainWindow *ui;
    config_widget* cfgwidget;

    WatchdogTimer* m_timer;
};






class WatchdogTimer:public QThread
{
    Q_OBJECT


public:
    WatchdogTimer(QObject *parent=nullptr);
    ~WatchdogTimer();
    void run()override;



public slots:
    void ontimeout();
signals:
    void SignalOntime();
private:
    QTimer* m_timer;
    //图像采集相关


};




#endif // MAINWINDOW_H
