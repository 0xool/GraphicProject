#include "mainwindow.h"
#include <QApplication>
#include "iostream"
#include "projectmanager.h"

int main(int argc, char *argv[])
{

    ProjectManager *pm;
    pm->instance().setDebug(true);

    std::vector<float> firstHole = {

        0 , 0,
        20 , 20,
        30 , 30,
        40 , 40

    };

    std::vector<float> secondHole = {

        100 , 120,
        200 , 400,
        220 , 410

    };

    InnerShape IS = pm->getInnserShape();
    IS.addShapeListToInnerShape(firstHole);
    IS.addShapeListToInnerShape(secondHole);

    pm->setInnserShape(IS);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
