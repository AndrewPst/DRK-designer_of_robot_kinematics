#include "manipulatorcontroller.h"

using namespace serialMan;

ManipulatorController::ManipulatorController():
    _joints(DEFAULT_DOF), _effector()
{
    initJoints();
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

const Effector_t& ManipulatorController::getEffector() const
{
    return _effector;
}


const QVector<Joint_t*>& ManipulatorController::getJoints() const
{
    return _joints;
}

ManipulatorController::~ManipulatorController()
{
    for(auto& j : _joints)
    {
        delete j;
    }
}


