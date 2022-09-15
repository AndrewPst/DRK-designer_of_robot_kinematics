#ifndef MANIPULATORCONTROLLER_H
#define MANIPULATORCONTROLLER_H

#include "models/joint_t.h"

#include <QObject>

namespace serialMan
{

class ManipulatorController : public QObject
{

public:
    ManipulatorController();

    void setDof(const int);
    int getDof() const;

    const QVector<Joint_t>& getJoints() const;

signals:

    void jointAdded(Joint_t*);
    void jointRemoved(Joint_t*);
    void dofChanged(int);

private:

    const int _defaultDof = 5;

    int _dof;

    QVector<Joint_t> _joints;

};

}
#endif // MANIPULATORCONTROLLER_H
