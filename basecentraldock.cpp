#include "basecentraldock.h"
#include "centralwindow.h"
#include "projectCore/projectsmanager.h"
#include "projectCore/baseprojectcontroller.h"

#include <QContextMenuEvent>

BaseCentralDock::BaseCentralDock(const QString& title,
                                 QMainWindow* parent ,
                                 Qt::WindowFlags flags)
    : QDockWidget(title, parent, flags), _flags(flags)
{
    setObjectName(title);
    setWindowTitle(title);

    //Delete this object after closing the widget
    setAttribute(Qt::WA_DeleteOnClose);

    //Callback to open a widget in the form of a window
    connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(onTopLevelChanged(bool)));

    _menu = new QMenu(objectName(), this);
    QAction *a = toggleViewAction();
    a->setText(tr("Enabled"));
    _menu->addAction(a);
}

QMenu *BaseCentralDock::getMenu()
{
    return _menu;
}

Qt::DockWidgetArea BaseCentralDock::getDefaultArea() const
{
    return Qt::TopDockWidgetArea;
}


//---public slots---

//---private slots
void BaseCentralDock::closeWidgetSlot(bool)
{
    //Close this widget if exit button are pressed
    close();
}

void BaseCentralDock::onTopLevelChanged(bool)
{
    if(isFloating())
    {
        //Hide titlebar if it is available
        if(titleBarWidget())
            setTitleBarWidget(nullptr);
        //Set flags for open as window
        setWindowFlags(Qt::CustomizeWindowHint |
                       Qt::Window | Qt::WindowMinimizeButtonHint |
                       Qt::WindowMaximizeButtonHint |
                       Qt::WindowCloseButtonHint);
        setMinimumSize(400, 400);
        show();
    }
    else
    {
        //Recover flags
        setWindowFlags(_flags);
        //Return titlebar
        if(_titleWidget){
            setTitleBarWidget(_titleWidget);
            titleBarWidget()->show();
        }

    }
}

//---Events---
void BaseCentralDock::closeEvent(QCloseEvent*)
{
    //Remove this widget from parent window
    CentralWindow* window = qobject_cast<CentralWindow*>(parent());
    window->removeDockWidget(this);
    projectsManager.getOpenedProject()->deleteCentralDock(this);
}

void BaseCentralDock::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::Type::WindowStateChange && isMinimized())
    {
        event->ignore();
        setWindowState(Qt::WindowState::WindowNoState);
        setFloating(!isFloating());
    }
}

void BaseCentralDock::contextMenuEvent(QContextMenuEvent *event)
{
    event->accept();
    _menu->exec(event->globalPos());
}
