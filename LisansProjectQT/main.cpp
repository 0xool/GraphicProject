#include "mainwindow.h"
#include <QApplication>
#include "iostream"
#include "projectmanager.h"

int main(int argc, char *argv[])
{

    ProjectManager *pm;
    pm->instance().DEBUG = "false";
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
