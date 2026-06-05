#ifndef CONFIG_SETTING_H
#define CONFIG_SETTING_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QVariant>
#include <QWidget>
#include <QWinEventNotifier>
#include <qapplication.h>
#include <windows.h>
#include <ConfigManager/configini.h>

//配置名称
//APP设置相关
// #define CONFIG_APP_FPS "Model/fps"
// #define CONFIG_APP_SOFT_START "Window/soft_start"
// #define CONFIG_APP_MODEL_CONTROL "Window/model_control"
// //模型文件相关
// #define CONFIG_MODEL_FOLDER "Model/folder"





// #define CONFIG_WINDOW_W "Window/Width"
// #define CONFIG_WINDOW_H "Window/Height"
// #define CONFIG_SLEEP_USE "Window/Sleep"
// #define CONFIG_SLEEP_TIMER "Window/Sleep_timer"
// #define CONFIG_ACTION_IDLE "Action/idle"
// #define CONFIG_ACTION_SLEEP "Action/sleep"
// #define CONFIG_ACTION_CLICK "Action/click"
// #define CONFIG_ACTION_MOVING "Action/moving"
// #define CONFIG_ACTION_IDLE_LISTS "Action/idle_lists"



























/***************************************************************
*  @FileName:   config_setting.h
*  @Brief:      临时资源管制中心
*  @Author:     LH
*  @Date:       2026-05-10
*  @note:
****************************************************************/
// class QRC_Manager:public QObject{
//     Q_OBJECT
// public:
//     static QRC_Manager& instance(){
//         static QRC_Manager instance;
//         return instance;
//     }
//     // 禁止拷贝构造
//     QRC_Manager(const QRC_Manager&) = delete;
//     QRC_Manager& operator=(const QRC_Manager&) = delete;

//     //动作管理
//     void ClearManager();
//     void AddMotion(QString name, motion_unit unit){m_manager[name] = unit;}
//     void Motionisuse(QString name,bool use){m_manager[name].isuse = use;}

//     motion_unit GetALLMotion(QString name){return m_manager[name];}
//     QMap<QString,motion_unit> GetActiveExpression();
//     QMap<QString,motion_unit> GetActiveMotion();
//     QMap<QString,motion_unit> GetHitareaMotion(QString area);
//     QMap<QString,motion_unit> GetQuickKeyMotion(QString quickkey_name);




// private:
//     QRC_Manager(){

//     };
//     QMap<QString,motion_unit> m_manager;
//     Face_info now_Face;
//     Face_info last_Face;

// };













// class GlobalKeyHook : public QObject
// {
//     Q_OBJECT
// public:
//     static GlobalKeyHook& instance(){
//         static GlobalKeyHook m_instance;
//         return m_instance;
//     }
//     explicit GlobalKeyHook(QObject *parent = nullptr);
//     ~GlobalKeyHook();

//     // 启动 / 停止监听
//     void start();
//     void stop();
//     static QString vkCodeToQString(int vkCode);

// signals:
//     // 全局按键信号：按键码、Ctrl、Alt、Shift
//     void keyPressed(int vkCode, bool ctrl, bool alt, bool shift);

// private:
//     static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
//     HHOOK m_keyboardHook;
// };







// // 工具类
// class ProgramUtils
// {
// public:
//     //动作字符串处理
//     static void StartMotionByString(QString name);
//     static bool Model3jsonfile_check(QString path);
//     //键盘热键处理
//     static bool parseHotKeyString(const QString &hotKeyStr, int &modifier,int &keyCode);
//     static int parseModifier(const QString &mod);
//     static int parseKeyCode(const QString &key);
// };


#endif // CONFIG_SETTING_H
