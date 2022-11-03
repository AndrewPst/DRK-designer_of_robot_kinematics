#include "kinematics.h"

using namespace serialMan;

template<>
CalculationError_t Kinematics<6>::forward(const QVector<double>& joints, Effector_t& out)
{
    (void)&joints;
    (void)&out;
    return CalculationError_t::CALC_SUCCESSFULL;
}

template<>
CalculationError_t Kinematics<6>::inverse(const Effector_t& pos, QVector<double>& out)
{
    (void)&pos;
    (void)&out;
    return CalculationError_t::CALC_SUCCESSFULL;
}


