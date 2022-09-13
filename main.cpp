#include "mainwindow.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);

    //Set application params
    QCoreApplication::setApplicationName("DRK (designer of robot kinematics)");
    QCoreApplication::setApplicationVersion("0.1.0.1 alfa");

    MainWindow w;
    w.resize(800, 600);
    w.show();

    return a.exec();
}
