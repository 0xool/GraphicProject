#ifndef OUTERSHAPE_H
#define OUTERSHAPE_H

#include <QMatrix2x2>
#include <QString>

class OuterShape
{
public:

    OuterShape();


    void setShapeMatrix(QMatrix2x2 shape);
    void setOuterShapeText(QString text);

    QString getOuterShapeText();
    QMatrix2x2 getShapeMatrix();

private:

    QString outerShapeText;
    QMatrix2x2 shape;

};

#endif // OUTERSHAPE_H
