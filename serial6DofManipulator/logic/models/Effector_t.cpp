#include "Effector_t.h"

using namespace serialMan;

Effector_t::Effector_t() : _min(0), _max(100)
{

}


Effector_t::Effector_t(double min, double max) : _min(min), _max(max)
{
}


void Effector_t::setValue(double v)
{
    QMutexLocker lock(&_valueMut);
    if(_value == v)
        return;
    _value = v;
    lock.unlock();
    emit valueChanged(_value);
}

double Effector_t::value() const
{
    QMutexLocker lock(&_valueMut);
    return _value;
}

void Effector_t::setMin(double min)
{
    QMutexLocker lock(&_minMut);
    _min = min;
}

void Effector_t::setMax(double max)
{
    QMutexLocker lock(&_maxMut);
    _max = max;
}

double Effector_t::min() const
{
    QMutexLocker lock(&_minMut);
    return _min;
}

double Effector_t::max() const
{
    QMutexLocker lock(&_maxMut);
    return _max;
}
