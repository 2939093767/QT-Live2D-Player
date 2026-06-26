#include "configini.h"

#include <QDir>
#include <QMessageBox>
#include "LAppLive2DManager.hpp"
#include "LAppModel.hpp"
#include <QDir>
#include <QCoreApplication>
#include <windows.h>
#include <qmessagebox.h>
#include <qevent.h>



ConfigManager::ConfigManager()
    : m_settings(CONFIG_FILE_PATH, QSettings::IniFormat)
{
    // 同步一下 → 自动创建空的 ini 文件
    m_settings.sync();

    if(m_settings.childGroups().isEmpty())
    {
        initDefaultConfig();
        return;
    }
    //判断文件是否有效
    if(!ProgramUtils::Model3jsonfile_check(getValue(CONFIG_MODEL_FOLDER).toString())){
        initDefaultConfig();
        QMessageBox::critical(nullptr, "错误", "未检测到模型文件!恢复默认");
    }
    setValue(CONFIG_IMAGE_CROP_RECT,QRect());
}

ConfigManager& ConfigManager::instance()
{
    static ConfigManager instance;
    return instance;
}

QVariant ConfigManager::getValue(const QString& key, const QVariant& defaultValue) const
{
    return m_settings.value(key, defaultValue);
}

void ConfigManager::setValue(const QString& key, const QVariant& value)
{
    m_settings.setValue(key, value);
}

void ConfigManager::sync()
{
    m_settings.sync();
}




void ConfigManager::initDefaultConfig()
{
    // ==================== 默认配置组 ====================
    //app
    KeyConfigDefault();
    //面捕
    FaceConfigDefault();
    // 同步保存默认配置
    m_settings.sync();
}

void ConfigManager::FaceConfigDefault()
{
    m_settings.setValue(CONFIG_MOTION_IMAGE_SIZE,1);
    m_settings.setValue(CONFIG_MOTION_OFFSET,0.5);
    m_settings.setValue(CONFIG_MOTION_SENSITIVITY,1.0);
    m_settings.setValue(CONFIG_MOTION_SMOOTH_PARAMTER,0.15);
}

void ConfigManager::KeyConfigDefault()
{
    m_settings.setValue(CONFIG_APP_SOFT_START, false);
    m_settings.setValue(CONFIG_APP_FPS, 0);
    m_settings.setValue(CONFIG_APP_MODEL_CONTROL, 0);
    m_settings.setValue(CONFIG_MODEL_FOLDER, QCoreApplication::applicationDirPath()+"/Resources/Haru");
}

void ConfigManager::setAutoStart(bool is)
{

    QString appName = QCoreApplication::applicationName(); // 自动取程序名
    QString appPath = QCoreApplication::applicationFilePath(); // 自动取exe路径
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (is) {
        reg.setValue(appName, appPath);
    } else {
        reg.remove(appName);
    }

}




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

        QMap<QString,motion_unit> motions;
        QRC_Manager::instance().MotionQuery(MOTION_KEYMOTION,vkCodeToQString(vkCode),motions);
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
