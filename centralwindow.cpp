#include "centralwindow.h"
#include "basecentraldock.h"
#include "projectCore/baseprojectcontroller.h"
#include "projectCore/projectsmanager.h"

#include <iostream>

#include <QTextEdit>
#include <QMenu>

#include <QGLWidget>

CentralWindow::CentralWindow(QWidget* parent) : QMainWindow(parent)
{
    setObjectName("centralWindow");
    setWindowTitle(tr("Graphics visualization"));
    setWindowFlags(Qt::Widget);

    connect(&projectsManager, &ProjectsManager::onProjectOpened, this, &CentralWindow::onProjectOpened);


    setCorner(Qt::Corner::TopRightCorner, Qt::RightDockWidgetArea);

    initMenu();

    //Setup openGl
    QGLFormat fmt;
    fmt.setSampleBuffers(true);
    fmt.setSamples(16);//antialiazing
    fmt.setDepthBufferSize(24);
    QGLFormat::setDefaultFormat(fmt);

    //Set dock options
    DockOptions opts;
    //opts |= AnimatedDocks;
    opts |= AllowNestedDocks;
    opts |= AllowTabbedDocks;
    QMainWindow::setDockOptions(opts);

    //add first widget
    //getNewDock(Qt::TopDockWidgetArea);
    //getNewDock(Qt::TopDockWidgetArea);

    //hide central widget
    setCentralWidget(nullptr);
}


//---- Getters ----
QMenu* CentralWindow::getMenu()
{
    return _menu;
}

//---Public slots---

void CentralWindow::onProjectOpened(BaseProjectController* const proj)
{
    Q_FOREACH(auto a, proj->getAviableCentralDocks())
    {
        addDockWidget(a->getDefaultArea(), a);
    }
}


//---Private methods---

void CentralWindow::initMenu()
{
    //Init menu
    _menu = new QMenu(objectName(), this);

}


//BaseCentralDock* CentralWindow::getNewDock(Qt::DockWidgetArea area)
//{
//    BaseCentralDock *dv = createDockByType(tr("my_widget"), CentralDockTypes_t::GL_VISUALIZATOR_CENTRAL, {area});
//    connect(dv, SIGNAL(onCreateDock(bool)), this, SLOT(createNewDockSlot(bool)));
//    connect(dv, SIGNAL(onSplitDock(BaseCentralDock*,Qt::Orientation)), this, SLOT(splitDockSlot(BaseCentralDock*,Qt::Orientation)));
//    connect(dv, SIGNAL(onTabDock(BaseCentralDock*)), this, SLOT(addTabDockSlot(BaseCentralDock*)));
//    addDockWidget(area, dv);
//    return dv;
//}

//void CentralWindow::splitDock(BaseCentralDock* w, Qt::Orientation o)
//{
//    if(!w)
//        return;
//    auto area = (o == Qt::Orientation::Vertical) ? Qt::BottomDockWidgetArea : Qt::RightDockWidgetArea;
//    BaseCentralDock *dv = getNewDock(area);
//    if(tabifiedDockWidgets(w).size() == 0)
//        splitDockWidget(w, dv, o);
//    else
//        addDockWidget(area, dv, o);

//}

//BaseCentralDock *CentralWindow::createDockByType(const QString& title,
//                                                 const CentralDockTypes_t& type,
//                                                 Qt::WindowFlags flags)
//{
//    switch (type) {
//    case EXEMPLE_CENTRAL:
//        return new ExempleCentralDock(title, this, flags);
//    case GL_VISUALIZATOR_CENTRAL:
//        return new glCentralDock(title, this, flags);
//    default:
//        std::cerr << "Unknown widget type!\n";
//        return nullptr;
//    }
//}

