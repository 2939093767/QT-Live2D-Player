#include "myopenglw.h"

#include <QGuiApplication>





MyOpenGL::MyOpenGL(QWidget *parent)
{

    t_modelupdate = new QTimer(this);
    connect(t_modelupdate,&QTimer::timeout,this,&MyOpenGL::modelupdate);
    t_modelupdate->start((1.0/invertFPS(ConfigManager::instance().getValue(CONFIG_APP_FPS).toInt()))*1000);
}

MyOpenGL::~MyOpenGL()
{



}

void MyOpenGL::initializeGL()
{
    try{
        ConfigManager::instance();

        LAppDelegate::GetInstance()->Initialize(this);
    }catch(const _exception& e){
        qDebug()<<"error";
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}

void MyOpenGL::resizeGL(int w, int h)
{
    LAppDelegate::GetInstance()->resize(w,h);


}



void MyOpenGL::paintGL()
{

    LAppDelegate::GetInstance()->update();



}




void MyOpenGL::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        qDebug()<<"pressing";


    }
}




void MyOpenGL::mouseMoveEvent(QMouseEvent *event)
{



}





void MyOpenGL::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint localPos = this->mapFromGlobal(QCursor::pos());
    int lx = localPos.x();
    int ly = localPos.y();
    auto Setting = LAppLive2DManager::GetInstance()->GetModel(0)->GetSetting();
    for(int i=0;i < Setting->GetHitAreasCount();i++){
        auto hitarea = Setting->GetHitAreaName(i);
        if(LAppDelegate::GetInstance()->GetView()->OnTouchesIsHit(hitarea,lx,ly)){
            auto expression = QRC_Manager::instance().GetActiveExpression();
            for (QString key:expression.keys()) {
                qDebug()<< expression[key].hitarea;
                if(expression[key].hitarea == hitarea){
                    LAppLive2DManager::GetInstance()->GetModel(0)->SetExpression(expression[key].name.toUtf8());
                    break;
                }
            }

            auto motion = QRC_Manager::instance().GetActiveMotion();
            for (QString key:motion.keys()) {
                qDebug()<< expression[key].hitarea;
                if(motion[key].hitarea == hitarea){
                    QStringList parts = motion[key].name.split('_');
                    QString group = parts[0];               // 前面：字符串
                    int no = parts[1].toInt();    // 后面：转int
                    LAppLive2DManager::GetInstance()->GetModel(0)->StartMotion(group.toUtf8(),no,1);
                    break;
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

    update();
    t_modelupdate->start((1.0/invertFPS(ConfigManager::instance().getValue(CONFIG_APP_FPS).toInt()))*1000);
}















