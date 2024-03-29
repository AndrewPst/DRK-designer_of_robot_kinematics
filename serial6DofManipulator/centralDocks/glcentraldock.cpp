#include "../centralDocks/glcentraldock.h"
#include "centralwindow.h"
#include "projectCore/projectsmanager.h"
#include "projectCore/baseprojectcontroller.h"
#include "../serial6dofmanipulator.h"
#include "../openGL/projectvisualizator.h"
#include "../logic/manipulatorcontroller.h"
#include "../openGL/editvisualizaionparamswidget.h"

#include <QAction>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

using namespace serialMan;
using namespace centralDocks;

glCentralDock::glCentralDock(ManipulatorController& man, const QString& title,
                             QMainWindow *parent,
                             Qt::WindowFlags flags)
    : BaseCentralDock(title, parent, flags), _man(man)
{
    _titleWidget = nullptr;

    setStyleSheet("QDockWidget > QWidget {border: 1px solid rgb(150, 150, 180);}");

    QDockWidget::setMinimumSize(400, 400);

    _mainWidget = new QWidget();
    _glWidget = new gl::glVisualizatorWidget(((Serial6DofManipulator*)projectsManager.getOpenedProject())->getVisualizator(), this);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(_glWidget);

    QVBoxLayout *bLayout = new QVBoxLayout();
    QPushButton *splitVButton = new QPushButton(tr("|"));
    connect(splitVButton, &QPushButton::clicked, this, &glCentralDock::onVSplit);
    splitVButton->setFixedSize(32, 32);
    QPushButton *splitHButton = new QPushButton(tr("-"));
    connect(splitHButton, &QPushButton::clicked, this, &glCentralDock::onHSplit);
    splitHButton->setFixedSize(32, 32);
    QPushButton *addTabButton = new QPushButton(tr("TAB"));
    connect(addTabButton, &QPushButton::clicked, this, &glCentralDock::onTabSplit);
    addTabButton->setFixedSize(32, 32);

    QPushButton* resetButton = new QPushButton(tr("Reset"));
    connect(resetButton, &QPushButton::clicked, this, &glCentralDock::onResetPos);
    resetButton->setFixedSize(32, 32);

    bLayout->setAlignment(Qt::AlignmentFlag::AlignTop);

    bLayout->addWidget(splitVButton);
    bLayout->addWidget(splitHButton);
    bLayout->addWidget(addTabButton);
    bLayout->addWidget(resetButton);
    layout->addLayout(bLayout);
    _mainWidget->setLayout(layout);

    QActionGroup *group = new QActionGroup(this);
    group->setExclusive(true);
    connect(group, &QActionGroup::triggered, this, &glCentralDock::setProjectionModeSlot);
    _pModePers = new QAction(tr("Perspective"), this);
    _pModePers->setCheckable(true);
    _pModePers->setChecked(true);
    _pModeOrtho = new QAction(tr("Orthogonal"), this);
    _pModeOrtho->setCheckable(true);
    group->addAction(_pModePers);
    group->addAction(_pModeOrtho);
    _menu->addSeparator();
    _menu->addActions(group->actions());

    group = new QActionGroup(this);
    _lookFromX = new QAction(tr("Looking along X-axis"), group);
    _lookFromY = new QAction(tr("Looking along Y-axis"), group);
    _lookFromZ = new QAction(tr("Looking along Z-axis"), group);
    connect(group, &QActionGroup::triggered, this, &glCentralDock::setLookDirectionSlot);

    _menu->addSeparator();
    _menu->addActions(group->actions());

    _reverseDirection = new QAction(tr("Reverse direction"));
    connect(_reverseDirection, &QAction::triggered, this, &glCentralDock::reverseDirecionSlot);
    _menu->addSeparator();
    _menu->addAction(_reverseDirection);

    _openVisualizateSettings = new QAction(tr("Open visualization settings"));
    connect(_openVisualizateSettings, &QAction::triggered, this, &glCentralDock::onOpenVisualizationSettings);
    _menu->addAction(_openVisualizateSettings);

    _glWidget->setProjectionMode(gl::ProjectionMode_t::PR_PERSPECTIVE);
    setWidget(_mainWidget);

    connect(&_man, &ManipulatorController::structureChanged, this, &glCentralDock::onUpdate);
}

void glCentralDock::onUpdate()
{
    _glWidget->updateGL();
}

void glCentralDock::setProjectionModeSlot(QAction* action)
{
    if(action == _pModeOrtho)
        _glWidget->setProjectionMode(gl::ProjectionMode_t::PR_ORTHOGONAL);
    else if(action == _pModePers)
        _glWidget->setProjectionMode(gl::ProjectionMode_t::PR_PERSPECTIVE);
}

void glCentralDock::setLookDirectionSlot(QAction* action)
{
    if(action == _lookFromX)
    {
        _glWidget->setAngleX(0);
        _glWidget->setAngleY(-M_PI_2);
    }
    else if(action == _lookFromY)
    {
        _glWidget->setAngleX(0);
        _glWidget->setAngleY(-M_PI);
    }
    else if(action == _lookFromZ)
    {
        _glWidget->setAngleX(M_PI_2);
        _glWidget->setAngleY(0);
    }
}

void glCentralDock::onHSplit()
{
    CentralWindow* window = qobject_cast<CentralWindow*>(parent());
    window->splitDockWidget(this, projectsManager.getOpenedProject()->getNewCentralDock<glCentralDock>(_man, "Visualization"), Qt::Orientation::Vertical);
}

void glCentralDock::onVSplit()
{
    CentralWindow* window = qobject_cast<CentralWindow*>(parent());
    window->splitDockWidget(this,  projectsManager.getOpenedProject()->getNewCentralDock<glCentralDock>(_man), Qt::Orientation::Horizontal);
}

void glCentralDock::onTabSplit()
{
    CentralWindow* window = qobject_cast<CentralWindow*>(parent());
    window->tabDockWidget(this,  projectsManager.getOpenedProject()->getNewCentralDock<glCentralDock>(_man));
}

void glCentralDock::onResetPos()
{
    _glWidget->setCameraX(0);
    _glWidget->setCameraY(0);
    _glWidget->setCameraZ(4);
}

void glCentralDock::onOpenVisualizationSettings()
{
    gl::EditVisualizaionParamsWidget w(((serialMan::Serial6DofManipulator*)projectsManager.getOpenedProject())->getVisualizator());
    w.exec();
}


void glCentralDock::reverseDirecionSlot()
{
    _glWidget->setAngleX(_glWidget->getAngleX() * -1);
    _glWidget->setAngleY(_glWidget->getAngleY() + M_PI);
}
