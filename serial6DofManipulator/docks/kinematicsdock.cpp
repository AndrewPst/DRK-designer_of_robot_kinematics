#include "kinematicsdock.h"
#include "../logic/models/joint_t.h"
#include "../logic/manipulatorcontroller.h"

#include <limits>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QListWidget>
#include <QGroupBox>

using namespace serialMan;

JointViewModelWidget::JointViewModelWidget(Joint_t* j)
    : QWidget(), _joint(j)
{
    _mainL = new QVBoxLayout();

    QFormLayout* fl = new QFormLayout();
    _value = new QDoubleSpinBox();
    _value->setSuffix(" Degrees");
    _value->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    _value->setValue(j->getValue());
    connect(_value, SIGNAL(valueChanged(double)), this, SLOT(onSpinsChanged()));
    fl->addRow(tr("Value"), _value);

    _min = new QDoubleSpinBox();
    _min->setSuffix(" Degrees");
    _min->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    _min->setValue(j->getMinValue());
    connect(_min, SIGNAL(valueChanged(double)), this, SLOT(onSpinsChanged()));
    fl->addRow(tr("Min"), _min);

    _max = new QDoubleSpinBox();
    _max->setSuffix(" Degrees");
    _max->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    _max->setValue(j->getMaxValue());
    connect(_max, SIGNAL(valueChanged(double)), this, SLOT(onSpinsChanged()));
    fl->addRow(tr("Max"), _max);

    connect(j, SIGNAL(valueChanged(double)), this, SLOT(onJointValueChanged()));
    connect(j, SIGNAL(minValueChanged(double)), this, SLOT(onJointMinChanged()));
    connect(j, SIGNAL(maxValueChanged(double)), this, SLOT(onJointMaxChanged()));

    _mainL->addLayout(fl);

    setLayout(_mainL);
}

void JointViewModelWidget::onJointValueChanged()
{
    _value->setValue(_joint->getValue());
}

void JointViewModelWidget::onJointMinChanged()
{
    _min->setValue(_joint->getMinValue());
}

void JointViewModelWidget::onJointMaxChanged()
{
    _max->setValue(_joint->getMaxValue());
}

void JointViewModelWidget::onSpinsChanged()
{
    _joint->setMinValue(_min->value());
    _joint->setMaxValue(_max->value());
    _joint->setValue(_value->value());
}




KinematicsDock::KinematicsDock(ManipulatorController* man,
                               const QString& title,
                               QWidget* parent,
                               Qt::WindowFlags flags)
    : BaseDock(title, parent, flags), _man(man)
{
    _mainL = new QVBoxLayout();
    _mainL->setAlignment(Qt::AlignmentFlag::AlignTop);

    _list = new QListWidget();

    initList();
    _mainL->addWidget(_list);

    QGroupBox* gb = new QGroupBox("Desired position");

    _posX = new QDoubleSpinBox();
    _posX->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    connect(_posX, SIGNAL(editingFinished()), this, SLOT(onPositionChanged()));

    _posY = new QDoubleSpinBox();
    _posY->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    connect(_posY, SIGNAL(editingFinished()), this, SLOT(onPositionChanged()));

    _posZ = new QDoubleSpinBox();
    _posZ->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    connect(_posZ, SIGNAL(editingFinished()), this, SLOT(onPositionChanged()));

    _rotX = new QDoubleSpinBox();
    _rotX->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    connect(_rotX, SIGNAL(editingFinished()), this, SLOT(onPositionChanged()));

    _rotY = new QDoubleSpinBox();
    _rotY->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    connect(_rotY, SIGNAL(editingFinished()), this, SLOT(onPositionChanged()));

    _rotZ = new QDoubleSpinBox();
    _rotZ->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    connect(_rotZ, SIGNAL(editingFinished()), this, SLOT(onPositionChanged()));

    onStructureChanged();

    QFormLayout *tempL = new QFormLayout();
    tempL->addRow("X", _posX);
    tempL->addRow("Y", _posY);
    tempL->addRow("Z", _posZ);

    tempL->addRow("Rot X", _rotX);
    tempL->addRow("Rot Y", _rotY);
    tempL->addRow("Rot Z", _rotZ);

    gb->setLayout(tempL);

    _mainL->addWidget(gb);

    _mainW = new QWidget();
    _mainW->setLayout(_mainL);
    setWidget(_mainW);

    connect(_man, SIGNAL(structureChanged()), this, SLOT(onStructureChanged()));
}

void KinematicsDock::onPositionChanged()
{
    Effector_t eff  =
    {
        _posX->value(),
        _posY->value(),
        _posZ->value(),
        _rotX->value(),
        _rotY->value(),
        _rotZ->value(),
    };

    _man->setEffector(eff);
}

void KinematicsDock::onStructureChanged()
{
    _posX->setValue(_man->getEffector().x);
    _posY->setValue(_man->getEffector().y);
    _posZ->setValue(_man->getEffector().z);
    _rotX->setValue(_man->getEffector().wx);
    _rotY->setValue(_man->getEffector().wy);
    _rotZ->setValue(_man->getEffector().wz);
}

void KinematicsDock::initList()
{
    _list->setStyleSheet( "QListWidget::item { border-bottom: 1px solid black; }" );
    auto& js = _man->getJoints();
    for(auto j : js)
    {
        JointViewModelWidget* jw = new JointViewModelWidget(j);
        QListWidgetItem* item = new QListWidgetItem(_list);
        item->setSizeHint( jw->sizeHint() );
        _list->setItemWidget(item, jw);
    }
}




