#include "actionscontroller.h"
#include "manipulatorcontroller.h"

#include <QTimer>
#include <QDebug>

using namespace serialMan;

ActionsController::ActionsController(ManipulatorController& man)
    : QObject(),  _man(man), _enivroment(man)
{
    //actions.append(createAction<LinearMovement>());

//    auto temp = _enivroment.createAction<actions::G1>();
//    temp->setArg(ArgKey_t{'X'}, {160.0});
//    temp->setArg(ArgKey_t{'Y'}, {0.0});
//    temp->setArg(ArgKey_t{'Z'}, {80.0});
//    temp->setArg(ArgKey_t{'B'}, {degToRad(180.0)});
//    temp->setArg(ArgKey_t{'F'}, {45});
//    _enivroment.program.append(temp);

//    temp = _enivroment.createAction<actions::G1>();
//    temp->setArg(ArgKey_t{'A'}, {degToRad(-90.0)});
//    temp->setArg(ArgKey_t{'G'}, {degToRad(-270.0)});
//    temp->setArg(ArgKey_t{'F'}, {0});
//    _enivroment.program.append(temp);

//    temp = _enivroment.createAction<actions::G1>();
//    temp->setArg(ArgKey_t{'Y'}, {-50});
//    temp->setArg(ArgKey_t{'B'}, {degToRad(225.0)});
//    temp->setArg(ArgKey_t{'F'}, {20});
//    _enivroment.program.append(temp);

//    temp = _enivroment.createAction<actions::G1>();
//    temp->setArg(ArgKey_t{'Z'}, {130});
//    temp->setArg(ArgKey_t{'B'}, {degToRad(180)});
//    temp->setArg(ArgKey_t{'F'}, {50});
//    _enivroment.program.append(temp);

//    temp = _enivroment.createAction<actions::G1>();
//    temp->setArg(ArgKey_t{'B'}, {degToRad(135.0)});
//    temp->setArg(ArgKey_t{'Z'}, {80.0});
//    temp->setArg(ArgKey_t{'Y'}, {0});
//    temp->setArg(ArgKey_t{'F'}, {60});
//    _enivroment.program.append(temp);

//    temp = _enivroment.createAction<actions::G1>();
//    temp->setArg(ArgKey_t{'B'}, {degToRad(180.0)});
//    temp->setArg(ArgKey_t{'F'}, {20});
//    _enivroment.program.append(temp);

//    temp = _enivroment.createAction<actions::G1>();
//    temp->setArg(ArgKey_t{'A'}, {degToRad(0.0)});
//    temp->setArg(ArgKey_t{'G'}, {degToRad(-180.0)});
//    temp->setArg(ArgKey_t{'F'}, {0});
//    _enivroment.program.append(temp);

//    temp = _enivroment.createAction<actions::G1>();
//    temp->setArg(ArgKey_t{'Z'}, {180.0});
//    temp->setArg(ArgKey_t{'A'}, {degToRad(0.0)});
//    temp->setArg(ArgKey_t{'B'}, {degToRad(90.0)});
//    temp->setArg(ArgKey_t{'G'}, {degToRad(-180.0)});
//    temp->setArg(ArgKey_t{'F'}, {30});
//    _enivroment.program.append(temp);
}

void ActionsController::startProgram()
{

    if(_enivroment.state() != ExecutionState::STATE_FINISHED)
        return;
    _executor = new ProgramExecutor(_enivroment);
    _thread = new QThread();

    //connect(this, &ActionsController::messageToThread, _executor, &serialMan::ProgramExecutor::setState, Qt::DirectConnection );
    //connect(_executor, &serialMan::ProgramExecutor::onStateChanged, this, &ActionsController::onStateChanged, Qt::DirectConnection );
    //connect(_executor, &serialMan::ProgramExecutor::onStateChanged, this, &ActionsController::stateChanged, Qt::DirectConnection );

    //connect(_thread, &QThread::started, _executor, &serialMan::ProgramExecutor::start);

    connect(_executor, &serialMan::ProgramExecutor::finished, _thread, &QThread::quit);
    connect(_thread, &QThread::finished, _executor, &serialMan::ProgramExecutor::deleteLater);
    connect(_thread, &QThread::finished, _thread, &QThread::deleteLater);

    _executor->moveToThread(_thread);

    _thread->start();
}

ExecutionEnivroment const& ActionsController::enivroment() const
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
ProgramExecutor::ProgramExecutor(ExecutionEnivroment& env)
    : QObject(), _env(env)
{
    //connect(&env, &serialMan::ExecutionEnivroment::stateChanged, this, &ProgramExecutor::onStateChanged);
}

//void ProgramExecutor::onStateChanged(serialMan::ExecutionState state)
//{

//}


void ProgramExecutor::onStarted()
{
    _env.setState(ExecutionState::STATE_IS_RUNNING);
    QTimer time;
    actions::ActionExectionResult_t execRes{actions::ActionExectionResult_t::RESULT_UNKNOWN};
    while (_env.state() != ExecutionState::STATE_FINISHED)
    {
        actions::IAction* action = _env.program().next().get();
        if (!action)
            break;
        execRes = action->startExecution(_env);
        time.start();
        while (execRes == actions::ActionExectionResult_t::RESULT_IN_PROCESS)
        {
            execRes = action->execute(_env, time.remainingTimeAsDuration().count(), actions::ExecuteConfig_t::EXECUTE_ANIMATION);
            if (execRes == actions::ActionExectionResult_t::RESULT_ERROR)
            {
                qDebug() << "Execution action error";
            }

        }

    }
}

//void ProgramExecutor::onStarted()
//{
//    emit onStateChanged(_state);
//    auto iter = _actions.program.begin();
//    bool exit = false;
//    _frameTime = QDateTime::currentMSecsSinceEpoch();

//    for(;iter != _actions.program.end() && exit == false; iter++)
//    {
//        ActionResult_t result = ActionResult_t::RESULT_IN_PROCESS;
//        IAction* action = *iter;
//        action->startExecution();
//        while(result == ActionResult_t::RESULT_IN_PROCESS)
//        {
//            qint64 diff = QDateTime::currentMSecsSinceEpoch() - _frameTime;
//            if(diff < _frameDiff)
//            {
//                QThread::yieldCurrentThread();
//                continue;
//            }
//            _frameTime = QDateTime::currentMSecsSinceEpoch();

//            if(_state == ProgramState_t::STATE_IS_RUNNING)
//            {
//                result = action->execute(diff, ExecuteConfig_t::EXECUTE_ANIMATION);
//            }
//            else if(_state == ProgramState_t::STATE_SUSPENDED)
//            {
//                QThread::yieldCurrentThread();
//            }
//            else
//            {
//                exit = true;
//                break;
//            }
//        }
//        action->endExecution();
//    }

//    _state = ProgramState_t::STATE_FINISHED;
//    emit onStateChanged(_state);
//    emit onFinished();
//}





