#include "projectCore/projectsmanager.h"
#include "serialManipulator/serialManipulatorProject.h"

//Add your project controller here!!!

void ProjectsManager::initAvailableControllers()
{
    //Adding to dictionary availables projects
    _projectsDictionary[ProjectType_t::PROJECT_SERIAL_MANIPULATOR] = tr("Serial manipulator");
}

BaseProjectController* ProjectsManager::getProjectByType(const ProjectType_t type)
{
    switch (type) {
    case ProjectType_t::PROJECT_SERIAL_MANIPULATOR:
        return new serialMan::SerialManipulatorProject();
    default:
        return nullptr;
    }
}
