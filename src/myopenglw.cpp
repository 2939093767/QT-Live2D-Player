#include "myopenglw.h"

#include "FaceHandle/cameracontrol.h"
#include <QGuiApplication>
#include <QRandomGenerator>





MyOpenGL::MyOpenGL(QWidget *parent)
{

    t_modelupdate = new QTimer(this);
    connect(t_modelupdate,&QTimer::timeout,this,[=]{update();});
    //t_modelupdate->start((1.0/invertFPS(ConfigManager::instance().getValue(CONFIG_APP_FPS).toInt()))*1000);
}

MyOpenGL::~MyOpenGL()
{
    qApp->quit();
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

    //glEnable(GL_BLEND);
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
    LAppDelegate::GetInstance()->resize(w,h);
}



void MyOpenGL::paintGL()
{
    if(ConfigManager::instance().getValue(CONFIG_APP_MODEL_CONTROL).toInt() == 1){
        FaceRenderUpdate();
    }else if(ConfigManager::instance().getValue(CONFIG_APP_MODEL_CONTROL).toInt() == 0){
        KeyRenderUpdate();
    }
    if(m_isrunning)t_modelupdate->start((1.0/invertFPS(ConfigManager::instance().getValue(CONFIG_APP_FPS).toInt()))*1000);
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


    if(ConfigManager::instance().getValue(CONFIG_APP_MODEL_CONTROL).toInt() == 1){
        CameraOpen::instance();
        CameraManager::instance().OpenCamera(0);
        //FaceDetectorThread::instance().starthandle();
    }

    t_modelupdate->start((1.0/invertFPS(ConfigManager::instance().getValue(CONFIG_APP_FPS).toInt()))*1000);
    m_isrunning = true;


}

void MyOpenGL::StopRender()
{
    t_modelupdate->stop();
    m_isrunning = false;
    if(ConfigManager::instance().getValue(CONFIG_APP_MODEL_CONTROL).toInt() == 1){
        CameraManager::instance().StopCamera(0);
        FaceDetectorThread::instance().stophandle();
    }

}


void MyOpenGL::ChangeModel(){
    LAppLive2DManager::GetInstance()->ChangeSceneNew();
}













