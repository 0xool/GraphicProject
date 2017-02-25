#include "mesh.h"

Mesh::Mesh()
{

}

QMatrix2x2 Mesh::getMeshMatrix()
{

    return this->meshMatrix;

}

void Mesh::setMeshMatrix(QMatrix2x2 matrix)
{

    this->meshMatrix = matrix;

}

void Mesh::parseMesh(){

}



