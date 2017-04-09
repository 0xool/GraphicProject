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

    void setMesh(std::vector<Node> nodes);
    void addNodeToMesh(Node node);
    void setMeshText(QString text);

    void addNodePointerToMesh(Node* node);


private :

    std::vector<Node> meshNodeList;
    std::vector<Node*> meshPointerNodeList;
    QString meshText;


};

#endif // MESH_H
