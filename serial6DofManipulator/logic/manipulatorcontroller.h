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
    const static int DEFAULT_DOF = 6;

    const QVector<Joint_t*>& getJoints() const;
    Effector_t getEffector() const;
    const DHTable_t<DEFAULT_DOF>& getDHTable() const;

    void forwardKinematics(QVector<double>& joints);
    CalculationResult_t inverseKinematics(const Effector_t& pos, char config);

    void setInvConfig(char);
    char getInvConfig() const;

    void setDHTable(const DHTable_t<DEFAULT_DOF>&);
    void setEffector(const Effector_t&);

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
