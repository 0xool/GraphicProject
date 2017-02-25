#ifndef INNERSHAPE_H
#define INNERSHAPE_H

#include <QMatrix2x2>

class InnerShape
{
public:
    InnerShape();
    QMatrix2x2 shapes[15];
    void setInnerShapes(QMatrix2x2 shapes[]);

    QMatrix2x2 getInnerShapes(int index);
    QMatrix2x2 getInnserShapeByIndex(int index);

};

#endif // INNERSHAPE_H
