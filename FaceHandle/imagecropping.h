#ifndef IMAGECROPPING_H
#define IMAGECROPPING_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QImage>


namespace Ui {
class ImageCropping;
}

class ImageCropping : public QWidget
{
    Q_OBJECT

public:
    explicit ImageCropping(QWidget *parent = nullptr);
    ~ImageCropping();
    void setImage(QImage img);
    void reset();
private:
    void closeEvent(QCloseEvent* event)override;
    void showEvent(QShowEvent* event)override;

public slots:
    void SaveRect();
    void ChangeInfo();

signals:
    void IsUseRect(bool is);

private:
    Ui::ImageCropping *ui;
    bool is_used;
};








class CropWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CropWidget(QWidget *parent = nullptr);

    // 设置要裁剪的图像
    void setImage(const QImage &img);
    // 获取裁剪后的图像
    QImage getCroppedImage();

    // 获取裁剪区域坐标
    QRect getCropRect();
    void reset();
signals:
    void CropChange();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QImage m_image;       // 原始图像
    QRect m_cropRect;     // 裁剪区域
    QPoint m_startPoint;  // 按下起点
    bool m_isSelecting;   // 是否正在拖动选择

    double scaleX,scaleY;

};





#endif // IMAGECROPPING_H
