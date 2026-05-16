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







class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initTray();
    void initConfig();
    void SetAppMode(APP_MODE mode);

protected:

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;


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
};








#endif // MAINWINDOW_H
