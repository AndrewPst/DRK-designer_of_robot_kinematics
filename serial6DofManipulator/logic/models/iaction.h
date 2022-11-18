#ifndef IACTION_H
#define IACTION_H

#include <QString>
#include <istream>
#include <ostream>
#include <QThreadStorage>
#include <QCache>
namespace serialMan
{

QT_FORWARD_DECLARE_CLASS(ManipulatorController)

enum ActionResult_t
{
    RESULT_UNKNOWN,
    RESULT_IN_PROCESS,
    RESULT_ERROR,
    RESULT_FINISH
};

struct IAction
{
    friend class ActionsController;
protected:

    IAction() = default;

    virtual ~IAction(){}

public:

    QThreadStorage<QCache<char, double>> args;

    virtual bool isCorrected(QString&) = 0;

    virtual void serializate(std::ostream&) = 0;
    virtual void deserializate(std::istream&) = 0;

    virtual void startExecution(){}
    virtual ActionResult_t execute(ManipulatorController&, qint64) = 0;
    virtual void endExecution(){}

};

}
#endif // IACTION_H
