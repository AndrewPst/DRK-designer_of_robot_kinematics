#ifndef UNITSCONVERTER_H
#define UNITSCONVERTER_H

#include "math.h"

namespace serialMan
{

inline double degToRad(double v)
{
    return v * M_PI/180.0;
}


inline double radToDeg(double v)
{
    return v * 180.0/M_PI;
}

}

#endif // UNITSCONVERTER_H
