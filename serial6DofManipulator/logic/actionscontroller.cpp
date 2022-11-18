#include "actionscontroller.h"
#include "projectCore/projectsmanager.h"
#include "manipulatorcontroller.h"

#include "models/Actions.h"
#include <QDateTime>

#include <QDebug>

using namespace serialMan;

ActionsController::ActionsController()
    : QObject(), _state(ProgramState_t::STATE_FINISHED)
{
    actions.append(createAction<LinearMovement>());
}

void ActionsController::startProgram()
{

    ManipulatorController* man = ((Serial6DofManipulator*)projectsManager.getOpenedProject())->getManipulatorController();

    if(_state != ProgramState_t::STATE_FINISHED)
        return;
    _executor = new ProgramExecutor(actions, *man);
    _thread = new QThread();

    connect(this, &ActionsController::messageToThread, _executor, &serialMan::ProgramExecutor::setState, Qt::DirectConnection );
    connect(_executor, &serialMan::ProgramExecutor::onStateChanged, this, &ActionsController::onStateChanged, Qt::DirectConnection );
    connect(_executor, &serialMan::ProgramExecutor::onStateChanged, this, &ActionsController::stateChanged, Qt::DirectConnection );

    connect(_thread, &QThread::started, _executor, &serialMan::ProgramExecutor::start);

    connect(_executor, &serialMan::ProgramExecutor::onFinished, _thread, &QThread::quit);
    connect(_thread, &QThread::finished, _executor, &serialMan::ProgramExecutor::deleteLater);
    connect(_thread, &QThread::finished, _thread, &QThread::deleteLater);

    _executor->moveToThread(_thread);

    _thread->start();
}

SerializingError_t ActionsController::serializate(std::ostream&)
{
    return SerializingError_t::ERROR_NONE;
}

SerializingError_t ActionsController::deserializate(std::istream&)
{
    return SerializingError_t::ERROR_NONE;
}

void ActionsController::executeAction(IAction&){
    if(_state != ProgramState_t::STATE_FINISHED)
        return;
    //ManipulatorController* man = ((Serial6DofManipulator*)projectsManager.getOpenedProject())->getManipulatorController();
    //action.execute(*man, 0);
}

void ActionsController::stop()
{
    emit messageToThread(serialMan::ProgramState_t::STATE_FINISHED);
}

void ActionsController::pause()
{
    emit messageToThread(serialMan::ProgramState_t::STATE_SUSPENDED);
}

void ActionsController::resume()
{
    emit messageToThread(serialMan::ProgramState_t::STATE_IS_RUNNING);
}

void ActionsController::setPosition(int)
{

}

int ActionsController::position()
{
    return 0;
}

void ActionsController::stateChanged(serialMan::ProgramState_t state)
{
    _state = state;
}

//Program executor

ProgramExecutor::ProgramExecutor(ActionsContainer& actions, ManipulatorController& man)
    : QObject(), _man(man), _actions(actions), _state(ProgramState_t::STATE_IS_RUNNING)
{
}

void ProgramExecutor::start()
{
    emit onStateChanged(_state);
    auto iter = _actions.begin();
    bool exit = false;
    _frameTime = QDateTime::currentMSecsSinceEpoch();

    for(;iter != _actions.end() && exit == false; iter++)
    {
        ActionResult_t result = ActionResult_t::RESULT_IN_PROCESS;
        IAction* action = *iter;
        action->startExecution();
        while(result == ActionResult_t::RESULT_IN_PROCESS)
        {
            qint64 diff = QDateTime::currentMSecsSinceEpoch() - _frameTime;
            if(diff < _frameDiff)
            {
                QThread::yieldCurrentThread();
                continue;
            }
            _frameTime = QDateTime::currentMSecsSinceEpoch();

            if(_state == ProgramState_t::STATE_IS_RUNNING)
            {
                result = action->execute(_man, diff);
            }
            else if(_state == ProgramState_t::STATE_SUSPENDED)
            {
                QThread::yieldCurrentThread();
            }
            else
            {
                exit = true;
                break;
            }
        }
        action->endExecution();
    }

    _state = ProgramState_t::STATE_FINISHED;
    emit onStateChanged(_state);
    emit onFinished();
}


void ProgramExecutor::suspend()
{
    _state = ProgramState_t::STATE_SUSPENDED;
    emit onStateChanged(_state);

}

void ProgramExecutor::resume()
{
    _state = ProgramState_t::STATE_IS_RUNNING;
    emit onStateChanged(_state);

}

void ProgramExecutor::stop()
{
    _state = ProgramState_t::STATE_FINISHED;
    emit onStateChanged(_state);

}

void ProgramExecutor::setState(serialMan::ProgramState_t state)
{
    _state = state;
    emit onStateChanged(_state);
}


