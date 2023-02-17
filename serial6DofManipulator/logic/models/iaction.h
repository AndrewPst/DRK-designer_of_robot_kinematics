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
    //ArgKey_t();
    explicit ArgKey_t(char k, ActionArgumentType_t = ActionArgumentType_t::ARGTYPE_DOUBLE, const QString& = QString());

private:
    const char _key {0};
    const ActionArgumentType_t _type {ActionArgumentType_t::ARGTYPE_DOUBLE};
    const QString _name;
public:

    char key() const;
    ActionArgumentType_t type() const;
    const QString& name() const;

    bool operator == (const serialMan::actions::ArgKey_t &) const;
    bool operator < (const serialMan::actions::ArgKey_t &) const;
};

struct IAction
{
    friend class ExecutionEnivroment;

public:

//    //Args functions
    virtual void setArg(const ArgKey_t& , const QVariant){};
    virtual bool getArg(const ArgKey_t& , QVariant& ) const{return false;};

    virtual const std::pair<char, uint16_t> key() = 0;

    virtual void serializate(std::ostream&) = 0;
    //virtual void deserializate(std::istream&) = 0;

    virtual ActionExectionResult_t startExecution(const ExecutionEnivroment&) = 0;
    virtual ActionExectionResult_t execute(const ExecutionEnivroment&, qint64, ExecuteConfig_t) = 0;
    virtual void endExecution(){}

    virtual ~IAction() {};
};

}
}
#endif // IACTION_H
