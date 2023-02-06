#ifndef ACTIONS_H
#define ACTIONS_H

#include "iaction.h"
#include <QThreadStorage>
#include <QDebug>
#include <QCache>

#include "../manipulatorcontroller.h"
#include "executionEnivroment.h"
#include "../../logic/models/units_t.h"


namespace serialMan {

namespace actions
{

struct G1 : public serialMan::actions::IAction
{

    constexpr static const char* _cmd1 = "G1";
    constexpr static const char _separator = ' ';

    static const QVector<ArgKey_t> _keys;

    Effector_t endPos;
    Effector_t argsSpeed;

    double _speed = 2;
    double _dist, _time = 0;

public:

    void serializate(std::ostream& out)override
    {
        out << _cmd1 << _separator;
        QVariant res;
//        if(getArg(ArgKey_t{'X'}, res)) out << 'X' << res.toDouble() << _separator;
//        if(getArg(ArgKey_t{'Y'}, res)) out << 'Y' << res.toDouble() << _separator;
//        if(getArg(ArgKey_t{'Z'}, res)) out << 'Z' << res.toDouble() << _separator;
//        if(getArg(ArgKey_t{'A'}, res)) out << 'A' << res.toDouble() << _separator;
//        if(getArg(ArgKey_t{'B'}, res)) out << 'B' << res.toDouble() << _separator;
//        if(getArg(ArgKey_t{'G'}, res)) out << 'G' << res.toDouble() << _separator;
//        if(getArg(ArgKey_t{'F'}, res)) out << 'F' << res.toDouble() << _separator;
    }

    void startExecution(const ExecutionEnivroment& env) override
    {
        Effector_t eff = env.manipulator().getEffector();
        endPos = eff;
        QVariant res;
//        if(getArg(ArgKey_t{'X'}, res)) endPos.x = res.toDouble();
//        if(getArg(ArgKey_t{'Y'}, res)) endPos.y = res.toDouble();
//        if(getArg(ArgKey_t{'Z'}, res)) endPos.z = res.toDouble();
//        if(getArg(ArgKey_t{'A'}, res)) endPos.wx = res.toDouble();
//        if(getArg(ArgKey_t{'B'}, res)) endPos.wy = res.toDouble();
//        if(getArg(ArgKey_t{'G'}, res)) endPos.wz = res.toDouble();
//        if(getArg(ArgKey_t{'F'}, res)) _speed = res.toDouble();

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
    }

    ActionExectionResult_t execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig_t config) override
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

    void endExecution() override
    {
    }

    static IAction* generate()
    {
        return new G1;
    }

    static constexpr const std::pair<char, uint16_t> key()
    {
        return {'G', 1};
    }

    const static QVector<ArgKey_t>* allowArgs()
    {
        return &_keys;
    }
};

const QVector<ArgKey_t> G1::_keys = {
    ArgKey_t('X', ActionArgumentType_t::ARGTYPE_DOUBLE, "X coord"),
    ArgKey_t('Y', ActionArgumentType_t::ARGTYPE_DOUBLE, "Y coord"),
    ArgKey_t('Z', ActionArgumentType_t::ARGTYPE_DOUBLE, "Z coord"),
    ArgKey_t('A', ActionArgumentType_t::ARGTYPE_DOUBLE, "Alfa degrees"),
    ArgKey_t('B', ActionArgumentType_t::ARGTYPE_DOUBLE, "Beta degrees"),
    ArgKey_t('G', ActionArgumentType_t::ARGTYPE_DOUBLE, "Gamma degrees"),
    ArgKey_t('F', ActionArgumentType_t::ARGTYPE_DOUBLE, "Move speed"),
};

}

}
#endif // ACTIONS_H
