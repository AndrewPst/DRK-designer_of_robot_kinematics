#include "iaction.h"

using namespace serialMan;

IAction::IAction(ActionsEnivroment& env) : _enivroment(&env)
{

}

void IAction::setEnivroment(ActionsEnivroment& env)
{
    _enivroment = &env;
}

ActionsEnivroment& IAction::enivroment() const
{
    return *_enivroment;
}

void IAction::setArg(ArgKey_t key, Argument_t value)
{
    QMutexLocker lock(&_argMut);
    if(auto keys = argsKeys())
    {
        if(keys->contains(key))
            _args[key] = value;
    }
}

bool IAction::getArg(ArgKey_t key, Argument_t& result) const
{
    QMutexLocker lock(&_argMut);
    if(_args.contains(key))
    {
        result = _args.value(key);
        return true;
    }
    return false;;
}


const QVector<ArgKey_t>* IAction::argsKeys() const {return nullptr;}
