#ifndef IACTION_H
#define IACTION_H

#include "../manipulatorcontroller.h"

#include <QString>
#include <istream>
#include <ostream>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <QVariant>


namespace serialMan
{

QT_FORWARD_DECLARE_STRUCT(ExecutionEnivroment)

namespace actions
{


enum ActionExectionResult
{
    RESULT_UNKNOWN,
    RESULT_IN_PROCESS,
    RESULT_ERROR,
    RESULT_FINISH
};

enum ExecuteConfig
{
    EXECUTE_ANIMATION,
    EXECUTE_MOMENTLY,
};

enum ActionArgumentParameter : uint16_t
{
    ARGPARAM_IS_ANGLE=1,
    ARGPARAM_UNLIMITED=2,
    ARGPARAM_NUM_POSITIVE_ONLY=4,
    ARGPARAM_IS_REQUIRED=8,
    ARGPARAM_NO_VALUE=16
};

using ArgKey_t = char;

struct ArgDescription_t
{
    const ArgKey_t key{0};
    const QVariant::Type type{QVariant::Type::Double};
    const QString name{};
    const uint16_t parameters{0};
    const ManipulatorController::ManipulatorParameterKey manParameter{ManipulatorController::ManipulatorParameterKey::PARAMETER_NONE};
};


struct Arg_t
{
private:
    bool _isUsable;
    QVariant _value;
public:

    Arg_t() : _isUsable(false), _value(QVariant()){}
    Arg_t(const QVariant& v) : _isUsable(false), _value(v){}
    Arg_t(const Arg_t& arg) : _isUsable(arg.isUseble()), _value(arg.getValue()){}
    //explicit Arg_t(QVariant&& v) : _value(std::move(v)){}

    bool isUseble() const{return _isUsable;}
    void setUsable(bool v){_isUsable = v;}

    const QVariant& getValue() const{return _value;}
    void setValue(const QVariant& val) {_value = val; setUsable(true);}

    Arg_t& operator=(const Arg_t& v)
    {
        setValue(v.getValue());
        setUsable(v.isUseble());
        return *this;
    }
};


struct IArgsCollection
{
protected:

    std::unordered_map<ArgKey_t, std::pair<QVariant, bool>> _args;

public:

    IArgsCollection()
    {}

    IArgsCollection(const std::unordered_map<ArgKey_t, std::pair<QVariant, bool>>& args) : _args(args)
    {
    }

    QVariant* getArg(ArgKey_t key)
    {
        if(_args.find(key) == _args.end())
            return nullptr;
        auto& a = _args.at(key);
        return &(a.first);

    }

    void setArg(ArgKey_t key, const QVariant& value)
    {
        if(_args.find(key) == _args.end())
            return;
        _args[key].first = value;
        _args[key].second = true;
    }

    void setArgUsable(ArgKey_t key, bool usable)
    {
        if(_args.find(key) == _args.end())
            return;
        _args[key].second = usable;
    }

    bool isArgUsable(ArgKey_t key) const
    {
        if(_args.find(key) == _args.end())
            return false;
        return _args.at(key).second;
    }
};

typedef std::pair<char, uint16_t> actionIdentificator_t;

struct IAction
{
    friend class ExecutionEnivroment;

public:

    virtual const std::pair<char, uint16_t> key() = 0;

    virtual ActionExectionResult startExecution(IArgsCollection&, const ExecutionEnivroment&) = 0;
    virtual ActionExectionResult execute(const ExecutionEnivroment&, qint64, ExecuteConfig) = 0;
    virtual void endExecution(){}

    virtual ~IAction() {};
};

}
}
#endif // IACTION_H
