#include "mesh.h"

Mesh::Mesh()
{

}
//=============================================================================================================================
void Mesh::addNodeToMesh(Node node)
{
    this->meshNodeList.push_back(node);
}
//=============================================================================================================================
std::vector<Node> Mesh::getMesh()
{
    return this->meshNodeList;
}
//=============================================================================================================================
void Mesh::setMesh(std::vector<Node> nodes)
{
    this->meshNodeList = nodes;
}
//=============================================================================================================================
void Mesh::setMeshText(QString text)
{
    this->meshText = text;
}
//=============================================================================================================================
QString Mesh::getMeshText()
{
    return this->meshText;
}

