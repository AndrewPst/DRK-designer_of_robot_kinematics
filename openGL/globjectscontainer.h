#ifndef GLOBJECTSCONTAINER_H
#define GLOBJECTSCONTAINER_H

#include <QVector>
#include <QVectorIterator>

#include "openGL/baseglobject.h"

class glObjectsContainer
{
public:
    glObjectsContainer() = default;

    static void addObject(BaseGLObject* obj);
    static void removeObject(BaseGLObject* obj);
    static QVectorIterator<BaseGLObject*> begin();
    static size_t getObjectsCount();

private:
    static QVector<BaseGLObject*> _objects;
};

#endif // GLOBJECTSCONTAINER_H
