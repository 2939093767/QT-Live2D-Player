#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H
#pragma once

#include "config_widget.h"

#include <QWidget>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QObject>
#include <QMouseEvent>
#include <QCamera>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QThread>
#include <QVideoSink>


#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <ConfigManager/qrc_manager.h>
#include <ConfigManager/configini.h>
#include <FaceHandle/imagecropping.h>

#include <llamacontrol.h>
#include <FaceHandle/cameracontrol.h>
#include "facedetector.h"
#include <windows.h>

#include <iostream>


#include <QMutex>
#include <QOpenGLVertexArrayObject>
#include <QReadWriteLock>




#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "oleaut32.lib")

using namespace std;


namespace Ui {
    class CameraControl;
}

class CameraOpen;


/***************************************************************
*  @FileName:   cameracontrol.h
*  @Brief:      总界面
*  @Author:     LH
*  @Date:       2026-06-02
*  @note:
****************************************************************/
class CameraControl : public config_card
{
    Q_OBJECT

public:
    explicit CameraControl(QWidget *parent = nullptr);
    ~CameraControl();
    void change_ui()override;
    void save_config()override;
    void return_default()override;
public slots:
    void ChangeOpenState(bool clicked);
    void ImageChange(int index);
    void MotionSmooth();
    void MotionSensitivty();

    void FrameCapture();
private:
    Ui::CameraControl *ui;
    int now_camera_handle;
    ImageCropping w_crop_image;
};




/***************************************************************
*  @FileName:   cameracontrol.h
*  @Brief:      相机显示
*  @Author:     LH
*  @Date:       2026-05-25
*  @note:
****************************************************************/
class ImageShow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    ImageShow(QWidget *parent = nullptr);
    ~ImageShow();

    void ChangeImage(int handle);
    void initShader();
    void initTexture();

    void StartRender();
    void StopRender();

protected:
    /**********************************************
* @brief         初始化gl
* @date          2026-05-03
**********************************************/
    void initializeGL() override;
    /**********************************************
* @brief         gl窗口更改
* @date          2026-05-03
**********************************************/
    void resizeGL(int w, int h) override;
    /**********************************************
* @brief         绘制函数
* @date          2026-05-03
**********************************************/
    void paintGL() override;

private:
    QTimer* t_modelupdate;

    bool f_running = false;
    int used_handle;



    QOpenGLShaderProgram *m_program;
    GLuint m_texture = 1;
    QOpenGLVertexArrayObject m_vao;
    QVector<QVector3D> vertices;
    QVector<QVector2D> texCoords;

};








#endif // CAMERACONTROL_H
