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


