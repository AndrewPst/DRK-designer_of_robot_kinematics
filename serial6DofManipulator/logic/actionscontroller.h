#ifndef ACTIONSCONTROLLER_H
#define ACTIONSCONTROLLER_H

#include "models/iaction.h"

#include <QObject>
#include <istream>
#include <ostream>
#include <QThread>

namespace serialMan
{

QT_FORWARD_DECLARE_CLASS(ManipulatorController);


enum class SerializingError_t
{
    ERROR_NONE
};


class ActionsController : public QObject
{
    Q_OBJECT

private:


    ManipulatorController& _man;



public:

    //QList<IAction*> program;

    ActionsController(ManipulatorController&);

    void startProgram();

    SerializingError_t serializate(std::ostream&);

    SerializingError_t deserializate(std::istream&);

    void executeAction(IAction&);

    void stop();
    void pause();
    void setPosition(int);
    int position();

    template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<T, IAction>>>
    T* createAction()
    {
        IAction* act = new T();
        return (T*)act;
    }

    void deleteAction(IAction*);

signals:

    void programStarted();
    void programEnded();
    void programSuspended();

    QThread* _thread;
    //void executeAction(const MovementParameter_t&);

};

}
#endif // ACTIONSCONTROLLER_H
