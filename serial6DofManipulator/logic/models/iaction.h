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
QT_FORWARD_DECLARE_STRUCT(ExecutionEnivroment)

namespace actions
{


enum ActionExectionResult_t
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
    ArgKey_t();
    explicit ArgKey_t(char k, ActionArgumentType_t = ActionArgumentType_t::ARGTYPE_DOUBLE, const QString& = QString());

    char key {0};
    ActionArgumentType_t type {ActionArgumentType_t::ARGTYPE_DOUBLE};
    QString name;
    bool operator == (const serialMan::actions::ArgKey_t &) const;
    bool operator < (const serialMan::actions::ArgKey_t &) const;
};

struct IAction
{
    friend class ExecutionEnivroment;

public:

//    //Args functions
//    void setArg(const ArgKey_t& key, const QVariant);
//    bool getArg(const ArgKey_t& key, QVariant& result) const;

    virtual void serializate(std::ostream&) = 0;
    //virtual void deserializate(std::istream&) = 0;

    virtual void startExecution(const ExecutionEnivroment&){}
    virtual ActionExectionResult_t execute(const ExecutionEnivroment&, qint64, ExecuteConfig_t) = 0;
    virtual void endExecution(){}

    virtual ~IAction() {};
};

}
}
#endif // IACTION_H
