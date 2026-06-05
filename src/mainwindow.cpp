#include "mainwindow.h"
#include "FaceHandle/cameracontrol.h"
#include "ui_mainwindow.h"

#include <MainWindow.h>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);

    SetAppMode(WINDOW_MODE);
    setMenuBar(nullptr);        // 移除菜单栏
    setStatusBar(nullptr);      // 移除底部状态栏
    initTray();
    initConfig();
    initSlots();

    //FaceDetectorThread::instance();
    //CameraOpen::instance();

    configAct->triggered(true);
    m_timer = new WatchdogTimer();
    connect(m_timer,&WatchdogTimer::SignalOntime,this,&MainWindow::ontime);
    m_timer->start(QThread::LowestPriority);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete cfgwidget;

    qApp->closeAllWindows();

    // 2. 退出主事件循环（最核心）
    qApp->quit();

    // 3. 强制终止所有子线程（清理卡死线程）
    qApp->thread()->quit();
    // qApp->thread()->wait();

    // 4. 暴力安全退出（Windows 进程直接结束）
    QCoreApplication::exit(0);

}




void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // 左键按下才开始拖拽
    if (event->button() == Qt::LeftButton)
    {
        m_isDrag = true;
        // 记录鼠标相对于控件左上角的偏移
        m_lastPos = event->globalPos() - this->pos();
    }
    QWidget::mousePressEvent(event);
}




void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

    if (m_isDrag)
    {
        // 实时更新窗口位置
        this->move(event->globalPos() - m_lastPos);
    }
    QWidget::mouseMoveEvent(event);



}





void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_isDrag = false;
    QWidget::mouseReleaseEvent(event);


}






void MainWindow::wheelEvent(QWheelEvent *event)
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    cfgwidget->close();
}



void MainWindow::initTray()
{
    // 1. 检查系统是否支持托盘
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::warning(this, "警告", "系统不支持托盘！");
        return;
    }

    // 2. 创建菜单与动作
    trayMenu = new QMenu(this);

    //业务初始化
    windowmodeAct = new QAction("恢复桌宠模式", this);
    trayMenu->addAction(windowmodeAct);
    connect(windowmodeAct, &QAction::triggered, this, [=]{SetAppMode(PET_MODE);});

    restoreAct = new QAction("转为窗口模式", this);
    connect(restoreAct, &QAction::triggered, this, [=]{SetAppMode(WINDOW_MODE);});
    trayMenu->addAction(restoreAct);

    configAct = new QAction("打开设置窗口", this);
    trayMenu->addAction(configAct);
    connect(configAct, &QAction::triggered, this, [=]{cfgwidget->show();});

    quitAct = new QAction("退出程序", this);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAct);
    connect(quitAct, &QAction::triggered, this, [=]{QApplication::exit(0);});

    // 3. 创建托盘图标
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/image/resource/images/pet.png")); // 资源文件图标
    trayIcon->setToolTip("托盘演示\n双击显示窗口");
    trayIcon->setContextMenu(trayMenu); // 绑定右键菜单
    //connect(trayIcon, &QSystemTrayIcon::activated, this, &main_widget::onTrayActivated);
    trayIcon->show(); // 显示托盘图标
}

void MainWindow::initConfig()
{
   cfgwidget = new config_widget();


}

void MainWindow::initSlots()
{
    connect(cfgwidget,&config_widget::SignalStartRender,ui->centralwidget,&MyOpenGL::StartRender);
    connect(cfgwidget,&config_widget::SignalStopRender,ui->centralwidget,&MyOpenGL::StopRender);
    connect(cfgwidget,&config_widget::SignalChangeModel,ui->centralwidget,&MyOpenGL::ChangeModel);
}


void MainWindow::SetAppMode(APP_MODE mode)
{
    if(mode == PET_MODE){
        setWindowFlag(Qt::FramelessWindowHint,true); /* 1 */
        setWindowFlag(Qt::WindowStaysOnTopHint, true);
        ui->centralwidget->SetMode(PET_MODE);
    }else if(mode == WINDOW_MODE){
        setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
        setWindowFlag(Qt::WindowStaysOnTopHint, false);
        ui->centralwidget->SetMode(WINDOW_MODE);
    }
    show();

}

void MainWindow::ontime(){

    // if(ConfigManager::instance().getValue(CONFIG_APP_ISOPACITY).toBool()){
    //     setWindowOpacity(0.0);
    // }else {
    //     setWindowOpacity(1.0);
    // }
    // auto flags = windowFlags();
    // setWindowFlags(Qt::Widget);    //临时改标记销毁原生窗口
    // setWindowFlags(flags);         //还原标记重建窗口
    // show();                        //刷新窗口
    // update();
}







WatchdogTimer::WatchdogTimer(QObject *parent)
{

}

WatchdogTimer::~WatchdogTimer()
{

}

void WatchdogTimer::run()
{
    m_timer = new QTimer();
    connect(m_timer,&QTimer::timeout,this,&WatchdogTimer::ontimeout);
    m_timer->start(1000);
    exec();
    delete m_timer;
}


void WatchdogTimer::ontimeout(){
    emit SignalOntime();



}
