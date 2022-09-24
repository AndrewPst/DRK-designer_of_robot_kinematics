#include "serialManipulator/serialManipulatorProject.h"
#include "serialManipulator/centralDocks/glcentraldock.h"
#include "serialManipulator/docks/manybuttonsdock.h"
#include "serialManipulator/docks/manipulatorstructureeditordock.h"

#include "openGL/projectvisualizator.h"
#include "logic/manipulatorcontroller.h"

using namespace serialMan;

SerialManipulatorProject::SerialManipulatorProject()
    : BaseProjectController(), _manipulatorController(new ManipulatorController),
      _projectVisualizator(new ProjectVisualizator)
{
}

void SerialManipulatorProject::init()
{
    //Init widgets here!!
    _avaiableCentralDocks << new glCentralDock(tr("Visualization"));
    _avaiableDocks << new ManyButtonsDock(tr("Buttons"));
    _avaiableDocks << new ManipulatorStructureEditorDock(tr("Structure"));
}

SerialManipulatorProject::~SerialManipulatorProject()
{
    delete _manipulatorController;
    delete _projectVisualizator;
}


ManipulatorController* SerialManipulatorProject::getManipulatorController() const
{
    return _manipulatorController;
}

ProjectVisualizator* SerialManipulatorProject::getVisualizator() const
{
    return _projectVisualizator;
}

