#ifndef ACTIONSENIVROMENT_H
#define ACTIONSENIVROMENT_H

#include "actionscontainer.h"
#include "iaction.h"
#include "../manipulatorcontroller.h"

#include <QObject>
#include <memory>
#include <QSharedPointer>

namespace serialMan
{

class ActionsEnivroment
{
private:

    QList<QSharedPointer<IAction>> _actionsBuf;

    ManipulatorController& _man;

public:

    //Some variables and methods

    ActionsContainer program;

    ActionsEnivroment(ManipulatorController& man) : _man(man){}

    ManipulatorController& manipulator() const
    {
        return _man;
    }

    template<typename T>
    T* createAction()
    {
        QSharedPointer<IAction> sp(new T(*this));
        _actionsBuf.push_back(sp);
        return (T*)sp.data();
    }

    bool deleteAction(IAction& act)
    {
        program.remove(&act);
        return _actionsBuf.removeAll(QSharedPointer<IAction>(&act)) > 0;
    }

};

}
#endif // ACTIONSENIVROMENT_H
