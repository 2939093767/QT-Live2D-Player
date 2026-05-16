/********************************************************************************
** Form generated from reading UI file 'ModelWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELWIDGET_H
#define UI_MODELWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModelWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QPushButton *pushButton;
    QFrame *line;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButton;
    QSpinBox *spinBox;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget;
    QFrame *line_2;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButton_2;
    QSpinBox *spinBox_2;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer;
    QWidget *widget_2;

    void setupUi(QWidget *ModelWidget)
    {
        if (ModelWidget->objectName().isEmpty())
            ModelWidget->setObjectName("ModelWidget");
        ModelWidget->resize(548, 402);
        verticalLayout = new QVBoxLayout(ModelWidget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(ModelWidget);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(ModelWidget);
        comboBox->setObjectName("comboBox");
        comboBox->setEditable(true);

        horizontalLayout->addWidget(comboBox);

        pushButton = new QPushButton(ModelWidget);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);

        horizontalLayout->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout);

        line = new QFrame(ModelWidget);
        line->setObjectName("line");
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        label_2 = new QLabel(ModelWidget);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        radioButton = new QRadioButton(ModelWidget);
        radioButton->setObjectName("radioButton");
        radioButton->setAutoExclusive(false);

        horizontalLayout_3->addWidget(radioButton);

        spinBox = new QSpinBox(ModelWidget);
        spinBox->setObjectName("spinBox");
        spinBox->setMinimum(1);
        spinBox->setMaximum(10);

        horizontalLayout_3->addWidget(spinBox);

        label_4 = new QLabel(ModelWidget);
        label_4->setObjectName("label_4");

        horizontalLayout_3->addWidget(label_4);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        horizontalLayout_3->setStretch(3, 1);

        verticalLayout->addLayout(horizontalLayout_3);

        widget = new QWidget(ModelWidget);
        widget->setObjectName("widget");

        verticalLayout->addWidget(widget);

        line_2 = new QFrame(ModelWidget);
        line_2->setObjectName("line_2");
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        label_3 = new QLabel(ModelWidget);
        label_3->setObjectName("label_3");

        verticalLayout->addWidget(label_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        radioButton_2 = new QRadioButton(ModelWidget);
        radioButton_2->setObjectName("radioButton_2");
        radioButton_2->setAutoExclusive(false);

        horizontalLayout_2->addWidget(radioButton_2);

        spinBox_2 = new QSpinBox(ModelWidget);
        spinBox_2->setObjectName("spinBox_2");
        spinBox_2->setMinimum(1);
        spinBox_2->setMaximum(10);

        horizontalLayout_2->addWidget(spinBox_2);

        label_5 = new QLabel(ModelWidget);
        label_5->setObjectName("label_5");

        horizontalLayout_2->addWidget(label_5);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout_2->setStretch(3, 1);

        verticalLayout->addLayout(horizontalLayout_2);

        widget_2 = new QWidget(ModelWidget);
        widget_2->setObjectName("widget_2");

        verticalLayout->addWidget(widget_2);

        verticalLayout->setStretch(4, 1);
        verticalLayout->setStretch(8, 1);

        retranslateUi(ModelWidget);
        QObject::connect(pushButton, SIGNAL(pressed()), ModelWidget, SLOT(folder_choose()));
        QObject::connect(radioButton_2, SIGNAL(clicked(bool)), ModelWidget, SLOT(motion_israndom(bool)));
        QObject::connect(radioButton, SIGNAL(clicked(bool)), ModelWidget, SLOT(expression_israndom(bool)));

        QMetaObject::connectSlotsByName(ModelWidget);
    } // setupUi

    void retranslateUi(QWidget *ModelWidget)
    {
        ModelWidget->setWindowTitle(QCoreApplication::translate("ModelWidget", "Form", nullptr));
        label->setText(QCoreApplication::translate("ModelWidget", "\346\250\241\345\236\213\346\226\207\344\273\266\345\244\271\350\267\257\345\276\204\357\274\232", nullptr));
        pushButton->setText(QCoreApplication::translate("ModelWidget", "\346\265\217\350\247\210", nullptr));
        label_2->setText(QCoreApplication::translate("ModelWidget", "\350\241\250\346\203\205\347\256\241\347\220\206", nullptr));
        radioButton->setText(QCoreApplication::translate("ModelWidget", "\351\232\217\346\234\272\350\241\250\346\203\205\345\210\267\346\226\260", nullptr));
        label_4->setText(QCoreApplication::translate("ModelWidget", "s/time", nullptr));
        label_3->setText(QCoreApplication::translate("ModelWidget", "\345\212\250\344\275\234\347\256\241\347\220\206", nullptr));
        radioButton_2->setText(QCoreApplication::translate("ModelWidget", "\351\232\217\346\234\272\345\212\250\344\275\234\345\210\267\346\226\260", nullptr));
        label_5->setText(QCoreApplication::translate("ModelWidget", "s/time", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModelWidget: public Ui_ModelWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELWIDGET_H
