
#include "mainwindow.h"
#include <QApplication>
#include "iostream"

#include "projectmanager.h"
#include "innershape.h"
#include "node.h"

int main(int argc, char *argv[])
{

    ProjectManager *pm;
    pm->instance().setDebug(true);

    Node aN;
    Node bN;
    Node cN;

    aN.setName("A");
    bN.setName("B");
    cN.setName("C");


    aN.addChildNode(&bN);
    aN.addChildNode(&cN);

    Node *temp = &bN;

    temp->setX(1000);
    temp->setY(1000);
    cN.setX(1000);
    cN.setY(1000);

    bN.setTreeParentNode(&aN);
    cN.setTreeParentNode(&aN);

    Mesh testMesh;
    Node node;
    testMesh.addNodePointerToMesh(&aN);
    testMesh.addNodePointerToMesh(&bN);
    testMesh.addNodePointerToMesh(&cN);

    pm->instance().setMesh(testMesh);

    InnerShape IS;

    for (int i = 0 ; i < 25 ; i++)
    {

        float randomX = rand() % 500;
        float randomY = rand() % 500;
        float XMinus = randomX - 20;
        float Xplus = randomX + 20;
        float Yminus = randomY - 20;
        float Yplus = randomY + 20;




        std::vector<float> hole = {

            XMinus , Yminus,
            Xplus , Yminus,
            Xplus , Yplus,
            XMinus , Yplus
        };


        IS.addShapeListToInnerShape(hole);

    }


    std::vector<float> firstHole = {

        10 , 10,
        30 , 10,
        30 , 30,
        10 , 30

    };

    std::vector<float> secondHole = {

        30 , 30,
        50 , 30,
        50 , 50,
        30 , 50

    };
    std::vector<float> thirdHole = {

        50 , 50,
        70 , 50,
        70 , 70,
        50 , 70

    };
    std::vector<float> forthHole = {

        70 , 70,
        90 , 70,
        90 , 90,
        70 , 90

    };
    std::vector<float> fifthHole = {

        90 , 90,
        110 , 90,
        110 , 110,
        90 , 110

    };


    IS.addShapeListToInnerShape(secondHole);
    IS.addShapeListToInnerShape(thirdHole);
    IS.addShapeListToInnerShape(forthHole);
    IS.addShapeListToInnerShape(fifthHole);

    pm->instance().setInnserShape(IS);


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
