#ifndef JOINT_T_H
#define JOINT_T_H

#include <QObject>

#include <QVector3D>

namespace serialMan
{

class Joint_t : public ::QObject
{
    Q_OBJECT

public:
    Joint_t();

    QVector3D getPosition() const;
    void setPosition(const QVector3D&);

    QVector3D getRotation() const;
    void setRotation(const QVector3D&);

    double getValue() const;
    void setValue(double);

    double getMinValue() const;
    void setMinValue(double);

    double getMaxValue() const;
    void setMaxValue(double);

signals:

    void positionChanged(QVector3D);
    void rotationChanged(QVector3D);
    void valueChanged(double);
    void minValueChanged(double);
    void maxValueChanged(double);

private:

    QVector3D _position;
    QVector3D _rotation;

    double _value;

    double _minValue;
    double _maxValue;
};

}
#endif // JOINT_T_H
