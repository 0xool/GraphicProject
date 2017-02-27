/********************************************************************************
** Form generated from reading UI file 'openglview.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENGLVIEW_H
#define UI_OPENGLVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include "openglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_openGlView
{
public:
    OpenGlWidget *openGLWidget;

    void setupUi(QDialog *openGlView)
    {
        if (openGlView->objectName().isEmpty())
            openGlView->setObjectName(QStringLiteral("openGlView"));
        openGlView->setWindowModality(Qt::ApplicationModal);
        openGlView->resize(800, 400);
        openGlView->setAutoFillBackground(true);
        openGLWidget = new OpenGlWidget(openGlView);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(-1, -1, 801, 400));

        retranslateUi(openGlView);

        QMetaObject::connectSlotsByName(openGlView);
    } // setupUi

    void retranslateUi(QDialog *openGlView)
    {
        openGlView->setWindowTitle(QApplication::translate("openGlView", "Dialog", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class openGlView: public Ui_openGlView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENGLVIEW_H
