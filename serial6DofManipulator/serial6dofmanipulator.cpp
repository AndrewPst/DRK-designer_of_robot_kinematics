#include "serial6dofmanipulator.h"

#include "centralDocks/glcentraldock.h"
//#include "docks/manybuttonsdock.h"
#include "docks/dhtabledock.h"
#include "docks/kinematicsdock.h"
#include "docks/programdock.h"

#include "openGL/projectvisualizator.h"
#include "logic/manipulatorcontroller.h"
#include "logic/actionscontroller.h"

#include "widgets/gcodeserializatorwidget.h"

using namespace serialMan;

Serial6DofManipulator::Serial6DofManipulator()
    : BaseProjectController(),
      _manipulatorController(new ManipulatorController()),
      _projectVisualizator(new gl::ProjectVisualizator),
      _actionsController(new ActionsController(*_manipulatorController)){}

void Serial6DofManipulator::init()
{
    //Init widgets here!!
    _avaiableCentralDocks << new centralDocks::glCentralDock(*_manipulatorController);
    //_avaiableDocks << new ManyButtonsDock(tr("Buttons"));

    _avaiableDocks << new docks::ProgramDock(*_actionsController);
    _avaiableDocks << new docks::DhTableDock(*_manipulatorController);
    _avaiableDocks << new docks::KinematicsDock(*_manipulatorController);

    auto exportGCode = new QAction("Export GCODE file");
    connect(exportGCode, SIGNAL(triggered(bool)), this, SLOT(onExportGCodeCall()));

    auto importGcode = new QAction("Inport GCODE file");
    connect(importGcode, SIGNAL(triggered(bool)), this, SLOT(onImportGcodeCall()));

    auto configJSON = new QAction("Export JSON config file");
    connect(configJSON, SIGNAL(triggered(bool)), this, SLOT(onExportJsonConfigCall()));

    _editMenu->addSeparator();
    _editMenu->addAction(exportGCode);
    _editMenu->addAction(importGcode);
    _editMenu->addSeparator();
    _editMenu->addAction(configJSON);
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

gl::ProjectVisualizator& Serial6DofManipulator::getVisualizator() const
{
    return *_projectVisualizator;
}

void Serial6DofManipulator::onExportGCodeCall()
{
    widgets::gcodeSerializatorWidget gcodeW(_mainWindow);
    gcodeW.setProgram(*_actionsController);
    gcodeW.show();
    gcodeW.exec();
//    QFileDialog fDialog;
//    QString name = fDialog.getSaveFileName(_mainWindow, "Open file", "/program", "GCODE file(*.gcode)");
}

void Serial6DofManipulator::onImportGcodeCall()
{
    qDebug() << "Import gcode";
}



void Serial6DofManipulator::onExportJsonConfigCall()
{
    qDebug() << "Export config";
}


