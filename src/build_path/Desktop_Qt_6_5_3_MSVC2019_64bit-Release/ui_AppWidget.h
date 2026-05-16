/********************************************************************************
** Form generated from reading UI file 'AppWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPWIDGET_H
#define UI_APPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AppWidget
{
public:
    QGridLayout *gridLayout;
    QComboBox *comboBox;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QComboBox *comboBox_2;
    QLabel *label_2;
    QCheckBox *checkBox;

    void setupUi(QWidget *AppWidget)
    {
        if (AppWidget->objectName().isEmpty())
            AppWidget->setObjectName("AppWidget");
        AppWidget->resize(455, 346);
        gridLayout = new QGridLayout(AppWidget);
        gridLayout->setSpacing(10);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(-1, -1, 20, -1);
        comboBox = new QComboBox(AppWidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        gridLayout->addWidget(comboBox, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(329, 234, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 1, 1, 1);

        label = new QLabel(AppWidget);
        label->setObjectName("label");

        gridLayout->addWidget(label, 1, 0, 1, 1);

        comboBox_2 = new QComboBox(AppWidget);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");

        gridLayout->addWidget(comboBox_2, 2, 1, 1, 1);

        label_2 = new QLabel(AppWidget);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        checkBox = new QCheckBox(AppWidget);
        checkBox->setObjectName("checkBox");

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);


        retranslateUi(AppWidget);

        QMetaObject::connectSlotsByName(AppWidget);
    } // setupUi

    void retranslateUi(QWidget *AppWidget)
    {
        AppWidget->setWindowTitle(QCoreApplication::translate("AppWidget", "Form", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("AppWidget", "\351\274\240\346\240\207\346\216\247\345\210\266", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("AppWidget", "\344\272\272\350\204\270\350\267\237\351\232\217", nullptr));

        label->setText(QCoreApplication::translate("AppWidget", "\346\250\241\345\236\213\346\216\247\345\210\266\346\226\271\345\274\217\357\274\232", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("AppWidget", "30FPS", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("AppWidget", "60FPS", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("AppWidget", "120FPS", nullptr));

        label_2->setText(QCoreApplication::translate("AppWidget", "\347\252\227\345\217\243\345\210\267\346\226\260\351\242\221\347\216\207\357\274\232", nullptr));
        checkBox->setText(QCoreApplication::translate("AppWidget", "\345\274\200\346\234\272\350\207\252\345\220\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AppWidget: public Ui_AppWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPWIDGET_H
