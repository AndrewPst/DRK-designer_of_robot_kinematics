#include "projectsmanager.h"
#include "projectCore/baseprojectcontroller.h"

ProjectsManager& projectsManager = ProjectsManager::getInstance();

ProjectsManager::ProjectsManager()
{
    initAvailableControllers();
}

ProjectsManager& ProjectsManager::getInstance()
{
    static ProjectsManager pm;
    return pm;
}

void ProjectsManager::createNewProject(const ProjectType_t type, const QString& name)
{
    _openedProject = getProjectByType(type);
    if(_openedProject == nullptr)
        return;
    _openedProject->setName(name);
    emit onProjectOpened(_openedProject);
}

const QMap<ProjectType_t, QString>& ProjectsManager::getAvailablesControllers() const
{
    return _projectsDictionary;
}


BaseProjectController* ProjectsManager::getOpenedProject() const
{
    return _openedProject;
}
void ProjectsManager::closeProject()
{
    if(!_openedProject)
        return;
    emit onProjectClosed(_openedProject);
    _openedProject->deleteLater();
    _openedProject = NULL;
}
