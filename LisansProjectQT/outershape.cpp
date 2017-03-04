#include "outershape.h"

OuterShape::OuterShape()
{

}

//================================================================================================================================================
std::vector<float> OuterShape::getShapeMatrix()
{
    return this->shape;
}

//================================================================================================================================================
void OuterShape::setShapeMatrix(std::vector<float> shapeMatrix)
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



