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









//相机运行线程
CameraOpen::~CameraOpen()
{
    flag = Stop;
    p_camera->deleteLater();
    m_videoSink->deleteLater();
    session->deleteLater();
    wait();       // 等待线程完全退出

}


void CameraOpen::run()
{
    qDebug() << "相机已启动，开始获取图像帧";
    while(true){
        if(flag == Running or flag == waitting){
            QThread::sleep(1000);
        }else if(flag == Stop){
            break;
        }
    }
    // ========================
    // 停止逻辑
    // ========================
    stopCamera();

}

void CameraOpen::startCamera(QCameraDevice* device)
{
    stopCamera();
    p_camera->setCameraDevice(*device);
    session->setCamera(p_camera);
    session->setVideoSink(m_videoSink);
    p_camera->start();
    flag = Running;
}



void CameraOpen::stopCamera()
{
    flag = waitting;
    p_camera->stop();

}

QImage CameraOpen::GetNowImage()
{
    QMutexLocker locker(&m_mutex);
    QImage copy = now_image.copy();
    return copy;
}


// 回调：每一帧图像都会来这里(只采集数据，处理放到面捕线程中）
void CameraOpen::onFrameAvailable(const QVideoFrame &frame)
{
    qDebug()<<"获取图像";
    if (!frame.isValid()) return;
    QMutexLocker locker(&m_mutex);
    now_image = frame.toImage().copy();  // 赋值
    emit ImageSend();

}

CameraOpen::CameraOpen(QObject *parent)
{
    session = new QMediaCaptureSession(this);
    p_camera = new QCamera(this);
    m_videoSink = new QVideoSink(this);
    connect(m_videoSink, &QVideoSink::videoFrameChanged,
            this, &CameraOpen::onFrameAvailable);

}

