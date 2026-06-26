#include "aiskillsettingwindow.h"
#include "ui_aiskillsettingwindow.h"
#include "skillinfoitem.h"

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QStyle>
#include <QTextStream>

AISkillSettingWindow::AISkillSettingWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AISkillSettingWindow)
{
    ui->setupUi(this);

    ui->gridLayout_skills->setColumnStretch(0, 1);
    ui->gridLayout_skills->setColumnStretch(1, 1);

    connect(ui->pushButton_addSkill, &QPushButton::clicked, this, &AISkillSettingWindow::onAddSkill);
    connect(ui->pushButton_removeSkill, &QPushButton::clicked, this, &AISkillSettingWindow::onRemoveSkill);
    connect(ui->pushButton_loadMd, &QPushButton::clicked, this, &AISkillSettingWindow::onLoadMd);

    ui->pushButton_removeSkill->setEnabled(false);
}

AISkillSettingWindow::~AISkillSettingWindow()
{
    delete ui;
}

void AISkillSettingWindow::onAddSkill()
{
    QString name = ui->lineEdit_skillName->text().trimmed();
    if (name.isEmpty())
        name = tr("未命名 Skill");

    QString desc = ui->lineEdit_skillDesc->text().trimmed();
    bool enabled = ui->checkBox_skillEnabled->isChecked();

    addSkillItem(name, desc, enabled);

    ui->lineEdit_skillName->clear();
    ui->lineEdit_skillDesc->clear();
}

void AISkillSettingWindow::addSkillItem(const QString &name, const QString &desc, bool enabled)
{
    SkillInfoItem *item = new SkillInfoItem(ui->scrollAreaWidgetContents_skills);

    QIcon icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
    item->setLogo(icon);
    item->setInfo(QString("%1\n%2").arg(name, desc));
    item->setSkillEnabled(enabled);

    int row = m_skillCount / 2;
    int col = m_skillCount % 2;
    ui->gridLayout_skills->addWidget(item, row, col);

    ++m_skillCount;
    ui->pushButton_removeSkill->setEnabled(true);
}

void AISkillSettingWindow::onRemoveSkill()
{
    if (m_skillCount <= 0)
        return;

    --m_skillCount;
    int row = m_skillCount / 2;
    int col = m_skillCount % 2;

    QLayoutItem *layoutItem = ui->gridLayout_skills->itemAtPosition(row, col);
    if (layoutItem && layoutItem->widget()) {
        delete layoutItem->widget();
    }

    ui->pushButton_removeSkill->setEnabled(m_skillCount > 0);
}

void AISkillSettingWindow::onLoadMd()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择 skill.md"), QString(), tr("Markdown (*.md);;所有文件 (*.*)"));
    if (filePath.isEmpty())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->plainTextEdit_mdInfo->setPlainText(tr("无法打开文件: %1").arg(filePath));
        return;
    }

    QTextStream stream(&file);
    ui->plainTextEdit_mdInfo->setPlainText(stream.readAll());
}
