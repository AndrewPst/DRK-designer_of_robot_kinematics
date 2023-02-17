#include "Actions.h"

#include "../../logic/models/unitsConverter.h"
#include "../manipulatorcontroller.h"


using namespace serialMan;
using namespace actions;

const QVector<ArgKey_t> G1::_keys = {
    ArgKey_t('X', ActionArgumentType_t::ARGTYPE_DOUBLE, "X coord"),
    ArgKey_t('Y', ActionArgumentType_t::ARGTYPE_DOUBLE, "Y coord"),
    ArgKey_t('Z', ActionArgumentType_t::ARGTYPE_DOUBLE, "Z coord"),
    ArgKey_t('A', ActionArgumentType_t::ARGTYPE_DOUBLE, "Alfa degrees"),
    ArgKey_t('B', ActionArgumentType_t::ARGTYPE_DOUBLE, "Beta degrees"),
    ArgKey_t('G', ActionArgumentType_t::ARGTYPE_DOUBLE, "Gamma degrees"),
    ArgKey_t('F', ActionArgumentType_t::ARGTYPE_DOUBLE, "Move/rotation speed"),
};

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

ActionExectionResult_t G1::startExecution(const ExecutionEnivroment& env)
{
    Effector_t eff = env.manipulator().getEffector();
    endPos = eff;
    QVariant res;
    if(getArg(ArgKey_t{'X'}, res)) endPos.x = res.toDouble();
    if(getArg(ArgKey_t{'Y'}, res)) endPos.y = res.toDouble();
    if(getArg(ArgKey_t{'Z'}, res)) endPos.z = res.toDouble();
    if(getArg(ArgKey_t{'A'}, res)) endPos.wx = res.toDouble();
    if(getArg(ArgKey_t{'B'}, res)) endPos.wy = res.toDouble();
    if(getArg(ArgKey_t{'G'}, res)) endPos.wz = res.toDouble();
    if(getArg(ArgKey_t{'F'}, res)) _speed = res.toDouble();

    _dist = sqrt(pow(endPos.x-eff.x, 2) + pow(endPos.y-eff.y, 2) + pow(endPos.z-eff.z, 2));
    if(_dist == 0)
    {
        _dist = radToDeg(sqrt(pow(endPos.wx-eff.wx, 2) + pow(endPos.wy-eff.wy, 2) + pow(endPos.wz-eff.wz, 2)));
    }
    if(_speed == 0)
        _time = 1;
    else
        _time = _dist / _speed * 1000;
    argsSpeed.x = (endPos.x - eff.x) / _time;
    argsSpeed.y = (endPos.y - eff.y) / _time;
    argsSpeed.z = (endPos.z - eff.z) / _time;
    argsSpeed.wx = (endPos.wx - eff.wx) / _time;
    argsSpeed.wy = (endPos.wy - eff.wy) / _time;
    argsSpeed.wz = (endPos.wz - eff.wz) / _time;
    return ActionExectionResult_t::RESULT_IN_PROCESS;
}

ActionExectionResult_t G1::execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig_t config)
{
    Effector_t eff = env.manipulator().getEffector();
    bool end = false;
    if(t >= _time || config == ExecuteConfig_t::EXECUTE_INSTANTLY)
    {
        eff = endPos;
        end = true;
    } else {
        eff.x += t*argsSpeed.x;
        eff.y += t*argsSpeed.y;
        eff.z += t*argsSpeed.z;
        eff.wx += t*argsSpeed.wx;
        eff.wy += t*argsSpeed.wy;
        eff.wz += t*argsSpeed.wz;
    }
    auto result = env.manipulator().inverseKinematics(eff);
    if(result == CalculationResult_t::CALC_ERROR)
        return ActionExectionResult_t::RESULT_ERROR;
    return end ? ActionExectionResult_t::RESULT_FINISH : ActionExectionResult_t::RESULT_IN_PROCESS;

}

void G1::endExecution()
{
}


const QVector<ArgKey_t> GTEST::_keys = {
    ArgKey_t('T', ActionArgumentType_t::ARGTYPE_DOUBLE, "test"),
};

void GTEST::serializate(std::ostream&)
{
}

ActionExectionResult_t GTEST::startExecution(const ExecutionEnivroment& )
{
    qDebug() << "Started";
    return ActionExectionResult_t::RESULT_IN_PROCESS;
}

ActionExectionResult_t GTEST::execute(const ExecutionEnivroment& , qint64 t, ExecuteConfig_t )
{
    qDebug()<<t;
    if(t >=_param)
    {
        qDebug() << "Ended";
        return ActionExectionResult_t::RESULT_FINISH;
    }

    return ActionExectionResult_t::RESULT_IN_PROCESS;
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
