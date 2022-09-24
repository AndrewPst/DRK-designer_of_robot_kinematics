#include "projectCore/projectsmanager.h"
#include "serialManipulator/serialManipulatorProject.h"
#include "projectGenerator.h"
#include "projectMetadata.h"

//Add your project controller here!!!


QVector<ProjectMetadata> ProjectsManager::_projectsContollers
{
    ProjectMetadata(tr("Serial manipulator"), GENERATE(serialMan::SerialManipulatorProject))
};
