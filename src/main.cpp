#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <config_setting.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;

    w.setAttribute(Qt::WA_StyledBackground, true);
    QFile file(":/qdarkstyle/dark/darkstyle.qss"); // 这里先改对路径
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        qApp->setStyleSheet(QString::fromUtf8(file.readAll())); // 全局生效更稳
        file.close();
    } else {
        qDebug() << "主题文件打开失败！路径：" << file.fileName(); // 方便你查错
    }
    //qDebug()<<(ConfigManager::instance().getValue(CONFIG_MOC3_FOLDER).toString()+"/").toUtf8();

    w.show();
    return a.exec();
}
