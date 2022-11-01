#include "joint_t.h"

using namespace serialMan;

double Joint_t::getValue() const
{
    return _value;
}

void Joint_t::setValue(double value)
{
    if(value == _value)
        return;
    _value = value;
    emit valueChanged(value);
}

double Joint_t::getMinValue() const
{
    return _minValue;
}

void Joint_t::setMinValue(double min)
{
    if(min == _minValue)
        return;
    _minValue = min;
    emit minValueChanged(min);
}

double Joint_t::getMaxValue() const
{
    return _maxValue;
}

void Joint_t::setMaxValue(double max)
{
    if(max == _maxValue)
        return;
    _maxValue = max;
    emit maxValueChanged(max);
}
