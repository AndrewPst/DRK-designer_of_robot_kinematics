#ifndef ACTIONSCONTROLLER_H
#define ACTIONSCONTROLLER_H

#include "models/iaction.h"
#include "models/executionEnivroment.h"

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

enum ProgramState_t
{
    STATE_UNKNOWN,
    STATE_IS_RUNNING,
    STATE_SUSPENDED,
    STATE_FINISHED,
};

class ProgramExecutor : public QObject
{
    Q_OBJECT
    friend class ActionsController;
private:

    ManipulatorController& _man;
    ExecutionEnivroment& _actions;

    serialMan::ProgramState_t _state;

    constexpr const static int _fps = 30;
    qint64 _frameDiff = 1000/_fps;
    qint64 _frameTime = 0;

    ProgramExecutor(ExecutionEnivroment&, ManipulatorController&);

public:


signals:

    void onStateChanged(serialMan::ProgramState_t);

    void onFinished();

    void executableActionChanged(serialMan::actions::IAction*, size_t);

public slots:

    void start();
    void suspend();
    void resume();
    void stop();

    void setState(serialMan::ProgramState_t);

};



class ActionsController : public QObject
{
    Q_OBJECT

private:

    ManipulatorController& _man;

    ProgramExecutor* _executor;
    ExecutionEnivroment _enivroment;

    ProgramState_t _state;

    //QMap<const QString, IAction

    //QList<QSharedPointer<IAction>> _actionsBuf;


public:

    ActionsController(ManipulatorController& man);

    SerializingError_t serializate(std::ostream&);
    SerializingError_t deserializate(std::istream&);

    ExecutionEnivroment const& enivroment() const;

    void executeAction(actions::IAction&);

    void startProgram();
    void pause();
    void resume();
    void stop();

    void setPosition(int);
    int position();

    ProgramState_t getState();

signals:

    void onStateChanged(serialMan::ProgramState_t);

    void messageToThread(serialMan::ProgramState_t);

private slots:

    void stateChanged(serialMan::ProgramState_t);
    void threadFinished();

private:

    QThread* _thread;

};

}
#endif // ACTIONSCONTROLLER_H
