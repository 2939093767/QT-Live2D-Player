#ifndef CONFIG_SETTING_H
#define CONFIG_SETTING_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QVariant>
#include <QWidget>
#include <qapplication.h>


//配置名称
//APP设置相关
#define CONFIG_APP_FPS "Model/fps"
#define CONFIG_APP_SOFT_START "Window/soft_start"
#define CONFIG_APP_MODEL_CONTROL "Window/model_control"
//模型文件相关
#define CONFIG_MODEL_FOLDER "Model/folder"





// #define CONFIG_WINDOW_W "Window/Width"
// #define CONFIG_WINDOW_H "Window/Height"
// #define CONFIG_SLEEP_USE "Window/Sleep"
// #define CONFIG_SLEEP_TIMER "Window/Sleep_timer"
// #define CONFIG_ACTION_IDLE "Action/idle"
// #define CONFIG_ACTION_SLEEP "Action/sleep"
// #define CONFIG_ACTION_CLICK "Action/click"
// #define CONFIG_ACTION_MOVING "Action/moving"
// #define CONFIG_ACTION_IDLE_LISTS "Action/idle_lists"




//APP当前模式
enum APP_MODE{
    PET_MODE = 0,
    WINDOW_MODE = 1
};

enum MOTION_TYPE{
    EXPRESSION = 0,
    MOTION = 1
};

struct motion_unit{
    QString name;
    MOTION_TYPE type;
    QString hitarea = "";
    int quickkey_id = 0;
    bool quickkey_isuse = false;
    bool isuse = true;
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

    QVariant getValue(const QString& key, const QVariant& defaultValue = QVariant()) const;

    void setValue(const QString& key, const QVariant& value);

    /**
     * @brief 同步保存配置到文件
     */
    void sync();

    /**
     * @brief 重置配置为默认值
     */
    //void resetToDefault();
    // 初始化默认配置
    void initDefaultConfig();
private:
    // 私有构造函数（单例）
    ConfigManager();
    QSettings m_settings; // Qt 内置配置类
};













/***************************************************************
*  @FileName:   config_setting.h
*  @Brief:      临时资源管制中心
*  @Author:     LH
*  @Date:       2026-05-10
*  @note:
****************************************************************/
class QRC_Manager:public QObject{
    Q_OBJECT
public:
    static QRC_Manager& instance(){
        static QRC_Manager instance;
        return instance;
    }
    // 禁止拷贝构造
    QRC_Manager(const QRC_Manager&) = delete;
    QRC_Manager& operator=(const QRC_Manager&) = delete;

    void SetupConfigByini();
    void SetupConfigByMoc();
    void ClearManager();
    void AddMotion(QString name, motion_unit unit){m_manager[name] = unit;}
    void Motionisuse(QString name,bool use){m_manager[name].isuse = use;}
    motion_unit GetALLMotion(QString name){return m_manager[name];}
    QMap<QString,motion_unit> GetActiveExpression();
    QMap<QString,motion_unit> GetActiveMotion();

    QMap<QString, motion_unit> GetHitareaMotion(QString area);
    QMap<QString, motion_unit> GetQuickKeyMotion(int quickkey_id);


private:
    QRC_Manager(){

    };

    QMap<QString,motion_unit> m_manager;

};



class GlobalHotKeyMgr : public QObject
{
    Q_OBJECT
public:
    // 单例
    explicit GlobalHotKeyMgr(QObject* parent = nullptr, QWidget* target = nullptr)
        :m_widget(target){}
    ~GlobalHotKeyMgr() override;
    // 无论App是否前台都有效
    int registerHotKey(int modifier, int keyCode);
    // 注销单个
    void unregisterHotKey(int id);
    // 注销全部
    void unregisterAll();


signals:
    // 热键触发（全局）
    void hotKeyTriggered(int id);

private:
    QWidget* m_widget;
    QMap<int, bool> m_ids;
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


#endif // CONFIG_SETTING_H
