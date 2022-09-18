#include "manipulatorstructureeditordock.h"

#include "../logic/manipulatorcontroller.h"
#include "projectCore/projectsmanager.h"
#include "serialManipulator/serialManipulatorProject.h"

#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace serialMan;

ManipulatorStructureEditorDock::ManipulatorStructureEditorDock(const QString& title)
    : BaseDock(title)
{
    setMinimumSize(200, 200);

    drawAsWindow(true);


    _dofSpin = new QSpinBox();
    _dofSpin->setRange(1, 16);
    connect(_dofSpin, SIGNAL(valueChanged(int)), this, SLOT(onDofSpinValueChanged(int)));

    _rebuildProject = new QPushButton();
    _rebuildProject->setText(tr("Rebuild"));

    QVBoxLayout* vbl = new QVBoxLayout();
    vbl->addWidget(_dofSpin);
    vbl->addWidget(_rebuildProject);

    QWidget* main = new QWidget(this);
    main->setLayout(vbl);

    setWidget(main);
}

void ManipulatorStructureEditorDock::onDofSpinValueChanged(int v)
{
    auto t = (SerialManipulatorProject*)projectsManager.getOpenedProject();
    _manipulator = t->getManipulatorController();
    if(v == _manipulator->getDof())
        _rebuildProject->setVisible(false);
    _rebuildProject->setVisible(true);
}
