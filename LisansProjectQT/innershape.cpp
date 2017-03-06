#include "innershape.h"

InnerShape::InnerShape()
{

}

std::vector<std::vector<float>> InnerShape::getShapes()
{

    return this->innerShapes;

}

std::vector<float> InnerShape::getShapeByIndex(int index)
{

    return this->innerShapes[index];

}

void InnerShape::addShapeListToInnerShape(std::vector<float> shapeList)
{

    this->innerShapes.push_back(shapeList);

}

void InnerShape::addShapeToInnerShape(int vectorNumber, float x, float y)
{

    std::vector<float> tmp = this->innerShapes[vectorNumber];
    tmp.push_back(x);
    tmp.push_back(y);

    this->innerShapes[vectorNumber] = tmp;

}

void InnerShape::setInnerShapes(std::vector<std::vector<float> > shapes)
{

    this->innerShapes = shapes;

}
