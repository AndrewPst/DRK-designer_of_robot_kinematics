#include "serial6dofmanipulator.h"

#include "centralDocks/glcentraldock.h"
//#include "docks/manybuttonsdock.h"
#include "docks/dhtabledock.h"
#include "docks/kinematicsdock.h"
#include "docks/programdock.h"

#include "messageWindows/ConfirmActionDialog.h"
#include "openGL/projectvisualizator.h"
#include "logic/manipulatorcontroller.h"
#include "logic/actionscontroller.h"

#include "widgets/gcodeserializatorwidget.h"
#include "widgets/gcodedeserializatiorwidget.h"

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

    auto importGcode = new QAction("Import GCODE file");
    connect(importGcode, SIGNAL(triggered(bool)), this, SLOT(onImportGcodeCall()));

    auto configJSON = new QAction("Export JSON config file");
    connect(configJSON, SIGNAL(triggered(bool)), this, SLOT(onExportJsonConfigCall()));

    auto newProgram = new QAction("New program");
    connect(newProgram, SIGNAL(triggered(bool)), this, SLOT(onNewProgramCall()));

    _editMenu->addSeparator();
    _editMenu->addAction(exportGCode);
    _editMenu->addAction(importGcode);
    _editMenu->addSeparator();
    _editMenu->addAction(newProgram);
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
    widgets::gcodeDeserializatiorWidget gcodeW(_mainWindow);
    gcodeW.setProgram(*_actionsController);
    gcodeW.show();
    gcodeW.exec();
}

void Serial6DofManipulator::onNewProgramCall()
{
    if(_actionsController->enivroment().program().size() == 0)
        return;
    ConfirmActionDialog closeProjDialog;
    closeProjDialog.setMessage(tr("Remove current program?"));
    closeProjDialog.setYesButtonText(tr("Yes"));
    closeProjDialog.setNoButtonText(tr("No"));
    closeProjDialog.show();
    if(closeProjDialog.exec() == true)
    {
        _actionsController->enivroment().program().clear();
    }
}

void Serial6DofManipulator::onExportJsonConfigCall()
{
    qDebug() << "Export config";
}


