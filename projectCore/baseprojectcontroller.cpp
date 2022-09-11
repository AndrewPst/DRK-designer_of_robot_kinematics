#include "baseprojectcontroller.h"

BaseProjectController::BaseProjectController()
{

}

QString BaseProjectController::getName() const
{
    return "";
}

void BaseProjectController::setName(QString&)
{
    //TODO make logic
}

QString BaseProjectController::getVersion() const
{
    return "";
}

void BaseProjectController::setVersion(Version_t&)
{
    //TODO make logic
}

const QList<BaseDock*>& BaseProjectController::getAviableDocks() const
{
    return _avaiableDocks;
}

const QList<BaseCentralDock*>& BaseProjectController::getAviableCentralDocks() const
{
    return _avaiableCentralDocks;
}



