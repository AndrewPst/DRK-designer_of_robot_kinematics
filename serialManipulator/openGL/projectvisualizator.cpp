#include "../openGL/projectvisualizator.h"
#include "../openGL/glvisualizatorwidget.h"

#include <QGLWidget>
#include <gl/glu.h>
#include <gl/gl.h>

ProjectVisualizator& projectVisualizator = ProjectVisualizator::getInstance();

ProjectVisualizator& ProjectVisualizator::getInstance()
{
    static ProjectVisualizator pv;
    return pv;
}

void ProjectVisualizator::visualizate(glVisualizatorWidget* glv)
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
    const int field_w = 10, field_h = 10;
    float step = 1;

    float angle_pos_S = (cos(_currentContext->getAngleY()) < 0 ? 1 : -1);
    float angle_pos_C = (sin(_currentContext->getAngleY()) > 0 ? 1 : -1);

    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPointSize(3.0 / (_currentContext->getDistance() * 0.01) + 1);
    glLineWidth(2);

    QColor lightColor = QColor({200, 200, 200});
    QColor darkColor = QColor({160, 160, 160});
    QColor textColor = QColor({80, 80, 80});

    glPushMatrix();
    for (float i = -field_w / 2.0 - fmod(-field_w/2.0, step); i <= field_w / 2.0; i+=step)
    {
        if (i == 0)
            glColor3f(0, 1, 0);
        else
            _currentContext->qglColor(fmod(i, 2) == 0.0 ? darkColor : lightColor );
        glPushMatrix();
        glTranslatef((float)i, -field_h / 2.f, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, field_h, 0);
        glEnd();
        _currentContext->qglColor(textColor);
        //Render values and calculate text width for projection in center
        QString text = QString("%1").arg(i);
        _currentContext->renderText(-(text.length()/2.0*(font.pointSize()*0.005)), -0.5 + (angle_pos_S > 0 ? field_h + 1 : 0), -0.5, text, font);
        glPopMatrix();
    }
    for (float i = -field_h / 2.0 - fmod(-field_h/2.0, step); i <= field_h / 2.0; i+=step)
    {
        if (i == 0)
            glColor3f(1, 0, 0);
        else
            _currentContext->qglColor(fmod(i, 2) == 0.0 ? darkColor : lightColor );
        glPushMatrix();
        glTranslatef(-field_w / 2.0, (float)i, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(field_w, 0, 0);
        glEnd();
        _currentContext->qglColor(textColor);
        QString text = QString("%1").arg(i);
        _currentContext->renderText(-0.5 + (angle_pos_C < 0 ? field_h + 1 : 0), -(text.length()/2.0*((font.pointSize()-1)*0.005)), -0.5, text, font);
        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glTranslatef(0, 0, field_w / 2.0 * angle_pos_C);
    for (float i = 0.0; i <= (float)field_w*1.0; i+=step)
    {
        _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        glPushMatrix();
        glTranslatef((float)-i, -field_h / 2.f, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, field_h, 0);
        glEnd();
        //Render values and calculate text width for projection in center
        _currentContext->qglColor(textColor);
        QString text = QString("%1").arg(i);
        _currentContext->renderText(0.1,
                                    -0.5 + (angle_pos_S > 0 ? field_h + 1 : 0),
                                    0.5 * angle_pos_C,
                                    text, font);
        glPopMatrix();
    }
    for (float i = -field_h / 2.0 - fmod(-field_h/2.0, step); i <= field_h / 2.0; i+=step)
    {
        _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        glPushMatrix();
        glTranslatef(-field_w, (float)i, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(field_w, 0, 0);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0, field_w / 2.0 * angle_pos_S);
    for (float i = - field_w / 2.0 - fmod(-field_w/2.0, step); i <= field_w / 2.0; i+=step)
    {
        _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        glPushMatrix();
        glTranslatef((float)i, 0, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, field_h, 0);
        glEnd();
        glPopMatrix();
    }
    for (float i = 0.0; i <= (float)field_h*1.0; i+=step)
    {
        _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        glPushMatrix();
        glTranslatef(-field_w/2.0, (float)i, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(field_w, 0, 0);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();

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

void ProjectVisualizator::drawAxis()
{

}

void ProjectVisualizator::drawCustomObjects()
{

}

