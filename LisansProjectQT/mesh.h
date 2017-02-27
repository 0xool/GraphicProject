#ifndef MESH_H
#define MESH_H

#include <QMatrix2x2>
#include <QString>


class Mesh
{

public:

    Mesh();

    QMatrix2x2 getMeshMatrix();
    QString getMeshText();

    void setMeshMatrix(QMatrix2x2);
    void parseMesh();
    void setMeshText(QString text);


private :

    QString meshText;
    QMatrix2x2 _meshMatrix;
    QMatrix2x2 meshMatrix;

};

#endif // MESH_H
