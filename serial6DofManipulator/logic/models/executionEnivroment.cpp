#include "executionEnivroment.h"

using namespace serialMan;

ExecutionEnivroment::ExecutionEnivroment(ManipulatorController& man) : _man(man){}

ManipulatorController& ExecutionEnivroment::manipulator() const
{
    return _man;
}

ExecutionState ExecutionEnivroment::state() const
{
    QMutexLocker lock(&_stateMut);
    return _state;
}

void ExecutionEnivroment::setState(ExecutionState state)
{
    QMutexLocker lock(&_stateMut);
    _state = state;
    lock.unlock();
    emit stateChanged(_state);
}

EnivromentProgram& ExecutionEnivroment::program()
{
    return _program;
}
