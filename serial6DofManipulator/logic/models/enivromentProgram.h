#ifndef ENIVROMENTPROGRAM_H
#define ENIVROMENTPROGRAM_H

#include "iaction.h"
#include "memory"

namespace serialMan
{

class EnivromentProgram
{
public:

    typedef std::shared_ptr<serialMan::actions::IAction> actionType_t;
    typedef std::vector<actionType_t> actionsBuf_t;

private:

    actionsBuf_t _prog;
    actionsBuf_t::iterator _executePos;

    mutable QMutex _progMutex;

public:

    EnivromentProgram() = default;
    EnivromentProgram(const actionsBuf_t&);
    EnivromentProgram(actionsBuf_t&&);

    size_t size() const;

    actionType_t at(size_t);
    void add(actionType_t);
    void insert(actionType_t, size_t);
    void remove(actionType_t);

    actionType_t begin();
    actionType_t next();

    uint8_t setPos(size_t);
    uint8_t setPos(actionType_t);


signals:

    void structureChanged();
    void executionPosChanged();

};
}
#endif // ENIVROMENTPROGRAM_H
