#include "openGL/globjectscontainer.h"

QVector<BaseGLObject*>  glObjectsContainer::_objects;

void glObjectsContainer::addObject(BaseGLObject* obj)
{
    if(!obj) return;
    _objects.append(obj);
}

void glObjectsContainer::removeObject(BaseGLObject* obj)
{
    _objects.removeAll(obj);
}

QVectorIterator<BaseGLObject*> glObjectsContainer::begin()
{
    return QVectorIterator<BaseGLObject*>(_objects);
}

size_t glObjectsContainer::getObjectsCount()
{
    return _objects.size();
}
