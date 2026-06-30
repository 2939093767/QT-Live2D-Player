#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CustomWidget/logowidget.h"
#include "monitor/systemmonitor.h"
#include "settings/appsettingwindow.h"
#include "settings/petsettingwindow.h"
#include "settings/facecapsettingwindow.h"
#include "settings/aisettingwindow.h"
#include "settings/aiskillsettingwindow.h"
#include "myopenglw.h"
#include "apputils.h"
#include <QApplication>
#include <QDateTime>
#include <QIcon>
#include <QMouseEvent>
#include <QPushButton>
#include <QStyle>
#include <QTimer>
#include <QShowEvent>
#include <QStackedWidget>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QRect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowIcon(QIcon(":/logo/logo.png"));

    installDragFilter(ui->widget_topBar);
    connect(ui->pushButton_close, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->pushButton_minimize, &QPushButton::clicked, this, &MainWindow::showMinimized);

    ui->drawerSwitch->addOption(QIcon(":/icons/appstore.png"), "App 设置", new AppSettingWindow(this));
    ui->drawerSwitch->addOption(QIcon(":/icons/jiaohumoshi.png"), "桌宠交互", new PetSettingWindow(this));
    ui->drawerSwitch->addOption(QIcon(":/icons/mianbushibie.png"), "面捕功能", new FaceCapSettingWindow(this));
    ui->drawerSwitch->addOption(QIcon(":/icons/aislogo.png"), "AI 助手", new AISettingWindow(this));
    ui->drawerSwitch->addOption(QIcon(":/icons/skill.png"), "AI Skill", new AISkillSettingWindow(this));
    ui->drawerSwitch->setCurrentIndex(0);

    m_monitor = new SystemMonitor(this);
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::updateSystemInfo);
    m_updateTimer->start(1000);
    updateSystemInfo();

    m_clockTimer = new QTimer(this);
    connect(m_clockTimer, &QTimer::timeout, this, &MainWindow::updateTime);
    updateTime();
    m_clockTimer->start(1000);

    init();
    initTrayIcon();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSystemInfo()
{
    SystemResourceInfo info = m_monitor->query();

    auto toGB = [](quint64 bytes) -> double {
        return static_cast<double>(bytes) / (1024.0 * 1024.0 * 1024.0);
    };

    QString text = QString("CPU: %1% | 内存: %2 GB / %3 GB (%4%)")
                       .arg(QString::number(info.cpuUsagePercent, 'f', 1))
                       .arg(QString::number(toGB(info.usedMemoryBytes), 'f', 1))
                       .arg(QString::number(toGB(info.totalMemoryBytes), 'f', 1))
                       .arg(QString::number(info.memoryUsagePercent, 'f', 1));

    if (m_monitor->isVramAvailable()) {
        text += QString(" | 显存: %1 GB / %2 GB (%3%)")
                    .arg(QString::number(toGB(info.usedVramBytes), 'f', 1))
                    .arg(QString::number(toGB(info.totalVramBytes), 'f', 1))
                    .arg(QString::number(info.vramUsagePercent, 'f', 1));
    } else {
        text += QString(" | 显存: 不可用");
    }

    ui->label_systemInfo->setText(text);
}

void MainWindow::installDragFilter(QWidget *widget)
{
    widget->installEventFilter(this);
    for (QObject *child : widget->children()) {
        if (auto *childWidget = qobject_cast<QWidget *>(child)) {
            if (!qobject_cast<QPushButton *>(childWidget))
                installDragFilter(childWidget);
        }
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    auto *watchedWidget = qobject_cast<QWidget *>(watched);
    bool isTopBar = watchedWidget && (watchedWidget == ui->widget_topBar || ui->widget_topBar->isAncestorOf(watchedWidget));
    bool isButton = watchedWidget && (watchedWidget == ui->pushButton_close || watchedWidget == ui->pushButton_minimize);

    if (isTopBar) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton && !isButton) {
                m_dragging = true;
                m_dragStartPos = mouseEvent->globalPosition().toPoint() - frameGeometry().topLeft();
            }
        } else if (event->type() == QEvent::MouseMove && m_dragging) {
            auto *mouseEvent = static_cast<QMouseEvent *>(event);
            move(mouseEvent->globalPosition().toPoint() - m_dragStartPos);
        } else if (event->type() == QEvent::MouseButtonRelease) {
            m_dragging = false;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::updateTime()
{
    ui->label_time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    AppUtils::instance().setTheme(0);
}


void MainWindow::init()
{
    m_openglWidget = new MyOpenGL(nullptr);
    m_openglWidget->setObjectName("openglWidget");
    m_openglWidget->setAttribute(Qt::WA_DeleteOnClose);
    m_openglWidget->setWindowTitle("Live2D");
    m_openglWidget->resize(800, 600);
    m_openglWidget->show();
    m_openglWidget->StartRender();

    // 连接 OpenGL 工具按钮信号
    connect(m_openglWidget, &MyOpenGL::openSettingsRequested, this, [this]() {
        // 切换到设置页面（索引0）
        ui->drawerSwitch->setCurrentIndex(0);
        this->show();
    }, Qt::QueuedConnection);

    connect(m_openglWidget, &MyOpenGL::trackModeChanged, this, [](MyOpenGL::TrackMode mode) {
        QString modeStr;
        switch (mode) {
        case MyOpenGL::TrackMode_None: modeStr = "None"; break;
        case MyOpenGL::TrackMode_Mouse: modeStr = "Mouse"; break;
        case MyOpenGL::TrackMode_Face: modeStr = "Face"; break;
        case MyOpenGL::TrackMode_AI: modeStr = "AI"; break;
        }
        qDebug() << "MainWindow: Track mode changed to" << modeStr;
    });


}



void MainWindow::faceCapSlotInit()
{
    QWidget *faceCapWidget = ui->drawerSwitch->Getwidget(2);
    FaceCapSettingWindow *faceCap = qobject_cast<FaceCapSettingWindow *>(faceCapWidget);
    if (!faceCap)
        return;

    QObject::connect(faceCap, &FaceCapSettingWindow::cameraChanged, this,
        [](int index) {
            qDebug() << "Camera changed:" << index;
        });

    QObject::connect(faceCap, &FaceCapSettingWindow::refreshCameraRequested, this,
        []() {
            qDebug() << "Refresh camera requested";
        });

    QObject::connect(faceCap, &FaceCapSettingWindow::resolutionChanged, this,
        [](int index) {
            qDebug() << "Resolution changed:" << index;
        });

    QObject::connect(faceCap, &FaceCapSettingWindow::fpsChanged, this,
        [](int index) {
            qDebug() << "FPS changed:" << index;
        });

    QObject::connect(faceCap, &FaceCapSettingWindow::previewToggled, this,
        [](bool opened) {
            qDebug() << "Preview toggled:" << opened;
        });

    QObject::connect(faceCap, &FaceCapSettingWindow::sensitivityChanged, this,
        [](int value) {
            qDebug() << "Sensitivity changed:" << value;
        });

    QObject::connect(faceCap, &FaceCapSettingWindow::smoothnessChanged, this,
        [](int value) {
            qDebug() << "Smoothness changed:" << value;
        });

    QObject::connect(faceCap, &FaceCapSettingWindow::headTrackToggled, this,
        [](bool enabled) {
            qDebug() << "Head track toggled:" << enabled;
        });

    QObject::connect(faceCap, &FaceCapSettingWindow::eyeTrackToggled, this,
        [](bool enabled) {
            qDebug() << "Eye track toggled:" << enabled;
        });

    QObject::connect(faceCap, &FaceCapSettingWindow::mouthTrackToggled, this,
        [](bool enabled) {
            qDebug() << "Mouth track toggled:" << enabled;
        });

    QObject::connect(faceCap, &FaceCapSettingWindow::blinkToggled, this,
        [](bool enabled) {
            qDebug() << "Blink toggled:" << enabled;
        });

    QObject::connect(faceCap, &FaceCapSettingWindow::expressionToggled, this,
        [](bool enabled) {
            qDebug() << "Expression toggled:" << enabled;
        });

    QObject::connect(faceCap, &FaceCapSettingWindow::cropRectChanged, this,
        [](const QRect &rect) {
            qDebug() << "Crop rect changed:" << rect;
        });
}

void MainWindow::initTrayIcon()
{
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/logo/logo.png"));
    m_trayIcon->setToolTip("Live2D Pet");

    m_trayMenu = new QMenu(this);
    
    QAction *showAction = new QAction("显示主窗口", this);
    connect(showAction, &QAction::triggered, this, &MainWindow::showMainWindow);
    m_trayMenu->addAction(showAction);

    QAction *hideAction = new QAction("隐藏主窗口", this);
    connect(hideAction, &QAction::triggered, this, &MainWindow::hide);
    m_trayMenu->addAction(hideAction);

    m_trayMenu->addSeparator();

    QAction *quitAction = new QAction("退出", this);
    connect(quitAction, &QAction::triggered, this, &MainWindow::quitApplication);
    m_trayMenu->addAction(quitAction);

    m_trayIcon->setContextMenu(m_trayMenu);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
    m_trayIcon->show();
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        showMainWindow();
        break;
    case QSystemTrayIcon::Trigger:
        break;
    default:
        break;
    }
}

void MainWindow::showMainWindow()
{
    show();
    activateWindow();
    raise();
}

void MainWindow::quitApplication()
{
    if (m_trayIcon) {
        m_trayIcon->hide();
    }
    if (m_openglWidget) {
        m_openglWidget->close();
    }
    qApp->quit();
}
