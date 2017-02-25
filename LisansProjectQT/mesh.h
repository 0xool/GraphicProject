#ifndef MESH_H
#define MESH_H

#include <QMatrix2x2>


class Mesh
{
public:
    Mesh();
    QMatrix2x2 meshMatrix;
    void setMeshMatrix(QMatrix2x2);
    QMatrix2x2 getMeshMatrix();
    void parseMesh();

private :
    QMatrix2x2 _meshMatrix;

};

#endif // MESH_H
