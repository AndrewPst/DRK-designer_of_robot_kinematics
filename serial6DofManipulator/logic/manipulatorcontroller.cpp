#include "manipulatorcontroller.h"

#include <math.h>

using namespace serialMan;


ManipulatorController::ManipulatorController():
    _joints(DEFAULT_DOF), _effector()
{
    initJoints();

    DHTable_t<DEFAULT_DOF> _dhTable;
    //Setup default parameters
    _dhTable.theta = {0, -90, 0, 0, 0, 0};
    _dhTable.alfa = {-90, 0, -90, 90, -90, 0};
    _dhTable.r = {4, 11, 2, 0, 0, 0};
    _dhTable.d = {13, 0, 0, 11, 0, 2};

    _kin.setDHTable(_dhTable);
    onJointsChanged();
}


void ManipulatorController::initJoints()
{
    //TODO: make deserialization joints from config json file
    QMutexLocker lock(&_mJoints);
    for(auto& j : _joints)
    {
        j = new Joint_t();
        connect(j, &Joint_t::valueChanged, this, &ManipulatorController::structureChanged);
        connect(j, &Joint_t::valueChanged, this, &ManipulatorController::onJointsChanged);
    }
}

void ManipulatorController::setDHTable(const DHTable_t<DEFAULT_DOF>& dh)
{
    QMutexLocker lock(&_mKin);
    _kin.setDHTable(dh);
    lock.unlock();
    emit structureChanged();
}

Effector_t ManipulatorController::getEffector() const
{
    QMutexLocker lock(&_mEffector);

    return _effector;
}

const QVector<Joint_t*>& ManipulatorController::getJoints()const
{
    QMutexLocker lock(&_mJoints);
    return _joints;
}

const DHTable_t<ManipulatorController::DEFAULT_DOF>& ManipulatorController::getDHTable() const
{
    QMutexLocker lock(&_mKin);
    return _kin.dhTable();
}

void ManipulatorController::forwardKinematics(QVector<double>& joints)
{
    Effector_t result;

    QMutexLocker klock(&_mKin);
    _kin.forward(joints, result);
    klock.unlock();

    QMutexLocker lock(&_mEffector);
    _effector = result;
    lock.unlock();

    emit structureChanged();
}

void ManipulatorController::inverseKinematics(const Effector_t& pos)
{
    QVector<double> out(DEFAULT_DOF);

    QMutexLocker klock(&_mKin);
    QMutexLocker clock(&_mConfig);
    _kin.inverse(pos, out, _kinConfig);
    klock.unlock();
    clock.unlock();

    QMutexLocker jlock(&_mJoints);
    for(int i = 0; i < DEFAULT_DOF; i++){
        _joints[i]->blockSignals(true);
        _joints[i]->setValue(out[i]);
        _joints[i]->blockSignals(false);
    }
    jlock.unlock();

    emit structureChanged();
}

void ManipulatorController::onJointsChanged()
{
    QVector<double> values(DEFAULT_DOF);

    QMutexLocker jlock(&_mJoints);
    for(int i = 0; i < DEFAULT_DOF; i++)
        values[i] = _joints[i]->getValue();
    jlock.unlock();

    forwardKinematics(values);
}

void ManipulatorController::setEffector(const Effector_t& eff)
{
    QMutexLocker lock(&_mEffector);
    _effector = eff;
    lock.unlock();
    inverseKinematics(eff);
}


void ManipulatorController::setInvConfig(char c)
{
    QMutexLocker lock(&_mConfig);
    _kinConfig = c;
    lock.unlock();
    inverseKinematics(_effector);
}

char ManipulatorController::getInvConfig() const
{
    QMutexLocker lock(&_mConfig);
    return _kinConfig;
}

ManipulatorController::~ManipulatorController()
{
    QMutexLocker lock(&_mJoints);
    for(auto& j : _joints)
    {
        delete j;
    }
}


