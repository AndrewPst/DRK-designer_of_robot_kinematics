#include "manipulatorcontroller.h"

//#include "models/joint_t.h"

using namespace serialMan;

ManipulatorController::ManipulatorController():
    _dof(DEFAULT_DOF), _joints(DEFAULT_DOF)
{
    //init joints
    for(int i = 0; i< DEFAULT_DOF; i++)
    {
        _joints[i] = new Joint_t();
    }
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
            emit jointAdded(newj);
            _joints.append(newj);
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
}

const QVector<Joint_t*>& ManipulatorController::getJoints() const
{
    return _joints;
}


