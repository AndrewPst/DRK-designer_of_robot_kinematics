#ifndef GLFIELD_H
#define GLFIELD_H

#include "openGL/baseglobject.h"

class glField : public BaseGLObject
{
public:
    glField();
    void draw(QGLWidget* w) override;
};

#endif // GLFIELD_H
