#include "camerawidget.h"
#include "ui_camerawidget.h"

#include <QMediaCaptureSession>
#include <QMessageBox>
#include <QMutexLocker>
#include <QTimer>




/***************************************************************
*  @FileName:   cameracontrol.cpp
*  @Brief:      相机设置界面
*  @Author:     LH
*  @Date:       2026-06-03
*  @note:
****************************************************************/
CameraControl::CameraControl(QWidget *parent)
    : config_card(parent)
    , ui(new Ui::CameraControl)
{
    ui->setupUi(this);
    ui->comboBox->addItems(CameraManager::instance().EnumAllCameras());



}

CameraControl::~CameraControl()
{
    delete ui;
}

void CameraControl::change_ui()
{
    ui->horizontalSlider->setValue((ConfigManager::instance().getValue(CONFIG_MOTION_SENSITIVITY).toDouble()+1)*50);
    ui->horizontalSlider_2->setValue((ConfigManager::instance().getValue(CONFIG_MOTION_SMOOTH_PARAMTER).toDouble())*100);
    ui->comboBox_2->setCurrentIndex(ConfigManager::instance().getValue(CONFIG_MOTION_IMAGE_SIZE).toInt());
}


void CameraControl::save_config()
{
    ConfigManager::instance().setValue(CONFIG_MOTION_IMAGE_SIZE,ui->comboBox_2->currentIndex());
    ConfigManager::instance().setValue(CONFIG_MOTION_SMOOTH_PARAMTER,ui->horizontalSlider_2->value()/100.0f);
    ConfigManager::instance().setValue(CONFIG_MOTION_SENSITIVITY,ui->horizontalSlider->value()/50.0f);
    w_crop_image.SaveRect();
}

void CameraControl::return_default()
{

}




void CameraControl::ChangeOpenState(bool clicked)
{
    if(clicked){

        ui->openGLWidget->StartRender();
    }else{
        ui->openGLWidget->StopRender();
    }
}

void CameraControl::ImageChange(int index)
{
    ConfigManager::instance().setValue(CONFIG_MOTION_IMAGE_SIZE,index);
}

void CameraControl::MotionSmooth()
{
    ConfigManager::instance().setValue(CONFIG_MOTION_SMOOTH_PARAMTER,ui->horizontalSlider_2->value()/100.0f);
}

void CameraControl::MotionSensitivty()
{
    ConfigManager::instance().setValue(CONFIG_MOTION_SENSITIVITY,ui->horizontalSlider->value()/50.0f);
}



void CameraControl::FrameCapture()
{
    QImage img = CameraOpen::instance().GetNowImage();
    if(img.isNull()){
        QMessageBox::warning(this, "错误", "无法获取xi图片，请检查后重试");
        return;
    }
    connect(&w_crop_image,&ImageCropping::IsUseRect,this,[=](bool is){
        if(is){
            ui->pushButton_3->setText("已启用");
        }else{
            ui->pushButton_3->setText("未启用");
        }
    });
    w_crop_image.setImage(img);
    w_crop_image.show();
    //setFocusPolicy(Qt::StrongFocus);
}






/***************************************************************
*  @FileName:   cameracontrol.cpp
*  @Brief:      opengl图像渲染
*  @Author:     LH
*  @Date:       2026-06-02
*  @note:
****************************************************************/
ImageShow::ImageShow(QWidget *parent)
{
    t_modelupdate = new QTimer(this);
    connect(t_modelupdate,&QTimer::timeout,this,[=]{
        update();
    });

}

ImageShow::~ImageShow()
{

}

void ImageShow::ChangeImage(int handle)
{
    used_handle = handle;
}




void ImageShow::initShader()
{
    const char *vertexShaderSrc = R"(
        attribute vec2 aPos;
        attribute vec2 aTex;
        varying vec2 vTex;
        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            vTex = aTex;
        }
    )";

    const char *fragmentShaderSrc = R"(
        varying vec2 vTex;
        uniform sampler2D uTexture;
        void main() {
            gl_FragColor = texture2D(uTexture, vTex);
        }
    )";

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc);
    m_program->bindAttributeLocation("aPos", 0);
    m_program->bindAttributeLocation("aTex", 1);
    m_program->link();
}




void ImageShow::initTexture()
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    // 设置纹理参数（固定值）
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // 设置像素行对齐，避免因图像宽度不是4的倍数导致错位
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void ImageShow::StartRender()
{
    t_modelupdate->start(1.0/60*1000);
    f_running = true;
}

void ImageShow::StopRender()
{
    t_modelupdate->stop();
    f_running = false;
}





void ImageShow::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    initShader();
    initTexture();
}


void ImageShow::resizeGL(int w, int h)
{
    qreal dpr = devicePixelRatioF();
    glViewport(0, 0, static_cast<GLsizei>(w * dpr), static_cast<GLsizei>(h * dpr));

}


void ImageShow::paintGL()
{
    QImage m_image = CameraManager::instance().GetImageFrame(0);

    qDebug()<<"显示刷新";
    if (m_image.isNull() || !f_running){
        return;
    }

    // qreal dpr = devicePixelRatioF();
    // glViewport(0, 0, static_cast<GLsizei>(width() * dpr), static_cast<GLsizei>(height() * dpr));

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    qDebug()<<"开始绘制";
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, m_texture);
    // 上传或更新纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 m_image.width(), m_image.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());


    m_program->bind();

    // 定义全屏四边形的顶点数据（NDC坐标 + 纹理坐标）
    // 注意：OpenGL 纹理原点在左下角，QImage 原点在左上角，所以纹理 V 坐标已翻转
    static const float vertices[] = {
        // posX, posY,  texU, texV
        -1.0f, -1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 1.0f,
        1.0f,  1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 0.0f
    };

    // 绑定顶点属性（假设 location 0: 位置, 1: 纹理坐标）
    int vertexSize = 4 * sizeof(float); // 4个float为一个顶点
    m_program->setAttributeArray(0, GL_FLOAT, vertices, 2, vertexSize);
    m_program->setAttributeArray(1, GL_FLOAT, vertices + 2, 2, vertexSize);
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // 清理状态
    m_program->disableAttributeArray(0);
    m_program->disableAttributeArray(1);
    m_program->release();
    glBindTexture(GL_TEXTURE_2D, 0);

}









