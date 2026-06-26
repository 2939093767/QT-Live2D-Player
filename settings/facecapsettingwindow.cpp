#include "facecapsettingwindow.h"
#include "ui_facecapsettingwindow.h"
#include "FaceHandle/imagecropping.h"
#include <QDebug>

FaceCapSettingWindow::FaceCapSettingWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FaceCapSettingWindow)
{
    ui->setupUi(this);

    connect(ui->comboBox_camera, &QComboBox::currentIndexChanged, this, &FaceCapSettingWindow::onCameraComboBoxChanged);
    connect(ui->pushButton_refreshCam, &QPushButton::clicked, this, &FaceCapSettingWindow::onRefreshCameraClicked);
    connect(ui->comboBox_resolution, &QComboBox::currentIndexChanged, this, &FaceCapSettingWindow::onResolutionComboBoxChanged);
    connect(ui->comboBox_fps, &QComboBox::currentIndexChanged, this, &FaceCapSettingWindow::onFpsComboBoxChanged);
    connect(ui->pushButton_preview, &QPushButton::clicked, this, &FaceCapSettingWindow::onPreviewClicked);
    connect(ui->horizontalSlider_sensitivity, &QSlider::valueChanged, this, &FaceCapSettingWindow::onSensitivitySliderChanged);
    connect(ui->spinBox_sensitivity, QOverload<int>::of(&QSpinBox::valueChanged), this, &FaceCapSettingWindow::onSensitivitySpinBoxChanged);
    connect(ui->horizontalSlider_smooth, &QSlider::valueChanged, this, &FaceCapSettingWindow::onSmoothnessSliderChanged);
    connect(ui->spinBox_smooth, QOverload<int>::of(&QSpinBox::valueChanged), this, &FaceCapSettingWindow::onSmoothnessSpinBoxChanged);
    connect(ui->checkBox_headTrack, &QCheckBox::toggled, this, &FaceCapSettingWindow::onHeadTrackToggled);
    connect(ui->checkBox_eyeTrack, &QCheckBox::toggled, this, &FaceCapSettingWindow::onEyeTrackToggled);
    connect(ui->checkBox_mouthTrack, &QCheckBox::toggled, this, &FaceCapSettingWindow::onMouthTrackToggled);
    connect(ui->checkBox_blink, &QCheckBox::toggled, this, &FaceCapSettingWindow::onBlinkToggled);
    connect(ui->checkBox_expression, &QCheckBox::toggled, this, &FaceCapSettingWindow::onExpressionToggled);
    connect(ui->pushButton_cropOptions, &QPushButton::clicked, this, &FaceCapSettingWindow::onCropOptionsClicked);
}

FaceCapSettingWindow::~FaceCapSettingWindow()
{
    delete ui;
}

void FaceCapSettingWindow::onCameraComboBoxChanged(int index)
{
    emit cameraChanged(index);
}

void FaceCapSettingWindow::onRefreshCameraClicked()
{
    emit refreshCameraRequested();
}

void FaceCapSettingWindow::onResolutionComboBoxChanged(int index)
{
    emit resolutionChanged(index);
}

void FaceCapSettingWindow::onFpsComboBoxChanged(int index)
{
    emit fpsChanged(index);
}

void FaceCapSettingWindow::onPreviewClicked()
{
    m_previewOpened = !m_previewOpened;
    if (m_previewOpened) {
        ui->pushButton_preview->setText("关闭摄像头预览");
    } else {
        ui->pushButton_preview->setText("打开摄像头预览");
    }
    emit previewToggled(m_previewOpened);
}

void FaceCapSettingWindow::onSensitivitySliderChanged(int value)
{
    ui->spinBox_sensitivity->blockSignals(true);
    ui->spinBox_sensitivity->setValue(value);
    ui->spinBox_sensitivity->blockSignals(false);
    emit sensitivityChanged(value);
}

void FaceCapSettingWindow::onSensitivitySpinBoxChanged(int value)
{
    ui->horizontalSlider_sensitivity->blockSignals(true);
    ui->horizontalSlider_sensitivity->setValue(value);
    ui->horizontalSlider_sensitivity->blockSignals(false);
    emit sensitivityChanged(value);
}

void FaceCapSettingWindow::onSmoothnessSliderChanged(int value)
{
    ui->spinBox_smooth->blockSignals(true);
    ui->spinBox_smooth->setValue(value);
    ui->spinBox_smooth->blockSignals(false);
    emit smoothnessChanged(value);
}

void FaceCapSettingWindow::onSmoothnessSpinBoxChanged(int value)
{
    ui->horizontalSlider_smooth->blockSignals(true);
    ui->horizontalSlider_smooth->setValue(value);
    ui->horizontalSlider_smooth->blockSignals(false);
    emit smoothnessChanged(value);
}

void FaceCapSettingWindow::onHeadTrackToggled(bool checked)
{
    emit headTrackToggled(checked);
}

void FaceCapSettingWindow::onEyeTrackToggled(bool checked)
{
    emit eyeTrackToggled(checked);
}

void FaceCapSettingWindow::onMouthTrackToggled(bool checked)
{
    emit mouthTrackToggled(checked);
}

void FaceCapSettingWindow::onBlinkToggled(bool checked)
{
    emit blinkToggled(checked);
}

void FaceCapSettingWindow::onExpressionToggled(bool checked)
{
    emit expressionToggled(checked);
}

void FaceCapSettingWindow::onCropOptionsClicked()
{
    if (!m_imageCropping) {
        m_imageCropping = new ImageCropping();
        connect(m_imageCropping, &ImageCropping::IsUseRect, this, [](bool isUsed) {
            qDebug() << "Crop rect is used:" << isUsed;
        });
    }
    m_imageCropping->setImage(QImage());
    m_imageCropping->show();
}
