#ifndef CONFIGINI_H
#define CONFIGINI_H

#include <QObject>
#include <QSettings>
#include <ConfigManager/qrc_manager.h>
#include <QString>
#include <QVariant>
#include <QWidget>
#include <QWinEventNotifier>
#include <qapplication.h>
#include <windows.h>



//APP设置相关
#define CONFIG_APP_FPS "Model/fps"
#define CONFIG_APP_SOFT_START "Window/soft_start"
#define CONFIG_APP_ISOPACITY "Window/isopacity"
#define CONFIG_APP_MODEL_CONTROL "Window/model_control"
#define CONFIG_MODEL_FOLDER "Model/folder"

//模型面捕相关设置
#define CONFIG_MOTION_SENSITIVITY "Motion/Sensitivity"
#define CONFIG_MOTION_OFFSET "Motion/Offset"
#define CONFIG_MOTION_SMOOTH_PARAMTER "Motion/Smooth"
#define CONFIG_MOTION_IMAGE_SIZE "Image/size"
#define CONFIG_IMAGE_CROP_RECT "Image/crop_rect"



//设置文件目录
#define CONFIG_FILE_PATH "config.ini"


//APP当前模式
enum APP_MODE{
    PET_MODE = 0,
    WINDOW_MODE = 1
};






/**
 * @brief 配置管理类
 * 负责应用配置的读取、保存与默认值管理
 * 配置文件存储路径：程序运行目录/config.ini
 */

class ConfigManager:public QObject
{
    Q_OBJECT
public:
    // 单例模式（全局唯一访问点）
    static ConfigManager& instance();

    // 禁止拷贝构造
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    //获取值
    QVariant getValue(const QString& key, const QVariant& defaultValue = QVariant()) const;
    //设置值
    void setValue(const QString& key, const QVariant& value);
    //同步
    void sync();
    //默认值初始化
    void initDefaultConfig();
    void FaceConfigDefault();
    void KeyConfigDefault();
    void setAutoStart(bool is);
private:
    // 私有构造函数（单例）
    ConfigManager();
    QSettings m_settings; // Qt 内置配置类

};






class GlobalKeyHook : public QObject
{
    Q_OBJECT
public:
    static GlobalKeyHook& instance(){
        static GlobalKeyHook m_instance;
        return m_instance;
    }
    explicit GlobalKeyHook(QObject *parent = nullptr);
    ~GlobalKeyHook();

    // 启动 / 停止监听
    void start();
    void stop();
    static QString vkCodeToQString(int vkCode);

signals:
    // 全局按键信号：按键码、Ctrl、Alt、Shift
    void keyPressed(int vkCode, bool ctrl, bool alt, bool shift);

private:
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    HHOOK m_keyboardHook;
};







// 工具类
class ProgramUtils
{
public:
    //动作字符串处理
    static void StartMotionByString(QString name);
    static bool Model3jsonfile_check(QString path);
    //键盘热键处理
    static bool parseHotKeyString(const QString &hotKeyStr, int &modifier,int &keyCode);
    static int parseModifier(const QString &mod);
    static int parseKeyCode(const QString &key);
};



#endif // CONFIGINI_H
