#ifndef ACTIONS_H
#define ACTIONS_H

#include "iaction.h"
#include <QThreadStorage>
#include <QDebug>
#include <QCache>

#include "../manipulatorcontroller.h"
#include "actionsenivroment.h"
#include "../../logic/models/units_t.h"


namespace serialMan {

namespace actions
{

struct G1 : public serialMan::IAction
{

    constexpr static const char* _cmd1 = "G1";
    constexpr static const char _separator = ' ';

    const QVector<ArgKey_t> _keys = {
        ArgKey_t('X', ActionArgumentType_t::ARGTYPE_DOUBLE, "X coord"),
        ArgKey_t('Y', ActionArgumentType_t::ARGTYPE_DOUBLE, "Y coord"),
        ArgKey_t('Z', ActionArgumentType_t::ARGTYPE_DOUBLE, "Z coord"),
        ArgKey_t('A', ActionArgumentType_t::ARGTYPE_DOUBLE, "Alfa degrees"),
        ArgKey_t('B', ActionArgumentType_t::ARGTYPE_DOUBLE, "Beta degrees"),
        ArgKey_t('G', ActionArgumentType_t::ARGTYPE_DOUBLE, "Gamma degrees"),
        ArgKey_t('F', ActionArgumentType_t::ARGTYPE_DOUBLE, "Move speed"),
    };

    Effector_t endPos;
    Effector_t argsSpeed;

    double _speed = 2;
    double _dist, _time = 0;

public:

    //interface methods
    bool isActionKey(QString& cmd) override
    {
        return  cmd == _cmd1;
    }

    void serializate(std::ostream& out)override
    {
        out << _cmd1 << _separator;
        Argument_t res;
        if(getArg(ArgKey_t{'X'}, res)) out << 'X' << res.value.toDouble() << _separator;
        if(getArg(ArgKey_t{'Y'}, res)) out << 'Y' << res.value.toDouble() << _separator;
        if(getArg(ArgKey_t{'Z'}, res)) out << 'Z' << res.value.toDouble() << _separator;
        if(getArg(ArgKey_t{'A'}, res)) out << 'A' << res.value.toDouble() << _separator;
        if(getArg(ArgKey_t{'B'}, res)) out << 'B' << res.value.toDouble() << _separator;
        if(getArg(ArgKey_t{'G'}, res)) out << 'G' << res.value.toDouble() << _separator;
        if(getArg(ArgKey_t{'F'}, res)) out << 'F' << res.value.toDouble() << _separator;
    }

    void deserializate(std::istream& in) override
    {
        while(in.peek() != '\n' && in.peek() != '\0')
        {
            char key;
            double param;
            in >> key;
            in >> param;
            setArg(ArgKey_t{key}, {param});
        }
    }

    void startExecution(const ActionsEnivroment& env) override
    {
        Effector_t eff = env.manipulator().getEffector();
        endPos = eff;
        Argument_t res;
        if(getArg(ArgKey_t{'X'}, res)) endPos.x = res.value.toDouble();
        if(getArg(ArgKey_t{'Y'}, res)) endPos.y = res.value.toDouble();
        if(getArg(ArgKey_t{'Z'}, res)) endPos.z = res.value.toDouble();
        if(getArg(ArgKey_t{'A'}, res)) endPos.wx = res.value.toDouble();
        if(getArg(ArgKey_t{'B'}, res)) endPos.wy = res.value.toDouble();
        if(getArg(ArgKey_t{'G'}, res)) endPos.wz = res.value.toDouble();
        if(getArg(ArgKey_t{'F'}, res)) _speed = res.value.toDouble();

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

    ActionResult_t execute(const ActionsEnivroment& env, qint64 t, ExecuteConfig_t config) override
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
        auto result = env.manipulator().inverseKinematics(eff, 0);
        if(result == CalculationResult_t::CALC_ERROR)
            return ActionResult_t::RESULT_ERROR;
        return end ? ActionResult_t::RESULT_FINISH : ActionResult_t::RESULT_IN_PROCESS;

    }

    void endExecution() override
    {
    }

    const QVector<ArgKey_t>* argsKeys() const override
    {
        return &_keys;
    }
};

}

}
#endif // ACTIONS_H
