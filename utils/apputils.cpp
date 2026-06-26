#include "apputils.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QLocale>

AppUtils& AppUtils::instance()
{
    static AppUtils inst;
    return inst;
}

AppUtils::AppUtils(QObject *parent)
    : QObject(parent)
    , m_currentTheme(0)
    , m_currentLanguage(0)
    , m_translator(nullptr)
{
    m_themeNames << "Dark Neon" << "Light" << "Classic";
    m_themePaths[0] = ":/styles/styles/darkneon.qss";
    m_themePaths[1] = "";
    m_themePaths[2] = "";

    m_languageNames << "简体中文" << "English" << "日本語";
    m_languageCodes[0] = "zh_CN";
    m_languageCodes[1] = "en_US";
    m_languageCodes[2] = "ja_JP";
}

AppUtils::~AppUtils()
{
    if (m_translator) {
        qApp->removeTranslator(m_translator);
        delete m_translator;
    }
}

void AppUtils::setTheme(int index)
{
    if (index < 0 || index >= m_themeNames.size())
        return;

    if (m_currentTheme == index)
        return;

    m_currentTheme = index;

    QString qssPath = m_themePaths.value(index, "");
    applyTheme(qssPath);

    emit themeChanged(index);
}

int AppUtils::currentTheme() const
{
    return m_currentTheme;
}

void AppUtils::setLanguage(int index)
{
    if (index < 0 || index >= m_languageNames.size())
        return;

    if (m_currentLanguage == index)
        return;

    m_currentLanguage = index;
    applyLanguage(index);

    emit languageChanged(index);
}

int AppUtils::currentLanguage() const
{
    return m_currentLanguage;
}

QStringList AppUtils::themeNames() const
{
    return m_themeNames;
}

QStringList AppUtils::languageNames() const
{
    return m_languageNames;
}

bool AppUtils::loadThemeFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to load QSS:" << fileName;
        return false;
    }

    QTextStream stream(&file);
    QString qss = stream.readAll();
    file.close();

    qApp->setStyleSheet(qss);
    return true;
}

void AppUtils::applyTheme(const QString &qssPath)
{
    if (qssPath.isEmpty()) {
        qApp->setStyleSheet("");
        return;
    }

    loadThemeFile(qssPath);
}

void AppUtils::applyLanguage(int index)
{
    Q_UNUSED(index);
    // 预留：翻译文件加载逻辑
    // if (m_translator) {
    //     qApp->removeTranslator(m_translator);
    //     delete m_translator;
    //     m_translator = nullptr;
    // }
    //
    // QString langCode = m_languageCodes.value(index, "zh_CN");
    // m_translator = new QTranslator();
    // if (m_translator->load(QString(":/translations/app_%1.qm").arg(langCode))) {
    //     qApp->installTranslator(m_translator);
    // }
}
