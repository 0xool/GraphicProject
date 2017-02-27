/********************************************************************************
** Form generated from reading UI file 'outershapeuserinput.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUTERSHAPEUSERINPUT_H
#define UI_OUTERSHAPEUSERINPUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include "outershapeinputopengl.h"

QT_BEGIN_NAMESPACE

class Ui_OuterShapeUserInput
{
public:
    OuterShapeInputOpenGL *openGLWidget;
    QPushButton *pushButton;

    void setupUi(QDialog *OuterShapeUserInput)
    {
        if (OuterShapeUserInput->objectName().isEmpty())
            OuterShapeUserInput->setObjectName(QStringLiteral("OuterShapeUserInput"));
        OuterShapeUserInput->resize(500, 500);
        openGLWidget = new OuterShapeInputOpenGL(OuterShapeUserInput);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(-1, -1, 501, 441));
        pushButton = new QPushButton(OuterShapeUserInput);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(200, 460, 121, 32));

        retranslateUi(OuterShapeUserInput);

        QMetaObject::connectSlotsByName(OuterShapeUserInput);
    } // setupUi

    void retranslateUi(QDialog *OuterShapeUserInput)
    {
        OuterShapeUserInput->setWindowTitle(QApplication::translate("OuterShapeUserInput", "Dialog", Q_NULLPTR));
        pushButton->setText(QApplication::translate("OuterShapeUserInput", "\330\256\330\261\331\210\330\254", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class OuterShapeUserInput: public Ui_OuterShapeUserInput {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTERSHAPEUSERINPUT_H
