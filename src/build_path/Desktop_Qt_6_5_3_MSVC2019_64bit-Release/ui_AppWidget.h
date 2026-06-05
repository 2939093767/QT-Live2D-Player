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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AppWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QLabel *label_2;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QLabel *label_4;

    void setupUi(QWidget *AppWidget)
    {
        if (AppWidget->objectName().isEmpty())
            AppWidget->setObjectName("AppWidget");
        AppWidget->resize(461, 434);
        verticalLayout = new QVBoxLayout(AppWidget);
        verticalLayout->setObjectName("verticalLayout");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        checkBox = new QCheckBox(AppWidget);
        checkBox->setObjectName("checkBox");

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 2, 1, 1);

        comboBox = new QComboBox(AppWidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        gridLayout->addWidget(comboBox, 1, 1, 1, 1);

        comboBox_2 = new QComboBox(AppWidget);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");

        gridLayout->addWidget(comboBox_2, 2, 1, 1, 1);

        label_2 = new QLabel(AppWidget);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label = new QLabel(AppWidget);
        label->setObjectName("label");

        gridLayout->addWidget(label, 1, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_3 = new QLabel(AppWidget);
        label_3->setObjectName("label_3");

        horizontalLayout->addWidget(label_3);

        lineEdit = new QLineEdit(AppWidget);
        lineEdit->setObjectName("lineEdit");

        horizontalLayout->addWidget(lineEdit);

        pushButton_2 = new QPushButton(AppWidget);
        pushButton_2->setObjectName("pushButton_2");

        horizontalLayout->addWidget(pushButton_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(3, 1);

        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(329, 234, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label_4 = new QLabel(AppWidget);
        label_4->setObjectName("label_4");

        verticalLayout->addWidget(label_4);


        retranslateUi(AppWidget);
        QObject::connect(pushButton_2, SIGNAL(pressed()), AppWidget, SLOT(folder_choose()));

        QMetaObject::connectSlotsByName(AppWidget);
    } // setupUi

    void retranslateUi(QWidget *AppWidget)
    {
        AppWidget->setWindowTitle(QCoreApplication::translate("AppWidget", "Form", nullptr));
        checkBox->setText(QCoreApplication::translate("AppWidget", "\345\274\200\346\234\272\350\207\252\345\220\257", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("AppWidget", "\351\224\256\351\274\240\350\267\237\350\270\252", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("AppWidget", "\351\235\242\351\203\250\350\267\237\351\232\217", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("AppWidget", "\350\207\252\344\270\273\350\277\220\350\241\214\357\274\210\346\241\214\345\256\240\343\200\201\345\257\271\350\257\235\346\234\272\345\231\250\344\272\272\357\274\211", nullptr));

        comboBox_2->setItemText(0, QCoreApplication::translate("AppWidget", "30FPS", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("AppWidget", "60FPS", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("AppWidget", "120FPS", nullptr));

        label_2->setText(QCoreApplication::translate("AppWidget", "\346\250\241\345\236\213\346\270\262\346\237\223\351\242\221\347\216\207\357\274\232", nullptr));
        label->setText(QCoreApplication::translate("AppWidget", "\350\277\220\350\241\214\346\226\271\345\274\217\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("AppWidget", "\346\250\241\345\236\213\346\226\207\344\273\266\345\244\271\350\267\257\345\276\204\357\274\232", nullptr));
        pushButton_2->setText(QCoreApplication::translate("AppWidget", "\346\265\217\350\247\210", nullptr));
        label_4->setText(QCoreApplication::translate("AppWidget", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AppWidget: public Ui_AppWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPWIDGET_H
