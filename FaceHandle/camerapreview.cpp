#include "camerapreview.h"
#include "ui_camerapreview.h"
#include "FaceHandle/facedetector.h"
#include <QCloseEvent>
#include <QDebug>

CameraPreview::CameraPreview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CameraPreview)
    , m_updateTimer(new QTimer(this))
{
    ui->setupUi(this);
    m_updateTimer->setInterval(33);
    connect(m_updateTimer, &QTimer::timeout, this, &CameraPreview::onUpdateTimer);
}

CameraPreview::~CameraPreview()
{
    stopPreview();
    delete ui;
}

void CameraPreview::startPreview()
{
    if (m_isPreviewing) return;
    m_isPreviewing = true;
    m_updateTimer->start();
    ui->label_preview->setText("正在加载摄像头...");
}

void CameraPreview::stopPreview()
{
    if (!m_isPreviewing) return;
    m_isPreviewing = false;
    m_updateTimer->stop();
    ui->label_preview->clear();
    ui->label_preview->setText("摄像头未开启");
}

void CameraPreview::closeEvent(QCloseEvent *event)
{
    stopPreview();
    event->accept();
}

void CameraPreview::onUpdateTimer()
{
    QImage img = FaceDetectorThread::instance().GetImage();

    if (img.isNull()){
        qDebug()<<"没有";
        return;
    }

    // 应用裁剪
    QImage displayImg;
    if (m_cropRect.isNull()) {
        displayImg = img;
    } else {
        // 确保裁剪区域不超出图像范围
        QRect validRect = m_cropRect.intersected(QRect(0, 0, img.width(), img.height()));
        if (validRect.isEmpty()) {
            return;
        }
        displayImg = img.copy(validRect);
    }

    QPixmap pixmap = QPixmap::fromImage(displayImg).scaled(
        ui->label_preview->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );
    ui->label_preview->setPixmap(pixmap);
}
