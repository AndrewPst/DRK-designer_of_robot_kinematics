#include "../openGL/projectvisualizator.h"
#include "../openGL/glvisualizatorwidget.h"
#include "../serialManipulatorProject.h"
#include "projectCore/projectsmanager.h"
#include "../logic/manipulatorcontroller.h"

#include <QGLWidget>
#include <gl/glu.h>
#include <gl/glcorearb.h>
#include <gl/glaux.h>
#include <gl/gl.h>

using namespace serialMan;

void serialMan::ProjectVisualizator::visualizate(serialMan::glVisualizatorWidget* glv)
{
    glv->makeCurrent();
    _currentContext = glv;
    drawField();
    drawManipulator();
    //    drawAxis();
    drawCustomObjects();
}

void serialMan::ProjectVisualizator::drawField()
{

    const int field_w = 100 / _fieldkKoef, field_h = 100 / _fieldkKoef;

    float angle_pos_S = (cos(_currentContext->getAngleY()) < 0 ? 1 : -1);
    float angle_pos_C = (sin(_currentContext->getAngleY()) > 0 ? 1 : -1);

    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPointSize(3.0 * _fieldkKoef / (_currentContext->getDistance() * 0.01) + 1);
    font.setFamily("Arial");
    font.setHintingPreference(QFont::HintingPreference::PreferNoHinting);
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
            _currentContext->qglColor(i % 2 == 0 ? darkColor : lightColor );
        float pos = i * _fieldkKoef;
        glPushMatrix();
        glTranslatef((float)pos, -field_h / 2.f * _fieldkKoef, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, field_h * _fieldkKoef, 0);
        glEnd();
        _currentContext->qglColor(textColor);
        //Render values and calculate text width for projection in center
        QString text = QString("%1").arg(pos);

        float tx = -(text.length()/2.0*(font.pointSize()*0.005)) * _fieldkKoef;
        float ty = (-0.5 + (angle_pos_S > 0 ? field_h + 1 : 0)) * _fieldkKoef;
        float tz = -0.5 * _fieldkKoef;

        //Depth buffer not working with function renderText, so we dont drawing text, if its distance < then camera distance
        float dist = sqrt(pow(i+tx, 2) + pow(-field_h / 2.f * _fieldkKoef, 2) + pow(_currentContext->getCameraZ() + tz, 2));
        if(dist < _currentContext->getDistance() || _currentContext->getProjectionMode() == ProjectionMode_t::PR_ORTHOGONAL)
            _currentContext->renderText(tx, ty, tz, text, font);
        glPopMatrix();
    }
    for (int i = -field_h / 2.0; i <= field_h / 2.0; i+=1)
    {
        if (i == 0)
            glColor3f(1, 0, 0);
        else
            _currentContext->qglColor(fmod(i, 2) == 0.0 ? darkColor : lightColor );
        float pos = i * _fieldkKoef;
        glPushMatrix();
        glTranslatef(-field_w / 2.f * _fieldkKoef, (float)pos, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(field_w * _fieldkKoef, 0, 0);
        glEnd();
        _currentContext->qglColor(textColor);
        QString text = QString("%1").arg(pos);


        float tx = (-0.5 + (angle_pos_C < 0 ? field_h + 1 : 0)) * _fieldkKoef;
        float ty = -(text.length()/2.0*((font.pointSize()-1)*0.005)) * _fieldkKoef;
        float tz = -0.5 * _fieldkKoef;


        //Depth buffer not working with function renderText, so we dont drawing text, if its distance < then camera distance
        float dist = sqrt(pow(i+tx, 2) + pow(-field_h / 2.f * _fieldkKoef, 2) + pow(_currentContext->getCameraZ() + tz, 2));
        if(dist < _currentContext->getDistance() || _currentContext->getProjectionMode() == ProjectionMode_t::PR_ORTHOGONAL)
            _currentContext->renderText(tx, ty, tz, text, font);
        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glTranslatef(0, 0, field_w / 2.0 * angle_pos_C * _fieldkKoef);
    for (int i = 0.0; i <= field_w; i+=1)
    {

        _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        float pos = i * _fieldkKoef;
        glPushMatrix();
        glTranslatef((float)-pos, -field_h / 2.f * _fieldkKoef, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, field_h * _fieldkKoef, 0);
        glEnd();
        //Render values and calculate text width for projection in center
        _currentContext->qglColor(textColor);
        QString text = QString("%1").arg(pos);

        _currentContext->renderText(0.1 * _fieldkKoef,
                                    (-0.5 + (angle_pos_S > 0 ? field_h + 1 : 0))*_fieldkKoef,
                                    0.5 * angle_pos_C * _fieldkKoef,
                                    text, font);
        glPopMatrix();
    }
    for (int i = -field_h / 2.0; i <= field_h / 2.0; i+=1)
    {
        if (i == 0)
            glColor3f(0, 0, 1);
        else
            _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        float pos = i * _fieldkKoef;
        glPushMatrix();
        glTranslatef(-field_w * _fieldkKoef, (float)pos, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(field_w * _fieldkKoef, 0, 0);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0, field_w / 2.0 * angle_pos_S * _fieldkKoef);
    for (int i = - field_w / 2.0; i <= field_w / 2.0; i+=1)
    {
        if (i == 0)
            glColor3f(0, 0, 1);
        else
            _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        float pos = i * _fieldkKoef;
        glPushMatrix();
        glTranslatef((float)pos, 0, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, field_h * _fieldkKoef, 0);
        glEnd();
        glPopMatrix();
    }
    for (int i = 0.0; i <=field_h; i+=1)
    {
        _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        float pos = i * _fieldkKoef;
        glPushMatrix();
        glTranslatef(-field_w/2.0 * _fieldkKoef, (float)pos, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(field_w * _fieldkKoef, 0, 0);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();

}

//----------draw Axis------------
void serialMan::ProjectVisualizator::drawAxis()
{
    glPushMatrix();
    glLineWidth(2);
    glTranslatef(0, 0, 0);
    glScalef(10, 10, 10);
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


void drawCube()
{
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(0, 1, 0);
    //Up
    glVertex3f(0, 0, 1);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(0, 1, 1);

    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glVertex3f(0, 1, 1);
    glVertex3f(0, 1, 0);

    glVertex3f(1, 0, 0);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(1, 1, 0);

    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(1, 0, 1);
    glVertex3f(0, 0, 1);

    glVertex3f(0, 1, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(1, 1, 1);
    glVertex3f(0, 1, 1);
    glEnd();
}

void serialMan::ProjectVisualizator::drawManipulator()
{
    auto joints = ((SerialManipulatorProject*)projectsManager.getOpenedProject())->getManipulatorController()->getJoints();
    glPushMatrix();

    Q_FOREACH(auto j, joints)
    {

        glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(-0.25, -0.25, -0.25);
        glScalef(0.5, 0.5, j->getPosition().z()+0.5*(j->getPosition().z() > 0));
        drawCube();
        glPopMatrix();

        glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(-0.25, -0.25, j->getPosition().z()-0.25);
        glScalef(j->getPosition().x()+0.5*(j->getPosition().x() > 0), 0.5, 0.5);
        drawCube();
        glPopMatrix();

        glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(j->getPosition().x()-0.25, -0.25, j->getPosition().z()-0.25);
        glScalef(0.5, j->getPosition().y()+0.5*(j->getPosition().y() > 0), 0.5);
        drawCube();
        glPopMatrix();

        glTranslatef(j->getPosition().x(), j->getPosition().y(), j->getPosition().z());
        glRotatef(j->getRotation().x(), 1, 0, 0);
        glRotatef(j->getRotation().y(), 0, 1, 0);
        glRotatef(j->getRotation().z(), 0, 0, 1);

        drawAxis();

        if(j->getType() == JointType_t::JOINT_ROTATION)
        {
            drawRotationJoint();
            glRotatef(j->getValue(), 0 ,0, 1);
        }
        else if(j->getType() == JointType_t::JOINT_LINEAR)
        {
            drawLinearJoint();

            glPushMatrix();
            glColor3f(1, 0.8, 0);
            glTranslatef(-0.3, -0.3, -0.3);
            glScalef(0.6, 0.6, j->getValue()+0.6*(j->getValue() > 0));
            drawCube();
            glPopMatrix();

            glTranslatef(0, 0, j->getValue());
        }
    }
    glPopMatrix();
}

void serialMan::ProjectVisualizator::drawRotationJoint()
{
    //    glPointSize(20);
    //    glBegin(GL_POINTS);
    //    glColor3f(1, 0, 0);
    //    glVertex3f(0, 0, 0);
    //    glEnd();
    glPushMatrix();
    glTranslatef(0, 0, -1.5*_jointKoef);
    GLUquadricObj *q = gluNewQuadric();
    glLineWidth(1);
    gluQuadricDrawStyle(q, GLU_LINE );
    glColor3f(0, 0, 0);
    gluCylinder(q, 1.f*_jointKoef, 1.f*_jointKoef, 3.f*_jointKoef, _jointResolution, 1);

    gluQuadricDrawStyle(q, GLU_FILL );
    glColor3f(0.8, 0.8, 0);
    gluCylinder(q, 1.f*_jointKoef, 1.f*_jointKoef, 3.f*_jointKoef, _jointResolution, 1);
    gluDisk(q, 0, 1.0*_jointKoef, _jointResolution, 1);
    glTranslatef(0, 0, 3*_jointKoef);
    gluDisk(q, 0, 1.0*_jointKoef, _jointResolution, 1);

    gluDeleteQuadric(q);
    glPopMatrix();

}

void serialMan::ProjectVisualizator::drawLinearJoint()
{
    //    glPointSize(20);
    //    glBegin(GL_POINTS);
    //    glColor3f(0, 1, 0);
    //    glVertex3f(0, 0, 0);
    //    glEnd();

    const static int modes[] {GL_QUADS, GL_LINE_LOOP};
    const static QColor colors[] {Qt::darkGray, Qt::red};

    glPushMatrix();
    glLineWidth(2);
    glScalef(_jointKoef, _jointKoef, _jointKoef);
    glTranslatef(-0.5, -0.5, -0.5);
    for(size_t i  =0; i < sizeof(modes) / sizeof(modes[0]); i++)
    {
        _currentContext->qglColor(colors[i]);

        //Down
        glBegin(modes[i]);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(0, 1, 0);
        glEnd();

        //Up
        glBegin(modes[i]);
        glVertex3f(0, 0, 1);
        glVertex3f(1, 0, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(0, 1, 1);
        glEnd();

        glBegin(modes[i]);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1);
        glVertex3f(0, 1, 1);
        glVertex3f(0, 1, 0);
        glEnd();

        glBegin(modes[i]);
        glVertex3f(1, 0, 0);
        glVertex3f(1, 0, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(1, 1, 0);
        glEnd();

        glBegin(modes[i]);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glVertex3f(1, 0, 1);
        glVertex3f(0, 0, 1);
        glEnd();

        glBegin(modes[i]);
        glVertex3f(0, 1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, 1, 1);
        glVertex3f(0, 1, 1);
        glEnd();

        //Upper plane
        glBegin(modes[i]);
        glVertex3f(0, 0, 1.5);
        glVertex3f(1, 0, 1.5);
        glVertex3f(1, 1, 1.5);
        glVertex3f(0, 1, 1.5);
        glEnd();

    }
    glPopMatrix();
}

void serialMan::ProjectVisualizator::drawCustomObjects()
{

}

