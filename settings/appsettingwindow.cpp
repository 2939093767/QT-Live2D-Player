#include "appsettingwindow.h"
#include "ui_appsettingwindow.h"
#include "ConfigManager/configini.h"
#include "utils/apputils.h"
#include "LAppLive2DManager.hpp"
#include <QFileDialog>
#include <QDir>
#include <QDebug>

AppSettingWindow::AppSettingWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AppSettingWindow)
    , m_glWidget(nullptr)
{
    ui->setupUi(this);

    // 从配置读取初始值（先阻塞信号）
    ui->checkBox_autoStart->blockSignals(true);
    ui->checkBox_autoStart->setChecked(ConfigManager::instance().getValue(CONFIG_APP_AUTO_START, false).toBool());
    ui->checkBox_autoStart->blockSignals(false);

    ui->comboBox_theme->blockSignals(true);
    ui->comboBox_theme->setCurrentIndex(ConfigManager::instance().getValue(CONFIG_APP_THEME, 1).toInt());
    ui->comboBox_theme->blockSignals(false);

    ui->comboBox_lang->blockSignals(true);
    ui->comboBox_lang->setCurrentIndex(ConfigManager::instance().getValue(CONFIG_APP_LANGUAGE, 0).toInt());
    ui->comboBox_lang->blockSignals(false);

    ui->comboBox_mode->blockSignals(true);
    ui->comboBox_mode->setCurrentIndex(ConfigManager::instance().getValue(CONFIG_APP_MODE, 0).toInt());
    ui->comboBox_mode->blockSignals(false);

    ui->lineEdit_bgImage->blockSignals(true);
    ui->lineEdit_bgImage->setText(ConfigManager::instance().getValue(CONFIG_BG_IMAGE, "").toString());
    ui->lineEdit_bgImage->blockSignals(false);

    int scaleVal = ConfigManager::instance().getValue(CONFIG_MODEL_SCALE, 100).toInt();
    ui->horizontalSlider_scale->blockSignals(true);
    ui->horizontalSlider_scale->setValue(scaleVal);
    ui->horizontalSlider_scale->blockSignals(false);
    ui->spinBox_scale->blockSignals(true);
    ui->spinBox_scale->setValue(scaleVal);
    ui->spinBox_scale->blockSignals(false);

    int opacityVal = ConfigManager::instance().getValue(CONFIG_MODEL_OPACITY, 100).toInt();
    ui->horizontalSlider_opacity->blockSignals(true);
    ui->horizontalSlider_opacity->setValue(opacityVal);
    ui->horizontalSlider_opacity->blockSignals(false);
    ui->spinBox_opacity->blockSignals(true);
    ui->spinBox_opacity->setValue(opacityVal);
    ui->spinBox_opacity->blockSignals(false);

    // 连接控件信号到槽函数（内部直接保存配置，不再发射信号）
    connect(ui->checkBox_autoStart, &QCheckBox::toggled, this, &AppSettingWindow::onAutoStartChanged);
    connect(ui->comboBox_theme, &QComboBox::currentIndexChanged, this, &AppSettingWindow::onThemeChanged);
    connect(ui->comboBox_lang, &QComboBox::currentIndexChanged, this, &AppSettingWindow::onLanguageChanged);
    connect(ui->comboBox_mode, &QComboBox::currentIndexChanged, this, &AppSettingWindow::onModeChanged);
    connect(ui->lineEdit_bgImage, &QLineEdit::textChanged, this, &AppSettingWindow::onBgImageChanged);
    connect(ui->horizontalSlider_scale, &QSlider::valueChanged, this, &AppSettingWindow::onScaleChanged);
    connect(ui->spinBox_scale, QOverload<int>::of(&QSpinBox::valueChanged), this, &AppSettingWindow::onScaleChanged);
    connect(ui->horizontalSlider_opacity, &QSlider::valueChanged, this, &AppSettingWindow::onOpacityChanged);
    connect(ui->spinBox_opacity, QOverload<int>::of(&QSpinBox::valueChanged), this, &AppSettingWindow::onOpacityChanged);
    connect(ui->comboBox_model, &QComboBox::currentIndexChanged, this, &AppSettingWindow::onModelIndexChanged);
    connect(ui->pushButton_loadModel, &QPushButton::clicked, this, &AppSettingWindow::onLoadModelClicked);
    connect(ui->pushButton_importModel, &QPushButton::clicked, this, &AppSettingWindow::onImportModelClicked);
    connect(ui->pushButton_browseBg, &QPushButton::clicked, this, &AppSettingWindow::onBrowseBgClicked);

    // 初始化模型列表
    //refreshModelList();
}

AppSettingWindow::~AppSettingWindow()
{
    delete ui;
}

void AppSettingWindow::setOpenGLWidget(void *glWidget)
{
    m_glWidget = glWidget;
}

void AppSettingWindow::saveConfig(const QString &key, const QVariant &value)
{
    ConfigManager::instance().setValue(key, value);
    ConfigManager::instance().sync();
}

void AppSettingWindow::syncSliderSpinBox(QSlider *slider, QSpinBox *spinBox, int value)
{
    if (slider && spinBox) {
        slider->blockSignals(true);
        slider->setValue(value);
        slider->blockSignals(false);
        spinBox->blockSignals(true);
        spinBox->setValue(value);
        spinBox->blockSignals(false);
    }
}

void AppSettingWindow::onAutoStartChanged(bool checked)
{
    saveConfig(CONFIG_APP_AUTO_START, checked);
    ConfigManager::instance().setAutoStart(checked);
    qDebug() << "Auto start changed:" << checked;
}

void AppSettingWindow::onThemeChanged(int index)
{
    saveConfig(CONFIG_APP_THEME, index);
    AppUtils::instance().setTheme(index);
    qDebug() << "Theme changed:" << index;
}

void AppSettingWindow::onLanguageChanged(int index)
{
    saveConfig(CONFIG_APP_LANGUAGE, index);
    AppUtils::instance().setLanguage(index);
    qDebug() << "Language changed:" << index;
}

void AppSettingWindow::onModeChanged(int index)
{
    saveConfig(CONFIG_APP_MODE, index);
    qDebug() << "Mode changed:" << index;
}

void AppSettingWindow::onBgImageChanged(const QString &text)
{
    saveConfig(CONFIG_BG_IMAGE, text);
    qDebug() << "Background image changed:" << text;
}

void AppSettingWindow::onScaleChanged(int value)
{
    syncSliderSpinBox(ui->horizontalSlider_scale, ui->spinBox_scale, value);
    saveConfig(CONFIG_MODEL_SCALE, value);
    qDebug() << "Scale changed:" << value;
}

void AppSettingWindow::onOpacityChanged(int value)
{
    syncSliderSpinBox(ui->horizontalSlider_opacity, ui->spinBox_opacity, value);
    saveConfig(CONFIG_MODEL_OPACITY, value);
    qDebug() << "Opacity changed:" << value;
}

void AppSettingWindow::onModelIndexChanged(int index)
{
    saveConfig(CONFIG_MODEL_INDEX, index);
    qDebug() << "Model index changed:" << index;
}

void AppSettingWindow::onLoadModelClicked()
{
    int index = ui->comboBox_model->currentIndex();
    if (index < 0) {
        qDebug() << "No model selected";
        return;
    }
    saveConfig(CONFIG_MODEL_FOLDER, ui->comboBox_model->itemText(index));
    //saveConfig(CONFIG_MODEL_INDEX, index);
    LAppLive2DManager::GetInstance()->ChangeScene(index);
    qDebug() << "Load model:" << ui->comboBox_model->itemText(index) << "index:" << index;
}


void AppSettingWindow::onImportModelClicked()
{
    QString startDir = ConfigManager::instance().getValue(CONFIG_MODEL_FOLDER, "").toString();
    if (startDir.isEmpty()) {
        startDir = QDir::currentPath();
    }

    QString modelFolder = QFileDialog::getExistingDirectory(
        this,
        "选择模型文件夹（包含 .model3.json 的目录）",
        startDir
    );

    if (modelFolder.isEmpty())
        return;

    LAppLive2DManager::GetInstance()->Add_SetUpModel(modelFolder);
    refreshModelList();
    qDebug() << "Model folder set:" << modelFolder;

}

void AppSettingWindow::onBrowseBgClicked()
{
    QString path = QFileDialog::getOpenFileName(this, "选择背景图片", "", "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif);;所有文件 (*.*)");
    if (!path.isEmpty()) {
        ui->lineEdit_bgImage->setText(path);
    }
}

void AppSettingWindow::refreshModelList()
{
    Csm::csmVector<Csm::csmString> modelDirs = LAppLive2DManager::GetInstance()->GetModelDir();
    for(int i=0;i<modelDirs.GetSize();i++)
        qDebug()<<modelDirs[i].GetRawString();
    ui->comboBox_model->blockSignals(true);
    ui->comboBox_model->clear();

    for (Csm::csmUint32 i = 0; i < modelDirs.GetSize(); i++) {
        QString modelName = QString::fromUtf8(modelDirs[i].GetRawString());
        ui->comboBox_model->addItem(modelName);
    }

    int savedIndex = ConfigManager::instance().getValue(CONFIG_MODEL_INDEX, 0).toInt();
    if (savedIndex >= 0 && savedIndex < ui->comboBox_model->count()) {
        ui->comboBox_model->setCurrentIndex(savedIndex);
    } else if (ui->comboBox_model->count() > 0) {
        ui->comboBox_model->setCurrentIndex(0);
    }

    ui->comboBox_model->blockSignals(false);

    qDebug() << "Model list refreshed, count:" << ui->comboBox_model->count();
}
