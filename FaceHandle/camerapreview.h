#ifndef CAMERAPREVIEW_H
#define CAMERAPREVIEW_H

#include <QWidget>
#include <QTimer>
#include <QImage>
#include <QRect>

QT_BEGIN_NAMESPACE
namespace Ui { class CameraPreview; }
QT_END_NAMESPACE

class CameraPreview : public QWidget
{
    Q_OBJECT

public:
    explicit CameraPreview(QWidget *parent = nullptr);
    ~CameraPreview();

    void startPreview();
    void stopPreview();

    // 设置裁剪区域（基于原图像坐标）
    void setCropRect(const QRect &rect) { m_cropRect = rect; }

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onUpdateTimer();

private:
    Ui::CameraPreview *ui;
    QTimer *m_updateTimer;
    bool m_isPreviewing = false;
    QRect m_cropRect;  // 裁剪区域，QRect() 表示不裁剪
};

#endif
