#ifndef DHTABLE_T_H
#define DHTABLE_T_H

#include <QVector>
#include <QArrayData>

namespace serialMan
{

template<int _Dof>
struct DHTable_t
{
    std::array<double, _Dof> theta;
    std::array<double, _Dof> alfa;
    std::array<double, _Dof> d;
    std::array<double, _Dof> r;

};

}

#endif // DHTABLE_T_H
