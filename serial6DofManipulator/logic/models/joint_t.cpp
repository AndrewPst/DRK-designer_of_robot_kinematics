#include "joint_t.h"

using namespace serialMan;

double Joint_t::getValue() const
{
    QMutexLocker lock(&_mValue);
    return _value;
}

void Joint_t::setValue(double value)
{
    QMutexLocker lock(&_mValue);
    if(value == _value)
        return;
    if(value < _minValue)
        _value = _minValue;
    else if(value > _maxValue)
        _value = _maxValue;
    else
        _value = value;
    lock.unlock();
    emit valueChanged(value);
}

double Joint_t::getMinValue() const
{
    QMutexLocker lock(&_mMinValue);
    return _minValue;
}

void Joint_t::setMinValue(double min)
{
    QMutexLocker lock(&_mMinValue);
    if(min == _minValue)
        return;
    _minValue = min;
    lock.unlock();
    emit minValueChanged(min);
}

double Joint_t::getMaxValue() const
{
    QMutexLocker lock(&_mMaxValue);
    return _maxValue;
}

void Joint_t::setMaxValue(double max)
{
    QMutexLocker lock(&_mMaxValue);
    if(max == _maxValue)
        return;
    _maxValue = max;
    lock.unlock();
    emit maxValueChanged(max);
}
