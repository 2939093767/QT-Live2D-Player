#ifndef APPSETTINGWINDOW_H
#define APPSETTINGWINDOW_H

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class AppSettingWindow; }
QT_END_NAMESPACE

class AppSettingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AppSettingWindow(QWidget *parent = nullptr);
    ~AppSettingWindow();

    // 公共接口：设置 OpenGL 窗口指针（用于实时应用配置）
    void setOpenGLWidget(void *glWidget);

private slots:
    // 通用控件值变化处理（内部直接保存配置）
    void onAutoStartChanged(bool checked);
    void onThemeChanged(int index);
    void onLanguageChanged(int index);
    void onModeChanged(int index);
    void onBgImageChanged(const QString &text);
    void onScaleChanged(int value);
    void onOpacityChanged(int value);
    void onModelIndexChanged(int index);

    // 按钮点击处理
    void onLoadModelClicked();
    void onImportModelClicked();
    void onBrowseBgClicked();

private:
    Ui::AppSettingWindow *ui;
    void *m_glWidget;  // OpenGL 窗口指针

    // 内部方法：同步保存配置并应用
    void saveConfig(const QString &key, const QVariant &value);
    void syncSliderSpinBox(QSlider *slider, QSpinBox *spinBox, int value);
    void refreshModelList();  // 刷新模型列表（从模型目录扫描）
};

#endif // APPSETTINGWINDOW_H
