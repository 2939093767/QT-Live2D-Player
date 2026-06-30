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
#include <QAtomicInt>
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

public:
    static CameraOpen& instance(){
        static CameraOpen m_instance;
        if(!m_instance.isRunning()){
            m_instance.start();
        }
        return m_instance;
    }
    CameraOpen(const CameraOpen&) = delete;
    CameraOpen& operator=(const CameraOpen&) = delete;

    void run()override;
    void startCamera(QCameraDevice *device);
    void stopCamera();

    QImage GetNowImage();

signals:
    void ImageSend();
    void sigStartCamera(QCameraDevice device);
    void sigStopCamera();

private:
    CameraOpen(QObject *parent=nullptr);
    ~CameraOpen();

    QMutex m_mutex;
    QImage now_image;
    QAtomicInt m_ready = 0;
};

class CameraWorker : public QObject
{
    Q_OBJECT
    Q_ENUMS(RunState)
public:
    enum RunState{
        Stop = 0,
        waitting,
        Running
    };

    explicit CameraWorker(QMutex *mutex, QImage *sharedImage, QObject *parent = nullptr);
    ~CameraWorker();

public slots:
    void onFrameAvailable(const QVideoFrame &frame);
    void onStartCamera(QCameraDevice device);
    void onStopCamera();

signals:
    void ImageSend();

private:
    RunState flag = Running;
    QCamera* p_camera;
    QMediaCaptureSession* session;
    QVideoSink* m_videoSink;
    QMutex* m_mutex;
    QImage* m_nowImage;
};






#endif // CAMERACONTROL_H
