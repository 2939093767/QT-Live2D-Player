#include "cameracontrol.h"

/***************************************************************
*  @FileName:   cameracontrol.cpp
*  @Brief:      相机控制
*  @Author:     LH
*  @Date:       2026-05-24
*  @note:
****************************************************************/
CameraManager::CameraManager(QWidget *parent)
{
    UpdateDevice();
}

CameraManager::~CameraManager()
{

}




//更新相机信息
void CameraManager::UpdateDevice()
{
    availableCameras = QMediaDevices::videoInputs();
}





QStringList CameraManager::EnumAllCameras() {
    if (availableCameras.isEmpty()) {
        qDebug() << "错误：未检测到任何可用的摄像头设备。";
        return QStringList();
    } else {
        qDebug() << "检测到以下摄像头：";
        QStringList devicename;
        for (const QCameraDevice &device : availableCameras) {
            qDebug() << "  名称：" << device.description();
            devicename.append(device.description());
        }
        return devicename;
    }
}


int CameraManager::OpenCamera(int index)
{

    CameraOpen::instance().startCamera(&availableCameras[index]);
    FaceDetectorThread::instance().starthandle();
    qDebug()<<"开始";
    return handle;
}


void CameraManager::StopCamera(int handle)
{
    CameraOpen::instance().stopCamera();
    FaceDetectorThread::instance().stophandle();
}






QImage CameraManager::GetImageFrame(int handle)
{
    QMutexLocker locker(&m_mutex);
    QImage img = FaceDetectorThread::instance().GetImage();
    if (img.isNull()) {
        qDebug() << "警告：图片未准备好 ->" << handle;
        return QImage();
    }
    qDebug() << "Send image success";
    return img.copy();
}









CameraWorker::CameraWorker(QMutex *mutex, QImage *sharedImage, QObject *parent)
    : QObject(parent)
    , p_camera(nullptr)
    , session(nullptr)
    , m_videoSink(nullptr)
    , m_mutex(mutex)
    , m_nowImage(sharedImage)
{
}

CameraWorker::~CameraWorker()
{
    if (m_videoSink) { delete m_videoSink; m_videoSink = nullptr; }
    if (p_camera) { delete p_camera; p_camera = nullptr; }
    if (session) { delete session; session = nullptr; }
}

void CameraWorker::onStartCamera(QCameraDevice device)
{
    onStopCamera();
    p_camera = new QCamera();
    session = new QMediaCaptureSession();
    m_videoSink = new QVideoSink();
    connect(m_videoSink, &QVideoSink::videoFrameChanged,
            this, &CameraWorker::onFrameAvailable);
    p_camera->setCameraDevice(device);
    session->setCamera(p_camera);
    session->setVideoSink(m_videoSink);
    p_camera->start();
    flag = Running;
}

void CameraWorker::onStopCamera()
{
    flag = waitting;
    if (p_camera) {
        p_camera->stop();
        delete p_camera;
        p_camera = nullptr;
    }
    if (m_videoSink) {
        delete m_videoSink;
        m_videoSink = nullptr;
    }
    if (session) {
        delete session;
        session = nullptr;
    }
}

void CameraWorker::onFrameAvailable(const QVideoFrame &frame)
{
    if (!frame.isValid()) return;
    if (m_mutex && m_nowImage) {
        QMutexLocker locker(m_mutex);
        *m_nowImage = frame.toImage().copy();
    }
    emit ImageSend();
}

CameraOpen::~CameraOpen()
{
    quit();
    wait();
}


void CameraOpen::run()
{
    CameraWorker worker(&m_mutex, &now_image);
    connect(this, &CameraOpen::sigStartCamera, &worker, &CameraWorker::onStartCamera);
    connect(this, &CameraOpen::sigStopCamera, &worker, &CameraWorker::onStopCamera);
    connect(&worker, &CameraWorker::ImageSend, this, &CameraOpen::ImageSend);
    m_ready.storeRelease(1);
    qDebug() << "相机线程已启动";
    exec();
}

void CameraOpen::startCamera(QCameraDevice* device)
{
    while (!m_ready.loadAcquire()) {
        msleep(5);
    }
    if (device) {
        emit sigStartCamera(*device);
    }
}

void CameraOpen::stopCamera()
{
    if (!m_ready.loadAcquire()) return;
    emit sigStopCamera();
}

QImage CameraOpen::GetNowImage()
{
    QMutexLocker locker(&m_mutex);
    return now_image.copy();
}

CameraOpen::CameraOpen(QObject *parent)
    : QThread(parent)
{
    start();
    setPriority(QThread::LowPriority);
}

