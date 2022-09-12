#include "serialmanipulatorprojectcontroller.h"
#include "CentralDocks/glcentraldock.h"
#include <dockWidgets/manybuttonsdock.h>

SerialManipulatorProjectController::SerialManipulatorProjectController() : BaseProjectController()
{
    _avaiableCentralDocks << new glCentralDock(tr("Visualization"));
    _avaiableDocks << new ManyButtonsDock(tr("Buttons"));
}


