#include "mainwindow.h"
#include "centralwindow.h"
#include "basedock.h"

#include "dockWidgets/manybuttonsdock.h"
#include "dockWidgets/manipulatoreditordock.h"

#include <QMenu>
#include <QMenuBar>
#include <QKeySequence>
#include <QShortcut>
#include <QActionGroup>
#include <QListIterator>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    setObjectName("MainWindow");
    setWindowTitle(tr("Project 1 - %1").arg(QCoreApplication::applicationName()));
    _centralWindow = new CentralWindow(this);

    DockOptions opts;
    opts |= AllowNestedDocks;
    opts |= AllowTabbedDocks;
    opts |= AnimatedDocks;
    QMainWindow::setDockOptions(opts);

    ManyButtonsDock *b = new ManyButtonsDock(tr("Exemple"), this);
    _dockWidgets.append(b);
    addDockWidget(Qt::BottomDockWidgetArea, b);

//    ManipulatorEditorDock *m = new ManipulatorEditorDock(tr("Manipulator editor"), this);
//    _dockWidgets.append(m);
//    addDockWidget(Qt::RightDockWidgetArea, m);

    setupMenuBar();

    setCentralWidget(_centralWindow);
    centralWidget()->show();
}

//----Privat methods----

void MainWindow::setupMenuBar()
{
    //----Setup File menu----
    _fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *action = _fileMenu->addAction(tr("&Create new project"));
    action->setShortcut(QKeySequence::New);
    connect(action, &QAction::triggered, this, &MainWindow::actionNewProjectSlot);

    action = _fileMenu->addAction(tr("&Open project"));
    action->setShortcut(QKeySequence::Open);
    connect(action, &QAction::triggered, this, &MainWindow::actionOpenProjectSlot);

    _fileMenu->addSeparator();

    action = _fileMenu->addAction(tr("&Save"));
    action->setShortcut(QKeySequence::Save);
    connect(action, &QAction::triggered, this, &MainWindow::actionSaveAllSlot);

    action = _fileMenu->addAction(tr("Save &as..."));
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    connect(action, &QAction::triggered, this, &MainWindow::actionSaveAllAsSlot);

    _fileMenu->addSeparator();

    action = _fileMenu->addAction(tr("&Return to last save"));
    connect(action, &QAction::triggered, this, &MainWindow::actionReturnToLastSaveSlot);

    _fileMenu->addSeparator();

    action = _fileMenu->addAction(tr("&Quit"));
    action->setShortcut(QKeySequence::Quit);
    connect(action, &QAction::triggered, this, &MainWindow::close);


    //----Setup edit menu----
    _editMenu = menuBar()->addMenu(tr("&Edit"));

    action = _editMenu->addAction(tr("&Undo"));
    action->setShortcut(QKeySequence::Undo);
    connect(action, &QAction::toggled, this, &MainWindow::actionUndoSlot);

    action = _editMenu->addAction(tr("&Redo"));
    action->setShortcut(QKeySequence::Redo);
    connect(action, &QAction::toggled, this, &MainWindow::actionRedoSlot);

    _editMenu->addSeparator();

    action = _editMenu->addAction(tr("Pr&eferences"));
    connect(action, &QAction::toggled, this, &MainWindow::actionOpenPreferencesSlot);

    //----Setup view menu----
    _viewMenu = menuBar()->addMenu(tr("&View"));
    _viewMenu->addMenu(_centralWindow->getMenu());

    _viewMenu->addSeparator();

    for(qsizetype i = 0; i < _dockWidgets.size(); i++)
        _viewMenu->addMenu(_dockWidgets[i]->getMenu());

    _viewMenu->addSeparator();
    _dockParametersMenu = new QMenu(tr("Dock widgets parameters"), this);
    action = _dockParametersMenu->addAction(tr("Animated docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AnimatedDocks);
    connect(action, &QAction::toggled, this, &MainWindow::actionSetDockOptionsSlot);

    action = _dockParametersMenu->addAction(tr("Force tabbed docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & ForceTabbedDocks);
    connect(action, &QAction::toggled, this, &MainWindow::actionSetDockOptionsSlot);

    action = _dockParametersMenu->addAction(tr("Vertical tabs"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & VerticalTabs);
    connect(action, &QAction::toggled, this, &MainWindow::actionSetDockOptionsSlot);

    action = _dockParametersMenu->addAction(tr("Grouped dragging"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & GroupedDragging);
    connect(action, &QAction::toggled, this, &MainWindow::actionSetDockOptionsSlot);

    _viewMenu->addMenu(_dockParametersMenu);
}




//----Public slots----

void MainWindow::actionSetDockOptionsSlot()
{
    DockOptions opts;
    QList<QAction*> actions = _dockParametersMenu->actions();

    opts |= AllowNestedDocks;
    opts |= AllowTabbedDocks;

    if (actions.at(0)->isChecked())
        opts |= AnimatedDocks;
    if (actions.at(1)->isChecked())
        opts |= ForceTabbedDocks;
    if (actions.at(2)->isChecked())
        opts |= VerticalTabs;
    if (actions.at(3)->isChecked())
        opts |= GroupedDragging;

    QMainWindow::setDockOptions(opts);
}

void MainWindow::actionNewProjectSlot()
{

}

void MainWindow::actionOpenProjectSlot()
{

}

void MainWindow::actionSaveAllSlot()
{

}

void MainWindow::actionSaveAllAsSlot()
{

}

void MainWindow::actionReturnToLastSaveSlot()
{

}

void MainWindow::actionCloseProjectSlot()
{

}

void MainWindow::actionUndoSlot()
{

}

void MainWindow::actionRedoSlot()
{

}

void MainWindow::actionOpenPreferencesSlot()
{

}

//----Destructor----
MainWindow::~MainWindow()
{
}

