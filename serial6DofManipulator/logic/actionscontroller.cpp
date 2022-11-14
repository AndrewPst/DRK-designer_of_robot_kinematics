#include "actionscontroller.h"

using namespace serialMan;

ActionsController::ActionsController(ManipulatorController& man) : QObject(), _man(man)
{

}

void ActionsController::startProgram()
{

}

SerializingError_t ActionsController::serializate(std::ostream&)
{
    return SerializingError_t::ERROR_NONE;
}

SerializingError_t ActionsController::deserializate(std::istream&)
{
    return SerializingError_t::ERROR_NONE;
}

void ActionsController::executeAction(IAction&){

}

void ActionsController::stop()
{

}

void ActionsController::pause()
{

}

void ActionsController::setPosition(int)
{

}

int ActionsController::position()
{
    return 0;
}
