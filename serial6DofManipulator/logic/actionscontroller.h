#ifndef ACTIONSCONTROLLER_H
#define ACTIONSCONTROLLER_H

#include "models/iaction.h"
#include "models/actionscontainer.h"

#include <QObject>
#include <istream>
#include <ostream>
#include <QThread>
#include <QTime>

namespace serialMan
{

QT_FORWARD_DECLARE_CLASS(ManipulatorController);
QT_FORWARD_DECLARE_CLASS(ActionsContainer);


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

    ActionsContainer& _actions;
    serialMan::ProgramState_t _state;

    constexpr const static int _fps = 30;
    qint64 _frameDiff = 1000/_fps;
    qint64 _frameTime = 0;

    ProgramExecutor(ActionsContainer&, ManipulatorController&);

public:


signals:

    void onStateChanged(serialMan::ProgramState_t);

    void onFinished();

    void executableActionChanged(serialMan::IAction*, size_t);

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

    ProgramState_t _state;
    ProgramExecutor* _executor;


public:

    ActionsController();

    ActionsContainer actions;

    SerializingError_t serializate(std::ostream&);
    SerializingError_t deserializate(std::istream&);

    void executeAction(IAction&);

    void startProgram();
    void pause();
    void resume();
    void stop();

    void setPosition(int);
    int position();

    ProgramState_t getState();

    template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<serialMan::IAction, T>>>
    T* createAction()
    {
        IAction* act = new T();
        return (T*)act;
    }

    void deleteAction(IAction*);

signals:

    void onStateChanged(serialMan::ProgramState_t);

    void messageToThread(serialMan::ProgramState_t);

private slots:

    void stateChanged(serialMan::ProgramState_t);

private:

    QThread* _thread;

};

}
#endif // ACTIONSCONTROLLER_H
