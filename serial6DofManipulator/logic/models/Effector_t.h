#ifndef EFFECTOR_T_H
#define EFFECTOR_T_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>

namespace serialMan
{



class Effector_t : public QObject
{
    Q_OBJECT
private:

    mutable QMutex _valueMut, _minMut, _maxMut;
    double _value{0};
    double _min, _max;

public:
    Effector_t();
    Effector_t(double, double);

    void setValue(double);
    double value() const;

    void setMin(double);
    void setMax(double);

    double min() const;
    double max() const;

signals:

    void valueChanged(double);

};

}
#endif // EFFECTOR_T_H
