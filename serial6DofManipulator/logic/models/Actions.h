#ifndef ACTIONS_H
#define ACTIONS_H

#include "iaction.h"

#include "../manipulatorcontroller.h"

namespace serialMan {

#define _IS_X(c) ((c) & 0b00000001)
#define _IS_Y(c) ((c) & 0b0000001)
#define _IS_Z(c) ((c) & 0b000001)
#define _IS_A(c) ((c) & 0b00001)
#define _IS_B(c) ((c) & 0b0001)
#define _IS_G(c) ((c) & 0b001)
#define _IS_F(c) ((c) & 0b01)

struct LinearMovement : public serialMan::IAction
{

    constexpr static const char* _cmd0 = "G0";
    constexpr static const char* _cmd1 = "G1";
    constexpr static const char _separator = ' ';

    //1 byte
    using config_t = char;

public:

    //variables
    double x, y, z, //position
    a, b, g, //orientation (alfa, beta, gamma)
    f; //speed

    config_t flag = (char)255;

private:

    //interface methods
    bool isCorrected(QString& cmd) override
    {
        return cmd.startsWith(_cmd0) || cmd.startsWith(_cmd1);
    }

    void serializate(std::ostream& out)override
    {
        out << _cmd1 << _separator;
        if(_IS_X(flag)) out << 'X' << x << _separator;
        if(_IS_Y(flag)) out << 'Y' << y << _separator;
        if(_IS_Z(flag)) out << 'Z' << z << _separator;
        if(_IS_A(flag)) out << 'A' << a << _separator;
        if(_IS_B(flag)) out << 'B' << b << _separator;
        if(_IS_G(flag)) out << 'G' << g << _separator;
        out << 'F' << f << _separator;
    }

    void deserializate(std::istream&) override;

    void execute(ManipulatorController& man) override
    {
        auto c_pos = man.getEffector();
        if(_IS_X(flag)) c_pos.x = x;
        if(_IS_Y(flag)) c_pos.y = y;
        if(_IS_Z(flag)) c_pos.z = z;
        if(_IS_A(flag)) c_pos.wx = a;
        if(_IS_B(flag)) c_pos.wy = b;
        if(_IS_G(flag)) c_pos.wz = g;
        man.inverseKinematics(c_pos);
    }


};

}
#endif // ACTIONS_H
