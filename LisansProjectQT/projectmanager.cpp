#include "projectmanager.h"



void ProjectManager::setDebug(bool debug)
{

    this->DEBUG = debug;

}

//================================================================================================================================================
// Debug mode for project testing.
bool ProjectManager::getDebug()
{
    return this->DEBUG;
}

//================================================================================================================================================
// get Graph overView from singleton class
Mesh ProjectManager::getMesh()
{
    return this->mesh;
}

//================================================================================================================================================
// Set Graph overview from within the singleton.
void ProjectManager::setMesh(Mesh mesh)
{
    this->mesh = mesh;

    //DEBUG
    if (DEBUG)
    {
        qDebug(this->mesh.getMeshText().toLatin1());

    }
}

//================================================================================================================================================
// Get shape which the graph is to be set in.
OuterShape ProjectManager::getOuterShape()
{
    return this->outerShape;
}

//================================================================================================================================================
// Set shape which the graph is to be set in.
void ProjectManager::setOuterShape(OuterShape outerShape)
{
    this->outerShape = outerShape;
}

//================================================================================================================================================
// Set innerShapes which create the holes for the project.
void ProjectManager::setInnserShape(InnerShape innerShape)
{
    this->innerShape = innerShape;
}

//================================================================================================================================================
// Get innerShapes which create the holes for the project.
InnerShape ProjectManager::getInnserShape()
{
    return this->innerShape;
}
//================================================================================================================================================
// Get algorithm to be used for implementing graph localozation.
Algorithm ProjectManager::getAlgorithm()
{
    return this->algorithm;
}
//================================================================================================================================================
// Set algorithm to be used for implementing graph localozation.
void ProjectManager::setAlgorithm(enum Algorithm algorithm)
{
    this->algorithm = algorithm;
}
//================================================================================================================================================
// Optional Algorithm initilization(propobly wont use this one!!!!).
void ProjectManager::Algorithm()
{

    switch (this->algorithm) {
    case SA:
        //here we implement the SA algorithm initilazation.
        break;
    case GA:
        //here we implement the GA algorithm initilization.
        break;
    default:
        break;
    }

}
//================================================================================================================================================



//================================================================================================================================================
