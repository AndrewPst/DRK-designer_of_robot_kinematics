#ifndef EXECUTIONENIVROMENT_H
#define EXECUTIONENIVROMENT_H

//#include "iaction.h"
#include "../manipulatorcontroller.h"

#include <QObject>
#include <memory>
#include "serial6DofManipulator/logic/models/iaction.h"

namespace serialMan
{

struct ExecutionEnivroment
{
    ManipulatorController& _man;

    std::list<std::shared_ptr<serialMan::actions::IAction>> _prog;

public:

    explicit ExecutionEnivroment(ManipulatorController& man) : _man(man)
    {}

    ManipulatorController& manipulator() const
    {
        return _man;
    }

};

}
#endif // EXECUTIONENIVROMENT_H
