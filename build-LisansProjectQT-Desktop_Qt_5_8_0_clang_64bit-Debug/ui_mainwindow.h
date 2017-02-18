/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *ExitBtn;
    QPushButton *startAlgorithmBtn;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboBox;
    QLabel *label;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *shapeTextEdit;
    QPushButton *getShapedBtn;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QTextEdit *graphTextEdit;
    QPushButton *getGraphBtn;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(522, 419);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ExitBtn = new QPushButton(centralWidget);
        ExitBtn->setObjectName(QStringLiteral("ExitBtn"));
        ExitBtn->setGeometry(QRect(150, 310, 211, 32));
        startAlgorithmBtn = new QPushButton(centralWidget);
        startAlgorithmBtn->setObjectName(QStringLiteral("startAlgorithmBtn"));
        startAlgorithmBtn->setGeometry(QRect(150, 280, 211, 32));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(340, 10, 171, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        comboBox = new QComboBox(horizontalLayoutWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout->addWidget(comboBox);

        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        horizontalLayoutWidget_2 = new QWidget(centralWidget);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 70, 501, 91));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        shapeTextEdit = new QTextEdit(horizontalLayoutWidget_2);
        shapeTextEdit->setObjectName(QStringLiteral("shapeTextEdit"));

        horizontalLayout_2->addWidget(shapeTextEdit);

        getShapedBtn = new QPushButton(horizontalLayoutWidget_2);
        getShapedBtn->setObjectName(QStringLiteral("getShapedBtn"));

        horizontalLayout_2->addWidget(getShapedBtn);

        horizontalLayoutWidget_3 = new QWidget(centralWidget);
        horizontalLayoutWidget_3->setObjectName(QStringLiteral("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(10, 170, 501, 91));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        graphTextEdit = new QTextEdit(horizontalLayoutWidget_3);
        graphTextEdit->setObjectName(QStringLiteral("graphTextEdit"));

        horizontalLayout_3->addWidget(graphTextEdit);

        getGraphBtn = new QPushButton(horizontalLayoutWidget_3);
        getGraphBtn->setObjectName(QStringLiteral("getGraphBtn"));
        getGraphBtn->setIconSize(QSize(16, 16));

        horizontalLayout_3->addWidget(getGraphBtn);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 522, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        comboBox->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        ExitBtn->setText(QApplication::translate("MainWindow", "\330\256\330\261\331\210\330\254", Q_NULLPTR));
        startAlgorithmBtn->setText(QApplication::translate("MainWindow", "\330\253\330\250\330\252", Q_NULLPTR));
        comboBox->setCurrentText(QString());
        label->setText(QApplication::translate("MainWindow", "\331\206\331\210\330\271 \330\247\331\204\332\257\331\210\330\261\333\214\330\252\331\205", Q_NULLPTR));
        getShapedBtn->setText(QApplication::translate("MainWindow", "\330\257\330\261\333\214\330\247\331\201\330\252 \331\201\330\247\333\214\331\204 \332\206\331\206\330\257 \330\266\331\204\330\271\333\214", Q_NULLPTR));
        getGraphBtn->setText(QApplication::translate("MainWindow", "\330\257\330\261\333\214\330\247\331\201\330\252 \331\201\330\247\333\214\331\204 \332\257\330\261\330\247\331\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
