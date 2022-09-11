#include "baseprojectcontroller.h"
#include <QMenu>

BaseProjectController::BaseProjectController()
    : _viewMenu(new QMenu(tr("Serial"))), _editMenu(new QMenu(tr("Serial")))
{
    _viewMenu->addAction(tr("test"));
    _editMenu->addAction(tr("test"));
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


QMenu* BaseProjectController::getEditTitlebarMenu() const
{
    return _editMenu;
}

QMenu* BaseProjectController::getViewTitlebarMenu() const
{
    return _viewMenu;
}


