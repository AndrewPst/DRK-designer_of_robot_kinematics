#include "projectCore/projectStructure.h"

Joint_t::Joint_t()
{
}

Joint_t::~Joint_t()
{
}

JointType_t Joint_t::getJointType() const
{
    return _type;
}

void Joint_t::setJointType(const JointType_t& type)
{
    if(type == _type) return;
    _type = type;
    emit onTypeChanged(type);
}

QVector3D Joint_t::getPosition() const
{
    return _position;
}

void Joint_t::setPosition(const QVector3D& pos)
{
    if(pos == _position) return;
    _position = pos;
    emit onPositionChanged(pos);
}

QVector3D Joint_t::getRotation() const
{
    return _rotation;
}

void Joint_t::setRotation(const QVector3D& rot)
{
    if(rot == _rotation) return;
    _rotation = rot;
    emit onRotationChanged(rot);
}

DHParameters_t Joint_t::getDHParams() const
{
    return _dhParameters;
}

void Joint_t::setDHParams(const DHParameters_t& dh)
{
    if((DHParameters_t&)dh == _dhParameters) return;
    _dhParameters = dh;
    emit onDHParamsChanged(dh);
}

double Joint_t::getMinValue() const
{
    return _minValue;
}

void Joint_t::setMinValue(double val)
{
    if(val == _minValue) return;
    _minValue = val;
    emit onMinValueChanged(val);
}

double Joint_t::getMaxValue() const
{
    return _maxValue;
}
void Joint_t::setMaxValue(double val)
{
    if(val == _maxValue) return;
    _maxValue = val;
    emit onMaxValueChanged(val);
}

double Joint_t::getCurrentValue() const
{
    return _currentValue;
}

void Joint_t::setCurrentValue(double val)
{
    if(val == _currentValue) return;
    _currentValue = val;
    emit onCurrentValueChanged(val);
}


bool Joint_t::isGrounded()
{
    return _isGround;
}

void Joint_t::setGrounded(bool val)
{
    if(val == _isGround) return;
    _isGround = val;
    emit onGroundedChanged(val);
}

int Joint_t::getId()
{
    return _id;
}

void Joint_t::setId(int val)
{
    if(val == _id) return;
    _id = val;
    emit onIdChanged(val);
}

QSet<int> Joint_t::getParents()
{
    return _parents;
}

void Joint_t::setParents(QSet<int>& val)
{
    if(val == _parents) return;
    _parents = val;
    emit onParentsChanged(val);
}

QSet<int> Joint_t::getChildren()
{
    return _children;
}

void Joint_t::setChildren(QSet<int>& val)
{
    if(val == _children) return;
    _children = val;
    emit onChildrenChanged(val);
}
