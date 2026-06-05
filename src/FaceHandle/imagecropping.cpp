#include "imagecropping.h"
#include "ui_imagecropping.h"
#include <ConfigManager/configini.h>

#include <QPainter>

ImageCropping::ImageCropping(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ImageCropping)
{
    ui->setupUi(this);
    connect(ui->widget,&CropWidget::CropChange,this,&ImageCropping::ChangeInfo);
    setFixedSize(width(), height());
    is_used = false;
}

ImageCropping::~ImageCropping()
{
    delete ui;
}

void ImageCropping::setImage(QImage img)
{
    ui->widget->setImage(img);
}

void ImageCropping::reset()
{
    ui->widget->reset();
}

void ImageCropping::closeEvent(QCloseEvent *event)
{
    releaseKeyboard();
    emit IsUseRect(is_used);
}

void ImageCropping::showEvent(QShowEvent *event)
{
    setFocusPolicy(Qt::StrongFocus);
    grabKeyboard();
}

void ImageCropping::SaveRect()
{
    QRect rect = ui->widget->getCropRect();
    if(!rect.isEmpty()){
        ConfigManager::instance().setValue(CONFIG_IMAGE_CROP_RECT,rect);
        is_used = true;
    }else{
        ConfigManager::instance().setValue(CONFIG_IMAGE_CROP_RECT,QRect());
    }
    close();
    //emit IsUseRect(use);
}

void ImageCropping::ChangeInfo()
{
    QRect rect = ui->widget->getCropRect();
    ui->label->setText(QString("x : %1").arg(rect.x()));
    ui->label_2->setText(QString("y : %1").arg(rect.y()));
    ui->label_3->setText(QString("width : %1").arg(rect.width()));
    ui->label_4->setText(QString("height : %1").arg(rect.height()));
}















CropWidget::CropWidget(QWidget *parent)
    : QWidget(parent)
    , m_isSelecting(false)
{
    setMouseTracking(true);
}

// 设置图像
void CropWidget::setImage(const QImage &img)
{
    m_image = img;
    // 计算缩放因子（图像原始宽度 → 显示宽度 的比例）

    update();

}

// 鼠标按下
void CropWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_startPoint = event->pos();
        m_isSelecting = true;
        m_cropRect = QRect(); // 重置选区
        update();
    }
}

// 鼠标拖动
void CropWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isSelecting) {
        m_cropRect = QRect(m_startPoint, event->pos()).normalized();
        update();
    }
}

// 鼠标松开
void CropWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isSelecting = false;
        update();
    }
}

// 绘制：图像 + 裁剪框
void CropWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // 背景灰色
    painter.fillRect(rect(), Qt::darkGray);
    // 绘制原图
    QImage scaledImg = m_image.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawImage(0, 0, scaledImg);
    scaleX = (double)width() / m_image.width();
    scaleY = (double)height() / m_image.height();

    // 绘制半透明遮罩
    if (!m_cropRect.isEmpty()) {
        QColor maskColor(0, 0, 0, 100);
        painter.fillRect(rect(), maskColor);
        // 露出裁剪区域
        painter.drawImage(m_cropRect, scaledImg, m_cropRect);
        // 画红色边框
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(m_cropRect);
    }
    emit CropChange();
}

// 获取裁剪后的图像
QImage CropWidget::getCroppedImage()
{
    if (m_cropRect.isEmpty() || m_image.isNull())
        return QImage();
    return m_image.copy(m_cropRect);
}

// 获取坐标
QRect CropWidget::getCropRect()
{
    QRect originalRect(
        m_cropRect.x() / scaleX,
        m_cropRect.y() / scaleY,
        m_cropRect.width()  / scaleX,
        m_cropRect.height() / scaleY
        );
    return originalRect;
}

void CropWidget::reset()
{
    m_cropRect = QRect();
}
