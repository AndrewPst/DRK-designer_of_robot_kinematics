#ifndef PROGRAMDOCK_H
#define PROGRAMDOCK_H

#include "basedock.h"
#include "../logic/actionscontroller.h"
//#include "../logic/models/iaction.h"
//#include "../logic/models/Actions.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QLineEdit>

#include <QLabel>

namespace serialMan
{

namespace docks
{

class ProgramDock  : public BaseDock
{
    Q_OBJECT
public:

    explicit ProgramDock(ActionsController& act, const QString& title = "Program",
                             QWidget* parent = nullptr,
                             Qt::WindowFlags flags = {});

private slots:

    void onPauseClick();
    void onResumeClick();
    void onStartClick();
    void onStopClick();

    void stateChanged(serialMan::ExecutionState);

private:

    ActionsController& _act;

    QWidget *_mainW;
    QVBoxLayout* _mainL;

    QPushButton *_startBut, *_stop, *_pause, *_resume;

    //QPushButton *_startStopBut, *_pauseResumeBut;

    QLabel* _label;

};

}

}
#endif // PROGRAMDOCK_H
