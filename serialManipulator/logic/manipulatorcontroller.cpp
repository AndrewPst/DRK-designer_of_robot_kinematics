#include "manipulatorcontroller.h"

//#include "models/joint_t.h"

using namespace serialMan;

ManipulatorController::ManipulatorController():
    _dof(DEFAULT_DOF), _joints(DEFAULT_DOF)
{

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
        for(int i = 0; i < value - _dof; i++)
        {
            Joint_t* newj = new Joint_t();
            emit jointAdded(newj);
            _joints.append(newj);
        }
    } else {
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


