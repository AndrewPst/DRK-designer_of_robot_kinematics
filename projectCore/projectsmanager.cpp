#include "projectsmanager.h"
#include "projectCore/projectControllers/serialmanipulatorprojectcontroller.h"

ProjectsManager& projectsManager = ProjectsManager::getInstance();

ProjectsManager& ProjectsManager::getInstance()
{
    static ProjectsManager pm;
    return pm;
}

void ProjectsManager::createNewProject(const ProjectType_t type)
{
    //TODO make type-class system

    switch (type) {
    case ProjectType_t::PROJECT_SERIAL_MANIPULATOR:
        _openedProject = new SerialManipulatorProjectController();
        break;
    default:
        break;
    }

    emit onProjectOpened(_openedProject);
}

BaseProjectController* ProjectsManager::getOpenedProject() const
{
    return _openedProject;
}
