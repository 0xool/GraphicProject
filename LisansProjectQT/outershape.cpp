#include "outershape.h"

OuterShape::OuterShape()
{

}

//================================================================================================================================================
QMatrix2x2 OuterShape::getShapeMatrix()
{
    return this->shape;
}

//================================================================================================================================================
void OuterShape::setShapeMatrix(QMatrix2x2 shapeMatrix)
{

    this->shape = shapeMatrix;

}

//================================================================================================================================================
void OuterShape::setOuterShapeText(QString text)
{
    this->outerShapeText = text;
}

//================================================================================================================================================
QString OuterShape::getOuterShapeText()
{
    return this->outerShapeText;
}



