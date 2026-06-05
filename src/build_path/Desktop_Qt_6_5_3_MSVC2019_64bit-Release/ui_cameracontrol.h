/********************************************************************************
** Form generated from reading UI file 'cameracontrol.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERACONTROL_H
#define UI_CAMERACONTROL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include "cameracontrol.h"

QT_BEGIN_NAMESPACE

class Ui_CameraControl
{
public:
    QHBoxLayout *horizontalLayout_2;
    ImageShow *openGLWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_6;
    QLabel *label_4;
    QPushButton *pushButton;
    QLabel *label;
    QSlider *horizontalSlider_2;
    QComboBox *comboBox;
    QPushButton *pushButton_3;
    QComboBox *comboBox_2;
    QSlider *horizontalSlider;

    void setupUi(QWidget *CameraControl)
    {
        if (CameraControl->objectName().isEmpty())
            CameraControl->setObjectName("CameraControl");
        CameraControl->resize(957, 602);
        horizontalLayout_2 = new QHBoxLayout(CameraControl);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        openGLWidget = new ImageShow(CameraControl);
        openGLWidget->setObjectName("openGLWidget");

        horizontalLayout_2->addWidget(openGLWidget);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout->addItem(verticalSpacer);


        gridLayout->addLayout(horizontalLayout, 6, 1, 1, 1);

        label_2 = new QLabel(CameraControl);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label_3 = new QLabel(CameraControl);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        label_6 = new QLabel(CameraControl);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        label_4 = new QLabel(CameraControl);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        pushButton = new QPushButton(CameraControl);
        pushButton->setObjectName("pushButton");
        pushButton->setCheckable(true);

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);

        label = new QLabel(CameraControl);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalSlider_2 = new QSlider(CameraControl);
        horizontalSlider_2->setObjectName("horizontalSlider_2");
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider_2, 5, 1, 1, 1);

        comboBox = new QComboBox(CameraControl);
        comboBox->setObjectName("comboBox");

        gridLayout->addWidget(comboBox, 0, 1, 1, 1);

        pushButton_3 = new QPushButton(CameraControl);
        pushButton_3->setObjectName("pushButton_3");

        gridLayout->addWidget(pushButton_3, 3, 1, 1, 1);

        comboBox_2 = new QComboBox(CameraControl);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");

        gridLayout->addWidget(comboBox_2, 2, 1, 1, 1);

        horizontalSlider = new QSlider(CameraControl);
        horizontalSlider->setObjectName("horizontalSlider");
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 4, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);

        horizontalLayout_2->setStretch(0, 3);
        horizontalLayout_2->setStretch(1, 1);

        retranslateUi(CameraControl);
        QObject::connect(pushButton, SIGNAL(clicked(bool)), CameraControl, SLOT(ChangeOpenState(bool)));
        QObject::connect(pushButton_3, SIGNAL(pressed()), CameraControl, SLOT(FrameCapture()));
        QObject::connect(horizontalSlider, SIGNAL(sliderReleased()), CameraControl, SLOT(MotionSensitivty()));
        QObject::connect(horizontalSlider_2, SIGNAL(sliderReleased()), CameraControl, SLOT(MotionSmooth()));
        QObject::connect(comboBox_2, SIGNAL(currentIndexChanged(int)), CameraControl, SLOT(ImageChange(int)));

        QMetaObject::connectSlotsByName(CameraControl);
    } // setupUi

    void retranslateUi(QWidget *CameraControl)
    {
        CameraControl->setWindowTitle(QCoreApplication::translate("CameraControl", "Form", nullptr));
        label_2->setText(QCoreApplication::translate("CameraControl", "\351\235\242\346\215\225\345\210\206\350\276\250\347\216\207\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("CameraControl", "\347\224\273\351\235\242\346\210\252\345\217\226\357\274\232", nullptr));
        label_6->setText(QCoreApplication::translate("CameraControl", "\345\212\250\344\275\234\345\271\263\346\273\221\345\272\246\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("CameraControl", "\347\247\273\345\212\250\347\201\265\346\225\217\345\272\246\357\274\232", nullptr));
        pushButton->setText(QCoreApplication::translate("CameraControl", "\346\225\210\346\236\234\346\265\213\350\257\225", nullptr));
        label->setText(QCoreApplication::translate("CameraControl", "\346\221\204\345\203\217\345\244\264\357\274\232", nullptr));
        pushButton_3->setText(QCoreApplication::translate("CameraControl", "\346\234\252\345\220\257\347\224\250", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("CameraControl", "1920x1080", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("CameraControl", "960x540", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("CameraControl", "480x270", nullptr));

    } // retranslateUi

};

namespace Ui {
    class CameraControl: public Ui_CameraControl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERACONTROL_H
