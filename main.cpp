#include "mainwindow.h"
#include "apputils.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
//#include <QTextCodec>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // // 设置 UTF-8 编码支持中文显示
    // QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    // QTextCodec::setCodecForLocale(codec);

    AppUtils::instance().setTheme(0);

    MainWindow w;
    return a.exec();
}
