#include "projectCore/projectsmanager.h"
#include "projectCore/projectControllers/serialmanipulatorprojectcontroller.h"

void ProjectsManager::initAvailableControllers()
{
    _projectsDictionary[ProjectType_t::PROJECT_SERIAL_MANIPULATOR] = tr("Serial manipulator");
}

BaseProjectController* ProjectsManager::getProjectByType(const ProjectType_t type)
{
    switch (type) {
    case ProjectType_t::PROJECT_SERIAL_MANIPULATOR:
        return new SerialManipulatorProjectController();
    default:
        return nullptr;
    }
}
