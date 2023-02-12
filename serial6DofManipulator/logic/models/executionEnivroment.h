#ifndef EXECUTIONENIVROMENT_H
#define EXECUTIONENIVROMENT_H


#include "enivromentProgram.h"
#include "../manipulatorcontroller.h"

#include "executionState.h"

#include "QMutex"
#include "QMutexLocker"

#include <QObject>
#include <memory>

namespace serialMan
{

class EnivromentProgramEditor;

struct ExecutionEnivroment
{

    friend class ActionsController;
    friend class ProgramExecutor;

private:

    ManipulatorController& _man;
    EnivromentProgram _program;

    ExecutionState _state{ExecutionState::STATE_UNKNOWN};
    mutable QMutex _stateMut;

    void setState(ExecutionState state);

public:

    explicit ExecutionEnivroment(ManipulatorController& man);

    ManipulatorController& manipulator() const;

    ExecutionState state() const;

    EnivromentProgram& program();

signals:

    void stateChanged(ExecutionState);

};

}
#endif // EXECUTIONENIVROMENT_H
