#include "manipulatorcontroller.h"
#include "models/unitsConverter.h"

#include <math.h>
#include <QDebug>
using namespace serialMan;


ManipulatorController::ManipulatorController():
    _joints(DEFAULT_DOF), _effector()
{
    initJoints();

    DHTable_t<DEFAULT_DOF> _dhTable;
    //Setup default parameters
    _dhTable.theta = {0, degToRad(-90), 0, 0, 0, 0};
    _dhTable.alfa = {degToRad(-90), 0,degToRad(-90), degToRad(90), degToRad(-90), 0};
    _dhTable.d = {83.5, 0, 0, 100, 0, 60};
    _dhTable.r = {0, 133,10, 0, 0, 0};

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

const Effector_t& ManipulatorController::getEffector() const
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

void ManipulatorController::resetJoints()
{
    QMutexLocker lock(&_mJoints);
    for(auto it : qAsConst(_joints))
    {
        it->blockSignals(true);
        it->setValue(it->getDefaultValue());
        it->blockSignals(false);
    }
    lock.unlock();
    onJointsChanged();
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

CalculationResult_t ManipulatorController::inverseKinematics(const Effector_t& pos)
{
    QVector<double> out(DEFAULT_DOF);

    QMutexLocker clock(&_mConfig);
    QMutexLocker klock(&_mKin);
    auto result = _kin.inverse(pos, out, _kinConfig);
    klock.unlock();
    clock.unlock();

    if(result == CalculationResult_t::CALC_SUCCESSFULL)
    {
        QMutexLocker jlock(&_mJoints);
        for(int i = 0; i < DEFAULT_DOF; i++)
        {
            _joints[i]->blockSignals(true);
            _joints[i]->setValue(out[i]);
            _joints[i]->blockSignals(false);
        }
        QMutexLocker elock(&_mEffector);
        _effector = pos;
    } else {
        qDebug() << "error!";
    }
    emit structureChanged();
    return result;
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


void ManipulatorController::setInvConfig(char c)
{
    QMutexLocker lock(&_mConfig);
    _kinConfig = c;
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


