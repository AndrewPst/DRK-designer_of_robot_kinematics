#include "actionscontroller.h"
#include "manipulatorcontroller.h"

#include "models/Actions.h"
#include <QDateTime>

#include <QDebug>

using namespace serialMan;

ActionsController::ActionsController(ManipulatorController& man)
    : QObject(),  _man(man), _enivroment(man), _state(ProgramState_t::STATE_FINISHED)
{
    //actions.append(createAction<LinearMovement>());
    auto temp = _enivroment.createAction<LinearMovement>();
    temp->setArg('X', {ActionArgumentType_t::ARGTYPE_DOUBLE, 15.0});
    temp->setArg('Y', {ActionArgumentType_t::ARGTYPE_DOUBLE,8.0});
    temp->setArg('Z', {ActionArgumentType_t::ARGTYPE_DOUBLE,15.0});
    temp->setArg('A', {ActionArgumentType_t::ARGTYPE_DOUBLE,90.0});
    temp->setArg('G', {ActionArgumentType_t::ARGTYPE_DOUBLE,-90.0});
    temp->setArg('F', {ActionArgumentType_t::ARGTYPE_DOUBLE,0.5});
    _enivroment.program.append(temp);
}

void ActionsController::startProgram()
{

    if(_state != ProgramState_t::STATE_FINISHED)
        return;
    _executor = new ProgramExecutor(_enivroment, _man);
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

void ActionsController::setPosition(int i)
{
    IAction& action = _enivroment.program.at(i);
    action.startExecution();
    action.execute(0, ExecuteConfig_t::EXECUTE_INSTANTLY);
    action.endExecution();
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

ProgramExecutor::ProgramExecutor(ActionsEnivroment& actions, ManipulatorController& man)
    : QObject(), _man(man), _actions(actions), _state(ProgramState_t::STATE_IS_RUNNING)
{
}

void ProgramExecutor::start()
{
    emit onStateChanged(_state);
    auto iter = _actions.program.begin();
    bool exit = false;
    _frameTime = QDateTime::currentMSecsSinceEpoch();

    for(;iter != _actions.program.end() && exit == false; iter++)
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
                result = action->execute(diff, ExecuteConfig_t::EXECUTE_ANIMATION);
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


