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
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QDebug>

using namespace serialMan;

//---------Joint detailed widget--------

JointDetailedWidget::JointDetailedWidget() : ::QWidget()
{
    setMinimumSize(100, 300);

    setEnabled(false);

    _currentValue = new QDoubleSpinBox();
    connect(_currentValue, SIGNAL(valueChanged(double)), this, SLOT(onCurrentValueChanged(double)));

    _minValue = new QDoubleSpinBox();
    _minValue->setRange(-360, 360);
    connect(_minValue, SIGNAL(valueChanged(double)), this, SLOT(onMinValueChanged(double)));

    _maxValue = new QDoubleSpinBox();
    _maxValue->setRange(-360, 360);
    connect(_maxValue, SIGNAL(valueChanged(double)), this, SLOT(onMaxValueChanged(double)));

    _typeJointBox = new QComboBox();
    _typeJointBox->addItem(tr("Rotation"), (int)(JointType_t::JOINT_ROTATION));
    _typeJointBox->addItem(tr("Linear"), (int)(JointType_t::JOINT_LINEAR));
    connect(_typeJointBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeUpdated(int)));

    QGroupBox *posGroup = new QGroupBox(tr("Position"));

    _posX = new QDoubleSpinBox();
    _posX->setRange(INT_MIN, INT_MAX);
    _posY = new QDoubleSpinBox();
    _posY->setRange(INT_MIN, INT_MAX);
    _posZ = new QDoubleSpinBox();
    _posZ->setRange(INT_MIN, INT_MAX);
    connect(_posX, SIGNAL(valueChanged(double)), this, SLOT(onPosUpdated()));
    connect(_posY, SIGNAL(valueChanged(double)), this, SLOT(onPosUpdated()));
    connect(_posZ, SIGNAL(valueChanged(double)), this, SLOT(onPosUpdated()));

    QFormLayout *formPos = new QFormLayout();
    formPos->addRow(tr("X coordinate"), _posX);
    formPos->addRow(tr("Y coordinate"), _posY);
    formPos->addRow(tr("Z coordinate"), _posZ);
    posGroup->setLayout(formPos);

    QGroupBox *rotGroup = new QGroupBox(tr("Rotation"));

    _rotX = new QDoubleSpinBox();
    _rotX->setRange(INT_MIN, INT_MAX);
    _rotY = new QDoubleSpinBox();
    _rotY->setRange(INT_MIN, INT_MAX);
    _rotZ = new QDoubleSpinBox();
    _rotZ->setRange(INT_MIN, INT_MAX);
    connect(_rotX, SIGNAL(valueChanged(double)), this, SLOT(onRotUpdated()));
    connect(_rotY, SIGNAL(valueChanged(double)), this, SLOT(onRotUpdated()));
    connect(_rotZ, SIGNAL(valueChanged(double)), this, SLOT(onRotUpdated()));

    QFormLayout *formRot = new QFormLayout();
    formRot->addRow(tr("X angle"), _rotX);
    formRot->addRow(tr("Y angle"), _rotY);
    formRot->addRow(tr("Z angle"), _rotZ);
    rotGroup->setLayout(formRot);

    QFormLayout *fl = new QFormLayout();
    fl->addRow(tr("Current value:"), _currentValue);
    fl->addRow(tr("Maximum value:"), _maxValue);
    fl->addRow(tr("Minimum value:"), _minValue);
    fl->addRow(tr("Joints type:"), _typeJointBox);
    fl->addRow(posGroup);
    fl->addRow(rotGroup);
    fl->setSizeConstraint(QFormLayout::SizeConstraint::SetFixedSize);
    fl->setSpacing(4);
    fl->setFormAlignment(Qt::AlignmentFlag::AlignCenter);
    fl->setAlignment(Qt::AlignmentFlag::AlignCenter);

    setLayout(fl);
}




Joint_t* JointDetailedWidget::getJoint() const
{
    return _joint;
}

void JointDetailedWidget::setJoint(Joint_t* j)
{
    if(j == _joint) return;
    if(j == NULL)
    {
        setEnabled(false);
        return;
    }

    resetConnections();
    _joint = j;
    createConnections();

    updateWidgets();

}

void JointDetailedWidget::createConnections()
{
    _connections << connect(_joint, &Joint_t::valueChanged, this, &JointDetailedWidget::onCurrentValueChanged);
    _connections << connect(_joint, &Joint_t::maxValueChanged, this, &JointDetailedWidget::onMaxValueChanged);
    _connections << connect(_joint, &Joint_t::minValueChanged, this, &JointDetailedWidget::onMinValueChanged);
    _connections << connect(_joint, &Joint_t::typeChanged, this, &JointDetailedWidget::onTypeChanged);
    _connections << connect(_joint, &Joint_t::positionChanged, this, &JointDetailedWidget::onPositionChanged);
    _connections << connect(_joint, &Joint_t::rotationChanged, this, &JointDetailedWidget::onRotationChanged);
}

void JointDetailedWidget::resetConnections()
{
    Q_FOREACH(auto c, _connections)
    {
        disconnect(c);
    }
}

void JointDetailedWidget::updateWidgets()
{
    if(_joint == NULL) return;
    _currentValue->setRange(_joint->getMinValue(), _joint->getMaxValue());
    _currentValue->setValue(_joint->getValue());
    _minValue->setValue(_joint->getMinValue());
    _maxValue->setValue(_joint->getMaxValue());
    _typeJointBox->setCurrentIndex(((int)_joint->getType())-1);
    onPositionChanged(_joint->getPosition());
    onRotationChanged(_joint->getRotation());
}

void JointDetailedWidget::onCurrentValueChanged(double value)
{
    _currentValue->setValue(value);
    _joint->setValue(value);
}

void JointDetailedWidget::onMinValueChanged(double v)
{
    _minValue->setValue(v);
    _maxValue->setMinimum(v);
    _currentValue->setMinimum(v);
    _joint->setMinValue(v);
}

void JointDetailedWidget::onMaxValueChanged(double v)
{
    _maxValue->setValue(v);
    _minValue->setMaximum(v);
    _currentValue->setMaximum(v);
    _joint->setMaxValue(v);
}

void JointDetailedWidget::onTypeChanged(JointType_t type)
{
    _typeJointBox->setCurrentIndex(((int)type)-1);
}

void JointDetailedWidget::onTypeUpdated(int t)
{
    _joint->setType(static_cast<serialMan::JointType_t>(t+1));
}

void JointDetailedWidget::onPositionChanged(QVector3D pos)
{
    _posX->setValue(pos.x());
    _posY->setValue(pos.y());
    _posZ->setValue(pos.z());
}

void JointDetailedWidget::onRotationChanged(QVector3D rot)
{
    _rotX->setValue(rot.x());
    _rotY->setValue(rot.y());
    _rotZ->setValue(rot.z());
}

void JointDetailedWidget::onPosUpdated()
{
    //QVector3D consists of float. TODO: Make a vector from a double
    _joint->setPosition({static_cast<float>(_posX->value()),
                         static_cast<float>(_posY->value()),
                         static_cast<float>(_posZ->value())});
}

void JointDetailedWidget::onRotUpdated()
{
    _joint->setRotation({static_cast<float>(_rotX->value()),
                         static_cast<float>(_rotY->value()),
                         static_cast<float>(_rotZ->value())});
}


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
    _value->setText(tr("Type: %1\nValue: %2")
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
    _jointsList->setMinimumSize(200, 400);
    _jointsList->setSizeAdjustPolicy(QListWidget::AdjustToContents);
    _jointsList->setResizeMode(QListWidget::ResizeMode::Adjust);
    _jointsList->setSpacing(0);
    updateJointsList();
    connect(_jointsList, &QListWidget::currentItemChanged, this, &ManipulatorStructureEditorDock::jointSelected);
    connect(_manipulator, &ManipulatorController::jointAdded, this, &ManipulatorStructureEditorDock::onJointAdded);
    connect(_manipulator, &ManipulatorController::jointRemoved, this, &ManipulatorStructureEditorDock::onJointRemoved);

    _detailed = new JointDetailedWidget();


    QVBoxLayout* vbl = new QVBoxLayout();
    vbl->addWidget(_dofSpin);
    vbl->addWidget(_rebuildProject);
    vbl->addWidget(_jointsList);
    vbl->addWidget(_detailed);
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

Qt::DockWidgetArea ManipulatorStructureEditorDock::getDefaultArea() const
{
    return Qt::LeftDockWidgetArea;
}

void ManipulatorStructureEditorDock::jointSelected()
{
    auto t = ((JointListElement*)(_jointsList->itemWidget(_jointsList->currentItem())))->getJoint();
    _detailed->setJoint(t);
    _detailed->setEnabled(true);
}
