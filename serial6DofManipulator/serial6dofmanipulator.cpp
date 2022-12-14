#include "serial6dofmanipulator.h"

#include "centralDocks/glcentraldock.h"
//#include "docks/manybuttonsdock.h"
#include "docks/dhtabledock.h"
#include "docks/kinematicsdock.h"
#include "docks/programdock.h"

#include "openGL/projectvisualizator.h"
#include "logic/manipulatorcontroller.h"
#include "logic/actionscontroller.h"


using namespace serialMan;

Serial6DofManipulator::Serial6DofManipulator()
    : BaseProjectController(),
      _manipulatorController(new ManipulatorController()),
      _projectVisualizator(new ProjectVisualizator),
      _actionsController(new ActionsController(*_manipulatorController))

{
}

void Serial6DofManipulator::init()
{

    //Init widgets here!!
    _avaiableCentralDocks << new glCentralDock(*_manipulatorController);
    //_avaiableDocks << new ManyButtonsDock(tr("Buttons"));

    _avaiableDocks << new ProgramDock(*_actionsController);
    _avaiableDocks << new DhTableDock(*_manipulatorController);
    _avaiableDocks << new KinematicsDock(*_manipulatorController);
}

Serial6DofManipulator::~Serial6DofManipulator()
{

}

ManipulatorController& Serial6DofManipulator::getManipulatorController() const
{
    return *_manipulatorController;
}

ActionsController& Serial6DofManipulator::getActionsController() const
{
    return *_actionsController;
}

ProjectVisualizator& Serial6DofManipulator::getVisualizator() const
{
    return *_projectVisualizator;
}

