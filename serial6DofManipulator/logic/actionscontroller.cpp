#include "actionscontroller.h"
#include "manipulatorcontroller.h"
#include "models/executionState.h"
#include "models/unitsConverter.h"

#include <QTimer>
#include <QDebug>
#include <QVariant>

using namespace serialMan;

ActionsController::ActionsController(ManipulatorController& man)
    : QObject(),  _man(man), _enivroment(man)
{
    //Generate first test action
    auto actParams = std::shared_ptr<actions::IArgsCollection>(_lib.argsCollectionGenerator({'G', 1})());
    actParams->setArg('Y', (-50.0));
    actParams->setArg('Z', (180.0));
    actParams->setArg('A', (degToRad(-15.0)));
    actParams->setArg('F', (10.0));
    _enivroment.program().add(EnivromentProgram::actionData_t({'G', 1}, actParams));

    actParams = std::shared_ptr<actions::IArgsCollection>(_lib.argsCollectionGenerator({'G', 1})());
    actParams->setArg('X', (220.0));
    actParams->setArg('F', (50.0));
    actParams->setArg('A', (degToRad(0.0)));
    _enivroment.program().add(EnivromentProgram::actionData_t({'G', 1}, actParams));

    actParams = std::shared_ptr<actions::IArgsCollection>(_lib.argsCollectionGenerator({'G', 255})());
    actParams->setArg('T', (1500.0));
    _enivroment.program().add(EnivromentProgram::actionData_t({'G', 255}, actParams));

    actParams = std::shared_ptr<actions::IArgsCollection>(_lib.argsCollectionGenerator({'G', 1})());
    actParams->setArg('Y', (0.0));
    actParams->setArg('X', (180.0));
    actParams->setArg('Z', (150.0));
    actParams->setArg('A', (degToRad(15.0)));
    actParams->setArg('F', (50.0));
    _enivroment.program().add(EnivromentProgram::actionData_t({'G', 1}, actParams));

    actParams = std::shared_ptr<actions::IArgsCollection>(_lib.argsCollectionGenerator({'G', 1})());
    actParams->setArg('Y', (0.0));
    actParams->setArg('X', (180.0));
    actParams->setArg('Z', (150.0));
    actParams->setArg('A', (degToRad(0.0)));
    actParams->setArg('F', (50.0));
    _enivroment.program().add(EnivromentProgram::actionData_t({'G', 1}, actParams));
}

void ActionsController::startProgram()
{

    if(_enivroment.state() != ExecutionState::STATE_FINISHED)
        return;
    _executor = new ProgramExecutor(_enivroment, library());
    _thread = new QThread();

    connect(_thread, &QThread::started, _executor, &serialMan::ProgramExecutor::onStarted);

    connect(_executor, &serialMan::ProgramExecutor::finished, _thread, &QThread::quit);
    connect(_thread, &QThread::finished, _executor, &serialMan::ProgramExecutor::deleteLater);
    connect(_thread, &QThread::finished, _thread, &QThread::deleteLater);

    _executor->moveToThread(_thread);
    _thread->start();
}

bool ActionsController::executeMomently(EnivromentProgram::actionData_t& actionData)
{
    if(enivroment().state() != ExecutionState::STATE_FINISHED)
        return false;

    auto action = std::shared_ptr<actions::IAction>(_lib.actionGenerator(actionData.first)());
    action->startExecution(*actionData.second, _enivroment);
    action->execute(_enivroment, 0, actions::ExecuteConfig::EXECUTE_MOMENTLY);
    action->endExecution();
    return true;
}


const actions::ActionsLibrary& ActionsController::library() const
{
    return _lib;
}

ExecutionEnivroment& ActionsController::enivroment()
{
    return _enivroment;
}

void ActionsController::stop()
{
    _enivroment.setState(serialMan::ExecutionState::STATE_FINISHED);
}

void ActionsController::pause()
{
    _enivroment.setState(serialMan::ExecutionState::STATE_SUSPENDED);
}

void ActionsController::resume()
{
    _enivroment.setState(serialMan::ExecutionState::STATE_IS_RUNNING);
}


//-------------------Program executor------------------
ProgramExecutor::ProgramExecutor(ExecutionEnivroment& env, const actions::ActionsLibrary& lib)
    : QObject(), _env(env), _lib(lib)
{
}


static const actions::actionIdentificator_t _last_action{0, 0};

void ProgramExecutor::onStarted()
{
    _env.setState(ExecutionState::STATE_IS_RUNNING);
    ExecutionState lastState{ExecutionState::STATE_IS_RUNNING};
    actions::ActionExectionResult execRes{actions::ActionExectionResult::RESULT_UNKNOWN};
    ExecutionExitCode exitCode{ExecutionExitCode::EXITCODE_NO_EXIT};
    while (exitCode == ExecutionExitCode::EXITCODE_NO_EXIT)
    {
        if(_env.state() == ExecutionState::STATE_FINISHED)
        {
            exitCode = ExecutionExitCode::EXITCODE_INTERRUPTION;
            break;
        }
        auto actionData = _env.program().next();
        if (actionData.first == _last_action)
            break;
        auto action = std::shared_ptr<actions::IAction>(_lib.actionGenerator(actionData.first)());
        int64_t startTime{QDateTime::currentMSecsSinceEpoch()};
        int64_t lastFrameTime{0};
        int64_t pauseTime{0};
        execRes = action->startExecution(*actionData.second, _env);
        while (execRes == actions::ActionExectionResult::RESULT_IN_PROCESS)
        {
            if(QDateTime::currentMSecsSinceEpoch() - lastFrameTime < _frameDiff)
            {
                QThread::yieldCurrentThread();
                continue;
            }
            lastFrameTime = QDateTime::currentMSecsSinceEpoch();
            if(_env.state() == ExecutionState::STATE_IS_RUNNING)
            {
                if(_env.state() != lastState)
                {
                    startTime += QDateTime::currentMSecsSinceEpoch() - pauseTime;
                    lastFrameTime = QDateTime::currentMSecsSinceEpoch();
                    lastState = _env.state();
                }
                execRes = action->execute(_env, QDateTime::currentMSecsSinceEpoch() - startTime, actions::ExecuteConfig::EXECUTE_ANIMATION);
                if (execRes == actions::ActionExectionResult::RESULT_ERROR)
                {
                    exitCode = ExecutionExitCode::EXITCODE_EXECUTION_ERROR;
                    break;
                }
            } else if(_env.state() == ExecutionState::STATE_SUSPENDED)
            {
                if(_env.state() != lastState)
                {
                    pauseTime = QDateTime::currentMSecsSinceEpoch();
                    lastState = _env.state();
                }
                QThread::yieldCurrentThread();
                continue;
            } else
            {
                exitCode = ExecutionExitCode::EXITCODE_INTERRUPTION;
                break;
            }
        }
        action->endExecution();
    }
    _env.setState(ExecutionState::STATE_FINISHED);
    _env.program().reset();
    emit finished(exitCode);
}



