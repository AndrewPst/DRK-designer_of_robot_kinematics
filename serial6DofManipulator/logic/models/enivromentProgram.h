#ifndef ENIVROMENTPROGRAM_H
#define ENIVROMENTPROGRAM_H

#include "iaction.h"

#include <memory>

namespace serialMan
{

class EnivromentProgram : public QObject
{
    Q_OBJECT
public:

    //typedef std::shared_ptr<serialMan::actions::IAction> actionType_t;
//    typedef std::vector<actionType_t> actionsBuf_t;
    typedef std::pair<actions::actionIdentificator_t, std::shared_ptr<actions::IArgsCollection>> actionData_t;
    typedef std::vector<actionData_t> actionsBuf_t;

private:

    actionsBuf_t _prog;
    actionsBuf_t::iterator _executePos;

    mutable QMutex _progMutex;

public:

    EnivromentProgram() = default;
    EnivromentProgram(const actionsBuf_t&);
    EnivromentProgram(actionsBuf_t&&);

    size_t size() const;

    actionData_t& at(size_t);
    void add(const actionData_t&);
    void insert(const actionData_t&, size_t);
    void remove(const actionData_t&);

    void clear();

    actionData_t& reset();
    actionData_t& next();

    uint8_t setPos(size_t);
    uint8_t setPos(const actionData_t&);


signals:

    void structureChanged();
    //void executionPosChanged();

};
}
#endif // ENIVROMENTPROGRAM_H
