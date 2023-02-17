#include "iaction.h"

using namespace serialMan;
using namespace actions;

//ArgKey_t::ArgKey_t()
//{}
ArgKey_t::ArgKey_t(char k, ActionArgumentType_t t, const QString& n) : _key(k), _type(t), _name(n)
{}

bool ArgKey_t::operator == (const serialMan::actions::ArgKey_t &p1) const
{
    return _key == p1._key;
}

bool ArgKey_t::operator < (const serialMan::actions::ArgKey_t &p1) const
{
    return _key < p1._key;
}

char ArgKey_t::key() const
{
    return _key;
}
ActionArgumentType_t ArgKey_t::type() const
{
    return _type;
}
const QString& ArgKey_t::name() const
{
    return _name;
}


//void IAction::setArg(const ArgKey_t& key, const QVa& value)
//{
//    QMutexLocker lock(&_argMut);
//    if(auto keys = argsKeys())
//    {
//        if(keys->contains(key))
//            _args[key] = value;
//    }
//}

//bool IAction::getArg(const ArgKey_t& key, Argument_t& result) const
//{
//    QMutexLocker lock(&_argMut);
//    if(_args.contains(key))
//    {
//        result = _args.value(key);
//        return true;
//    }
//    return false;;
//}

