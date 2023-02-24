#include "Actions.h"

#include "../../logic/models/unitsConverter.h"
#include "../manipulatorcontroller.h"


using namespace serialMan;
using namespace actions;

namespace args
{
const static auto xPos = std::shared_ptr<ArgDescription_t>(new ArgDescription_t{.key='X', .type = QVariant::Type::Double, .name = "X coord",
                                                                                .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED, .manParameter = ManipulatorController::PARAMETER_X_EFF_POS});
const static auto yPos = std::shared_ptr<ArgDescription_t>(new ArgDescription_t{.key='Y', .type = QVariant::Type::Double, .name = "Y coord",
                                                                                .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED, .manParameter = ManipulatorController::PARAMETER_Y_EFF_POS});
const static auto zPos = std::shared_ptr<ArgDescription_t>(new ArgDescription_t{.key='Z', .type = QVariant::Type::Double, .name = "Z coord",
                                                                                .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED, .manParameter = ManipulatorController::PARAMETER_Z_EFF_POS});
const static auto aRot = std::shared_ptr<ArgDescription_t>(new ArgDescription_t{.key='A', .type = QVariant::Type::Double, .name = "Alfa degrees",
                                                                                .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_ANGLE,
                                                                                .manParameter = ManipulatorController::PARAMETER_A_EFF_POS});
const static auto bRot = std::shared_ptr<ArgDescription_t>(new ArgDescription_t{.key='B', .type = QVariant::Type::Double, .name = "Beta degrees",
                                                                                .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_ANGLE,
                                                                                .manParameter = ManipulatorController::PARAMETER_B_EFF_POS});
const static auto gRot = std::shared_ptr<ArgDescription_t>(new ArgDescription_t{.key='G', .type = QVariant::Type::Double, .name = "Gamma degrees",
                                                                                .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_ANGLE,
                                                                                .manParameter = ManipulatorController::PARAMETER_G_EFF_POS});
const static auto speed = std::shared_ptr<ArgDescription_t>(new ArgDescription_t{.key='F', .type = QVariant::Type::Double, .name = "Move/rotation speed",
                                                                                 .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_IS_REQUIRED});
const static auto time = std::shared_ptr<ArgDescription_t>(new ArgDescription_t{.key='T', .type = QVariant::Type::Double, .name = "Time",
                                                                                .parameters = ActionArgumentParameter::ARGPARAM_UNLIMITED | ActionArgumentParameter::ARGPARAM_NUM_POSITIVE_ONLY | ActionArgumentParameter::ARGPARAM_IS_REQUIRED});
}

const QVector<std::shared_ptr<ArgDescription_t>> G1::_keys = {args::xPos, args::yPos, args::zPos, args::aRot, args::bRot, args::gRot, args::speed};

void G1::serializate(std::ostream&)
{
    //    out << _cmd1 << _separator;
    //    QVariant res;
    //        if(getArg(ArgKey_t{'X'}, res)) out << 'X' << res.toDouble() << _separator;
    //        if(getArg(ArgKey_t{'Y'}, res)) out << 'Y' << res.toDouble() << _separator;
    //        if(getArg(ArgKey_t{'Z'}, res)) out << 'Z' << res.toDouble() << _separator;
    //        if(getArg(ArgKey_t{'A'}, res)) out << 'A' << res.toDouble() << _separator;
    //        if(getArg(ArgKey_t{'B'}, res)) out << 'B' << res.toDouble() << _separator;
    //        if(getArg(ArgKey_t{'G'}, res)) out << 'G' << res.toDouble() << _separator;
    //        if(getArg(ArgKey_t{'F'}, res)) out << 'F' << res.toDouble() << _separator;
}

ActionExectionResult G1::startExecution(IArgsCollection& args, const ExecutionEnivroment& env)
{
    startPos = env.manipulator().getEffector();
    endPos = startPos;
    QVariant *arg;
    if(args.getArg('X', arg)) endPos.x = arg->toDouble();
    if(args.getArg('Y', arg)) endPos.y = arg->toDouble();
    if(args.getArg('Z', arg)) endPos.z = arg->toDouble();
    if(args.getArg('A', arg)) endPos.wx = arg->toDouble();
    if(args.getArg('B', arg)) endPos.wy = arg->toDouble();
    if(args.getArg('G', arg)) endPos.wz = arg->toDouble();
    if(args.getArg('F', arg)) _speed = arg->toDouble();

    double _dist = sqrt(pow(endPos.x-startPos.x, 2) + pow(endPos.y-startPos.y, 2) + pow(endPos.z-startPos.z, 2));
    if(_dist == 0)
    {
        _dist = radToDeg(sqrt(pow(endPos.wx-startPos.wx, 2) + pow(endPos.wy-startPos.wy, 2) + pow(endPos.wz-startPos.wz, 2)));
    }
    if(_speed == 0)
        _time = 1;
    else
        _time = _dist / _speed * 1000;
    return ActionExectionResult::RESULT_IN_PROCESS;
}

ActionExectionResult G1::execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig config)
{
    Effector_t eff = env.manipulator().getEffector();
    bool end = false;
    if(t >= _time || config == ExecuteConfig::EXECUTE_MOMENTLY)
    {
        eff = endPos;
        end = true;
    } else {
        eff.x = startPos.x + (endPos.x - startPos.x) * (double)t / _time;
        eff.y = startPos.y + (endPos.y - startPos.y) * (double)t / _time;
        eff.z = startPos.z + (endPos.z - startPos.z) * (double)t / _time;
        eff.wx = startPos.wx + (endPos.wx - startPos.wx) * (double)t / _time;
        eff.wy = startPos.wy + (endPos.wy - startPos.wy) * (double)t / _time;
        eff.wz = startPos.wz + (endPos.wz - startPos.wz) * (double)t / _time;
    }
    auto result = env.manipulator().inverseKinematics(eff);
    if(result == CalculationResult_t::CALC_ERROR)
        return ActionExectionResult::RESULT_ERROR;
    return end ? ActionExectionResult::RESULT_FINISH : ActionExectionResult::RESULT_IN_PROCESS;

}

void G1::endExecution()
{
}


const QVector<std::shared_ptr<ArgDescription_t>> GTEST::_keys = {args::time};

void GTEST::serializate(std::ostream&)
{
}

ActionExectionResult GTEST::startExecution(IArgsCollection& args, const ExecutionEnivroment& )
{
    qDebug() << "Started";
    QVariant *arg;
    if(args.getArg('T', arg))
        _param = arg->toDouble();
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

//void GTEST::setArg(const ArgKey_t& key, const QVariant value)
//{
//    if(key.key() == 'T')
//        _param = value.toDouble();
//}

//bool GTEST::getArg(const ArgKey_t& key, QVariant& result) const
//{
//    if(key.key() == 'T')
//        result = _param;
//    else
//        return false;
//    return true;
//}

void GTEST::endExecution()
{

}
