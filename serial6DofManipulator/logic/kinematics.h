#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "models/DHTable_t.h"
#include "models/matrix.h"
#include "models/Effector_t.h"

#include <math.h>

#include <QVector>

namespace serialMan
{

enum class CalculationResult_t
{
    CALC_ERROR = 0,
    CALC_SUCCESSFULL,
};

#define RAD(x)((x) / 180.0 * M_PI)
#define DEG(x)((x) * 180.0 / M_PI)

template<int _Dof>
class Kinematics
{
private:

    DHTable_t<_Dof> _dh;

    using calc_t = double;
    using config_t = char;

public:

    Kinematics()
    {
    }

    void setDHTable(const DHTable_t<_Dof>& dh)
    {
        _dh = dh;
    }

    const DHTable_t<_Dof>& dhTable() const noexcept
    {
        return _dh;
    }

    CalculationResult_t forward(const QVector<calc_t>& joints, Effector_t& out);

    CalculationResult_t inverse(const Effector_t& pos, QVector<calc_t>& out, config_t conf);

private:
    void inverseTransformMatrix(Matrix<calc_t> &m, Matrix<calc_t> &out)
    {
        out.at(0, 0) = m.at(0, 0);
        out.at(0, 1) = m.at(1, 0);
        out.at(0, 2) = m.at(2, 0);
        out.at(0, 3) = -m.at(0, 0) * m.at(0, 3) - m.at(1, 0) * m.at(1, 3) - m.at(2, 0) * m.at(2, 3);

        out.at(1, 0) = m.at(0, 1);
        out.at(1, 1) = m.at(1, 1);
        out.at(1, 2) = m.at(2, 1);
        out.at(1, 3) = -m.at(0, 1) * m.at(0, 3) - m.at(1, 1) * m.at(1, 3) - m.at(2, 1) * m.at(2, 3);

        out.at(2, 0) = m.at(0, 2);
        out.at(2, 1) = m.at(1, 2);
        out.at(2, 2) = m.at(2, 2);
        out.at(2, 3) = -m.at(0, 2) * m.at(0, 3) - m.at(1, 2) * m.at(1, 3) - m.at(2, 2) * m.at(2, 3);

        out.at(3, 0) = 0.0;
        out.at(3, 1) = 0.0;
        out.at(3, 2) = 0.0;
        out.at(3, 3) = 1.0;
    }


    void transformMatrixToPosition(Matrix<calc_t> &m, Effector_t &out)
    {
        out.x = m.at(0, 3);
        out.y = m.at(1, 3);
        out.z = m.at(2, 3);
        out.wy = DEG(atan2(sqrt(m.at(2, 0) * m.at(2, 0) + m.at(2, 1) * m.at(2, 1)), m.at(2, 2)));
        out.wx = DEG(atan2(m.at(1, 2) / sin(out.wy), m.at(0, 2) / sin(out.wy)));
        out.wz = DEG(atan2(m.at(2, 1) / sin(out.wy), -m.at(2, 0) / sin(out.wy)));
    }

    void positionToTransformMatrix(const Effector_t &pos, Matrix<calc_t> &out)
    {
        out.at(0, 0) = cos(RAD(pos.wx)) * cos(RAD(pos.wy)) * cos(RAD(pos.wz)) - sin(RAD(pos.wx)) * sin(RAD(pos.wz));
        out.at(0, 1) = -cos(RAD(pos.wx)) * cos(RAD(pos.wy)) * sin(RAD(pos.wz)) - sin(RAD(pos.wx)) * cos(RAD(pos.wz));
        out.at(0, 2) = cos(RAD(pos.wx)) * sin(RAD(pos.wy));
        out.at(0, 3) = pos.x;

        out.at(1, 0) = sin(RAD(pos.wx)) * cos(RAD(pos.wy)) * cos(RAD(pos.wz)) + cos(RAD(pos.wx)) * sin(RAD(pos.wz));
        out.at(1, 1) = -sin(RAD(pos.wx)) * cos(RAD(pos.wy)) * sin(RAD(pos.wz)) + cos(RAD(pos.wx)) * cos(RAD(pos.wz));
        out.at(1, 2) = sin(RAD(pos.wx)) * sin(RAD(pos.wy));
        out.at(1, 3) = pos.y;

        out.at(2, 0) = -sin(RAD(pos.wy)) * cos(RAD(pos.wz));
        out.at(2, 1) = sin(RAD(pos.wy)) * sin(RAD(pos.wz));
        out.at(2, 2) = cos(RAD(pos.wy));
        out.at(2, 3) = pos.z;

        out.at(3, 0) = 0.0;
        out.at(3, 1) = 0.0;
        out.at(3, 2) = 0.0;
        out.at(3, 3) = 1.0;
    }


    void createDHFrameMatrix(calc_t theta, calc_t alfa, calc_t r, calc_t d, Matrix<calc_t> &out)
    {
        out.at(0, 0) = cos(theta);
        out.at(0, 1) = -sin(theta) * cos(alfa);
        out.at(0, 2) = sin(theta) * sin(alfa);
        out.at(0, 3) = r * cos(theta);

        out.at(1, 0) = sin(theta);
        out.at(1, 1) = cos(theta) * cos(alfa);
        out.at(1, 2) = -cos(theta) * sin(alfa);
        out.at(1, 3) = r * sin(theta);

        out.at(2, 0) = 0.0;
        out.at(2, 1) = sin(alfa);
        out.at(2, 2) = cos(alfa);
        out.at(2, 3) = d;

        out.at(3, 0) = 0.0;
        out.at(3, 1) = 0.0;
        out.at(3, 2) = 0.0;
        out.at(3, 3) = 1.0;
    }

};

}
#endif // KINEMATICS_H
