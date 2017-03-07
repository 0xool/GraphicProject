#ifndef OUTERSHAPEINPUTOPENGL_H
#define OUTERSHAPEINPUTOPENGL_H

#include <QOpenGLWidget>
#include <QWidget>
#include <QOpenGLFunctions>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <vector>

class OuterShapeInputOpenGL : public QOpenGLWidget
{

public:
    OuterShapeInputOpenGL(QWidget *parent = 0);
    ~OuterShapeInputOpenGL();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);

private:

    std::vector<float> dataVector;

};

#endif // OUTERSHAPEINPUTOPENGL_H
