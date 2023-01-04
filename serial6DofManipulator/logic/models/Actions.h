#ifndef ACTIONS_H
#define ACTIONS_H

#include "iaction.h"
#include <QThreadStorage>
#include <QDebug>
#include <QCache>

#include "../manipulatorcontroller.h"
#include "actionsenivroment.h"


namespace serialMan {

struct LinearMovement : public serialMan::IAction
{

    constexpr static const char* _cmd0 = "G0";
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

    double speed = 2;
    double dist, time = 0, curtime = 0;

public:

    explicit LinearMovement(ActionsEnivroment& env) : IAction(env){}

    //interface methods
    bool isKey(QString& cmd) override
    {
        return cmd == _cmd0 || cmd == _cmd1;
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
        while(in.peek() != '\n')
        {
            char key;
            double param;
            in >> key;
            in >> param;
            setArg(ArgKey_t{key}, {param});
        }
    }

    void startExecution() override
    {
        Effector_t eff = _enivroment->manipulator().getEffector();
        endPos = eff;
        Argument_t res;
        if(getArg(ArgKey_t{'X'}, res)) endPos.x = res.value.toDouble();
        if(getArg(ArgKey_t{'Y'}, res)) endPos.y = res.value.toDouble();
        if(getArg(ArgKey_t{'Z'}, res)) endPos.z = res.value.toDouble();
        if(getArg(ArgKey_t{'A'}, res)) endPos.wx = res.value.toDouble();
        if(getArg(ArgKey_t{'B'}, res)) endPos.wy = res.value.toDouble();
        if(getArg(ArgKey_t{'G'}, res)) endPos.wz = res.value.toDouble();
        if(getArg(ArgKey_t{'F'}, res)) speed = res.value.toDouble();

        dist = sqrt(pow(endPos.x-eff.x, 2) + pow(endPos.y-eff.y, 2) + pow(endPos.z-eff.z, 2));
        time = dist / speed * 1000;
        argsSpeed.x = (endPos.x - eff.x) / time;
        argsSpeed.y = (endPos.y - eff.y) / time;
        argsSpeed.z = (endPos.z - eff.z) / time;
        argsSpeed.wx = (endPos.wx - eff.wx) / time;
        argsSpeed.wy = (endPos.wy - eff.wy) / time;
        argsSpeed.wz = (endPos.wz - eff.wz) / time;

    }

    ActionResult_t execute(qint64 t, ExecuteConfig_t config) override
    {
        Effector_t eff = _enivroment->manipulator().getEffector();
        bool end = false;
        if(curtime + t >= time || config == ExecuteConfig_t::EXECUTE_INSTANTLY)
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
        curtime += t;
        auto result = _enivroment->manipulator().inverseKinematics(eff, 0);
        if(result == CalculationResult_t::CALC_ERROR)
            return ActionResult_t::RESULT_ERROR;
        return end ? ActionResult_t::RESULT_FINISH : ActionResult_t::RESULT_IN_PROCESS;

    }

    const QVector<ArgKey_t>* argsKeys() const override
    {
        return &_keys;
    }

    void endExecution() override
    {
        curtime = 0;
    }

};

}
#endif // ACTIONS_H
