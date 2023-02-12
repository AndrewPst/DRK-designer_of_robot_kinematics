#include "mainwindow.h"

#include "version_t.h"

#include <QApplication>
#include <serial6DofManipulator/logic/models/executionState.h>

Version_t _version {1, 1, 0, VersionStage_t::VERSION_ALFA};

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    qRegisterMetaType<serialMan::ExecutionState>("serialMan::ProgramState_t");

    QApplication a(argc, argv);

    //Set application params
    QCoreApplication::setApplicationName("DRK (designer of robot kinematics)");
    QCoreApplication::setApplicationVersion(_version.toStr());

    MainWindow w;
    w.resize(1920, 1080);
    w.show();

    return a.exec();
}
