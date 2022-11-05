#include "manipulatorcontroller.h"

#include <math.h>

using namespace serialMan;


ManipulatorController::ManipulatorController():
    _joints(DEFAULT_DOF), _effector()
{
    initJoints();

    //Setup default parameters
    _dhTable.theta = {0, -90, 0, 0, 0, 0};
    _dhTable.alfa = {-90, 0, -90, 90, -90, 0};
    _dhTable.r = {4, 11, 2, 0, 0, 0};
    _dhTable.d = {13, 0, 0, 11, 0, 2};

    _kin.setDHTable(_dhTable);
}


void ManipulatorController::initJoints()
{
    //TODO: make deserialization joints from config json file

    for(auto& j : _joints)
    {
        j = new Joint_t();
        connect(j, &Joint_t::valueChanged, this, &ManipulatorController::structureChanged);
        connect(j, &Joint_t::valueChanged, this, &ManipulatorController::onJointsChanged);
    }
}

void ManipulatorController::setDHTable(DHTable_t<DEFAULT_DOF>&& dh)
{
    _dhTable = std::move(dh);
    _kin.setDHTable(_dhTable);
    emit structureChanged();
}

const Effector_t& ManipulatorController::getEffector() const
{
    return _effector;
}

const QVector<Joint_t*>& ManipulatorController::getJoints() const
{
    return _joints;
}

const DHTable_t<ManipulatorController::DEFAULT_DOF>& ManipulatorController::getDHTable() const
{
    return _dhTable;
}

void ManipulatorController::forwardKinematics(QVector<double>& joints)
{
    Effector_t result;
    _kin.forward(joints, result);
    _effector = result;
    emit structureChanged();
}

void ManipulatorController::inverseKinematics(const Effector_t& pos)
{
    QVector<double> out(DEFAULT_DOF);
    _kin.inverse(pos, out, _kinConfig);
    for(int i = 0; i < DEFAULT_DOF; i++){
        _joints[i]->blockSignals(true);
        _joints[i]->setValue(out[i]);
        _joints[i]->blockSignals(false);
    }
    emit structureChanged();
}

void ManipulatorController::onJointsChanged()
{
    QVector<double> values(DEFAULT_DOF);
    for(int i = 0; i < DEFAULT_DOF; i++)
        values[i] = _joints[i]->getValue();
    forwardKinematics(values);
}

void ManipulatorController::setEffector(const Effector_t& eff)
{
    inverseKinematics(eff);
    _effector = eff;
    emit structureChanged();
}


void ManipulatorController::setInvConfig(char c)
{
    _kinConfig = c;
    inverseKinematics(_effector);
}

char ManipulatorController::getInvConfig()
{
    return _kinConfig;
}

ManipulatorController::~ManipulatorController()
{
    for(auto& j : _joints)
    {
        delete j;
    }
}


