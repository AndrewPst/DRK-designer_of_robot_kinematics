#include "projectsmanager.h"
#include "projectCore/projectControllers/serialmanipulatorprojectcontroller.h"

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

void ProjectsManager::createNewProject(const ProjectType_t type)
{
    //TODO make type-class system
    _openedProject = getProjectByType(type);
    if(_openedProject == nullptr)
        return;

    _openedProject->setName(tr("Project test"));
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
