#ifndef JOINT_T_H
#define JOINT_T_H

#include "math.h"

#include <QObject>

#include <QVector3D>

#include <QMutex>
#include <QMutexLocker>

namespace serialMan
{

class Joint_t : public QObject
{
    Q_OBJECT

public:
    Joint_t() = default;

    double getValue() const;
    void setValue(double);

    double getMinValue() const;
    void setMinValue(double);

    double getMaxValue() const;
    void setMaxValue(double);

signals:

    void valueChanged(double);
    void minValueChanged(double);
    void maxValueChanged(double);

private:

    double _value {0};

    double _minValue {-M_PI_2};
    double _maxValue {M_PI_2};
    mutable QMutex _mValue, _mMinValue, _mMaxValue;

};

}
#endif // JOINT_T_H
