#include "projectmanager.h"

void ProjectManager::setDebug(bool debug)
{

    this->DEBUG = debug;

}

//================================================================================================================================================
bool ProjectManager::getDebug()
{
    return this->DEBUG;
}

//================================================================================================================================================
Mesh ProjectManager::getMesh()
{
    return this->mesh;
}

//================================================================================================================================================
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
OuterShape ProjectManager::getOuterShape()
{
    return this->outerShape;
}

//================================================================================================================================================
void ProjectManager::setOuterShape(OuterShape outerShape)
{
    this->outerShape = outerShape;
}

//================================================================================================================================================
void ProjectManager::setInnserShape(InnerShape innerShape)
{
    this->innerShape = innerShape;
}

//================================================================================================================================================
InnerShape ProjectManager::getInnserShape()
{
    return this->innerShape;
}
