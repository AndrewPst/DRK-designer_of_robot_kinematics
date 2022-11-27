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

    qint64 _tempTime = 0;
    long _counter = 0;
    //1 byte
    using config_t = char;

public:

    explicit LinearMovement(ActionsEnivroment& env) : IAction(env){}

    //interface methods
    bool isCorrected(QString& cmd) override
    {
        return cmd == _cmd0 || cmd == _cmd1;
    }

    void serializate(std::ostream& out)override
    {
        out << _cmd1 << _separator;
//        if(double* x = args.localData().object('X')) out << 'X' << *x << _separator;
//        if(double* y = args.localData().object('Y')) out << 'Y' << *y << _separator;
//        if(double* z = args.localData().object('Z')) out << 'Z' << *z << _separator;
//        if(double* a = args.localData().object('A')) out << 'A' << *a << _separator;
//        if(double* b = args.localData().object('B')) out << 'B' << *b << _separator;
//        if(double* g = args.localData().object('G')) out << 'G' << *g << _separator;
//        if(double* f = args.localData().object('F')) out << 'F' << *f << _separator;
    }

    void deserializate(std::istream& in) override
    {
        while(in.peek() != '\n')
        {
            char key;
            double param;
            in >> key;
            in >> param;
            //args.localData().insert(key, new double(param));
        }
    }

    ActionResult_t execute(ManipulatorController&, qint64) override
    {
        Effector_t eff = _enivroment->manipulator().getEffector();
        eff.x-=0.1;
        eff.wx+=90.0/100.0;
        _enivroment->manipulator().setEffector(eff);
        _counter++;
        //auto c_pos = man.getEffector();
        //        if(double* x = args.localData().object("X")) c_pos.x = *x;
        //        if(double* y = args.localData().object("Y")) c_pos.y = *y;
        //        if(double* z = args.localData().object("Z")) c_pos.z = *z;
        //        if(double* a = args.localData().object("A")) c_pos.wz = *a;
        //        if(double* b = args.localData().object("B")) c_pos.wy = *b;
        //        if(double* g = args.localData().object("G")) c_pos.wz = *g;
        //        if(double* f = args.localData().object("F")) c_pos.f = *f;
        //man.inverseKinematics(c_pos);
        return (_counter < 100) ? ActionResult_t::RESULT_IN_PROCESS : ActionResult_t::RESULT_FINISH;
    }

    void endExecution() override
    {
        _tempTime = 0;
        _counter = 0;
    }

};

}
#endif // ACTIONS_H
