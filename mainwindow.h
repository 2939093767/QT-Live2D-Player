#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SystemMonitor;
class QTimer;
class QEvent;
class MyOpenGL;
class AppSettingWindow;
class FaceCapSettingWindow;
class AISettingWindow;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Slotsinit();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void updateSystemInfo();
    void updateTime();
    void init();
    // void appSettingSlotInit();
    void faceCapSlotInit();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMainWindow();
    void quitApplication();

private:
    void installDragFilter(QWidget *widget);
    void initTrayIcon();

    Ui::MainWindow *ui;
    SystemMonitor *m_monitor = nullptr;
    QTimer *m_updateTimer = nullptr;
    QTimer *m_clockTimer = nullptr;
    MyOpenGL *m_openglWidget = nullptr;
    QSystemTrayIcon *m_trayIcon = nullptr;
    QMenu *m_trayMenu = nullptr;

    bool m_dragging = false;
    QPoint m_dragStartPos;
};

#endif // MAINWINDOW_H
