#include "serialManipulator/serialManipulatorProject.h"
#include "serialManipulator/centralDocks/glcentraldock.h"
#include "serialManipulator/docks/manybuttonsdock.h"

using namespace serialMan;

SerialManipulatorProject::SerialManipulatorProject() : BaseProjectController()
{
    _avaiableCentralDocks << new glCentralDock(tr("Visualization"));
    _avaiableDocks << new ManyButtonsDock(tr("Buttons"));
}


