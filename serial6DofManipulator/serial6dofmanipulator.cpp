#include "serial6dofmanipulator.h"

#include "centralDocks/glcentraldock.h"
#include "docks/manybuttonsdock.h"
#include "docks/dhtabledock.h"
#include "docks/kinematicsdock.h"

#include "openGL/projectvisualizator.h"
#include "logic/manipulatorcontroller.h"


using namespace serialMan;

Serial6DofManipulator::Serial6DofManipulator()
    : BaseProjectController(), _manipulatorController(new ManipulatorController),
      _projectVisualizator(new ProjectVisualizator)
{

}

void Serial6DofManipulator::init()
{
    //Init widgets here!!
    _avaiableCentralDocks << new glCentralDock(tr("Visualization"), _manipulatorController);
    _avaiableDocks << new ManyButtonsDock(tr("Buttons"));
    _avaiableDocks << new DhTableDock(tr("DH parameters"), _manipulatorController);
    _avaiableDocks << new KinematicsDock(_manipulatorController);
}

Serial6DofManipulator::~Serial6DofManipulator()
{
    delete _manipulatorController;
    delete _projectVisualizator;
}

ManipulatorController* Serial6DofManipulator::getManipulatorController() const
{
    return _manipulatorController;
}

ProjectVisualizator* Serial6DofManipulator::getVisualizator() const
{
    return _projectVisualizator;
}

