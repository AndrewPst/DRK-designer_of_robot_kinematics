#include "manipulatorcontroller.h"

#include <math.h>

using namespace serialMan;

ManipulatorController::ManipulatorController():
    _joints(DEFAULT_DOF), _effector()
{
    initJoints();

    //Setup default parameters
    _dhTable.theta = {0, -M_PI_2, 0, 0, 0, 0};
    _dhTable.alfa = {-M_PI_2, 0, -M_PI_2, M_PI_2, -M_PI_2, 0};
    _dhTable.r = {0, 110, 0, 0, 0, 0};
    _dhTable.d = {133, 0, 0, 117.5, 0, 28};

}


void ManipulatorController::initJoints()
{
    //TODO: make deserialization joints from config json file

    for(auto& j : _joints)
    {
        j = new Joint_t();
        connect(j, &Joint_t::valueChanged, this, &ManipulatorController::structureChanged);
    }
}

void ManipulatorController::setDHTable(DHTable_t<DEFAULT_DOF>&& dh)
{
    _dhTable = dh;
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


ManipulatorController::~ManipulatorController()
{
    for(auto& j : _joints)
    {
        delete j;
    }
}


