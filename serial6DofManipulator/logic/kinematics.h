#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "models/DHTable_t.h"

#include <QVector>

namespace serialMan
{

QT_FORWARD_DECLARE_STRUCT(Effector_t);

enum class CalculationError_t
{
    CALC_ERROR = 0,
    CALC_SUCCESSFULL,
};

template<int _Dof>
class Kinematics
{
private:

    const DHTable_t<_Dof> _dh;

public:
    explicit Kinematics(DHTable_t<_Dof>&& dh) : _dh(dh)
    {
    }

    CalculationError_t forward(const QVector<double>& joints, Effector_t& out);

    CalculationError_t inverse(const Effector_t& pos, QVector<double>& out);

};

}
#endif // KINEMATICS_H
