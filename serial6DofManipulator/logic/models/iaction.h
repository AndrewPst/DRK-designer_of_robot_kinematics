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

using ArgKey_t = char;

struct ArgDescription_t
{
    //ArgKey_t();
    explicit ArgDescription_t(ArgKey_t k, ActionArgumentType_t = ActionArgumentType_t::ARGTYPE_DOUBLE, const QString& = QString());

private:
    const ArgKey_t _key {0};
    const ActionArgumentType_t _type {ActionArgumentType_t::ARGTYPE_DOUBLE};
    const QString _name;

public:
    ArgKey_t key() const;
    ActionArgumentType_t type() const;
    const QString& name() const;

    bool operator == (const serialMan::actions::ArgDescription_t &) const;
    bool operator < (const serialMan::actions::ArgDescription_t &) const;
};


struct Arg_t
{
private:
    bool _isUsable;
    QVariant _value;
public:

    Arg_t() : _isUsable(true), _value(QVariant()){}
    Arg_t(const QVariant& v) : _isUsable(true), _value(v){}
    Arg_t(const Arg_t& arg) : _isUsable(arg.isUseble()), _value(arg.getValue()){}
    //explicit Arg_t(QVariant&& v) : _value(std::move(v)){}

    bool isUseble() const{return _isUsable;}
    void setUsable(bool v){_isUsable = v;}

    const QVariant& getValue() const{return _value;}
    void setValue(const QVariant& val) {_value = val;}

    Arg_t& operator=(const Arg_t& v)
    {
        setValue(v.getValue());
        setUsable(v.isUseble());
        return *this;
    }

};


struct IArgsCollection
{
public:

    virtual bool getValue(ArgKey_t, Arg_t&) const = 0;
    virtual void setValue(ArgKey_t, const Arg_t&) = 0;
};

typedef std::pair<char, uint16_t> actionIdentificator_t;

struct IAction
{
    friend class ExecutionEnivroment;

public:

    virtual const std::pair<char, uint16_t> key() = 0;

    virtual void serializate(std::ostream&) = 0;
    //virtual void deserializate(std::istream&) = 0;

    virtual ActionExectionResult_t startExecution(const IArgsCollection&, const ExecutionEnivroment&) = 0;
    virtual ActionExectionResult_t execute(const ExecutionEnivroment&, qint64, ExecuteConfig_t) = 0;
    virtual void endExecution(){}

    virtual ~IAction() {};
};

}
}
#endif // IACTION_H
