#ifndef AISKILLSETTINGWINDOW_H
#define AISKILLSETTINGWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class AISkillSettingWindow; }
QT_END_NAMESPACE

class AISkillSettingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AISkillSettingWindow(QWidget *parent = nullptr);
    ~AISkillSettingWindow();

private slots:
    void onAddSkill();
    void onRemoveSkill();
    void onLoadMd();

private:
    void addSkillItem(const QString &name, const QString &desc, bool enabled);

    Ui::AISkillSettingWindow *ui;
    int m_skillCount = 0;
};

#endif // AISKILLSETTINGWINDOW_H
