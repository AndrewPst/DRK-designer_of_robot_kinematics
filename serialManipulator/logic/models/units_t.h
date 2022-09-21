#ifndef UNITS_T_H
#define UNITS_T_H

struct Unit
{
    explicit Unit(float f) : factor(f)
    {
    }

    const float factor;

    const static Unit millimeters;
    const static Unit centimeters;
    const static Unit metres;
};

const Unit Unit::millimeters = Unit(1.0);
const Unit Unit::centimeters = Unit(10.0);
const Unit Unit::metres = Unit(100.0);

#endif // UNITS_T_H
