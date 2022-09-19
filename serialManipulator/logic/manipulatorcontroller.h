#ifndef MANIPULATORCONTROLLER_H
#define MANIPULATORCONTROLLER_H

#include "models/joint_t.h"

#include <QObject>

namespace serialMan
{

class ManipulatorController : public QObject
{
    Q_OBJECT

public:
    ManipulatorController();

    void setDof(const int);
    int getDof() const;

    const QVector<Joint_t*>& getJoints() const;

    const int DEFAULT_DOF = 5;
    const int MAX_DOF = 16;

signals:

    void jointAdded(serialMan::Joint_t*);
    void jointRemoved(serialMan::Joint_t*);
    void dofChanged(int);

private:

    int _dof;

    QVector<Joint_t*> _joints;

};

}
#endif // MANIPULATORCONTROLLER_H
