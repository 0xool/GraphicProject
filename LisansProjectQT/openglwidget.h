#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QWidget>
#include <QOpenGLFunctions>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <QMatrix4x4>

class OpenGlWidget : public QOpenGLWidget
{


public:
    OpenGlWidget(QWidget *parent = 0);
    ~OpenGlWidget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    QMatrix4x4 m_projection;

};

#endif // OPENGLWIDGET_H
