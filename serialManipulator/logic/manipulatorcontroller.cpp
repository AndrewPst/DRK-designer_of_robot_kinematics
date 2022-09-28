#include "manipulatorcontroller.h"

//#include "models/joint_t.h"

using namespace serialMan;

ManipulatorController::ManipulatorController():
    _dof(0), _joints(), _effector(new Joint_t)
{
    setDof(DEFAULT_DOF);
    _effector->setType(JointType_t::JOINT_EFFECTOR);
    connect(_effector, &Joint_t::changed, this, &ManipulatorController::structureChanged);
}

int ManipulatorController::getDof() const
{
    return _dof;
}

void ManipulatorController::setDof(const int value)
{
    if(value == _dof || value < 1)
        return;
    if(value > _dof){
        //Saving first joints and add new
        for(int i = 0; i < value - _dof; i++)
        {
            Joint_t* newj = new Joint_t();
            _joints.append(newj);
            connect(newj, &Joint_t::changed, this, &ManipulatorController::structureChanged);
            emit jointAdded(newj);
        }
    } else {
        //delete last joints
        for(int i = 0; i < _dof - value; i++)
        {
            Joint_t* toDel = _joints.last();
            emit jointRemoved(toDel);
            _joints.removeLast();
            toDel->deleteLater();
        }
    }
    _dof = value;
    emit dofChanged(_dof);
    emit structureChanged();
}

Joint_t* ManipulatorController::getEffector() const
{
    return _effector;
}


const QVector<Joint_t*>& ManipulatorController::getJoints() const
{
    return _joints;
}

ManipulatorController::~ManipulatorController()
{
    Q_FOREACH(auto j, _joints)
        delete j;

    delete _effector;
}


