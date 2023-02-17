#include "programdock.h"
#include "../logic/models/executionEnivroment.h"

using namespace serialMan;
using namespace docks;


ProgramDock::ProgramDock(ActionsController& act,
                         const QString& title,
                         QWidget* parent,
                         Qt::WindowFlags flags) : BaseDock(title, parent, flags), _act(act)
{
    setMinimumSize(200, 200);

    _startBut = new QPushButton("Start");
    connect(_startBut, SIGNAL(clicked(bool)), this, SLOT(onStartClick()));

    _stop = new QPushButton("Stop");
    connect(_stop, SIGNAL(clicked(bool)), this, SLOT(onStopClick()));

    _pause = new QPushButton("Pause");
    connect(_pause, SIGNAL(clicked(bool)), this, SLOT(onPauseClick()));

    _resume  = new QPushButton("Resume");
    connect(_resume, SIGNAL(clicked(bool)), this, SLOT(onResumeClick()));

    _label = new QLabel();
    connect(&act.enivroment(), SIGNAL(stateChanged(serialMan::ExecutionState)), this, SLOT(stateChanged(serialMan::ExecutionState)), Qt::DirectConnection );

    _mainW = new QWidget();
    _mainL = new QVBoxLayout();

    _mainL->addWidget(_label);
    _mainL->addWidget(_startBut);
    _mainL->addWidget(_stop);
    _mainL->addWidget(_pause);
    _mainL->addWidget(_resume);

    _mainW->setLayout(_mainL);

    _mainL->addWidget(_startBut);

    setWidget(_mainW);
}

void ProgramDock::onPauseClick()
{
    _act.pause();
}

void ProgramDock::onResumeClick()
{
    _act.resume();
}

void ProgramDock::onStartClick()
{
    _act.startProgram();
}

void ProgramDock::onStopClick()
{
    _act.stop();
}

void ProgramDock::stateChanged(serialMan::ExecutionState state)
{
    QString text;
    if (state == ExecutionState::STATE_SUSPENDED)
        text = "Suspended";
    else if(state == ExecutionState::STATE_IS_RUNNING)
        text = "Running";
    else if (state == ExecutionState::STATE_FINISHED)
        text = "Finished";
    else
        text = "Unknown";
    _label->setText(text);
}
