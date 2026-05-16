#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint); /* 1 */
    setAttribute(Qt::WA_TranslucentBackground); /* 2 */
    setWindowFlag(Qt::WindowStaysOnTopHint, true);

    setMenuBar(nullptr);        // 移除菜单栏
    setStatusBar(nullptr);      // 移除底部状态栏
    //centralWidget()->layout()->setContentsMargins(0,0,0,0);



    initTray();
    initConfig();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete cfgwidget;
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


void MainWindow::SetAppMode(APP_MODE mode)
{
    if(mode == PET_MODE){
        setWindowFlag(Qt::FramelessWindowHint,true); /* 1 */
        setAttribute(Qt::WA_TranslucentBackground,true); /* 2 */
        setWindowFlag(Qt::WindowStaysOnTopHint, true);
        ui->centralwidget->SetMode(PET_MODE);
    }else if(mode == WINDOW_MODE){
        setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
        setAttribute(Qt::WA_TranslucentBackground, false);
        setWindowFlag(Qt::WindowStaysOnTopHint, false);
        ui->centralwidget->SetMode(WINDOW_MODE);
        // setAttribute(Qt::WA_AlwaysStackOnTop, false);
        // setAttribute(Qt::WA_NoSystemBackground,false);
        // 必须刷新窗口才能生效
    }
    show();

}
