#ifndef MANIPULATORCONTROLLER_H
#define MANIPULATORCONTROLLER_H

#include "../serial6dofmanipulator.h"
#include "models/Effector_t.h"
#include "models/joint_t.h"
#include "models/DHTable_t.h"

#include <QObject>
#include <QVector>

namespace serialMan
{

//This class controls joints and other maniulators attribute
class ManipulatorController : public QObject
{
    Q_OBJECT

    friend class serialMan::Serial6DofManipulator;

    ManipulatorController();
    ~ManipulatorController();

    ManipulatorController& operator=(const ManipulatorController&) = delete;
    ManipulatorController(const ManipulatorController&) = delete;

    void initJoints();

public:
    const static int DEFAULT_DOF = 6;

    const QVector<Joint_t*>& getJoints() const;
    const Effector_t& getEffector() const;
    const DHTable_t<DEFAULT_DOF>& getDHTable() const;

    void setDHTable(DHTable_t<DEFAULT_DOF>&&);

signals:

    void structureChanged();

private:

    QVector<Joint_t*> _joints;

    Effector_t _effector;
    Effector_t _realEffector;

    DHTable_t<DEFAULT_DOF> _dhTable;
};

}
#endif // MANIPULATORCONTROLLER_H
