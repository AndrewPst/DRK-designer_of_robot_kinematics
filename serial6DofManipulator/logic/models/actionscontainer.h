#ifndef ACTIONSCONTAINER_H
#define ACTIONSCONTAINER_H

#include "iaction.h"

#include <QObject>
#include <QMutex>
#include <QList>
#include <QMutexLocker>

namespace serialMan
{

class ActionsContainer : QObject
{
    Q_OBJECT

private:

    QList<serialMan::IAction*> _data;

    mutable QMutex _mutex;

public:
    ActionsContainer();

    size_t size() const;

    void append(IAction*);
    void insert(size_t, IAction*);

    void remove(size_t);
    void remove(IAction*);

    void replace(size_t, IAction*);
    void replace(IAction*, IAction*);

    IAction& at(size_t);
    IAction& first();
    IAction& last();

    QList<serialMan::IAction*>::Iterator begin();
    QList<serialMan::IAction*>::Iterator end();

    void swap(size_t, size_t);

    IAction& operator[](size_t i);

};

}
#endif // ACTIONSCONTAINER_H
