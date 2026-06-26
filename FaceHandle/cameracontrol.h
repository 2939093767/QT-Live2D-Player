#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

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
#include <QMediaCaptureSession>
#include <QThread>
#include <QVideoSink>


#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <ConfigManager/qrc_manager.h>
#include <ConfigManager/configini.h>
#include <FaceHandle/imagecropping.h>

#include <LLamaAIControl/llamacontrol.h>
#include "facedetector.h"

#include <windows.h>
// #include <dshow.h>
#include <iostream>

/***************************************************************
*  @FileName:   cameracontrol.h
*  @Brief:      相机管理器
*  @Author:     LH
*  @Date:       2026-05-25
*  @note:
****************************************************************/
class CameraManager:public QObject
{
    Q_OBJECT
public:

    static CameraManager& instance(){
        static CameraManager m_instance;
        return m_instance;
    }
    // 禁止拷贝构造
    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;

    void UpdateDevice();
    QStringList EnumAllCameras();
    //相机运行相关
    int OpenCamera(int index);
    void StopCamera(int handle);
    QImage GetImageFrame(int handle);
    const int GetNowHandle(){return handle;};



    static void test_llama();
private:
    CameraManager(QWidget *parent = nullptr);
    ~CameraManager();

    QList<QCameraDevice> availableCameras;

    QMap<int,QImage> p_images;
    QMutex m_mutex;
    int handle;
};





/***************************************************************
*  @FileName:   cameracontrol.h
*  @Brief:      面部捕捉线程（单例）
*  @Author:     LH
*  @Date:       2026-05-26
*  @note:
****************************************************************/


class CameraOpen:public QThread
{
    Q_OBJECT
    enum RunState{
        Stop =0,
        waitting,
        Running
    };

public:
    static CameraOpen& instance(){
        static CameraOpen m_instance;
        if(!m_instance.isRunning()){
            m_instance.start();
        }
        return m_instance;
    }
    // 禁止拷贝构造
    CameraOpen(const CameraOpen&) = delete;
    CameraOpen& operator=(const CameraOpen&) = delete;

    void run()override;
    void startCamera(QCameraDevice *device);
    void stopCamera();

    QImage GetNowImage();

private slots:
    void onFrameAvailable(const QVideoFrame &frame);

signals:
    void ImageSend();

private:
    CameraOpen(QObject *parent=nullptr);
    ~CameraOpen();
    //图像采集相关
    RunState flag = Running;
    QCamera* p_camera;
    QMediaCaptureSession* session;
    QVideoSink* m_videoSink;
    //数据保存相关
    QImage now_image ;
    QMutex m_mutex;

};






#endif // CAMERACONTROL_H
