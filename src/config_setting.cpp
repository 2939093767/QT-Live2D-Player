#include "config_setting.h"
#include "LAppLive2DManager.hpp"
#include "LAppModel.hpp"
#include <QDir>
#include <QCoreApplication>
#include <windows.h>
#include <qmessagebox.h>
#include <qevent.h>



// 配置文件路径（固定：程序目录下的 config.ini）
//#define CONFIG_FILE_PATH "config.ini"




//自启动功能注册
class AutoStart
{
public:
    // enable: 开启/关闭；appName: 注册表键名；appPath: 程序路径
    static bool set(bool enable,
                    const QString &appName = "MyQtTrayApp",
                    const QString &appPath = qApp->applicationFilePath())
    {
        QSettings settings(
            "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
            QSettings::NativeFormat
            );

        if (enable) {
            // 路径含空格会自动加引号
            settings.setValue(appName, QDir::toNativeSeparators(appPath));
        } else {
            settings.remove(appName);
        }
        return true;
    }

    // 判断是否已启用
    static bool isEnabled(
        const QString &appName = "MyQtTrayApp"
        ) {
        QSettings settings(
            "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
            QSettings::NativeFormat
            );
        return settings.contains(appName);
    }
};





























// void QRC_Manager::ClearManager()
// {
//     m_manager.clear();
// }

// QMap<QString, motion_unit> QRC_Manager::GetHitareaMotion(QString area)
// {
//     QMap<QString, motion_unit> resultMap;
//     // 遍历内部的总 map
//     for (auto it = m_manager.begin(); it != m_manager.end(); ++it)
//     {
//         const QString& key = it.key();
//         const motion_unit& unit = it.value();

//         // ====================== 筛选条件 ======================

//         if (unit.hitarea == area && unit.isuse && unit.quickkey_isuse)
//         {
//             resultMap.insert(key, unit);
//         }
//     }
//     return resultMap;
// }




// QMap<QString, motion_unit> QRC_Manager::GetQuickKeyMotion(QString quickkey_name)
// {
//     QMap<QString, motion_unit> resultMap;
//     // 遍历内部的总 map
//     for (auto it = m_manager.begin(); it != m_manager.end(); ++it)
//     {
//         const QString& key = it.key();
//         const motion_unit& unit = it.value();

//         // ====================== 筛选条件 ======================
//         // 提取 type == "expression" 的项
//         if (unit.quickkey == quickkey_name && unit.isuse && unit.quickkey_isuse)
//         {
//             resultMap.insert(key, unit);
//         }
//     }
//     return resultMap;
// }





// QMap<QString, motion_unit> QRC_Manager::GetActiveExpression()
// {
//     QMap<QString, motion_unit> resultMap;
//     // 遍历内部的总 map
//     for (auto it = m_manager.begin(); it != m_manager.end(); ++it)
//     {
//         const QString& key = it.key();
//         const motion_unit& unit = it.value();

//         // ====================== 筛选条件 ======================
//         // 提取 type == "expression" 的项
//         if (unit.type == EXPRESSION && unit.isuse)
//         {
//             resultMap.insert(key, unit);
//         }
//     }
//     return resultMap;
// }



// QMap<QString, motion_unit> QRC_Manager::GetActiveMotion()
// {
//     QMap<QString, motion_unit> resultMap;
//     // 遍历内部的总 map
//     for (auto it = m_manager.begin(); it != m_manager.end(); ++it)
//     {
//         const QString& key = it.key();
//         const motion_unit& unit = it.value() ;

//         // ====================== 筛选条件 ======================
//         // 提取 type == "expression" 的项
//         if (unit.type == MOTION && unit.isuse)
//         {
//             resultMap.insert(key, unit);
//         }
//     }
//     return resultMap;
// }











GlobalKeyHook::GlobalKeyHook(QObject *parent)
    : QObject{parent}, m_keyboardHook(nullptr)
{

}

GlobalKeyHook::~GlobalKeyHook()
{
    stop();
}

void GlobalKeyHook::start()
{
    if (m_keyboardHook) return;

    m_keyboardHook = SetWindowsHookEx(
        WH_KEYBOARD_LL,
        LowLevelKeyboardProc,
        GetModuleHandle(NULL),
        0
        );
}

void GlobalKeyHook::stop()
{
    if (m_keyboardHook) {
        UnhookWindowsHookEx(m_keyboardHook);
        m_keyboardHook = nullptr;
    }
}

// 全局底层键盘钩子（核心）
LRESULT CALLBACK GlobalKeyHook::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
    {
        qDebug()<<"pressing key";
        KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;
        int vkCode = pKeyBoard->vkCode;

        // 获取 Ctrl / Alt / Shift 状态
        // bool ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
        // bool alt  = (GetKeyState(VK_MENU) & 0x8000) != 0;
        // bool shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;


        auto motions = QRC_Manager::instance().MotionQuery(GetQuickKeyMotion(vkCodeToQString(vkCode));
        for(auto motion:motions){
            if(motion.type == EXPRESSION){
                LAppLive2DManager::GetInstance()->GetModel(0)->SetExpression(motion.name.toUtf8());
            }else if(motion.type == MOTION){
                QStringList parts = motion.name.split('_');
                QString group = parts[0];               // 前面：字符串
                int no = parts[1].toInt();    // 后面：转int
                qDebug()<<group<<no;
                LAppLive2DManager::GetInstance()->GetModel(0)->StartMotion(group.toUtf8(),no,1);
            }
        }
    }
    // 【关键】不拦截按键 → 不影响输入
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}




QString GlobalKeyHook::vkCodeToQString(int vkCode)
{
    // 处理 Windows 特殊虚拟键码
    switch (vkCode)
    {
    // 功能键
    case VK_RETURN:      return "Enter";
    case VK_ESCAPE:      return "Esc";
    case VK_TAB:         return "Tab";
    case VK_SPACE:       return "Space";
    case VK_BACK:        return "Backspace";
    case VK_DELETE:      return "Delete";
    case VK_INSERT:      return "Insert";

    // 控制键
    case VK_SHIFT:       return "Shift";
    case VK_CONTROL:     return "Ctrl";
    case VK_MENU:        return "Alt";
    case VK_CAPITAL:     return "CapsLock";
    case VK_NUMLOCK:     return "NumLock";
    case VK_SCROLL:      return "ScrollLock";

    // 方向键
    case VK_UP:          return "Up";
    case VK_DOWN:        return "Down";
    case VK_LEFT:        return "Left";
    case VK_RIGHT:       return "Right";

    // 符号键
    case VK_OEM_1:       return ";";
    case VK_OEM_2:       return "/";
    case VK_OEM_3:       return "`";
    case VK_OEM_4:       return "[";
    case VK_OEM_5:       return "\\";
    case VK_OEM_6:       return "]";
    case VK_OEM_7:       return "'";
    case VK_OEM_PERIOD:  return ".";
    case VK_OEM_COMMA:   return ",";
    case VK_OEM_MINUS:   return "-";
    case VK_OEM_PLUS:    return "=";
    }

    // 普通字母 / 数字 / F1-F12 自动转换
    return QKeySequence(vkCode).toString();
}











void ProgramUtils::StartMotionByString(QString name)
{

}

bool ProgramUtils::Model3jsonfile_check(QString path)
{
    QDir dir(path);

    // 如果文件夹不存在 → 直接返回 false
    if (!dir.exists())
        return false;

    // 设置过滤：只找文件，后缀为 moc3
    QStringList filters;
    filters << "*.moc3";

    // 列出所有 .moc3 文件
    QStringList fileList = dir.entryList(filters, QDir::Files);

    // 有文件 → 返回 true，没有 → false
    return !fileList.isEmpty();
}

bool ProgramUtils::parseHotKeyString(const QString &hotKeyStr, int &modifier, int &keyCode)
{
    return true;
}

int ProgramUtils::parseModifier(const QString &mod)
{
    return 0;
}

int ProgramUtils::parseKeyCode(const QString &key)
{
    return 0;
}
