/********************************************************************************
** Form generated from reading UI file 'config_widget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIG_WIDGET_H
#define UI_CONFIG_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "config_widget.h"

QT_BEGIN_NAMESPACE

class Ui_config_widget
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    AppWidget *scrollAreaWidgetContents;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea_2;
    ModelWidget *scrollAreaWidgetContents_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QWidget *config_widget)
    {
        if (config_widget->objectName().isEmpty())
            config_widget->setObjectName("config_widget");
        config_widget->resize(1025, 703);
        verticalLayout = new QVBoxLayout(config_widget);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(config_widget);
        tabWidget->setObjectName("tabWidget");
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName("verticalLayout_2");
        scrollArea = new QScrollArea(tab);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setStyleSheet(QString::fromUtf8(""));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new AppWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 981, 606));
        scrollAreaWidgetContents->setStyleSheet(QString::fromUtf8(""));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_2->addWidget(scrollArea);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName("verticalLayout_3");
        scrollArea_2 = new QScrollArea(tab_2);
        scrollArea_2->setObjectName("scrollArea_2");
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new ModelWidget();
        scrollAreaWidgetContents_2->setObjectName("scrollAreaWidgetContents_2");
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 981, 606));
        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_3->addWidget(scrollArea_2);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pushButton = new QPushButton(config_widget);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(config_widget);
        pushButton_2->setObjectName("pushButton_2");

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(config_widget);
        pushButton_3->setObjectName("pushButton_3");

        horizontalLayout->addWidget(pushButton_3);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(config_widget);
        QObject::connect(pushButton, SIGNAL(pressed()), config_widget, SLOT(config_save()));
        QObject::connect(pushButton_2, SIGNAL(pressed()), config_widget, SLOT(return_default()));
        QObject::connect(pushButton_3, SIGNAL(pressed()), config_widget, SLOT(software_restart()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(config_widget);
    } // setupUi

    void retranslateUi(QWidget *config_widget)
    {
        config_widget->setWindowTitle(QCoreApplication::translate("config_widget", "Form", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("config_widget", "APP\350\256\276\347\275\256", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("config_widget", "\346\250\241\345\236\213\347\256\241\347\220\206", nullptr));
        pushButton->setText(QCoreApplication::translate("config_widget", "\350\256\276\347\275\256\344\277\235\345\255\230", nullptr));
        pushButton_2->setText(QCoreApplication::translate("config_widget", "\346\201\242\345\244\215\351\273\230\350\256\244", nullptr));
        pushButton_3->setText(QCoreApplication::translate("config_widget", "\350\275\257\344\273\266\351\207\215\345\220\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class config_widget: public Ui_config_widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIG_WIDGET_H
