#ifndef OUTERSHAPE_H
#define OUTERSHAPE_H

#include <QMatrix2x2>

class OuterShape
{
public:
    OuterShape();
    QMatrix2x2 shape;
    void setShapeMatrix(QMatrix2x2 shape);

    QMatrix2x2 getShapeMatrix();


};

#endif // OUTERSHAPE_H
