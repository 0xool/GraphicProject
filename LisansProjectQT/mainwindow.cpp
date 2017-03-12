#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

#include "QMessageBox"
#include"QTextStream"
#include "QFileDialog"

#include "openglview.h"
#include "mesh.h"
#include "projectmanager.h"

#include "outershapeuserinput.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("GA");
    ui->comboBox->addItem("SA");
}

//================================================================================================================================================
MainWindow::~MainWindow()
{

    delete ui;
}

//================================================================================================================================================
void MainWindow::on_ExitBtn_clicked()
{

    QCoreApplication::quit();

}

//================================================================================================================================================
void MainWindow::on_startAlgorithmBtn_clicked()
{

    openGlView OV;
    QPalette pal = palette();

    // set black background
    pal.setColor(QPalette::Background, Qt::yellow);
    OV.setAutoFillBackground(true);
    OV.setPalette(pal);
    OV.setModal(true);
    OV.exec();

}

//================================================================================================================================================
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

            QString fileText = in.readAll();

            ProjectManager *pm;
            Mesh mesh = pm->instance().getMesh();
            mesh.setMeshText(fileText);
            pm->instance().setMesh(mesh);

            ui->graphTextEdit->setText(fileText);
            file.close();
        }

}

//================================================================================================================================================
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

            QString fileText = in.readAll();

            ProjectManager *pm;
            OuterShape outerShape = pm->instance().getOuterShape();
            outerShape.setOuterShapeText(fileText);
            pm->instance().setOuterShape(outerShape);


            ui->shapeTextEdit->setText(fileText);
            file.close();
        }

}

//================================================================================================================================================
void MainWindow::parseInnerShapeData(QString text)
{




}

//================================================================================================================================================
void MainWindow::parseMeshData(QString text)
{




}

void MainWindow::on_pushButton_clicked()
{

    OuterShapeUserInput OV;
    QPalette pal = palette();

    // set black background
    pal.setColor(QPalette::Background, Qt::white);
    OV.setAutoFillBackground(true);
    OV.setPalette(pal);
    OV.setModal(true);
    OV.exec();

}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{

    ProjectManager *pm;
    Algorithm algorithm;
    if (pm->instance().getDebug())
    {
        qDebug() << index;
    }

    switch (index) {
    case 0:
        algorithm = GA;
        pm->instance().setAlgorithm(algorithm);
        break;
    case 1:
        algorithm = SA;
        pm->instance().setAlgorithm(algorithm);
        break;
    default:
        break;
    }




}
