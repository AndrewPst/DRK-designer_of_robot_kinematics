#include "serialManipulator/serialManipulatorController.h"
#include "serialManipulator/centralDocks/glcentraldock.h"
#include "serialManipulator/docks/manybuttonsdock.h"

SerialManipulatorProjectController::SerialManipulatorProjectController() : BaseProjectController()
{
    _avaiableCentralDocks << new glCentralDock(tr("Visualization"));
    _avaiableDocks << new ManyButtonsDock(tr("Buttons"));
}


