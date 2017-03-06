#ifndef INNERSHAPE_H
#define INNERSHAPE_H

#include <vector>

class InnerShape
{
public:
    InnerShape();

    std::vector<std::vector<float>> innerShapes;


    void setInnerShapes(std::vector<std::vector<float>> shapes);
    void addShapeToInnerShape(int vectorNumber , float x , float y);
    void addShapeListToInnerShape(std::vector<float> shapeList);

    std::vector<float> getShapeByIndex(int index);
    std::vector<std::vector<float>> getShapes();


};

#endif // INNERSHAPE_H
