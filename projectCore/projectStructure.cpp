#include "projectCore/projectStructure.h"
#include "projectCore/projectcore.h"

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


bool Joint_t::isGrounded() const
{
    return _isGround;
}

void Joint_t::setGrounded(bool val)
{
    if(val == _isGround) return;
    _isGround = val;
    emit onGroundedChanged(val);
}

int Joint_t::getId() const
{
    return _id;
}

void Joint_t::setId(int val)
{
    if(val == _id) return;
    _id = val;
    emit onIdChanged(val);
}

int Joint_t::getParentId() const
{
    return _parents;
}

void Joint_t::setParentId(int val)
{
    if(val == _parents) return;
    if(val == _id) return;
    if(val != -1)
    {
        bool result = false;
        Joint_t* parent = projectManager.getOpenedProject()->getJoint(val, &result);
        if(result)
            parent->setChildId(_id);
        else
            return;
        if(_parents != -1)
        {
            bool result = false;
            Joint_t* parent = projectManager.getOpenedProject()->getJoint(_parents, &result);
            if(result)
                parent->setChildId(-1);
        }

    }
    _parents = val;
    emit onParentIdChanged(val);
}

int Joint_t::getChildId() const
{
    return _children;
}

void Joint_t::setChildId(int val)
{
    if(val == _children) return;
    bool result = false;
    Joint_t* parent = projectManager.getOpenedProject()->getJoint(_children, &result);
    if(result)
        parent->setParentId(-1);
    _children = val;
    emit onChildIdChanged(val);
}
