#ifndef APPUTILS_H
#define APPUTILS_H

#include <QObject>
#include <QString>
#include <QTranslator>
#include <QMap>

class AppUtils : public QObject
{
    Q_OBJECT
public:
    static AppUtils& instance();

    void setTheme(int index);
    int currentTheme() const;

    void setLanguage(int index);
    int currentLanguage() const;

    QStringList themeNames() const;
    QStringList languageNames() const;

    bool loadThemeFile(const QString &fileName);

signals:
    void themeChanged(int index);
    void languageChanged(int index);

private:
    explicit AppUtils(QObject *parent = nullptr);
    ~AppUtils();

    void applyTheme(const QString &qssPath);
    void applyLanguage(int index);

    int m_currentTheme;
    int m_currentLanguage;
    QTranslator *m_translator;
    QMap<int, QString> m_themePaths;
    QMap<int, QString> m_languageCodes;

    QStringList m_themeNames;
    QStringList m_languageNames;
};

#endif // APPUTILS_H
