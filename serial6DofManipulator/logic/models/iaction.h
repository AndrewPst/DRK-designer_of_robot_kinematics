#ifndef IACTION_H
#define IACTION_H

#include <QString>
#include <istream>
#include <ostream>

namespace serialMan
{

QT_FORWARD_DECLARE_CLASS(ManipulatorController)

struct IAction
{
    friend class ActionsController;
private:

    IAction();
    IAction(const IAction&);

protected:
    virtual ~IAction(){}

public:

    virtual bool isCorrected(QString&) = 0;

    virtual void serializate(std::ostream&) = 0;
    virtual void deserializate(std::istream&) = 0;

    virtual void execute(ManipulatorController&) = 0;

};

}
#endif // IACTION_H
