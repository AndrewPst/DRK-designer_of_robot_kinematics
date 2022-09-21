#include "../openGL/projectvisualizator.h"

#include <QGLWidget>
#include <gl/glu.h>
#include <gl/gl.h>

using namespace serialMan;

serialMan::ProjectVisualizator& serialMan::projectVisualizator = serialMan::ProjectVisualizator::getInstance();

ProjectVisualizator& ProjectVisualizator::getInstance()
{
    static ProjectVisualizator pv;
    return pv;
}

void serialMan::ProjectVisualizator::visualizate(serialMan::glVisualizatorWidget* glv)
{
    glv->makeCurrent();
    _currentContext = glv;
    drawField();
    drawAxis();
    drawCustomObjects();
}

enum FIELD_AXIS : byte
{
    AXIS_Z = 0,
    AXIS_X = 1,
    AXIS_Y = 2
};

void ProjectVisualizator::drawField()
{

    const int field_w = 100 / _koef, field_h = 100 / _koef;

    float angle_pos_S = (cos(_currentContext->getAngleY()) < 0 ? 1 : -1);
    float angle_pos_C = (sin(_currentContext->getAngleY()) > 0 ? 1 : -1);

    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPointSize(3.0 * _koef / (_currentContext->getDistance() * 0.01) + 1);
    glLineWidth(2);

    QColor lightColor = QColor({200, 200, 200});
    QColor darkColor = QColor({160, 160, 160});
    QColor textColor = QColor({80, 80, 80});

    glPushMatrix();
    for (int i = -field_w / 2.0; i <= field_w / 2.0; i+=1)
    {
        if (i == 0)
            glColor3f(0, 1, 0);
        else
            _currentContext->qglColor(fmod(i, 2) == 0.0 ? darkColor : lightColor );
        float pos = i * _koef;
        glPushMatrix();
        glTranslatef((float)pos, -field_h / 2.f * _koef, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, field_h * _koef, 0);
        glEnd();
        _currentContext->qglColor(textColor);
        //Render values and calculate text width for projection in center
        QString text = QString("%1").arg(pos);
        _currentContext->renderText(-(text.length()/2.0*(font.pointSize()*0.005)) * _koef,
                                    (-0.5 + (angle_pos_S > 0 ? field_h + 1 : 0)) * _koef,
                                    -0.5 * _koef,
                                    text, font);
        glPopMatrix();
    }
    for (int i = -field_h / 2.0; i <= field_h / 2.0; i+=1)
    {
        if (i == 0)
            glColor3f(1, 0, 0);
        else
            _currentContext->qglColor(fmod(i, 2) == 0.0 ? darkColor : lightColor );
        float pos = i * _koef;
        glPushMatrix();
        glTranslatef(-field_w / 2.f * _koef, (float)pos, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(field_w * _koef, 0, 0);
        glEnd();
        _currentContext->qglColor(textColor);
        QString text = QString("%1").arg(pos);
        _currentContext->renderText((-0.5 + (angle_pos_C < 0 ? field_h + 1 : 0)) * _koef,
                                    -(text.length()/2.0*((font.pointSize()-1)*0.005)) * _koef,
                                    -0.5 * _koef,
                                    text, font);
        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glTranslatef(0, 0, field_w / 2.0 * angle_pos_C * _koef);
    for (int i = 0.0; i <= field_w; i+=1)
    {

        _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        float pos = i * _koef;
        glPushMatrix();
        glTranslatef((float)-pos, -field_h / 2.f * _koef, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, field_h * _koef, 0);
        glEnd();
        //Render values and calculate text width for projection in center
        _currentContext->qglColor(textColor);
        QString text = QString("%1").arg(pos);
        _currentContext->renderText(0.1 * _koef,
                                    (-0.5 + (angle_pos_S > 0 ? field_h + 1 : 0))*_koef,
                                    0.5 * angle_pos_C * _koef,
                                    text, font);
        glPopMatrix();
    }
    for (int i = -field_h / 2.0; i <= field_h / 2.0; i+=1)
    {
        if (i == 0)
            glColor3f(0, 0, 1);
        else
            _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        float pos = i * _koef;
        glPushMatrix();
        glTranslatef(-field_w * _koef, (float)pos, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(field_w * _koef, 0, 0);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0, field_w / 2.0 * angle_pos_S * _koef);
    for (int i = - field_w / 2.0; i <= field_w / 2.0; i+=1)
    {
        if (i == 0)
            glColor3f(0, 0, 1);
        else
            _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        float pos = i * _koef;
        glPushMatrix();
        glTranslatef((float)pos, 0, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, field_h * _koef, 0);
        glEnd();
        glPopMatrix();
    }
    for (int i = 0.0; i <=field_h; i+=1)
    {
        _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        float pos = i * _koef;
        glPushMatrix();
        glTranslatef(-field_w/2.0 * _koef, (float)pos, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(field_w * _koef, 0, 0);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();

    //----------draw Axis------------
    //    glPushMatrix();
    //    glLineWidth(4);
    //    glTranslatef(0, 0, 0);
    //    glBegin(GL_LINES);

    //    glColor3f(1, 0, 0);
    //    glVertex3f(0, 0, 0);
    //    glVertex3f(1, 0, 0);

    //    glColor3f(0, 1, 0);
    //    glVertex3f(0, 0, 0);
    //    glVertex3f(0, 1, 0);

    //    glColor3f(0, 0, 1);
    //    glVertex3f(0, 0, 0);
    //    glVertex3f(0, 0, 1);

    //    glEnd();
    //    glPopMatrix();
}

void ProjectVisualizator::drawAxis()
{

}

void ProjectVisualizator::drawCustomObjects()
{

}

