#ifndef IACTION_H
#define IACTION_H

#include <QString>
#include <istream>
#include <ostream>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <QVariant>

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

enum ExecuteConfig_t
{
    EXECUTE_ANIMATION,
    EXECUTE_INSTANTLY,
};

enum ActionArgumentType_t
{
    ARGTYPE_DOUBLE,
    ARGTYPE_STRING,
};

struct ArgKey_t
{
    char key {0};
    ActionArgumentType_t type {ActionArgumentType_t::ARGTYPE_DOUBLE};
    QString name;

    bool operator==(const ArgKey_t& arg)
    {
        return key == arg.key;
    }

    bool operator>(const ArgKey_t& arg)
    {
        return key > arg.key;
    }

    bool operator<(const ArgKey_t& arg)
    {
        return key < arg.key;
    }
};

struct Argument_t
{
    QVariant value;
};

struct IAction
{
    friend class ActionsEnivroment;

private:

    mutable QMap<ArgKey_t, Argument_t> _args;
    mutable QMutex _argMut;

protected:

    ActionsEnivroment* _enivroment;

public:

    explicit IAction(ActionsEnivroment& env);

    //Enivroment
    void setEnivroment(ActionsEnivroment& env);
    ActionsEnivroment& enivroment() const;

    //Args functions
    void setArg(ArgKey_t key, Argument_t value);
    bool getArg(ArgKey_t key, Argument_t& result) const;
    virtual const QVector<ArgKey_t>* argsKeys() const;

    virtual bool isKey(QString&) = 0;

    virtual void serializate(std::ostream&) = 0;
    virtual void deserializate(std::istream&) = 0;

    virtual void startExecution(){}
    virtual ActionResult_t execute(qint64, ExecuteConfig_t) = 0;
    virtual void endExecution(){}

    virtual ~IAction() {};
};

}
#endif // IACTION_H
