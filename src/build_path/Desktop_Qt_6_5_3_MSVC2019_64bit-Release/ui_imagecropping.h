/********************************************************************************
** Form generated from reading UI file 'imagecropping.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGECROPPING_H
#define UI_IMAGECROPPING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "FaceHandle/imagecropping.h"

QT_BEGIN_NAMESPACE

class Ui_ImageCropping
{
public:
    QHBoxLayout *horizontalLayout;
    CropWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton;

    void setupUi(QWidget *ImageCropping)
    {
        if (ImageCropping->objectName().isEmpty())
            ImageCropping->setObjectName("ImageCropping");
        ImageCropping->resize(771, 442);
        horizontalLayout = new QHBoxLayout(ImageCropping);
        horizontalLayout->setObjectName("horizontalLayout");
        widget = new CropWidget(ImageCropping);
        widget->setObjectName("widget");

        horizontalLayout->addWidget(widget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(20, 20, 20, 10);
        label = new QLabel(ImageCropping);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        label_2 = new QLabel(ImageCropping);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(ImageCropping);
        label_3->setObjectName("label_3");

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(ImageCropping);
        label_4->setObjectName("label_4");

        verticalLayout->addWidget(label_4);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        pushButton = new QPushButton(ImageCropping);
        pushButton->setObjectName("pushButton");

        verticalLayout->addWidget(pushButton);


        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 5);
        horizontalLayout->setStretch(1, 1);

        retranslateUi(ImageCropping);
        QObject::connect(pushButton, SIGNAL(pressed()), ImageCropping, SLOT(SaveRect()));

        QMetaObject::connectSlotsByName(ImageCropping);
    } // setupUi

    void retranslateUi(QWidget *ImageCropping)
    {
        ImageCropping->setWindowTitle(QCoreApplication::translate("ImageCropping", "Form", nullptr));
        label->setText(QCoreApplication::translate("ImageCropping", "x : ", nullptr));
        label_2->setText(QCoreApplication::translate("ImageCropping", "y :", nullptr));
        label_3->setText(QCoreApplication::translate("ImageCropping", "width : ", nullptr));
        label_4->setText(QCoreApplication::translate("ImageCropping", "height : ", nullptr));
        pushButton->setText(QCoreApplication::translate("ImageCropping", "\344\277\235\345\255\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImageCropping: public Ui_ImageCropping {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGECROPPING_H
