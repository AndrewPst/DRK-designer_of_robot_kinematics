#ifndef ACTIONSCONTROLLER_H
#define ACTIONSCONTROLLER_H

#include "models/iaction.h"
#include "models/executionEnivroment.h"
#include "models/actionsLibrary.h"

#include <QObject>
#include <istream>
#include <ostream>
#include <QThread>
#include <QTime>
#include <QSharedPointer>

namespace serialMan
{

QT_FORWARD_DECLARE_CLASS(ManipulatorController);


enum ExecutionExitCode : uint8_t
{
    EXITCODE_SUCCESS = 0,
    EXITCODE_INTERRUPTION,
    EXITCODE_EXECUTION_ERROR,
    EXITCODE_NO_EXIT=255,
};


class ProgramExecutor : public QObject
{
    Q_OBJECT
    friend class ActionsController;
private:

    ExecutionEnivroment& _env;

    constexpr const static int _fps = 30;
    qint64 _frameDiff = 1000/_fps;
    qint64 _frameTime = 0;

    ProgramExecutor(ExecutionEnivroment&);


signals:

    void finished(uint8_t);

private slots:

    void onStarted();
    //void onStateChanged(serialMan::ExecutionState);

};



class ActionsController : public QObject
{
    Q_OBJECT

private:

    ManipulatorController& _man;
    actions::ActionsLibrary _lib;

    ProgramExecutor* _executor;
    ExecutionEnivroment _enivroment;

public:

    ActionsController(ManipulatorController& man);

    ExecutionEnivroment const& enivroment() const;
    const actions::ActionsLibrary& library() const;
    //void executeAction(actions::IAction&);

    void startProgram();
    void pause();
    void resume();
    void stop();

private:

    QThread* _thread;

};

}
#endif // ACTIONSCONTROLLER_H
