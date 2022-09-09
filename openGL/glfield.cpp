#include "openGL/glfield.h"

glField::glField()
{
}

void glField::draw(QGLWidget* w)
{
    w->makeCurrent();
    //---------draw lines------------
    const int field_w = 10, field_h = 10;

    for (int i = -field_w / 2; i <= field_w / 2; i++)
    {
        if (i == 0)
            glColor3f(0, 1, 0);
        else if (i % 2 == 0)
            glColor3f(0.6, 0.6, 0.6);
        else
            glColor3f(0.8f, 0.8f, 0.8f);
        glPushMatrix();
        glTranslatef((float)i, -field_h / 2.f, 0);
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, field_h, 0);
        glEnd();
        glPopMatrix();
    }

    for (int i = -field_h / 2; i <= field_h / 2; i++)
    {
        if (i == 0)
            glColor3f(1, 0, 0);
        else if (i % 2 == 0)
            glColor3f(0.6, 0.6, 0.6);
        else
            glColor3f(0.8f, 0.8f, 0.8f);
        glPushMatrix();
        glTranslatef(-field_w / 2, (float)i, 0);
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(field_w, 0, 0);
        glEnd();
        glPopMatrix();
    }

    //----------draw Axis------------
    glPushMatrix();
    glLineWidth(4);
    glTranslatef(0, 0, 0);
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
    glPopMatrix();
}
