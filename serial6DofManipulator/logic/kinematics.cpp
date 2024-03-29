#include "kinematics.h"
#include "cmath"
#include "QDebug"

using namespace serialMan;

#define IS_V1(x)((x) & 0b00000001)
#define IS_V2(x)((x) & 0b00000010)

template<>
CalculationResult_t Kinematics<6>::forward(const QVector<double>& joints, Position_t& out)
{

    Matrix<calc_t> baseT(4, 4);  // Zero position of the base frame
    Matrix<calc_t> workT(4, 4);  // The resulting homogeneous transformation matrix
    Matrix<calc_t> transT(4, 4); // Transformation matrix from N-1 to N frame

    positionToTransformMatrix(Position_t(), baseT); // Get base matrix
    workT = baseT;

    // Calculation the transformation matrix - T06 = T01 * T12 * T23 * T34 * T45 * T56
    for (int i = 0; i < 6; i++)
    {
        createDHFrameMatrix((_dh.theta[i] + joints[i]), (_dh.alfa[i]), _dh.r[i], _dh.d[i], transT);
        workT = workT * transT;
    }
    baseT = workT * baseT;

    // Get position by calculated matrix
    transformMatrixToPosition(baseT, out);

    return CalculationResult_t::CALC_SUCCESSFULL;
}

template<>
CalculationResult_t Kinematics<6>::inverse(const Position_t& pos, QVector<double>& out, config_t conf)
{    
    Matrix<calc_t> Twf(4, 4); // work frame matrix;
    positionToTransformMatrix(Position_t(), Twf);
    Matrix<calc_t> Ttf(Twf); // tool frame transform matrix;

    Matrix<calc_t> Twt(4, 4); // total transform matrix
    positionToTransformMatrix(pos, Twt);

    // find T06 - transformation matrix from 0 to N frame
    Matrix<calc_t> inTwf(4, 4), inTtf(4, 4);
    inverseTransformMatrix(Twf, inTwf);
    inverseTransformMatrix(Ttf, inTtf);
    Matrix<calc_t> t06 = inTwf * (Twt * inTtf);

    calc_t tempPos[3]; // Buffer for calculation first 3 joints. This is position of N=5 joint
    tempPos[0] = t06.at(0, 3) - _dh.d[5] * t06.at(0, 2)  - _dh.r[5] * t06.at(0, 0);
    tempPos[1] = t06.at(1, 3) - _dh.d[5] * t06.at(1, 2)  - _dh.r[5] * t06.at(1, 0);
    tempPos[2] = t06.at(2, 3) - _dh.d[5] * t06.at(2, 2)  - _dh.r[5] * t06.at(2, 0);

    //    tempPos[0] = t06.at(0, 3) - _dh.d[5] * t06.at(0, 2) - _dh.r[5] * t06.at(0, 0) * std::atan2(sin((_dh.theta[5])) * _dh.r[5], _dh.d[5]);
    //    tempPos[1] = t06.at(1, 3) - _dh.d[5] * t06.at(1, 2)  - _dh.r[5] * t06.at(1, 0);
    //    tempPos[2] = t06.at(2, 3) - _dh.d[5] * t06.at(2, 2)  - _dh.r[5] * t06.at(2, 0);

    // First joint
    double gipotinuze = sqrt(tempPos[0] * tempPos[0] + tempPos[1] * tempPos[1] - (_dh.d[2] + _dh.d[1]) * (_dh.d[2] + _dh.d[1]));
    out[0] = atan2(tempPos[1], tempPos[0]) - atan2((_dh.d[2] + _dh.d[1]) * (IS_V1(conf) ? -1 : 1), gipotinuze) + (IS_V1(conf) ? M_PI : 0);

    // Second joint

    //comp = components
    double comp1 = (_dh.theta[1]) * (IS_V1(conf) ? 1 : -1);
    gipotinuze = gipotinuze - _dh.r[0] * (IS_V1(conf) ? -1 : 1);
    double zDif = (tempPos[2] - _dh.d[0]);
    double comp2 = acos((_dh.r[1] * _dh.r[1] + zDif * zDif + gipotinuze * gipotinuze - (_dh.r[2] * _dh.r[2] + _dh.d[3] * _dh.d[3])) / (2.0 * _dh.r[1] * sqrt(zDif * zDif + gipotinuze * gipotinuze)));
    double comp3 = atan(zDif / gipotinuze);
    out[1] = comp1 - (comp2 + comp3) * (IS_V1(conf) ? -1 : 1);

    if(gipotinuze < 0)
        out[1] += M_PI;

    // third joint
    out[2] = (IS_V1(conf) ? -M_PI : M_PI) - ((_dh.theta[2]) + acos((_dh.r[1] * _dh.r[1] + _dh.r[2] * _dh.r[2] + _dh.d[3] * _dh.d[3] - zDif * zDif - gipotinuze * gipotinuze) / (2.0 * _dh.r[1] * sqrt(_dh.r[2] * _dh.r[2] + _dh.d[3] * _dh.d[3]))) + atan(_dh.d[3]/ _dh.r[2]) * (IS_V1(conf) ? -1 : 1)) * (IS_V1(conf) ? -1 : 1); // out(3)=pi-acos((r(2)^2+r(3)^2+d(4)^2-(tempPos(3)-d(1))^2-(sqrt(tempPos(1)^2+tempPos(2)^2-d(3)^2)-r(1))^2)/(2*r(2)*sqrt(r(3)^2+d(4)^2)))-atan(d(4)/r(3));

    // Calculate T36 transoform matrix
    Matrix<calc_t> t_n_to_n1(4, 4), t_n1_to_n2(4, 4), invT03(4, 4);
    // T03 = T01 * T12 * T23, there t_n_to_n1 = T01, t_n1_to_n2 = T12 and T23(overwriting)
    createDHFrameMatrix((_dh.theta[0]) + out[0], (_dh.alfa[0]), _dh.r[0], _dh.d[0], t_n_to_n1);
    createDHFrameMatrix((_dh.theta[1]) + out[1], (_dh.alfa[1]), _dh.r[1], _dh.d[1], t_n1_to_n2);
    Matrix<calc_t> t03 = t_n_to_n1 * t_n1_to_n2;
    createDHFrameMatrix((_dh.theta[2]) + out[2], (_dh.alfa[2]), _dh.r[2], _dh.d[2], t_n1_to_n2);
    t03 = t03 * t_n1_to_n2;

    inverseTransformMatrix(t03, invT03);
    Matrix<calc_t> t36 = invT03 * t06;



    //    double tmp = t03.at(2, 3);
    //    config_t rev_conf = conf;
    //    if(tmp > tempPos[2])
    //    {
    //        rev_conf ^= 2;
    //        qDebug() << "Reverse\n";
    //    }

    config_t rev_conf = conf;
    if(t36.at(0, 2) < 0)
    {
        rev_conf ^= 2;
    }


    //DEBUG: (index)
    //3 = atan2(6, 2)
    //4 = atan2(,)
    //5 = atan2(9, 8)F
    // Find last joints
    out[3] = atan2(t36.at(1, 2) * (IS_V2(rev_conf) ? -1 : 1), t36.at(0, 2) * (IS_V2(rev_conf) ? -1 : 1)) /*+ M_PI * (IS_V2(rev_conf) ? -1 : 1)*/;
    out[4] = (IS_V2(rev_conf) ? 1 : -1) * atan2(sqrt(t36.at(0, 2) * t36.at(0, 2) + t36.at(1, 2) * t36.at(1, 2)) , t36.at(2, 2));
    out[5] = atan2((t36.at(2, 1) * (IS_V2(rev_conf) ? -1 : 1)) , (t36.at(2, 0) * (IS_V2(rev_conf) ? 1 : -1))) /*+ M_PI * (IS_V2(rev_conf) ? 1 : -1)*/;

    for(int i = 0; i < out.size(); i++)
    {
        if(std::isnan(out[i]))
            return CalculationResult_t::CALC_ERROR;
        //        if( out[i] >= 360.0)
        //        {
        //            out[i] = fmod(out[i], 360.0);
        //        } else if (out[i] < 0)
        //        {
        //            out[i] = -fmod(out[i], 360.0);
        //        }
        //out[i] = out[i] >= 360 ? fmod(out[i], 360.0) : out[i] < 0 ? -fmod(out[i], 360.0) : out[i];
    }

    if(abs(out[4]) <= 0.0001)
    {
        out[5] += out[3] - M_PI * (IS_V2(conf) ? 1 : 0);
        out[3] = M_PI * (IS_V2(conf) ? 1 : 0);
    }

    return CalculationResult_t::CALC_SUCCESSFULL;
}

