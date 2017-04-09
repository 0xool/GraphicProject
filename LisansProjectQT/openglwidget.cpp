#include "openglwidget.h"
#include "mesh.h"
#include "projectmanager.h"

#include <QDebug>
#include <QMouseEvent>
#include <anealalgorithm.h>

OpenGlWidget::OpenGlWidget(QWidget *parnet) : QOpenGLWidget(parnet)
{

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    setFormat(format);
    QPalette pal = palette();
    // set black background
    pal.setColor(QPalette::Background, Qt::blue);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    AnealAlgorithm AA;
    AA.SimulatedAnnealingForGraph();

}

//==========================================================================================================================================

OpenGlWidget::~OpenGlWidget()
{


}

//==========================================================================================================================================

void OpenGlWidget::initializeGL(){

    glClearColor(0,0,0,1);

}
//==========================================================================================================================================

void OpenGlWidget::resizeGL(int w, int h)
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
void OpenGlWidget::paintGL()
{

    ProjectManager *pm;

        glClear(GL_COLOR_BUFFER_BIT);

            glEnableClientState(GL_VERTEX_ARRAY);
            glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
            float tmp[60];

            this->outerShapeData = pm->instance().getOuterShape().getShapeMatrix();
            std::copy(this->outerShapeData.begin() , this->outerShapeData.end() , tmp);
            glVertexPointer(2 , GL_FLOAT , 0 , tmp);
            glDrawArrays(GL_POLYGON , 0 , this->outerShapeData.size() / 2);
            glDisableClientState(GL_VERTEX_ARRAY);


            this->innerShape = pm->instance().getInnserShape();
            for (std::vector<float> &shape : this->innerShape.getShapes()){
                if (pm->instance().getDebug())
                {

                    qDebug() << this->outerShapeData;
                    qDebug() << shape;
                }

                float innerShapeTmp[60];

                    glEnableClientState(GL_VERTEX_ARRAY);
                    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
                    std::copy(shape.begin() , shape.end() , innerShapeTmp);
                    glVertexPointer(2 , GL_FLOAT , 0 , innerShapeTmp);
                    glDrawArrays(GL_POLYGON , 0 , shape.size() / 2);
                    glDisableClientState(GL_VERTEX_ARRAY);

            }

                glEnableClientState(GL_VERTEX_ARRAY);
                glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
                this->outerShapeData = pm->instance().getOuterShape().getShapeMatrix();
                std::copy(this->outerShapeData.begin() , this->outerShapeData.end() , tmp);
                glVertexPointer(2 , GL_FLOAT , 0 , tmp);
                glDrawArrays(GL_POLYGON , 0 , this->outerShapeData.size() / 2);
                glDisableClientState(GL_VERTEX_ARRAY);


                for (int i = 0 ; i < pm->instance().getMesh().getMeshPointer().size() ; i++)
                {
                    Node tempNode = *(pm->instance().getMesh().getMeshPointer()[i]);
                    qDebug() << tempNode.getChildNodes().size() << "Node child size!!!";
                    if (tempNode.getChildNodes().size() > 0)
                    {
                        for (Node* &node : tempNode.getChildNodes())
                        {
                            glLineWidth(2.5);
                            glColor3f(1.0, 0.0, 0.0);
                            glBegin(GL_LINES);

                            glVertex3f(node->getX(), node->getY(), 0.0);
                            glVertex3f(tempNode.getX(), tempNode.getY(), 0);

                            if (pm->instance().getDebug() == true)
                            {
                                qDebug() << "node X&Y Y:" << node->getX() << "X:" << node->getY()  ;
                                qDebug( "for Node name : " + node->getName().toLatin1());
                                qDebug() << "tempNode X&Y Y:" << tempNode.getX() << "X:" << tempNode.getY()<< "for Node name : ";
                            }

                            glEnd();
                        }
                    }
                }
}
//==========================================================================================================================================
void OpenGlWidget::mousePressEvent(QMouseEvent *event)
{
//    int testX = mapFromGlobal(QCursor::pos()).x();
//    int testY = mapFromGlobal(QCursor::pos()).y();


//    this->outerShapeData.push_back(float(testX));
//    this->outerShapeData.push_back(float(testY));


//    qDebug() << testX << "  " << testY;
//    update();

//    for (float &f : this->outerShapeData){
//        qDebug() << f;
//    }

}



