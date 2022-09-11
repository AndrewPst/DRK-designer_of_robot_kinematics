#include "basedock.h"
#include <QTextEdit>
#include <QContextMenuEvent>


BaseDock::BaseDock(const QString& title,
                   QWidget* parent,
                   Qt::WindowFlags flags) : QDockWidget(parent, flags)
{
    setObjectName(title);
    setWindowTitle(objectName());

    _flags = flags;
    //Callback to open a widget in the form of a window
    connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(onTopLevelChanged(bool)));

    //Create the menu
    _menu = new QMenu(objectName(), this);
    _menu->addAction(toggleViewAction()); //Add action for show/hide current widget
}

void BaseDock::contextMenuEvent(QContextMenuEvent *event)
{
    event->accept();
    _menu->exec(event->globalPos());
}

//---Getters---
QMenu* BaseDock::getMenu()
{
    return _menu;
}


Qt::DockWidgetArea BaseDock::getDefaultArea() const
{
    return Qt::RightDockWidgetArea;
}

//---Setters---
void BaseDock::drawAsWindow(bool v)
{
    _drawAsWindow = v;
}

//---private slots---
void BaseDock::onTopLevelChanged(bool)
{
    if(isFloating() && _drawAsWindow) //if _drawAsWindow  is true
    {
        if(titleBarWidget())
            titleBarWidget()->hide();
        setWindowFlags(Qt::CustomizeWindowHint |
                       Qt::Window | Qt::WindowMinimizeButtonHint |
                       Qt::WindowMaximizeButtonHint |
                       Qt::WindowCloseButtonHint);
        show();
    }
    else
    {
        if(titleBarWidget())
            titleBarWidget()->show();
        setWindowFlags(_flags);
    }
}


