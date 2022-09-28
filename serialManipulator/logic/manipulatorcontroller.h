#ifndef MANIPULATORCONTROLLER_H
#define MANIPULATORCONTROLLER_H

#include "models/joint_t.h"

#include <QObject>

namespace serialMan
{

//This class controls joints and other maniulators attribute
class ManipulatorController : public QObject
{
    Q_OBJECT

public:
    ManipulatorController();
    ~ManipulatorController();

    void setDof(const int);
    int getDof() const;

    const QVector<Joint_t*>& getJoints() const;
    Joint_t* getEffector() const;

    const int DEFAULT_DOF = 2;
    const int MAX_DOF = 16;

signals:

    void jointAdded(serialMan::Joint_t*);
    void jointRemoved(serialMan::Joint_t*);
    void dofChanged(int);
    void structureChanged();

private:

    //Count of degree of freedom
    int _dof;

    QVector<Joint_t*> _joints;
    Joint_t* _effector;
};

}
#endif // MANIPULATORCONTROLLER_H
