#include "projectCore/projectsmanager.h"
#include "serial6DofManipulator/serial6dofmanipulator.h"
#include "projectGenerator.h"
#include "projectMetadata.h"

//Add your project controller here!!!

QVector<ProjectMetadata> ProjectsManager::_projectsContollers
{
    ProjectMetadata(tr("Serial manipulator"), GENERATE(serialMan::Serial6DofManipulator))
};
