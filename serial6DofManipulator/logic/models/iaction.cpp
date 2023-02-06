#include "iaction.h"

using namespace serialMan;
using namespace actions;

ArgKey_t::ArgKey_t()
{}
ArgKey_t::ArgKey_t(char k, ActionArgumentType_t t, const QString& n) : key(k), type(t), name(n)
{}

bool ArgKey_t::operator == (const serialMan::actions::ArgKey_t &p1) const
{
    return key == p1.key;
}

bool ArgKey_t::operator < (const serialMan::actions::ArgKey_t &p1) const
{
    return key < p1.key;
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

