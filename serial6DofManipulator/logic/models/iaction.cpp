#include "iaction.h"

using namespace serialMan;

ArgKey_t::ArgKey_t()
{}
ArgKey_t::ArgKey_t(char k, ActionArgumentType_t t, const QString& n) : key(k), type(t), name(n)
{}

bool ArgKey_t::operator == (const serialMan::ArgKey_t &p1) const
{
    return key == p1.key;
}

bool ArgKey_t::operator < (const serialMan::ArgKey_t &p1) const
{
    return key < p1.key;
}


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

void IAction::setArg(const ArgKey_t& key, const Argument_t& value)
{
    QMutexLocker lock(&_argMut);
    if(auto keys = argsKeys())
    {
        if(keys->contains(key))
            _args[key] = value;
    }
}

bool IAction::getArg(const ArgKey_t& key, Argument_t& result) const
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
