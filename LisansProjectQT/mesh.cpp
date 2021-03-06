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
void Mesh::addNodePointerToMesh(Node* node)
{
    this->meshPointerNodeList.push_back(node);
}
//=============================================================================================================================
std::vector<Node*> Mesh::getMeshPointer()
{
    return this->meshPointerNodeList;
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
//=============================================================================================================================
void Mesh::setCost(int cost)
{
    this->cost = cost;
}
//=============================================================================================================================
int Mesh::getCost()
{
    return this->cost;
}
//=============================================================================================================================
void Mesh::setHasIntersectionWithOuterSHape(bool value)
{
    this->hasIntersectionWithOutershape = value;
}
//=============================================================================================================================
bool Mesh::isMeshAcaptable()
{
 return !hasIntersectionWithOutershape;
}
//=============================================================================================================================
