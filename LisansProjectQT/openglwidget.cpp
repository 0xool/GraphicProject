#include "openglwidget.h"
#include <QDebug>

OpenGlWidget::OpenGlWidget(QWidget *parnet) : QOpenGLWidget(parnet)
{

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    setFormat(format);
    QPalette pal = palette();

    qDebug("init the opengl");

    // set black background
    pal.setColor(QPalette::Background, Qt::blue);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

}
//==========================================================================================================================================

OpenGlWidget::~OpenGlWidget()
{


}
//==========================================================================================================================================

void OpenGlWidget::initializeGL(){

    glClearColor(0,0,0,1);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);

}
//==========================================================================================================================================

void OpenGlWidget::resizeGL(int w, int h)
{

    glViewport(0,0,w,h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45, (float)w/h, 0.01, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0,0,5,0,0,0,0,1,0);

}
//==========================================================================================================================================

void OpenGlWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(-0.5, -0.5, 0);
            glColor3f(0.0, 1.0, 0.0);
            glVertex3f( 0.5, -0.5, 0);
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f( 0.0,  0.5, 0);
        glEnd();

}





