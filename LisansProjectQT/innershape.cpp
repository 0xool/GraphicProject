#include "innershape.h"

InnerShape::InnerShape()
{

}

void InnerShape::setInnerShapes(QMatrix2x2 shapes[])
{

    memcpy(this->shapes , shapes , sizeof(this->shapes));

}

QMatrix2x2 InnerShape::getInnerShapes(int index)
{

    return this->shapes[index];

}

QMatrix2x2 InnerShape::getInnserShapeByIndex(int index)
{

    return this->shapes[index];

}
