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
    QString getMeshText();

    void setMesh(std::vector<Node> nodes);
    void addNodeToMesh(Node node);
    void setMeshText(QString text);


private :

    std::vector<Node> meshNodeList;
    QString meshText;


};

#endif // MESH_H
