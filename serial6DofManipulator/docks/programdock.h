#ifndef PROGRAMDOCK_H
#define PROGRAMDOCK_H

#include "basedock.h"
#include "../logic/actionscontroller.h"

#include <QVBoxLayout>
#include <QPushButton>

#include <QLabel>

namespace serialMan {

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

    void stateChanged(serialMan::ProgramState_t);

private:

    ActionsController& _act;

    QWidget *_mainW;
    QVBoxLayout* _mainL;

    QPushButton *_startBut, *_stop, *_pause, *_resume;
    QLabel* _label;

};

}
#endif // PROGRAMDOCK_H
