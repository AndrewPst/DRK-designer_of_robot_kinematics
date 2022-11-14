#include "kinematicsdock.h"
#include "../logic/models/joint_t.h"
#include "../logic/manipulatorcontroller.h"

#include <limits>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QListWidget>
#include <QGroupBox>
#include <QCheckBox>

using namespace serialMan;

JointViewModelWidget::JointViewModelWidget(Joint_t* j, ManipulatorController* man)
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
    connect(man, SIGNAL(structureChanged()), this, SLOT(onJointValueChanged()));

    _mainL->addLayout(fl);

    setLayout(_mainL);
}

void JointViewModelWidget::onJointValueChanged()
{
    _value->blockSignals(true);
    _value->setValue(_joint->getValue());
    _value->blockSignals(false);
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

    QGroupBox* gb = new QGroupBox("Desired position");

    _posX = new QDoubleSpinBox();
    _posX->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    _posX->setSingleStep(0.2);
    connect(_posX, SIGNAL(valueChanged(double)), this, SLOT(onPositionChanged()));

    _posY = new QDoubleSpinBox();
    _posY->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    _posY->setSingleStep(0.2);
    connect(_posY, SIGNAL(valueChanged(double)), this, SLOT(onPositionChanged()));

    _posZ = new QDoubleSpinBox();
    _posZ->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    _posZ->setSingleStep(0.2);
    connect(_posZ, SIGNAL(valueChanged(double)), this, SLOT(onPositionChanged()));

    _rotX = new QDoubleSpinBox();
    _rotX->setSingleStep(5);
    _rotX->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    connect(_rotX, SIGNAL(valueChanged(double)), this, SLOT(onPositionChanged()));

    _rotY = new QDoubleSpinBox();
    _rotY->setSingleStep(5);
    _rotY->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    connect(_rotY, SIGNAL(valueChanged(double)), this, SLOT(onPositionChanged()));

    _rotZ = new QDoubleSpinBox();
    _rotZ->setSingleStep(5);
    _rotZ->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    connect(_rotZ, SIGNAL(valueChanged(double)), this, SLOT(onPositionChanged()));


    _v1 = new QCheckBox();
    _v1->setText("Variant 1");

    _v2 = new QCheckBox();
    _v2->setText("Variant 2");

    connect(_v1, SIGNAL(toggled(bool)), this, SLOT(onConfigChanged()));
    connect(_v2, SIGNAL(toggled(bool)), this, SLOT(onConfigChanged()));

    onStructureChanged();

    QFormLayout *tempL = new QFormLayout();
    tempL->addRow("X", _posX);
    tempL->addRow("Y", _posY);
    tempL->addRow("Z", _posZ);

    tempL->addRow("Rot X", _rotX);
    tempL->addRow("Rot Y", _rotY);
    tempL->addRow("Rot Z", _rotZ);

    gb->setLayout(tempL);

    _mainL->addWidget(_list);
    _mainL->addWidget(gb);
    _mainL->addWidget(_v1);
    _mainL->addWidget(_v2);

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

void KinematicsDock::onConfigChanged()
{
    char conf = 0;
    if(_v1->isChecked()) conf |= 0b00000001;
    if(_v2->isChecked()) conf |= 0b00000010;
    _man->setInvConfig(conf);
}

void KinematicsDock::onStructureChanged()
{
    _posX->blockSignals(true);
    _posX->setValue(_man->getEffector().x);
    _posX->blockSignals(false);

    _posY->blockSignals(true);
    _posY->setValue(_man->getEffector().y);
    _posY->blockSignals(false);

    _posZ->blockSignals(true);
    _posZ->setValue(_man->getEffector().z);
    _posZ->blockSignals(false);

    _rotX->blockSignals(true);
    _rotX->setValue(_man->getEffector().wx);
    _rotX->blockSignals(false);

    _rotY->blockSignals(true);
    _rotY->setValue(_man->getEffector().wy);
    _rotY->blockSignals(false);

    _rotZ->blockSignals(true);
    _rotZ->setValue(_man->getEffector().wz);
    _rotZ->blockSignals(false);


    _v1->blockSignals(true);
    _v2->blockSignals(true);
    _v1->setChecked(_man->getInvConfig() & 0b00000001);
    _v2->setChecked(_man->getInvConfig() & 0b00000010);
    _v1->blockSignals(false);
    _v2->blockSignals(false);
}

void KinematicsDock::initList()
{
    _list->setStyleSheet( "QListWidget::item { border-bottom: 1px solid black; }" );
    auto& js = _man->getJoints();
    for(auto j : js)
    {
        JointViewModelWidget* jw = new JointViewModelWidget(j, _man);
        QListWidgetItem* item = new QListWidgetItem(_list);
        item->setSizeHint( jw->sizeHint() );
        _list->setItemWidget(item, jw);
    }
}




