#include "outershape.h"

OuterShape::OuterShape()
{

    this->xMax = 0;
    this->yMin = std::numeric_limits<double>::infinity();
    this->xMin = std::numeric_limits<double>::infinity();
    this->yMax = 0;


}

//================================================================================================================================================
std::vector<float> OuterShape::getShapeMatrix()
{
    return this->shape;
}

//================================================================================================================================================
void OuterShape::setShapeMatrix(std::vector<float> shapeMatrix)
{

    for (int i = 0 ; i < shapeMatrix.size() / 2 ; i++)
    {
        if (shapeMatrix[2 * i] < xMin)
            xMin = shapeMatrix[2 * i];
        if (shapeMatrix[2 * i] > xMax)
            xMax = shapeMatrix[2 * i];

        if (shapeMatrix[2 * i + 1] < yMin)
            yMin = shapeMatrix[2 * i + 1];
        if (shapeMatrix[2 * i + 1] > yMax)
            yMax = shapeMatrix[2 * i + 1];
    }

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



