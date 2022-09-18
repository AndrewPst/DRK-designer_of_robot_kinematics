#include "manipulatorcontroller.h"

//#include "models/joint_t.h"

using namespace serialMan;

ManipulatorController::ManipulatorController():
    _dof(_defaultDof), _joints(_defaultDof)
{

}

int ManipulatorController::getDof() const
{
    return _dof;
}

void ManipulatorController::setDof(const int value)
{
    if(value == _dof)
        return;
    _dof = value;
    //emit dofChanged(_dof);
}


