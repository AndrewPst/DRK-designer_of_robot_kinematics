#include "projectsmanager.h"
#include "projectCore/baseprojectcontroller.h"
#include <QDebug>

ProjectsManager& projectsManager = ProjectsManager::getInstance();

ProjectsManager::ProjectsManager()
{

}

ProjectsManager& ProjectsManager::getInstance()
{
    static ProjectsManager pm;
    return pm;
}

void ProjectsManager::createNewProject(const ProjectMetadata& meta, const QString& name)
{
    _openedProject = meta.generate();
    if(_openedProject == nullptr)
        return;
    _openedProject->init();
    _openedProject->setName(name);
    emit onProjectOpened(_openedProject);
}

const QVector<ProjectMetadata>& ProjectsManager::getAvailablesControllers() const
{
    return _projectsContollers;
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

void ProjectsManager::saveProject()
{
    qDebug() << "Save by default path" << '\n';
}

void ProjectsManager::saveProjectByPath(const QString& path)
{
    qDebug() << "Save by " << path << '\n';
}
