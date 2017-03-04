#include "outershapeinputopengl.h"
#include "mesh.h"
#include "projectmanager.h"
#include <QDebug>
#include <QMouseEvent>



OuterShapeInputOpenGL::OuterShapeInputOpenGL(QWidget *parnet) : QOpenGLWidget(parnet)
{

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    setFormat(format);
    QPalette pal = palette();
    // set black background
    pal.setColor(QPalette::Background, Qt::blue);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

}

//==========================================================================================================================================

OuterShapeInputOpenGL::~OuterShapeInputOpenGL()
{


}

//==========================================================================================================================================

void OuterShapeInputOpenGL::initializeGL(){

    glClearColor(0,0,0,1);

}
//==========================================================================================================================================

void OuterShapeInputOpenGL::resizeGL(int w, int h)
{

    glViewport(0,0,w,h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
//        gluOrtho2D(0, width(), 0, height());
        glOrtho(0, w, h, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
//        gluLookAt(0,0,5,0,0,0,0,1,0);


}
//==========================================================================================================================================

void OuterShapeInputOpenGL::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT);

        glEnableClientState(GL_VERTEX_ARRAY);
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        float tmp[60];
        std::copy(this->dataVector.begin() , this->dataVector.end() , tmp);
        glVertexPointer(2 , GL_FLOAT , 0 , tmp);
        glDrawArrays(GL_POLYGON , 0 , this->dataVector.size() / 2);
        glDisableClientState(GL_VERTEX_ARRAY);

}

void OuterShapeInputOpenGL::mousePressEvent(QMouseEvent *event)
{
    int testX = mapFromGlobal(QCursor::pos()).x();
    int testY = mapFromGlobal(QCursor::pos()).y();


    this->dataVector.push_back(float(testX));
    this->dataVector.push_back(float(testY));


    qDebug() << testX << "  " << testY;

   ProjectManager *pm;
   OuterShape OS = pm->instance().getOuterShape();
   OS.setShapeMatrix(this->dataVector);
   pm->instance().setOuterShape(OS);
    update();

    for (float &f : this->dataVector){
        qDebug() << f;
    }

}





