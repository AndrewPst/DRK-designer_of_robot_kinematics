#ifndef IACTION_H
#define IACTION_H

#include <QString>
#include <istream>
#include <ostream>
#include <QThreadStorage>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>

namespace serialMan
{

QT_FORWARD_DECLARE_CLASS(ManipulatorController)
QT_FORWARD_DECLARE_CLASS(ActionsEnivroment)

enum ActionResult_t
{
    RESULT_UNKNOWN,
    RESULT_IN_PROCESS,
    RESULT_ERROR,
    RESULT_FINISH
};

struct IAction
{
private:

    mutable QMap<char, double> _args;
    mutable QMutex _argMut;

protected:

    ActionsEnivroment* _enivroment;

public:

    explicit IAction(ActionsEnivroment& env) : _enivroment(&env){}

    void setEnivroment(ActionsEnivroment& env)
    {
        _enivroment = &env;
    }

    void setArg(char key, double& value)
    {
        QMutexLocker lock(&_argMut);
        _args[key] = value;
    }

    double getArg(char key, double defaultValue = 0) const
    {
        QMutexLocker lock(&_argMut);
        return _args.value(key, defaultValue);
    }

    virtual bool isCorrected(QString&) = 0;

    virtual void serializate(std::ostream&) = 0;
    virtual void deserializate(std::istream&) = 0;

    virtual void startExecution(){}
    virtual ActionResult_t execute(ManipulatorController&, qint64) = 0;
    virtual void endExecution(){}

    virtual ~IAction() {};
};

}
#endif // IACTION_H
