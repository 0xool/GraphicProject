#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QString>

class ProjectManager
{
public:
   QString DEBUG = "true";

  static ProjectManager& instance()
  {
    static ProjectManager *instance = new ProjectManager();
    return *instance;
  }

private:
  ProjectManager() {}

};

#endif // PROJECTMANAGER_H
