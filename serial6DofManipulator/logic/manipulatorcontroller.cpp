#include "manipulatorcontroller.h"
#include "models/units_t.h"

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

CalculationResult_t ManipulatorController::inverseKinematics(const Effector_t& pos, char config)
{
    QVector<double> out(DEFAULT_DOF);

    QMutexLocker klock(&_mKin);
    auto result = _kin.inverse(pos, out, config);
    klock.unlock();

    if(result == CalculationResult_t::CALC_SUCCESSFULL)
    {
        QMutexLocker clock(&_mConfig);
        _kinConfig = config;
        QMutexLocker jlock(&_mJoints);
//        for(int i = 0; i < DEFAULT_DOF; i++)
//        {
//            if(out[i] < _joints[i]->getMinValue() || out[i] > _joints[i]->getMaxValue())
//                return CalculationResult_t::CALC_ERROR;
//        }
        for(int i = 0; i < DEFAULT_DOF; i++)
        {
            _joints[i]->blockSignals(true);
            _joints[i]->setValue(out[i]);
            _joints[i]->blockSignals(false);
        }
        QMutexLocker elock(&_mEffector);
        _effector = pos;

//        _effector.wx = _effector.wx >= 2.0 * M_PI ? fmod(_effector.wx, 2.0 * M_PI) : _effector.wx < 0 ? 2.0 * M_PI - fmod(-_effector.wx, 2.0 * M_PI) : pos.wx;
//        _effector.wy = _effector.wy >= 2.0 * M_PI ? fmod(_effector.wy, 2.0 * M_PI) : _effector.wy < 0 ? 2.0 * M_PI - fmod(-_effector.wy, 2.0 * M_PI) : pos.wy;
//        _effector.wz = _effector.wz >= 2.0 * M_PI ? fmod(_effector.wz, 2.0 * M_PI) : _effector.wz < 0 ? 2.0 * M_PI - fmod(-_effector.wz, 2.0 * M_PI) : pos.wz;

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

void ManipulatorController::setEffector(const Effector_t& eff)
{
    QMutexLocker clock(&_mConfig);
    inverseKinematics(eff, _kinConfig);
}


void ManipulatorController::setInvConfig(char c)
{
    QMutexLocker lock(&_mConfig);
    inverseKinematics(_effector, c);
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


