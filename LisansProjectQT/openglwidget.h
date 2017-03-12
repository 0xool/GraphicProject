#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QWidget>
#include <QOpenGLFunctions>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <vector>

#include "innershape.h"

class OpenGlWidget : public QOpenGLWidget
{


public:
    OpenGlWidget(QWidget *parent = 0);
    ~OpenGlWidget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);

private:

    std::vector<float> outerShapeData;
    InnerShape innerShape;

};

#endif // OPENGLWIDGET_H
