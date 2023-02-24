#ifndef MANIPULATORCONTROLLER_H
#define MANIPULATORCONTROLLER_H

#include "../serial6dofmanipulator.h"
#include "kinematics.h"
#include "models/Effector_t.h"
#include "models/joint_t.h"
#include "models/DHTable_t.h"

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QRecursiveMutex>

namespace serialMan
{

//This class controls joints and other maniulators attribute
class ManipulatorController : public QObject
{
    Q_OBJECT

    friend class serialMan::Serial6DofManipulator;

    ManipulatorController();

    ManipulatorController& operator=(const ManipulatorController&) = delete;
    ManipulatorController(const ManipulatorController&) = delete;

    void initJoints();

public:

    enum ManipulatorParameterKey : uint8_t
    {
        PARAMETER_NONE=0,
        PARAMETER_X_EFF_POS,
        PARAMETER_Y_EFF_POS,
        PARAMETER_Z_EFF_POS,
        PARAMETER_A_EFF_POS,
        PARAMETER_B_EFF_POS,
        PARAMETER_G_EFF_POS,
        PARAMETER_JOINT_A_VALUE,
        PARAMETER_JOINT_B_VALUE,
        PARAMETER_JOINT_C_VALUE,
        PARAMETER_JOINT_D_VALUE,
        PARAMETER_JOINT_E_VALUE,
        PARAMETER_JOINT_F_VALUE,
        PARAMETER_EFFECTOR_VALUE,
    };

    template<typename _pT>
    const _pT getParameter(ManipulatorParameterKey key) const
    {
        if(key == PARAMETER_NONE)
            return _pT();
        if(key == PARAMETER_X_EFF_POS)
        {
            QMutexLocker lock(&_mEffector);
            return _effector.x;
        }
        if(key == PARAMETER_Y_EFF_POS)
        {
            QMutexLocker lock(&_mEffector);
            return _effector.y;
        }
        if(key == PARAMETER_Z_EFF_POS)
        {
            QMutexLocker lock(&_mEffector);
            return _effector.z;
        }
        if(key == PARAMETER_A_EFF_POS)
        {
            QMutexLocker lock(&_mEffector);
            return _effector.wx;
        }
        if(key == PARAMETER_B_EFF_POS)
        {
            QMutexLocker lock(&_mEffector);
            return _effector.wy;
        }
        if(key == PARAMETER_G_EFF_POS)
        {
            QMutexLocker lock(&_mEffector);
            return _effector.wz;
        }
        if(key == PARAMETER_EFFECTOR_VALUE)
            return _pT();
        if(key >= PARAMETER_JOINT_A_VALUE && key <= PARAMETER_JOINT_F_VALUE)
        {
            QMutexLocker lock(&_mJoints);
            return _joints[key - PARAMETER_JOINT_A_VALUE]->getValue();
        }
        return _pT();
    }

    const static int DEFAULT_DOF = 6;

    const QVector<Joint_t*>& getJoints() const;
    const Effector_t& getEffector() const;
    const DHTable_t<DEFAULT_DOF>& getDHTable() const;

    void resetJoints();

    void forwardKinematics(QVector<double>& joints);
    CalculationResult_t inverseKinematics(const Effector_t& pos);

    void setInvConfig(char);
    char getInvConfig() const;

    void setDHTable(const DHTable_t<DEFAULT_DOF>&);

    ~ManipulatorController();

signals:

    void structureChanged();

private slots:

    void onJointsChanged();

private:

    QVector<Joint_t*> _joints;

    Effector_t _effector;

    char _kinConfig {0};

    Kinematics<DEFAULT_DOF> _kin;

    mutable QRecursiveMutex _mKin, _mEffector, _mJoints, _mConfig;
};

}
#endif // MANIPULATORCONTROLLER_H
