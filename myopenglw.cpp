#include "myopenglw.h"

//#include "FaceHandle/cameracontrol.h"
#include <QGuiApplication>
#include <QLabel>
#include <QRandomGenerator>





MyOpenGL::MyOpenGL(QWidget *parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    t_modelupdate = new QTimer(this);
    connect(t_modelupdate, &QTimer::timeout, this, [=]{update();});

    // ========== 顶部小箭头触发条 ==========
    m_stubWidget = new QWidget(this);
    m_stubWidget->setFixedHeight(18);
    m_stubWidget->setFixedWidth(32);
    m_stubWidget->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    m_stubWidget->setAttribute(Qt::WA_TranslucentBackground);
    m_stubWidget->setAutoFillBackground(false);
    m_stubWidget->setStyleSheet(
        "QWidget {"
        "   background-color: rgba(50, 50, 55, 200);"
        "   border-radius: 4px;"
        "}"
    );
    m_stubWidget->setCursor(Qt::PointingHandCursor);

    // 触发条内部只有一个箭头
    m_stubLayout = new QHBoxLayout(m_stubWidget);
    m_stubLayout->setContentsMargins(0, 2, 0, 2);
    m_stubLayout->setSpacing(0);
    QLabel *stubLabel = new QLabel("▼", m_stubWidget);
    stubLabel->setAlignment(Qt::AlignCenter);
    stubLabel->setStyleSheet("QLabel { color: rgba(200, 200, 200, 230); font-size: 12px; }");
    m_stubLayout->addWidget(stubLabel);

    // 点击触发条切换面板
    m_stubWidget->installEventFilter(this);

    // ========== 弹出按钮面板（横向半透明） ==========
    m_panelWidget = new QWidget(this);
    m_panelWidget->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    m_panelWidget->setAttribute(Qt::WA_TranslucentBackground);
    m_panelWidget->setAutoFillBackground(false);
    m_panelWidget->setStyleSheet(
        "QWidget {"
        "   background-color: rgba(30, 30, 35, 200);"
        "   border-radius: 10px;"
        "}"
    );
    m_panelWidget->hide();  // 默认隐藏

    // 按钮面板水平布局
    m_panelLayout = new QHBoxLayout(m_panelWidget);
    m_panelLayout->setContentsMargins(10, 8, 10, 8);
    m_panelLayout->setSpacing(8);

    ToolBtnInit();
}

MyOpenGL::~MyOpenGL()
{
    qApp->quit();
}

void MyOpenGL::ToolBtnInit(){
    // 退出
    addTextButton("退出", [this]() {
        qApp->quit();
    });

    // 无边框
    addTextButton("无框", [this]() {
        if (windowFlags() & Qt::FramelessWindowHint) {
            setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
        } else {
            setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
        }
        show();
    });

    // 设置
    addTextButton("设置", [this]() {
        emit openSettingsRequested();
    });

    // 分隔
    //addSeparator();

    // 模式按钮（横向）
    addModeButton("鼠标", TrackMode_Mouse);
    addModeButton("人脸", TrackMode_Face);
    addModeButton("AI", TrackMode_AI);
}

void MyOpenGL::addTextButton(const QString &text, std::function<void()> onClicked)
{
    QPushButton *btn = new QPushButton(text, m_panelWidget);
    btn->setFixedHeight(32);
    btn->setMinimumWidth(50);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(120, 180, 250, 180);"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   font-size: 12px;"
        "   padding: 0px 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(130, 190, 255, 220);"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(60, 140, 230, 255);"
        "}"
    );

    m_panelLayout->addWidget(btn);
    m_toolButtons.append(btn);

    if (onClicked) {
        connect(btn, &QPushButton::clicked, this, [this, onClicked, btn]() {
            onClicked();
        });
    }
}

void MyOpenGL::addSeparator()
{
    QFrame *sep = new QFrame(m_panelWidget);
    sep->setFrameShape(QFrame::VLine);
    sep->setStyleSheet("QFrame { color: rgba(100, 100, 100, 150); }");
    sep->setFixedWidth(1);
    sep->setFixedHeight(24);
    m_panelLayout->addWidget(sep);
}

void MyOpenGL::addToolButton(const QString &iconPath, std::function<void()> onClicked)
{
    QPushButton *btn = new QPushButton(m_panelWidget);
    btn->setFixedSize(36, 32);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(120, 180, 250, 180);"
        "   border: none;"
        "   border-radius: 6px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(130, 190, 255, 220);"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(60, 140, 230, 255);"
        "}"
    );

    if (!iconPath.isEmpty()) {
        btn->setIcon(QIcon(iconPath));
        btn->setIconSize(QSize(24, 24));
    }

    m_panelLayout->addWidget(btn);
    m_toolButtons.append(btn);

    if (onClicked) {
        connect(btn, &QPushButton::clicked, onClicked);
    }
}

void MyOpenGL::addModeButton(const QString &text, TrackMode mode)
{
    QPushButton *btn = new QPushButton(text, m_panelWidget);
    btn->setFixedHeight(32);
    btn->setMinimumWidth(50);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setCheckable(true);
    btn->setProperty("trackMode", mode);
    btn->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(120, 180, 250, 180);"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   font-size: 12px;"
        "   padding: 0px 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(130, 190, 255, 220);"
        "}"
        "QPushButton:checked {"
        "   background-color: rgba(91, 166, 246, 255);"
        "   color: white;"
        "}"
    );

    m_panelLayout->addWidget(btn);
    m_modeButtons.append(btn);

    connect(btn, &QPushButton::clicked, this, [this, mode, btn]() {
        if (m_trackMode == mode) {
            setTrackMode(TrackMode_None);
        } else {
            setTrackMode(mode);
        }
    });
}

bool MyOpenGL::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_stubWidget && event->type() == QEvent::MouseButtonPress) {
        togglePanel();
        return true;
    }
    return QOpenGLWidget::eventFilter(watched, event);
}

void MyOpenGL::enterEvent(QEnterEvent *event)
{
    QOpenGLWidget::enterEvent(event);
}

void MyOpenGL::leaveEvent(QEvent *event)
{
    QOpenGLWidget::leaveEvent(event);
}

void MyOpenGL::togglePanel()
{
    if (m_panelVisible) {
        hidePanel();
    } else {
        showPanel();
    }
}

void MyOpenGL::showPanel()
{
    if (m_panelVisible) return;
    m_panelVisible = true;

    // 箭头朝上
    if (m_stubLayout) {
        QLabel *label = qobject_cast<QLabel*>(m_stubLayout->itemAt(0)->widget());
        if (label) label->setText("▲");
    }

    // 定位面板到触发条下方
    QPoint stubGlobal = m_stubWidget->mapToGlobal(QPoint(0, 0));
    int panelWidth = m_panelWidget->sizeHint().width();
    int stubWidth = m_stubWidget->width();

    // 居中于触发条
    int panelX = stubGlobal.x() + (stubWidth - panelWidth) / 2;
    int panelY = stubGlobal.y() + m_stubWidget->height() + 4;

    m_panelWidget->move(panelX, panelY);
    m_panelWidget->adjustSize();
    m_panelWidget->show();
}

void MyOpenGL::hidePanel()
{
    if (!m_panelVisible) return;
    m_panelVisible = false;

    // 箭头朝下
    if (m_stubLayout) {
        QLabel *label = qobject_cast<QLabel*>(m_stubLayout->itemAt(0)->widget());
        if (label) label->setText("▼");
    }

    m_panelWidget->hide();
}


//键鼠控制方式渲染函数
void MyOpenGL::KeyRenderUpdate()
{
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    int screenW = screenRect.width();
    int screenH = screenRect.height();

    // 当前画布（你的widget）尺寸
    int canvasW = this->width();
    int canvasH = this->height();

    // 鼠标屏幕坐标
    QPoint mouseScreen = QCursor::pos();

    // ==============================================
    // 核心：屏幕坐标 → 画布坐标（全屏映射）
    // ==============================================
    qreal canvasX = (qreal)mouseScreen.x() * canvasW / screenW;
    qreal canvasY = (qreal)mouseScreen.y() * canvasH / screenH;
    LAppDelegate::GetInstance()->GetView()->OnTouchesMoved(canvasX,canvasY);
    LAppDelegate::GetInstance()->update();
}


//面捕控制方式渲染函数
void MyOpenGL::FaceRenderUpdate()
{
    FaceInfo info;
    LAppDelegate::GetInstance()->update();
    QRC_Manager::instance().GetFaceInfo(info);
    LAppLive2DManager::GetInstance()->GetModel(0)->ChangeValueParamter(Live2D::Cubism::Framework::DefaultParameterId::ParamAngleX,info.yaw);
    LAppLive2DManager::GetInstance()->GetModel(0)->ChangeValueParamter(Live2D::Cubism::Framework::DefaultParameterId::ParamAngleY,info.pitch);
    LAppLive2DManager::GetInstance()->GetModel(0)->ChangeValueParamter(Live2D::Cubism::Framework::DefaultParameterId::ParamAngleZ,info.roll);
    LAppLive2DManager::GetInstance()->GetModel(0)->ChangeValueParamter(Live2D::Cubism::Framework::DefaultParameterId::ParamBodyAngleX,info.yaw/3);
    LAppLive2DManager::GetInstance()->GetModel(0)->ChangeValueParamter(Live2D::Cubism::Framework::DefaultParameterId::ParamBodyAngleY,info.pitch/3);
    LAppLive2DManager::GetInstance()->GetModel(0)->ChangeValueParamter(Live2D::Cubism::Framework::DefaultParameterId::ParamBodyAngleZ,info.roll/3);
    LAppLive2DManager::GetInstance()->GetModel(0)->ChangeValueParamter(Live2D::Cubism::Framework::DefaultParameterId::ParamEyeLOpen,info.left_eye_height);
    LAppLive2DManager::GetInstance()->GetModel(0)->ChangeValueParamter(Live2D::Cubism::Framework::DefaultParameterId::ParamEyeROpen,info.right_eye_height);
    LAppLive2DManager::GetInstance()->GetModel(0)->ChangeValueParamter(Live2D::Cubism::Framework::DefaultParameterId::ParamMouthOpenY,info.mouth_height);

}


//初始化
void MyOpenGL::initializeGL()
{

    try{
        ConfigManager::instance();
        LAppDelegate::GetInstance()->Initialize(this);
        qDebug()<<"初始化结束";
    }catch(const _exception& e){
        qDebug()<<"error";
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    LAppLive2DManager::GetInstance()->GetModel(0)->ShowParamterUpDown(Live2D::Cubism::Framework::DefaultParameterId::ParamAngleX);
    LAppLive2DManager::GetInstance()->GetModel(0)->ShowParamterUpDown(Live2D::Cubism::Framework::DefaultParameterId::ParamAngleY);
    LAppLive2DManager::GetInstance()->GetModel(0)->ShowParamterUpDown(Live2D::Cubism::Framework::DefaultParameterId::ParamAngleZ);
    LAppLive2DManager::GetInstance()->GetModel(0)->ShowParamterUpDown(Live2D::Cubism::Framework::DefaultParameterId::ParamBodyAngleX);
    LAppLive2DManager::GetInstance()->GetModel(0)->ShowParamterUpDown(Live2D::Cubism::Framework::DefaultParameterId::ParamBodyAngleY);
    LAppLive2DManager::GetInstance()->GetModel(0)->ShowParamterUpDown(Live2D::Cubism::Framework::DefaultParameterId::ParamBodyAngleZ);
    LAppLive2DManager::GetInstance()->GetModel(0)->ShowParamterUpDown(Live2D::Cubism::Framework::DefaultParameterId::ParamEyeLOpen);
    LAppLive2DManager::GetInstance()->GetModel(0)->ShowParamterUpDown(Live2D::Cubism::Framework::DefaultParameterId::ParamEyeROpen);
    LAppLive2DManager::GetInstance()->GetModel(0)->ShowParamterUpDown(Live2D::Cubism::Framework::DefaultParameterId::ParamEyeLSmile);
    LAppLive2DManager::GetInstance()->GetModel(0)->ShowParamterUpDown(Live2D::Cubism::Framework::DefaultParameterId::ParamEyeRSmile);
    LAppLive2DManager::GetInstance()->GetModel(0)->ShowParamterUpDown(Live2D::Cubism::Framework::DefaultParameterId::ParamMouthOpenY);
    LAppLive2DManager::GetInstance()->GetModel(0)->ShowParamterUpDown(Live2D::Cubism::Framework::DefaultParameterId::ParamMouthForm);

}

void MyOpenGL::resizeGL(int w, int h)
{
    LAppDelegate::GetInstance()->resize(w, h);

    if (m_stubWidget && isVisible()) {
        // 延迟更新位置，避免频繁移动导致闪烁
        QTimer::singleShot(10, this, [this]() {
            if (m_stubWidget && isVisible()) {
                updateToolWidgetPosition();
            }
        });
    }
}

void MyOpenGL::moveEvent(QMoveEvent *event)
{
    QOpenGLWidget::moveEvent(event);
    if (m_stubWidget && isVisible()) {
        QTimer::singleShot(10, this, [this]() {
            if (m_stubWidget && isVisible()) {
                updateToolWidgetPosition();
            }
        });
    }
}

void MyOpenGL::showEvent(QShowEvent *event)
{
    QOpenGLWidget::showEvent(event);
    if (m_stubWidget) {
        QTimer::singleShot(10, this, [this]() {
            if (m_stubWidget && isVisible()) {
                updateToolWidgetPosition();
                m_stubWidget->show();
            }
        });
    }
}

void MyOpenGL::updateToolWidgetPosition()
{
    if (!m_stubWidget)
        return;

    // 固定在窗口顶部正中间
    int stubX = (width() - m_stubWidget->width()) / 2;
    QPoint localTopCenter = QPoint(stubX, 8);
    QPoint globalTopCenter = mapToGlobal(localTopCenter);

    // 边界检查
    QScreen *screen = QGuiApplication::screenAt(globalTopCenter);
    if (screen) {
        QRect screenGeo = screen->geometry();
        if (globalTopCenter.x() + m_stubWidget->width() > screenGeo.right()) {
            globalTopCenter.setX(screenGeo.right() - m_stubWidget->width());
        }
        if (globalTopCenter.x() < screenGeo.left()) {
            globalTopCenter.setX(screenGeo.left());
        }
    }

    m_stubWidget->move(globalTopCenter);
}

void MyOpenGL::hideEvent(QHideEvent *event)
{
    QOpenGLWidget::hideEvent(event);
    if (m_stubWidget) {
        m_stubWidget->hide();
    }
}



void MyOpenGL::paintGL()
{
    // 根据跟踪模式选择渲染逻辑
    switch (m_trackMode) {
    case TrackMode_None:
    case TrackMode_Mouse:
        KeyRenderUpdate();
        break;
    case TrackMode_Face:
        FaceRenderUpdate();
        break;
    case TrackMode_AI:
        // AI 模式下使用键盘模式渲染
        KeyRenderUpdate();
        break;
    }

    if (m_isrunning) {
        t_modelupdate->start((1.0 / invertFPS(ConfigManager::instance().getValue(CONFIG_APP_FPS).toInt())) * 1000);
    }
}




void MyOpenGL::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        qDebug()<<"pressing";
        // LAppDelegate::GetInstance()->GetView()->OnTouchesBegan();
    }



}




void MyOpenGL::mouseMoveEvent(QMouseEvent *event)
{



}





void MyOpenGL::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    float px = pos.x();
    float py = pos.y();

    // 2. 获取视口尺寸
    float w = (float)this->width();
    float h = (float)this->height();

    // 3. 映射到 [-1, 1] 坐标系
    float lx =  2.0f * px / w - 1.0f;
    float ly = -2.0f * py / h + 1.0f; // Y轴翻转对齐OpenGL
    auto Setting = LAppLive2DManager::GetInstance()->GetModel(0)->GetSetting();
    for(int i=0;i < Setting->GetHitAreasCount();i++){
        auto hitarea = Setting->GetHitAreaName(i);
        auto areaid = Setting->GetHitAreaId(i);

        if(LAppLive2DManager::GetInstance()->GetModel(0)->IsHit(areaid,lx,ly)){
        // if(LAppDelegate::GetInstance()->GetView()->OnTouchesIsHit(hitarea,lx,ly)){
            QMap<QString,motion_unit> expression;
            QRC_Manager::instance().MotionQuery(MOTION_HITAREAMOTION,hitarea,expression);

            for (QString key:expression.keys()) {
                if(expression[key].type == EXPRESSION){
                    LAppLive2DManager::GetInstance()->GetModel(0)->SetExpression(expression[key].name.toUtf8());
                }else if(expression[key].type == MOTION){
                    QStringList parts = expression[key].name.split('_');
                    QString group = parts[0];               // 前面：字符串
                    int no = parts[1].toInt();    // 后面：转int
                    LAppLive2DManager::GetInstance()->GetModel(0)->StartMotion(group.toUtf8(),no,1);
                }
            }
        }
    };



}






void MyOpenGL::wheelEvent(QWheelEvent *event)
{

}

const int MyOpenGL::invertFPS(int mode)
{
    int FPS[] = {30,60,120};
    return FPS[mode];
}







void MyOpenGL::modelupdate(){







}

void MyOpenGL::StartRender()
{


    if (m_trackMode == TrackMode_Face) {
        // 人脸模式已在 setTrackMode 中启动
    }

    t_modelupdate->start((1.0/invertFPS(ConfigManager::instance().getValue(CONFIG_APP_FPS).toInt()))*1000);
    m_isrunning = true;


}

void MyOpenGL::StopRender()
{
    t_modelupdate->stop();
    m_isrunning = false;
    if (m_trackMode == TrackMode_Face) {
        stopFaceTracking();
    }

}


void MyOpenGL::ChangeModel(){
    //LAppLive2DManager::GetInstance()->ChangeSceneNew();
}

void MyOpenGL::setBackgroundImage(const QString &path)
{
    makeCurrent();
    LAppDelegate::GetInstance()->GetView()->SetBackgroundImage(path.toStdString());
    update();
}

void MyOpenGL::setModelScale(float scale)
{
    m_modelScale = qBound(0.1f, scale, 3.0f);
    makeCurrent();
    LAppView* view = LAppDelegate::GetInstance()->GetView();
    if (view) {
        Csm::CubismViewMatrix* viewMatrix = view->GetViewMatrix();
        if (viewMatrix) {
            viewMatrix->Scale(m_modelScale, m_modelScale);
        }
    }
    update();
}

void MyOpenGL::setModelOpacity(float opacity)
{
    m_modelOpacity = qBound(0.0f, opacity, 1.0f);
    // 透明度通过 LAppView::GetSpriteAlpha 或模型渲染时控制
    // 可以在 paintGL 或 LAppView 中使用 m_modelOpacity
    update();
}

float MyOpenGL::getModelScale() const
{
    return m_modelScale;
}

float MyOpenGL::getModelOpacity() const
{
    return m_modelOpacity;
}

void MyOpenGL::setTrackMode(TrackMode mode)
{
    // 如果切换到人脸模式，启动相机和检测线程
    if (mode == TrackMode_Face) {
        startFaceTracking();
    } else {
        // 如果之前是人脸模式，停止相机和检测线程
        if (m_trackMode == TrackMode_Face) {
            stopFaceTracking();
        }
    }

    m_trackMode = mode;

    // 更新按钮选中状态
    for (QPushButton *btn : m_modeButtons) {
        TrackMode btnMode = static_cast<TrackMode>(btn->property("trackMode").toInt());
        btn->blockSignals(true);
        btn->setChecked(btnMode == mode);
        btn->blockSignals(false);
    }

    emit trackModeChanged(m_trackMode);

    QString modeStr;
    switch (mode) {
    case TrackMode_None: modeStr = "None"; break;
    case TrackMode_Mouse: modeStr = "Mouse"; break;
    case TrackMode_Face: modeStr = "Face"; break;
    case TrackMode_AI: modeStr = "AI"; break;
    }
    qDebug() << "Track mode set to:" << modeStr;

    update();
}

void MyOpenGL::startFaceTracking()
{
    // 使用 CameraManager 启动相机和检测线程
    CameraManager::instance().OpenCamera(0);

    qDebug() << "Face tracking started";
}

void MyOpenGL::stopFaceTracking()
{
    // 使用 CameraManager 停止相机和检测线程
    CameraManager::instance().StopCamera(CameraManager::instance().GetNowHandle());
    qDebug() << "Face tracking stopped";
}










