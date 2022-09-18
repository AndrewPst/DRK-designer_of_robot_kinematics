#include "serialManipulator/serialManipulatorProject.h"
#include "serialManipulator/centralDocks/glcentraldock.h"
#include "serialManipulator/docks/manybuttonsdock.h"
#include "serialManipulator/docks/manipulatorstructureeditordock.h"

using namespace serialMan;

SerialManipulatorProject::SerialManipulatorProject()
    : BaseProjectController(), _manipulatorController(new ManipulatorController)
{
    _avaiableCentralDocks << new glCentralDock(tr("Visualization"));
    _avaiableDocks << new ManyButtonsDock(tr("Buttons"));
    _avaiableDocks << new ManipulatorStructureEditorDock(tr("Structure"));
}

SerialManipulatorProject::~SerialManipulatorProject()
{
    delete _manipulatorController;
}


ManipulatorController* SerialManipulatorProject::getManipulatorController()
{
    return _manipulatorController;
}

