#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QString>
#include "mesh.h"
#include "innershape.h"
#include "outershape.h"

//
//Summary : This class is singleton implemenation of the project only one instance of this class exists only for managing this project
//
class ProjectManager
{
public:

  //getter :
  bool getDebug();
  Mesh getMesh();
  InnerShape getInnserShape();
  OuterShape getOuterShape();

  //setter :
  void setDebug(bool debug);
  void setMesh(Mesh mesh);
  void setInnserShape(InnerShape innerShape);
  void setOuterShape(OuterShape outerShape);



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

  ProjectManager() {}

};

#endif // PROJECTMANAGER_H
