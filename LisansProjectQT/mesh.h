#ifndef MESH_H
#define MESH_H

#include <vector>
#include "node.h"
#include <QString>


class Mesh
{

public:

    Mesh();

    std::vector<Node> getMesh();
    std::vector<Node*> getMeshPointer();
    QString getMeshText();

    int getCost();
    bool isMeshAcaptable();

    void setMesh(std::vector<Node> nodes);
    void addNodeToMesh(Node node);
    void setMeshText(QString text);

    void addNodePointerToMesh(Node* node);
    void setCost(int cost);
    void setHasIntersectionWithOuterSHape(bool value);



private :

    std::vector<Node> meshNodeList;
    std::vector<Node*> meshPointerNodeList;
    QString meshText;

    int cost = 0;
    bool hasIntersectionWithOutershape = false;


};

#endif // MESH_H
