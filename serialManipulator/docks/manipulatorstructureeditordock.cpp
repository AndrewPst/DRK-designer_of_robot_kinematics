#include "manipulatorstructureeditordock.h"
#include "../logic/models/joint_t.h"

#include "../logic/manipulatorcontroller.h"
#include "../serialManipulatorProject.h"

#include "projectCore/projectsmanager.h"

#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QListWidgetItem>

using namespace serialMan;

//------------JointListElement------

JointListElement::JointListElement(serialMan::Joint_t* joint) : QWidget(), _joint(joint)
{
    _value = new QLabel();

    connect(joint, &Joint_t::valueChanged, this, &JointListElement::updateValues);
    connect(joint, &Joint_t::typeChanged, this, &JointListElement::updateValues);

    updateValues();

    QVBoxLayout *mainL = new QVBoxLayout();
    mainL->addWidget(_value);

    setLayout(mainL);

}

void JointListElement::updateValues()
{
    _value->setText(tr("Type: %1\tValue: %2")
                    .arg(_joint->getType() == JointType_t::JOINT_ROTATION ? "Rotation" : "Linear")
                    .arg(_joint->getValue()));
}


Joint_t* JointListElement::getJoint() const
{
    return _joint;
}


//-------------Dock Widget-----------

ManipulatorStructureEditorDock::ManipulatorStructureEditorDock(const QString& title)
    : BaseDock(title)
{
    setMinimumSize(200, 200);

    drawAsWindow(true);

    auto t = (SerialManipulatorProject*)projectsManager.getOpenedProject();
    _manipulator = t->getManipulatorController();

    _dofSpin = new QSpinBox();
    _dofSpin->setRange(1, 16);
    _dofSpin->setValue(_manipulator->getDof());
    connect(_dofSpin, SIGNAL(valueChanged(int)), this, SLOT(onDofSpinValueChanged(int)));

    connect(_manipulator, SIGNAL(dofChanged(int)), this, SLOT(onDofChanged(int)));

    _rebuildProject = new QPushButton();
    _rebuildProject->setText(tr("Rebuild"));
    _rebuildProject->setEnabled(false);
    connect(_rebuildProject, &QPushButton::clicked, this, &ManipulatorStructureEditorDock::onRebuildClicked);


    _jointsList = new QListWidget();
    _jointsList->setSizeAdjustPolicy(QListWidget::AdjustToContents);
    _jointsList->setResizeMode(QListWidget::ResizeMode::Adjust);
    _jointsList->setSpacing(5);
    updateJointsList();
    connect(_manipulator, &ManipulatorController::jointAdded, this, &ManipulatorStructureEditorDock::onJointAdded);
    connect(_manipulator, &ManipulatorController::jointRemoved, this, &ManipulatorStructureEditorDock::onJointRemoved);

    QVBoxLayout* vbl = new QVBoxLayout();
    vbl->addWidget(_dofSpin);
    vbl->addWidget(_rebuildProject);
    vbl->addWidget(_jointsList);
    //vbl->setAlignment(Qt::AlignmentFlag::AlignTop);

    QWidget* main = new QWidget();
    main->setLayout(vbl);

    setWidget(main);
}

void ManipulatorStructureEditorDock::updateJointsList()
{
    auto list = _manipulator->getJoints();
    Q_FOREACH(Joint_t* i, list)
    {
        onJointAdded(i);
    }
}

void ManipulatorStructureEditorDock::onDofSpinValueChanged(int v)
{
    if(v == _manipulator->getDof())
        _rebuildProject->setEnabled(false);
    else
        _rebuildProject->setEnabled(true);
}

void ManipulatorStructureEditorDock::onRebuildClicked()
{
    _manipulator->setDof(_dofSpin->value());
    _rebuildProject->setEnabled(false);
}

void ManipulatorStructureEditorDock::onDofChanged(int dof)
{
    _dofSpin->setValue(dof);
}

void ManipulatorStructureEditorDock::onJointAdded(serialMan::Joint_t* joint)
{
    QListWidgetItem* _listItem = new QListWidgetItem();
    JointListElement *jli = new JointListElement(joint);
    _jointsList->addItem(_listItem);
    _listItem->setSizeHint(jli->sizeHint() );
    _jointsList->setItemWidget(_listItem, jli);
}

void ManipulatorStructureEditorDock::onJointRemoved(serialMan::Joint_t* joint)
{
    for(int i = 0; i < _jointsList->count(); i++)
    {
        auto elem = _jointsList->item(i);
        auto w = qobject_cast<JointListElement*>( _jointsList->itemWidget(elem));
        if(w->getJoint() == joint)
        {
            _jointsList->removeItemWidget(elem);
            delete _jointsList->takeItem(i);
            w->deleteLater();
            return;
        }
    }
}
