#ifndef PROJECTSMANAGER_H
#define PROJECTSMANAGER_H

#include "projectCore/projectType.h"

#include <QObject>


QT_FORWARD_DECLARE_CLASS(BaseProjectController);

class ProjectsManager : QObject
{
    Q_OBJECT
private:
    ProjectsManager() = default;
    ProjectsManager(const ProjectsManager&) = delete;
    ProjectsManager& operator=(const ProjectsManager&) = delete;

public:

    static ProjectsManager& getInstance();

    BaseProjectController* getOpenedProject() const;

    void createNewProject(const ProjectType_t);

signals:

    void onProjectOpened(BaseProjectController* const);

private:

    BaseProjectController* _openedProject;

};

extern ProjectsManager& projectsManager;

#endif // PROJECTSMANAGER_H
