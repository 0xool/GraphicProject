#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QString>
#include "mesh.h"
#include "innershape.h"

#include "outershape.h"
#include <functional>
#include <openglwidget.h>

enum Algorithm {

    SA,
    GA

};


//
//Summary : This class is singleton implemenation of the project only one instance of this class exists only for managing this project
//
class ProjectManager
{
public:

    OpenGlWidget* mainAlgorithmView;

  //getter :
  bool getDebug();
  Mesh getMesh();
  InnerShape getInnserShape();

  OuterShape getOuterShape();
  Algorithm getAlgorithm();
  void Algorithm();

  Mesh getSolutionMesh();


  //setter :
  void setDebug(bool debug);
  void setMesh(Mesh mesh);
  void setInnserShape(InnerShape innerShape);

  void setOuterShape(OuterShape outerShape);
  void setAlgorithm(enum Algorithm algorithm);
  void setSolutionMesh(Mesh mesh);

  //singleton implimantation
  static ProjectManager& instance()
  {
    static ProjectManager *instance = new ProjectManager();
    return *instance;
  }



private:

  bool DEBUG = false;
  Mesh mesh;
  InnerShape innerShape;

  OuterShape outerShape;
  enum Algorithm algorithm = GA;
  Mesh solutionMesh;

  ProjectManager() {

  }

};

#endif // PROJECTMANAGER_H
