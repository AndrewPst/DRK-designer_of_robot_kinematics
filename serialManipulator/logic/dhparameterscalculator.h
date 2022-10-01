#ifndef DHPARAMETERSCALCULATOR_H
#define DHPARAMETERSCALCULATOR_H

#include "../serialManipulatorProject.h"

#include <QObject>

namespace serialMan {

QT_FORWARD_DECLARE_CLASS(Joint_t);

struct DHParameters_t
{
    double teta{0},
    alfa{0},
    r{0},
    d{0};

    bool operator==(const DHParameters_t& dh)
    {
        return teta == dh.teta &&
                alfa == dh.alfa &&
                r == dh.r &&
                d == dh.d;
    }
};

class dhParametersCalculator : public QObject
{
    Q_OBJECT

    friend class serialMan::SerialManipulatorProject;

private:

    dhParametersCalculator(SerialManipulatorProject*);
    ~dhParametersCalculator();

    dhParametersCalculator& operator=(const dhParametersCalculator&) = delete;
    dhParametersCalculator(const dhParametersCalculator&) = delete;

public:

    void calculate();


signals:

    void dhParametersTableChanged(const QVector<serialMan::DHParameters_t>&);

private:

    void correctRotateJoint(Joint_t*, Joint_t*);

private:

    QVector<DHParameters_t> _table;
    SerialManipulatorProject* _proj;
};

}
#endif // DHPARAMETERSCALCULATOR_H
