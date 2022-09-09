#ifndef BASEGLOBJECT_H
#define BASEGLOBJECT_H

#include <QGLWidget>

class BaseGLObject
{
public:
    BaseGLObject();

    virtual void draw(QGLWidget* w) = 0;

protected:

};

#endif // BASEGLOBJECT_H
