#include "actionscontainer.h"

using namespace serialMan;

ActionsContainer::ActionsContainer()
{
}

size_t ActionsContainer::size() const
{
    QMutexLocker lock(&_mutex);
    return _data.size();
}

void ActionsContainer::insert(size_t i, IAction* a)
{
    QMutexLocker lock(&_mutex);
    _data.insert(i, a);
}

void ActionsContainer::append(IAction* a)
{
    QMutexLocker lock(&_mutex);
    _data.append(a);
}

void ActionsContainer::remove(size_t i)
{
    QMutexLocker lock(&_mutex);
    _data.removeAt(i);
}

void ActionsContainer::remove(IAction* a)
{
    QMutexLocker lock(&_mutex);
    _data.removeAll(a);
}

void ActionsContainer::replace(size_t i, IAction* a)
{
    QMutexLocker lock(&_mutex);
    _data.replace(i, a);
}

void ActionsContainer::replace(IAction* a, IAction* b)
{
    QMutexLocker lock(&_mutex);
    _data.replace(_data.indexOf(a), b);
}

IAction& ActionsContainer::at(size_t i)
{
    QMutexLocker lock(&_mutex);
    return *_data.at(i);
}

IAction& ActionsContainer::first()
{
    QMutexLocker lock(&_mutex);
    return *_data.first();
}

IAction& ActionsContainer::last()
{
    QMutexLocker lock(&_mutex);
    return *_data.last();
}

IAction& ActionsContainer::operator[](size_t i)
{
    QMutexLocker lock(&_mutex);
    return *_data[i];
}

void ActionsContainer::swap(size_t i, size_t b)
{
    QMutexLocker lock(&_mutex);
    _data.swapItemsAt(i, b);
}

QList<serialMan::IAction*>::Iterator ActionsContainer::begin()
{
    QMutexLocker lock(&_mutex);
    return _data.begin();
}

QList<serialMan::IAction*>::Iterator ActionsContainer::end()
{
    QMutexLocker lock(&_mutex);
    return _data.end();
}



