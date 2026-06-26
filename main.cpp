#include "mainwindow.h"
#include "apputils.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AppUtils::instance().setTheme(0);

    MainWindow w;
    return a.exec();
}
