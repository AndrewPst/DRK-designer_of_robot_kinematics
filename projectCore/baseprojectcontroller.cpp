#include "baseprojectcontroller.h"
#include "basedock.h"
#include "basecentraldock.h"
#include <QMenu>


BaseProjectController::BaseProjectController()
    : _viewMenu(new QMenu(tr("Views"))), _editMenu(new QMenu(tr("Project")))
{
    Version_t ver {1, 0, 0, VersionStage_t::VERSION_ALFA};//default version
    setVersion(ver);
    //TEMP
    _viewMenu->addAction(tr("test"));
    _editMenu->addAction(tr("test"));
}


void BaseProjectController::deleteCentralDock(BaseCentralDock* dock)
{
    _avaiableCentralDocks.removeOne(dock);
}

//----Getters and setters------

QString BaseProjectController::getName() const
{
    return _projectName;
}

void BaseProjectController::setName(const QString& name)
{
    if(name == _projectName) return;
    _projectName = name;
    emit onRenamed(_projectName);
}

Version_t BaseProjectController::getVersion() const
{
    return _projectVersion;
}

void BaseProjectController::setVersion(const Version_t& ver)
{
    if(_projectVersion == ver) return;
    _projectVersion = ver;
    emit onVersionChanged(_projectVersion);
}

const QList<BaseDock*>& BaseProjectController::getAviableDocks() const
{
    return _avaiableDocks;
}

const QList<BaseCentralDock*>& BaseProjectController::getAviableCentralDocks() const
{
    return _avaiableCentralDocks;
}

void BaseProjectController::setMainWindow(QMainWindow* mw)
{
    _mainWindow = mw;
}

QMenu* BaseProjectController::getEditTitlebarMenu() const
{
    return _editMenu;
}

QMenu* BaseProjectController::getViewTitlebarMenu() const
{
    return _viewMenu;
}

BaseProjectController::~BaseProjectController()
{
    delete _viewMenu;
    delete _editMenu;

    Q_FOREACH(BaseDock* a, _avaiableDocks)
    {
        a->deleteLater();
    }

    Q_FOREACH(BaseCentralDock* a, _avaiableCentralDocks)
    {
        a->deleteLater();
    }
}



