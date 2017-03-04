#ifndef OUTERSHAPE_H
#define OUTERSHAPE_H

#include <vector>
#include <QString>

class OuterShape
{
public:

    OuterShape();


    void setShapeMatrix(std::vector<float> shape);
    void setOuterShapeText(QString text);

    QString getOuterShapeText();
    std::vector<float> getShapeMatrix();

private:

    QString outerShapeText;
    std::vector<float> shape;

};

#endif // OUTERSHAPE_H
