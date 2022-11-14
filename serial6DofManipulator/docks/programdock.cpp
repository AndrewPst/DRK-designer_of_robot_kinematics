#include "programdock.h"

ProgramDock::ProgramDock(const QString& title,
                         QWidget* parent,
                         Qt::WindowFlags flags) : BaseDock(title, parent, flags)
{
    setMinimumSize(200, 200);

    _startBut = new QPushButton("Start");

    _mainW = new QWidget();
    _mainL = new QVBoxLayout();
    _mainW->setLayout(_mainL);

    _mainL->addWidget(_startBut);

    setWidget(_mainW);
}
