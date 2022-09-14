#include "manybuttonsdock.h"
#include <QListWidget>

using namespace serialMan;

ManyButtonsDock::ManyButtonsDock(const QString& title,
                                 QWidget* parent,
                                 Qt::WindowFlags flags) : BaseDock(title, parent, flags)
{
    setMinimumSize(200, 200);

    addBut = new QAction(tr("Add button"), this);
    rmBut = new QAction(tr("Remove button"), this);
    rmBut->setEnabled(false);

    drawAsWindow(true);

    addBut->setShortcut(QKeySequence("ALT+D"));
    rmBut->setShortcut(QKeySequence("ALT+R"));

    connect(addBut, &QAction::triggered, this, &ManyButtonsDock::actionAddButSlot);
    connect(rmBut, &QAction::triggered, this, &ManyButtonsDock::actionRmButSlot);

    getMenu()->addAction(addBut);
    getMenu()->addAction(rmBut);

    _mainW = new QWidget();
    _mainL = new QVBoxLayout();
    _mainW->setLayout(_mainL);

    setWidget(_mainW);
}


void ManyButtonsDock::actionAddButSlot()
{
    _buttons.append(new QPushButton(tr("%1").arg(_buttons.size())));
    _mainL->addWidget(_buttons.last());
    rmBut->setEnabled(true);
}

void ManyButtonsDock::actionRmButSlot()
{
    if(_buttons.size() == 0)
        return;
    _mainL->removeWidget(_buttons.last());
    delete _buttons.last();
    _buttons.removeLast();
    if(_buttons.size() == 0)
        rmBut->setEnabled(false);
}
