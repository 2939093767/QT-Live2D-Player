#ifndef FACECAPSETTINGWINDOW_H
#define FACECAPSETTINGWINDOW_H

#include <QWidget>
#include <QImage>
#include <QRect>

QT_BEGIN_NAMESPACE
namespace Ui { class FaceCapSettingWindow; }
QT_END_NAMESPACE

class ImageCropping;
class CameraPreview;

class FaceCapSettingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FaceCapSettingWindow(QWidget *parent = nullptr);
    ~FaceCapSettingWindow();

signals:
    void cameraChanged(int index);
    void refreshCameraRequested();
    void resolutionChanged(int index);
    void fpsChanged(int index);
    void previewToggled(bool opened);
    void sensitivityChanged(int value);
    void smoothnessChanged(int value);
    void headTrackToggled(bool enabled);
    void eyeTrackToggled(bool enabled);
    void mouthTrackToggled(bool enabled);
    void blinkToggled(bool enabled);
    void expressionToggled(bool enabled);
    void cropOptionsRequested();
    void cropRectChanged(const QRect &rect);
    void accepted();
    void rejected();

public slots:
    void onCameraComboBoxChanged(int index);
    void onRefreshCameraClicked();
    void onResolutionComboBoxChanged(int index);
    void onFpsComboBoxChanged(int index);
    void onPreviewClicked();
    void onSensitivitySliderChanged(int value);
    void onSensitivitySpinBoxChanged(int value);
    void onSmoothnessSliderChanged(int value);
    void onSmoothnessSpinBoxChanged(int value);
    void onHeadTrackToggled(bool checked);
    void onEyeTrackToggled(bool checked);
    void onMouthTrackToggled(bool checked);
    void onBlinkToggled(bool checked);
    void onExpressionToggled(bool checked);
    void onCropOptionsClicked();
    void onResetClicked();
    void onCancelClicked();
    void onOkClicked();

private:
    Ui::FaceCapSettingWindow *ui;
    bool m_previewOpened = false;
    ImageCropping *m_imageCropping = nullptr;
    CameraPreview *m_cameraPreview = nullptr;
};

#endif // FACECAPSETTINGWINDOW_H
