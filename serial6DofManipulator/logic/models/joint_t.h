#ifndef JOINT_T_H
#define JOINT_T_H

#include <QObject>

#include <QVector3D>

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

    double _minValue {-180};
    double _maxValue {180};
};

}
#endif // JOINT_T_H
