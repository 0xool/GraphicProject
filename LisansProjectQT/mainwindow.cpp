#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

#include "QMessageBox"
#include"QTextStream"
#include "QFileDialog"

#include "openglview.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("GA");
    ui->comboBox->addItem("SA");
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::on_ExitBtn_clicked()
{

    QCoreApplication::quit();

}


void MainWindow::on_startAlgorithmBtn_clicked()
{

    openGlView OV;
    OV.setModal(true);
    OV.exec();

}

void MainWindow::on_getGraphBtn_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
                return;
            }
            QTextStream in(&file);
            ui->graphTextEdit->setText(in.readAll());
            file.close();
        }

}

void MainWindow::on_getShapedBtn_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
                return;
            }
            QTextStream in(&file);
            ui->shapeTextEdit->setText(in.readAll());
            file.close();
        }

}
