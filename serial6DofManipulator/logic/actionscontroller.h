#ifndef ACTIONSCONTROLLER_H
#define ACTIONSCONTROLLER_H

#include "models/iaction.h"
#include "models/executionEnivroment.h"
#include "models/executionState.h"

#include <QObject>
#include <istream>
#include <ostream>
#include <QThread>
#include <QTime>
#include <QSharedPointer>

namespace serialMan
{

QT_FORWARD_DECLARE_CLASS(ManipulatorController);


enum SerializingError_t
{
    ERROR_NONE
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

    void onFinished();

private slots:

    void onStarted();
    void onStateChanged(serialMan::ExecutionState);

};



class ActionsController : public QObject
{
    Q_OBJECT

private:

    ManipulatorController& _man;

    ProgramExecutor* _executor;
    ExecutionEnivroment _enivroment;

public:

    ActionsController(ManipulatorController& man);

    ExecutionEnivroment const& enivroment() const;
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
