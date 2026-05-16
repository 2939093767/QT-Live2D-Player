#include "config_setting.h"
#include "LAppLive2DManager.hpp"
#include "LAppModel.hpp"
#include <QDir>
#include <QCoreApplication>
#include <windows.h>
#include <qmessagebox.h>


// 配置文件路径（固定：程序目录下的 config.ini）
#define CONFIG_FILE_PATH "config.ini"




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
    if(ProgramUtils::Model3jsonfile_check(getValue(CONFIG_MODEL_FOLDER).toString())){
        initDefaultConfig();
        QMessageBox::critical(nullptr, "错误", "未检测到模型文件!恢复默认");
    }
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
    AutoStart::set(false,"pet");
    m_settings.setValue(CONFIG_APP_SOFT_START, false);
    m_settings.setValue(CONFIG_APP_FPS, 0);
    m_settings.setValue(CONFIG_APP_MODEL_CONTROL, 0);
    //qDebug()<<QCoreApplication::applicationDirPath();
    m_settings.setValue(CONFIG_MODEL_FOLDER, QCoreApplication::applicationDirPath().toUtf8()+"Resources/Haru");

    // 同步保存默认配置
    m_settings.sync();
}



















void QRC_Manager::SetupConfigByMoc()
{


}

void QRC_Manager::ClearManager()
{
    m_manager.clear();
}

QMap<QString, motion_unit> QRC_Manager::GetHitareaMotion(QString area)
{
    QMap<QString, motion_unit> resultMap;
    // 遍历内部的总 map
    for (auto it = m_manager.begin(); it != m_manager.end(); ++it)
    {
        const QString& key = it.key();
        const motion_unit& unit = it.value();

        // ====================== 筛选条件 ======================

        if (unit.hitarea == area && unit.isuse && unit.quickkey_isuse)
        {
            resultMap.insert(key, unit);
        }
    }
    return resultMap;
}




QMap<QString, motion_unit> QRC_Manager::GetQuickKeyMotion(int quickkey_id)
{
    QMap<QString, motion_unit> resultMap;
    // 遍历内部的总 map
    for (auto it = m_manager.begin(); it != m_manager.end(); ++it)
    {
        const QString& key = it.key();
        const motion_unit& unit = it.value();

        // ====================== 筛选条件 ======================
        // 提取 type == "expression" 的项
        if (unit.quickkey_id == quickkey_id && unit.isuse && unit.quickkey_isuse)
        {
            resultMap.insert(key, unit);
        }
    }
    return resultMap;
}

QMap<QString, motion_unit> QRC_Manager::GetActiveExpression()
{
    QMap<QString, motion_unit> resultMap;
    // 遍历内部的总 map
    for (auto it = m_manager.begin(); it != m_manager.end(); ++it)
    {
        const QString& key = it.key();
        const motion_unit& unit = it.value();

        // ====================== 筛选条件 ======================
        // 提取 type == "expression" 的项
        if (unit.type == EXPRESSION && unit.isuse)
        {
            resultMap.insert(key, unit);
        }
    }
    return resultMap;
}

QMap<QString, motion_unit> QRC_Manager::GetActiveMotion()
{
    QMap<QString, motion_unit> resultMap;
    // 遍历内部的总 map
    for (auto it = m_manager.begin(); it != m_manager.end(); ++it)
    {
        const QString& key = it.key();
        const motion_unit& unit = it.value() ;

        // ====================== 筛选条件 ======================
        // 提取 type == "expression" 的项
        if (unit.type == MOTION && unit.isuse)
        {
            resultMap.insert(key, unit);
        }
    }
    return resultMap;
}















GlobalHotKeyMgr::~GlobalHotKeyMgr()
{
    unregisterAll();

}

int GlobalHotKeyMgr::registerHotKey(int modifier, int keyCode)
{
    static int id = 0;
    id++;
    HWND hwnd = reinterpret_cast<HWND>(m_widget->winId());
    BOOL ok = RegisterHotKey(hwnd, id, modifier, keyCode);
    if (ok)
    {
        m_ids[id] = true;
        qDebug() << "[全局热键] 注册成功 id=" << id;
        return id;
    }
    qDebug() << "[全局热键] 注册失败 id=" << id << "（被占用？）";
    return 0;
}

void GlobalHotKeyMgr::unregisterHotKey(int id)
{
    if (!m_ids.contains(id)) return;
    HWND hwnd = reinterpret_cast<HWND>(m_widget->winId());
    UnregisterHotKey(hwnd, id);
    m_ids.remove(id);
}

void GlobalHotKeyMgr::unregisterAll()
{
    for (int id : m_ids.keys()) unregisterHotKey(id);
    m_ids.clear();
}





























void ProgramUtils::StartMotionByString(QString name)
{
    QStringList parts = name.split('_');
    QString group = parts[0];               // 前面：字符串
    int no = parts[1].toInt();    // 后面：转int
    LAppLive2DManager::GetInstance()->GetModel(0)->StartMotion(group.toUtf8(),no,1);
}

bool ProgramUtils::Model3jsonfile_check(QString path)
{
    QDir dir(path);

    // 过滤：只找文件，匹配后缀
    QStringList filters;
    filters << "*.model3.json";  // 你要的匹配符

    // 设置过滤规则
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);  // 只找文件，不找文件夹

    // 获取匹配的文件列表
    QFileInfoList fileList = dir.entryInfoList();
    return fileList.isEmpty();
}


bool ProgramUtils::parseHotKeyString(const QString &hotKeyStr, int &modifier, int &keyCode)
{
    modifier = 0;
    keyCode = 0;

    // 按 + 分割
    QStringList parts = hotKeyStr.split("+", Qt::SkipEmptyParts);
    if (parts.isEmpty()) return false;

    // 最后一段是按键，前面都是修饰符
    for (int i = 0; i < parts.size() - 1; ++i) {
        QString mod = parts[i].trimmed().toLower();
        modifier |= parseModifier(mod);
    }

    // 解析按键
    QString key = parts.last().trimmed();
    keyCode = parseKeyCode(key);

    return keyCode != 0;
}

int ProgramUtils::parseModifier(const QString &mod)
{
    if (mod == "ctrl" || mod == "control") return MOD_CONTROL;
    if (mod == "alt") return MOD_ALT;
    if (mod == "shift") return MOD_SHIFT;
    if (mod == "win" || mod == "windows") return MOD_WIN;
    return 0;
}

int ProgramUtils::parseKeyCode(const QString &key)
{
    QString k = key.toLower();

    // 字母 A-Z
    if (k.length() == 1 && k[0] >= 'a' && k[0] <= 'z')
        return k.at(0).toUpper().toLatin1();

    // 数字 0-9
    if (k.length() == 1 && k[0] >= '0' && k[0] <= '9')
        return k.at(0).toLatin1();

    // 功能键 F1-F12
    if (k.startsWith("f")) {
        bool ok;
        int n = k.mid(1).toInt(&ok);
        if (ok && n >= 1 && n <= 12)
            return VK_F1 + n - 1;
    }

    // 常用按键
    if (k == "esc" || k == "escape") return VK_ESCAPE;
    if (k == "enter" || k == "return") return VK_RETURN;
    if (k == "space" || k == "空格") return VK_SPACE;
    if (k == "tab") return VK_TAB;
    if (k == "up") return VK_UP;
    if (k == "down") return VK_DOWN;
    if (k == "left") return VK_LEFT;
    if (k == "right") return VK_RIGHT;

    return 0;
}







