#include "Actions.h"

#include "../../logic/models/unitsConverter.h"
#include "../manipulatorcontroller.h"


using namespace serialMan;
using namespace actions;

typedef std::shared_ptr<ArgDescription_t> argShell_t;

namespace args
{

//POSITION
static auto xPos =  ArgDescription_t{.key='X', .type = QVariant::Type::Double, .name = "X coord",
        .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED, .manParameter = ManipulatorController::PARAMETER_X_EFF_POS};
static auto yPos =  ArgDescription_t{.key='Y', .type = QVariant::Type::Double, .name = "Y coord",
        .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED, .manParameter = ManipulatorController::PARAMETER_Y_EFF_POS};
static auto zPos =  ArgDescription_t{.key='Z', .type = QVariant::Type::Double, .name = "Z coord",
        .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED, .manParameter = ManipulatorController::PARAMETER_Z_EFF_POS};
//ROTATION
static auto aRot =  ArgDescription_t{.key='A', .type = QVariant::Type::Double, .name = "Alfa degrees",
        .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_ANGLE,
        .manParameter = ManipulatorController::PARAMETER_A_EFF_POS};
static auto bRot =  ArgDescription_t{.key='B', .type = QVariant::Type::Double, .name = "Beta degrees",
        .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_ANGLE,
        .manParameter = ManipulatorController::PARAMETER_B_EFF_POS};
static auto gRot =  ArgDescription_t{.key='G', .type = QVariant::Type::Double, .name = "Gamma degrees",
        .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_ANGLE,
        .manParameter = ManipulatorController::PARAMETER_G_EFF_POS};
//Move parameters
static auto speed =  ArgDescription_t{.key='F', .type = QVariant::Type::Double, .name = "Move/rotation speed",
        .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_REQUIRED};
static auto time =  ArgDescription_t{.key='T', .type = QVariant::Type::Double, .name = "Time",
        .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_NUM_POSITIVE_ONLY | ActionArgumentParameter::ARGPARAM_IS_REQUIRED};

//Effector
static auto effectorKin =  ArgDescription_t{.key='P', .type = QVariant::Type::Double, .name = "Effector",
        .manParameter = ManipulatorController::PARAMETER_EFFECTOR_VALUE};

//Joints
static auto jointA =  ArgDescription_t{.key='A', .type = QVariant::Type::Double, .name = "Joint A (0)", .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_ANGLE,
        .manParameter = ManipulatorController::PARAMETER_JOINT_A_VALUE};

static auto jointB =  ArgDescription_t{.key='B', .type = QVariant::Type::Double, .name = "Joint B (1)", .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_ANGLE,
        .manParameter = ManipulatorController::PARAMETER_JOINT_B_VALUE};

static auto jointC =  ArgDescription_t{.key='C', .type = QVariant::Type::Double, .name = "Joint C (2)", .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_ANGLE,
        .manParameter = ManipulatorController::PARAMETER_JOINT_C_VALUE};

static auto jointD =  ArgDescription_t{.key='D', .type = QVariant::Type::Double, .name = "Joint D (3)", .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_ANGLE,
        .manParameter = ManipulatorController::PARAMETER_JOINT_D_VALUE};

static auto jointE =  ArgDescription_t{.key='E', .type = QVariant::Type::Double, .name = "Joint E (4)", .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_ANGLE,
        .manParameter = ManipulatorController::PARAMETER_JOINT_E_VALUE};

static auto jointF =  ArgDescription_t{.key='F', .type = QVariant::Type::Double, .name = "Joint F (5)", .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_ANGLE,
        .manParameter = ManipulatorController::PARAMETER_JOINT_F_VALUE};

static auto speedJoint =  ArgDescription_t{.key='S', .type = QVariant::Type::Double, .name = "Rotation speed",
        .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_REQUIRED | ActionArgumentParameter::ARGPARAM_NUM_POSITIVE_ONLY};

static auto xAxis =  ArgDescription_t{.key='X', .type = QVariant::Type::Invalid, .name = "X axis",
        .parameters = ARGPARAM_NO_VALUE};

static auto yAxis =  ArgDescription_t{.key='Y', .type = QVariant::Type::Invalid, .name = "Y axis",
        .parameters = ARGPARAM_NO_VALUE};

static auto zAxis =  ArgDescription_t{.key='Z', .type = QVariant::Type::Invalid, .name = "Z axis",
        .parameters = ARGPARAM_NO_VALUE};

static auto xRotAxis =  ArgDescription_t{.key='A', .type = QVariant::Type::Invalid, .name = "X axis",
        .parameters = ARGPARAM_NO_VALUE};

static auto yRotAxis =  ArgDescription_t{.key='B', .type = QVariant::Type::Invalid, .name = "Y axis",
        .parameters = ARGPARAM_NO_VALUE};

static auto zRotAxis =  ArgDescription_t{.key='G', .type = QVariant::Type::Invalid, .name = "Z axis",
        .parameters = ARGPARAM_NO_VALUE};

}



const QVector<ArgDescription_t*> G0::_keys = {
    (&args::xPos),
    (&args::yPos),
    (&args::zPos),
    (&args::aRot),
    (&args::bRot),
    (&args::gRot),
    (&args::effectorKin),
    (&args::time),
};

ActionExectionResult G0::startExecution(IArgsCollection& args, const ExecutionEnivroment& env)
{
    _startJointsPos.resize(ManipulatorController::DEFAULT_DOF+1);
    _endJointsPos.resize(ManipulatorController::DEFAULT_DOF+1);
    size_t i = 0;
    for(auto& j : env.manipulator().getJoints())
    {
        _startJointsPos[i++] = j->getValue();
    }
    _endPos = env.manipulator().getEffectorPosition();
    if(args.isArgUsable('X')) _endPos.x = args.getArg('X')->toDouble();
    if(args.isArgUsable('Y')) _endPos.y = args.getArg('Y')->toDouble();
    if(args.isArgUsable('Z')) _endPos.z = args.getArg('Z')->toDouble();
    if(args.isArgUsable('A')) _endPos.wx = degToRad(args.getArg('A')->toDouble());
    if(args.isArgUsable('B')) _endPos.wy = degToRad(args.getArg('B')->toDouble());
    if(args.isArgUsable('G')) _endPos.wz = degToRad(args.getArg('G')->toDouble());
    if(args.isArgUsable('T')) _time = args.getArg('T')->toDouble();
    if(args.isArgUsable('P')) _endJointsPos[ManipulatorController::DEFAULT_DOF] = args.getArg('P')->toDouble();
    auto result = env.manipulator().kinematics().inverse(_endPos, _endJointsPos, 0);
    if(result == CalculationResult_t::CALC_ERROR)
        return ActionExectionResult::RESULT_ERROR;
    _startJointsPos[ManipulatorController::DEFAULT_DOF] = env.manipulator().getEffector().value();
    return ActionExectionResult::RESULT_IN_PROCESS;
}

ActionExectionResult G0::execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig config)
{
    QVector<double> jBuffer(ManipulatorController::DEFAULT_DOF+1);
    bool end{false};
    if(t >= _time || config == ExecuteConfig::EXECUTE_MOMENTLY)
    {
        end = true;
        env.manipulator().blockSignals(true);
        env.manipulator().getEffector().setValue(_endJointsPos[ManipulatorController::DEFAULT_DOF]);
        env.manipulator().blockSignals(false);
        env.manipulator().inverseKinematics(_endPos);
    }else
    {
        for(size_t i = 0; i < ManipulatorController::DEFAULT_DOF+1; i++)
        {
            jBuffer[i] = _startJointsPos[i] + (_endJointsPos[i] - _startJointsPos[i]) * (double)t / _time;
        }
        env.manipulator().blockSignals(true);
        env.manipulator().getEffector().setValue(jBuffer[ManipulatorController::DEFAULT_DOF]);
        env.manipulator().blockSignals(false);
        env.manipulator().forwardKinematics(jBuffer);
    }
    return end ? ActionExectionResult::RESULT_FINISH : ActionExectionResult::RESULT_IN_PROCESS;
}

void G0::endExecution()
{
}


const QVector<ArgDescription_t*> G1::_keys = {
    (&args::xPos),
    (&args::yPos),
    (&args::zPos),
    (&args::aRot),
    (&args::bRot),
    (&args::gRot),
    (&args::effectorKin),
    (&args::speed),
};

ActionExectionResult G1::startExecution(IArgsCollection& args, const ExecutionEnivroment& env)
{
    _startPos = env.manipulator().getEffectorPosition();
    _endPos = _startPos;
    _effStartPos = env.manipulator().getEffector().value();
    _effEndPos = _effStartPos;

    if(args.isArgUsable('X')) _endPos.x = args.getArg('X')->toDouble();
    if(args.isArgUsable('Y')) _endPos.y = args.getArg('Y')->toDouble();
    if(args.isArgUsable('Z')) _endPos.z = args.getArg('Z')->toDouble();
    if(args.isArgUsable('A')) _endPos.wx = degToRad(args.getArg('A')->toDouble());
    if(args.isArgUsable('B')) _endPos.wy = degToRad(args.getArg('B')->toDouble());
    if(args.isArgUsable('G')) _endPos.wz = degToRad(args.getArg('G')->toDouble());
    if(args.isArgUsable('F')) _speed = args.getArg('F')->toDouble();
    if(args.isArgUsable('P')) _effEndPos = args.getArg('P')->toDouble();

    double _dist = sqrt(pow(_endPos.x-_startPos.x, 2) + pow(_endPos.y-_startPos.y, 2) + pow(_endPos.z-_startPos.z, 2));
    if(_dist == 0)
        _dist = radToDeg(sqrt(pow(_endPos.wx-_startPos.wx, 2) + pow(_endPos.wy-_startPos.wy, 2) + pow(_endPos.wz-_startPos.wz, 2)));
    if(_dist == 0)
        _dist = abs(_effEndPos - _effStartPos);
    if(_speed == 0)
        _time = 1;
    else
        _time = _dist / _speed * 1000;
    return ActionExectionResult::RESULT_IN_PROCESS;
}

ActionExectionResult G1::execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig config)
{
    Position_t eff = env.manipulator().getEffectorPosition();
    double effValue = env.manipulator().getEffector().value();
    bool end = false;
    if(t >= _time || config == ExecuteConfig::EXECUTE_MOMENTLY)
    {
        eff = _endPos;
        effValue = _effEndPos;
        end = true;
    } else {
        eff.x = _startPos.x + (_endPos.x - _startPos.x) * (double)t / _time;
        eff.y = _startPos.y + (_endPos.y - _startPos.y) * (double)t / _time;
        eff.z = _startPos.z + (_endPos.z - _startPos.z) * (double)t / _time;
        eff.wx = _startPos.wx + (_endPos.wx - _startPos.wx) * (double)t / _time;
        eff.wy = _startPos.wy + (_endPos.wy - _startPos.wy) * (double)t / _time;
        eff.wz = _startPos.wz + (_endPos.wz - _startPos.wz) * (double)t / _time;
        effValue = _effStartPos + (_effEndPos - _effStartPos) * (double)t / _time;
    }
    //avoid double updating of the structure (crutch)
    env.manipulator().blockSignals(true);
    env.manipulator().getEffector().setValue(effValue);
    env.manipulator().blockSignals(false);
    auto result = env.manipulator().inverseKinematics(eff);
    if(result == CalculationResult_t::CALC_ERROR)
        return ActionExectionResult::RESULT_ERROR;
    return end ? ActionExectionResult::RESULT_FINISH : ActionExectionResult::RESULT_IN_PROCESS;

}

void G1::endExecution()
{
}


const QVector<ArgDescription_t*> G5::_keys = {
    (&args::jointA),
    (&args::jointB),
    (&args::jointC),
    (&args::jointD),
    (&args::jointE),
    (&args::jointF),
    (&args::effectorKin),
    (&args::speedJoint),
};

ActionExectionResult G5::startExecution(IArgsCollection& args, const ExecutionEnivroment& env)
{
    _startJointsPos.resize(ManipulatorController::DEFAULT_DOF+1);
    _endJointsPos.resize(ManipulatorController::DEFAULT_DOF+1);

    auto& j = env.manipulator().getJoints();
    for(int i = 0; i < ManipulatorController::DEFAULT_DOF; i++)
    {
        _startJointsPos[i] = _endJointsPos[i] = j[i]->getValue();
    }

    _startJointsPos[ManipulatorController::DEFAULT_DOF] = env.manipulator().getEffector().value();
    _endJointsPos[ManipulatorController::DEFAULT_DOF] = _startJointsPos[ManipulatorController::DEFAULT_DOF];

    if(args.isArgUsable('A')) _endJointsPos[0] = degToRad(args.getArg('A')->toDouble());
    if(args.isArgUsable('B')) _endJointsPos[1] = degToRad(args.getArg('B')->toDouble());
    if(args.isArgUsable('C')) _endJointsPos[2] = degToRad(args.getArg('C')->toDouble());
    if(args.isArgUsable('D')) _endJointsPos[3] = degToRad(args.getArg('D')->toDouble());
    if(args.isArgUsable('E')) _endJointsPos[4] = degToRad(args.getArg('E')->toDouble());
    if(args.isArgUsable('F')) _endJointsPos[5] = degToRad(args.getArg('F')->toDouble());
    if(args.isArgUsable('P')) _endJointsPos[ManipulatorController::DEFAULT_DOF] = args.getArg('P')->toDouble();
    if(args.isArgUsable('S')) _speed = args.getArg('S')->toDouble();

    return ActionExectionResult::RESULT_IN_PROCESS;
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

ActionExectionResult G5::execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig config)
{
    QVector<double> joints(ManipulatorController::DEFAULT_DOF+1);
    bool end {true};
    if(config == ExecuteConfig::EXECUTE_MOMENTLY)
    {
        joints = _endJointsPos;
    } else
    {
        for(int i = 0; i < ManipulatorController::DEFAULT_DOF+1; i++)
        {
            int koef = _startJointsPos[i] > _endJointsPos[i] ? -1 : _startJointsPos[i] < _endJointsPos[i] ? 1 : 0;
            if(koef == 0)
            {
                joints[i] = _endJointsPos[i];
                continue;
            }
            double t1{_speed};
            if(i != ManipulatorController::DEFAULT_DOF)
                t1= degToRad(_speed);
            double t2 = (double)t / 1000.0;
            double new_pos = _startJointsPos[i] + t1 * t2 * koef;
            if(sgn(_endJointsPos[i] - new_pos) != koef)
            {
                joints[i] = _endJointsPos[i];
            }
            else
            {
                end = false;
                joints[i] = new_pos;
            }
        }
    }
    env.manipulator().blockSignals(true);
    env.manipulator().getEffector().setValue(joints[ManipulatorController::DEFAULT_DOF]);
    env.manipulator().blockSignals(false);
    env.manipulator().forwardKinematics(joints);
    return end ? ActionExectionResult::RESULT_FINISH : ActionExectionResult::RESULT_IN_PROCESS;
}

void G5::endExecution()
{
}



const QVector<ArgDescription_t*> G6::_keys = {
    (&args::jointA),
    (&args::jointB),
    (&args::jointC),
    (&args::jointD),
    (&args::jointE),
    (&args::jointF),
    (&args::effectorKin),
    (&args::time),
};

ActionExectionResult G6::startExecution(IArgsCollection& args, const ExecutionEnivroment& env)
{
    _startJointsPos.resize(ManipulatorController::DEFAULT_DOF+1);
    _endJointsPos.resize(ManipulatorController::DEFAULT_DOF+1);
    auto& j = env.manipulator().getJoints();

    for(int i = 0; i < ManipulatorController::DEFAULT_DOF; i++)
    {
        _startJointsPos[i] = _endJointsPos[i] = j[i]->getValue();
    }

    _startJointsPos[ManipulatorController::DEFAULT_DOF] = env.manipulator().getEffector().value();
    _endJointsPos[ManipulatorController::DEFAULT_DOF] = _startJointsPos[ManipulatorController::DEFAULT_DOF];

    if(args.isArgUsable('A')) _endJointsPos[0] = degToRad(args.getArg('A')->toDouble());
    if(args.isArgUsable('B')) _endJointsPos[1] = degToRad(args.getArg('B')->toDouble());
    if(args.isArgUsable('C')) _endJointsPos[2] = degToRad(args.getArg('C')->toDouble());
    if(args.isArgUsable('D')) _endJointsPos[3] = degToRad(args.getArg('D')->toDouble());
    if(args.isArgUsable('E')) _endJointsPos[4] = degToRad(args.getArg('E')->toDouble());
    if(args.isArgUsable('F')) _endJointsPos[5] = degToRad(args.getArg('F')->toDouble());
    if(args.isArgUsable('P')) _endJointsPos[ManipulatorController::DEFAULT_DOF] = args.getArg('P')->toDouble();
    if(args.isArgUsable('T')) _time = args.getArg('T')->toDouble();

    return ActionExectionResult::RESULT_IN_PROCESS;
}

ActionExectionResult G6::execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig config)
{
    QVector<double> joints(ManipulatorController::DEFAULT_DOF+1);
    bool end {false};
    if(config == ExecuteConfig::EXECUTE_MOMENTLY || t >= _time)
    {
        joints = _endJointsPos;
        end = true;
    } else
    {
        for(int i = 0; i < ManipulatorController::DEFAULT_DOF; i++)
        {
            joints[i] = _startJointsPos[i] + (_endJointsPos[i] - _startJointsPos[i]) * (double)t / _time;
        }
    }
    env.manipulator().blockSignals(true);
    env.manipulator().getEffector().setValue(joints[ManipulatorController::DEFAULT_DOF]);
    env.manipulator().blockSignals(false);
    env.manipulator().forwardKinematics(joints);
    return end ? ActionExectionResult::RESULT_FINISH : ActionExectionResult::RESULT_IN_PROCESS;
}

void G6::endExecution()
{
}



const QVector<ArgDescription_t*> G28::_keys = {
    (&args::jointA),
    (&args::jointB),
    (&args::jointC),
    (&args::jointD),
    (&args::jointE),
    (&args::jointF),
    (&args::time),
};

ActionExectionResult G28::startExecution(IArgsCollection& args, const ExecutionEnivroment& env)
{
    _startJointsPos.resize(ManipulatorController::DEFAULT_DOF);
    _endJointsPos.resize(ManipulatorController::DEFAULT_DOF);
    auto& j = env.manipulator().getJoints();
    for(int i = 0; i < ManipulatorController::DEFAULT_DOF; i++)
    {
        _startJointsPos[i] = _endJointsPos[i] = j[i]->getValue();
    }

    if(args.isArgUsable('A')) _endJointsPos[0] = degToRad(args.getArg('A')->toDouble());
    if(args.isArgUsable('B')) _endJointsPos[1] = degToRad(args.getArg('B')->toDouble());
    if(args.isArgUsable('C')) _endJointsPos[2] = degToRad(args.getArg('C')->toDouble());
    if(args.isArgUsable('D')) _endJointsPos[3] = degToRad(args.getArg('D')->toDouble());
    if(args.isArgUsable('E')) _endJointsPos[4] = degToRad(args.getArg('E')->toDouble());
    if(args.isArgUsable('F')) _endJointsPos[5] = degToRad(args.getArg('F')->toDouble());
    if(args.isArgUsable('T')) _time = args.getArg('T')->toDouble();

    return ActionExectionResult::RESULT_IN_PROCESS;
}

ActionExectionResult G28::execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig config)
{
    QVector<double> joints(ManipulatorController::DEFAULT_DOF);
    bool end {false};
    if(config == ExecuteConfig::EXECUTE_MOMENTLY || t >= _time)
    {
        joints = _endJointsPos;
        end = true;
    } else
    {
        for(int i = 0; i < ManipulatorController::DEFAULT_DOF; i++)
        {
            joints[i] = _startJointsPos[i] + (_endJointsPos[i] - _startJointsPos[i]) * (double)t / _time;
        }
    }
    env.manipulator().forwardKinematics(joints);
    return end ? ActionExectionResult::RESULT_FINISH : ActionExectionResult::RESULT_IN_PROCESS;
}

void G28::endExecution()
{
}


const QVector<ArgDescription_t*> GTEST::_keys = {(&args::time)};

ActionExectionResult GTEST::startExecution(IArgsCollection& args, const ExecutionEnivroment& )
{
    qDebug() << "Started";
    if(args.isArgUsable('T'))
        _param = args.getArg('T')->toDouble();
    return ActionExectionResult::RESULT_IN_PROCESS;
}

ActionExectionResult GTEST::execute(const ExecutionEnivroment& , qint64 t, ExecuteConfig config)
{
    qDebug()<<t;
    if(t >=_param || config == ExecuteConfig::EXECUTE_MOMENTLY)
    {
        qDebug() << "Ended";
        return ActionExectionResult::RESULT_FINISH;
    }

    return ActionExectionResult::RESULT_IN_PROCESS;
}

void GTEST::endExecution()
{

}
