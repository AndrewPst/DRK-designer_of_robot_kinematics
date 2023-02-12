#include "../openGL/projectvisualizator.h"
#include "../openGL/glvisualizatorwidget.h"
#include "../serial6dofmanipulator.h"
//#include "../logic/models/Effector_t.h"
#include "../logic/manipulatorcontroller.h"
#include "projectCore/projectsmanager.h"
#include "../logic/models/unitsConverter.h"

#include <QGLWidget>
#include <gl/glu.h>
#include <gl/glcorearb.h>
#include <gl/glaux.h>
#include <gl/gl.h>


using namespace serialMan;

void VisualizationParams::setFieldWidth(const int v)
{
    _fieldWidth = v;
}

int VisualizationParams::fieldWidth() const
{
    return _fieldWidth;
}

void VisualizationParams::setFieldHeight(const int v)
{
    _fieldHeight = v;
}

int VisualizationParams::fieldHeight() const
{
    return _fieldHeight;
}

void VisualizationParams::setFieldStep(const float v)
{
    _step = v;
}

float VisualizationParams::fieldStep() const
{
    return _step;
}

void VisualizationParams::setJointResolution(const int v)
{
    _jointResolution = v;
}

int VisualizationParams::jointResolution() const
{
    return _jointResolution;
}

void VisualizationParams::setJointSizeKoef(const float v)
{
    _jointSizeKoef = v;
}

float VisualizationParams::jointSizeKoef() const
{
    return _jointSizeKoef;
}

void VisualizationParams::setTestSize(const float v)
{
    _textSize = v;
}

float VisualizationParams::testSize() const
{
    return _textSize;
}



void serialMan::ProjectVisualizator::visualizate(serialMan::glVisualizatorWidget* glv)
{
    glv->makeCurrent();
    _currentContext = glv;
    VisualizationParams curParams = _params;
    drawField(curParams);
    drawManipulator(curParams);
    //    drawAxis();
    drawCustomObjects();
}

void serialMan::ProjectVisualizator::drawField(VisualizationParams& curParams)
{
    //curParams.fieldStep() =  ((int)_currentContext->getDistance()) / 10;


    float angle_pos_S = (cos(_currentContext->getAngleY()) < 0 ? 1 : -1);
    float angle_pos_C = (sin(_currentContext->getAngleY()) > 0 ? 1 : -1);

    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPointSize(curParams.testSize() * curParams.fieldStep() / (_currentContext->getDistance() * 0.01) + 1);
    font.setFamily("Arial");
    font.setHintingPreference(QFont::HintingPreference::PreferNoHinting);
    glLineWidth(2);

    QColor lightColor = QColor({200, 200, 200});
    QColor darkColor = QColor({160, 160, 160});
    QColor textColor = QColor({80, 80, 80});

    //double _curDist = _currentContext->getDistance();
    //            + sqrt(_currentContext->getCameraX() * _currentContext->getCameraX()
    //                   + _currentContext->getCameraY() * _currentContext->getCameraY()
    //                   + _currentContext->getCameraZ() * _currentContext->getCameraZ());

    glPushMatrix();
    for (int i = -curParams.fieldWidth() / curParams.fieldStep() / 2.0; i <= curParams.fieldWidth() / curParams.fieldStep() / 2.0; i+=1)
    {
        if (i == 0)
            glColor3f(0, 1, 0);
        else
            _currentContext->qglColor(i % 2 == 0 ? darkColor : lightColor );
        float pos = i * curParams.fieldStep();
        glPushMatrix();
        glTranslatef((float)pos, -curParams.fieldHeight() / 2.f, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, curParams.fieldHeight(), 0);
        glEnd();
        _currentContext->qglColor(textColor);
        //Render values and calculate text width for projection in center
        QString text = QString("%1").arg(pos);

        float tx = -(text.length()/2.0*(font.pointSize()*0.005)) * curParams.fieldStep();
        float ty = (-0.5 + (angle_pos_S > 0 ? curParams.fieldHeight() + 1 : 0));
        float tz = -0.5 * curParams.fieldStep();

        //Depth buffer not working with function renderText, so we dont drawing text, if its distance < then camera distance
        //float dist = sqrt(pow(i+tx, 2) + pow(-curParams.fieldHeight() / 2.f * curParams.fieldStep(), 2) + pow(_currentContext->getCameraZ() + tz, 2));
        //if(dist < _curDist || _currentContext->getProjectionMode() == ProjectionMode_t::PR_ORTHOGONAL)
        _currentContext->renderText(tx, ty, tz, text, font);
        glPopMatrix();
    }
    for (int i = -curParams.fieldHeight() / curParams.fieldStep() / 2.0; i <= curParams.fieldHeight() / curParams.fieldStep() / 2.0; i+=1)
    {

        if (i == 0)
            glColor3f(1, 0, 0);
        else
            _currentContext->qglColor(fmod(i, 2) == 0.0 ? darkColor : lightColor );
        glPushMatrix();
        float pos = i * curParams.fieldStep();
        glTranslatef(-curParams.fieldWidth() / 2.f, (float)pos, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(curParams.fieldWidth(), 0, 0);
        glEnd();

        _currentContext->qglColor(textColor);
        QString text = QString("%1").arg(pos);

        float tx = (-0.5 + (angle_pos_C < 0 ? curParams.fieldWidth() + 1 : 0));
        float ty = -(text.length()/2.0*((font.pointSize()-1)*0.005)) * curParams.fieldStep();
        float tz = -0.5 * curParams.fieldStep();


        //Depth buffer not working with function renderText, so we dont drawing text, if its distance < then camera distance
        //float dist = sqrt(pow(i+tx, 2) + pow(-curParams.fieldHeight() / 2.f * curParams.fieldStep(), 2) + pow(_currentContext->getCameraZ() + tz, 2));
        //if(dist < _curDist || _currentContext->getProjectionMode() == ProjectionMode_t::PR_ORTHOGONAL)
        _currentContext->renderText(tx, ty, tz, text, font);

        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glTranslatef(0, 0, curParams.fieldWidth() / 2.0 * angle_pos_C);
    for (int i = 0.0; i <= curParams.fieldWidth() / curParams.fieldStep(); i+=1)
    {

        _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        float pos = i * curParams.fieldStep();
        glPushMatrix();
        glTranslatef((float)-pos, -curParams.fieldHeight() / 2.f, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, curParams.fieldHeight(), 0);
        glEnd();
        //Render values and calculate text width for projection in center
        _currentContext->qglColor(textColor);
        QString text = QString("%1").arg(pos);

        _currentContext->renderText(0.1 * curParams.fieldStep(),
                                    (-0.5 + (angle_pos_S > 0 ? curParams.fieldHeight() + 1 : 0)),
                                    0.5 * angle_pos_C * curParams.fieldStep(),
                                    text, font);
        glPopMatrix();
    }
    for (int i = -curParams.fieldHeight() / curParams.fieldStep() / 2.0; i <= curParams.fieldHeight() / curParams.fieldStep() / 2.0; i+=1)
    {
        if (i == 0)
            glColor3f(0, 0, 1);
        else
            _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        float pos = i * curParams.fieldStep();
        glPushMatrix();
        glTranslatef(-curParams.fieldWidth(), (float)pos, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(curParams.fieldWidth(), 0, 0);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0, curParams.fieldHeight() / 2.0 * angle_pos_S);
    for (int i = - curParams.fieldWidth()/ curParams.fieldStep() / 2.0; i <= curParams.fieldWidth() / curParams.fieldStep() / 2.0; i+=1)
    {
        if (i == 0)
            glColor3f(0, 0, 1);
        else
            _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        float pos = i * curParams.fieldStep();
        glPushMatrix();
        glTranslatef((float)pos, 0, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, curParams.fieldWidth(), 0);
        glEnd();
        glPopMatrix();
    }
    for (int i = 0; i <=curParams.fieldWidth() / curParams.fieldStep(); i+=1)
    {
        _currentContext->qglColor(fmod(i, 2) == 0 ? darkColor : lightColor );
        float pos = i * curParams.fieldStep();
        glPushMatrix();
        glTranslatef(-curParams.fieldWidth()/2.0, (float)pos, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(curParams.fieldWidth(), 0, 0);
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


void drawCube(GLenum mode)
{
    glBegin(mode);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(0, 1, 0);
    glEnd();

    //Up
    glBegin(mode);
    glVertex3f(0, 0, 1);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(0, 1, 1);
    glEnd();

    glBegin(mode);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glVertex3f(0, 1, 1);
    glVertex3f(0, 1, 0);
    glEnd();

    glBegin(mode);
    glVertex3f(1, 0, 0);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(1, 1, 0);
    glEnd();

    glBegin(mode);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(1, 0, 1);
    glVertex3f(0, 0, 1);
    glEnd();

    glBegin(mode);
    glVertex3f(0, 1, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(1, 1, 1);
    glVertex3f(0, 1, 1);
    glEnd();
}

void serialMan::ProjectVisualizator::drawManipulator(VisualizationParams& curParams)
{
    auto& mc = ((Serial6DofManipulator*)projectsManager.getOpenedProject())->getManipulatorController();
    auto& joints = mc.getJoints();
    auto& dh = mc.getDHTable();
    glPushMatrix();
    for(int i = 0; i < ManipulatorController::DEFAULT_DOF; i++) //-1 - ignore frame between J6 and effector
    {
        if(i == 0)
            drawRotationJoint(curParams);

        glLineWidth(5);
        glColor3f(1, 0, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, dh.d[i]);
        glEnd();

        glTranslatef(0, 0, dh.d[i]);
        glRotatef((radToDeg(dh.theta[i]) + radToDeg(joints[i]->getValue())), 0, 0, 1);
        glRotatef(radToDeg(dh.alfa[i]), 1, 0, 0);

        glColor3f(0, 1, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(dh.r[i], 0, 0);
        glEnd();

        glTranslatef(dh.r[i], 0, 0);

        if(i == ManipulatorController::DEFAULT_DOF - 1)
        {
            //            glPointSize(10);
            //            glColor3f(0, 0, 1);
            //            glBegin(GL_POINTS);
            //            glVertex3f(0, 0, 0);
            //            glEnd();
            GLUquadricObj *q = gluNewQuadric();
            glColor3f(0, 0, 1);
            gluSphere(q, curParams.jointSizeKoef()/4,curParams.jointResolution(),curParams.jointResolution());
        }
        else
        {
            drawRotationJoint(curParams);
        }
    }
    glPopMatrix();

    glPushMatrix();
    for(int i = 0; i < ManipulatorController::DEFAULT_DOF; i++) //-1 - ignore frame between J6 and effector
    {
        if(i == 0)
        {
            glPushMatrix();
            glScalef(2*curParams.jointSizeKoef(), 2*curParams.jointSizeKoef(), 2*curParams.jointSizeKoef());
            glLineWidth(4);
            glDisable(GL_DEPTH_TEST);
            drawAxis();
            glEnable(GL_DEPTH_TEST);
            glPopMatrix();
        }
        glTranslatef(0, 0, dh.d[i]);
        glRotatef((radToDeg(dh.theta[i]) + radToDeg(joints[i]->getValue())), 0, 0, 1);
        glRotatef(radToDeg(dh.alfa[i]), 1, 0, 0);
        glTranslatef(dh.r[i], 0, 0);

        glPushMatrix();
        if(abs(dh.r[i]) <=curParams.jointSizeKoef()*1.5 && abs(dh.d[i]) <=curParams.jointSizeKoef()*1.5)
        {
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1,0XF0F0);

            glColor3f(0, 0, 0);
            glBegin(GL_LINES);
            glVertex3f(0, 0, 0);
            glVertex3f(0, 2 *curParams.jointSizeKoef()* 1.5, 0);
            glEnd();

            glTranslatef(0, 2 *curParams.jointSizeKoef()* 1.5, 0);
            glDisable(GL_LINE_STIPPLE);
        }
        glScalef(2*curParams.jointSizeKoef(), 2*curParams.jointSizeKoef(), 2*curParams.jointSizeKoef());
        glDisable(GL_DEPTH_TEST);
        drawAxis();
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
    }
    glPopMatrix();

}

void serialMan::ProjectVisualizator::drawRotationJoint(VisualizationParams& curParams)
{
    glPushMatrix();
    glTranslatef(0, 0, -1.f*curParams.jointSizeKoef());
    GLUquadricObj *q = gluNewQuadric();
    glLineWidth(1);
    gluQuadricDrawStyle(q, GLU_LINE );
    glColor3f(0, 0, 0);
    gluCylinder(q, 0.5*curParams.jointSizeKoef(),
                0.5*curParams.jointSizeKoef(),
                2.f*curParams.jointSizeKoef(),
                curParams.jointResolution(), 1);

    gluQuadricDrawStyle(q, GLU_FILL );
    glColor3f(0.8, 0.4, 0);
    gluCylinder(q,
                0.5*curParams.jointSizeKoef(),
                0.5*curParams.jointSizeKoef(),
                2.f*curParams.jointSizeKoef(),
                curParams.jointResolution(), 1);

    gluDisk(q, 0, 0.5*curParams.jointSizeKoef(),curParams.jointResolution(), 1);
    glTranslatef(0, 0, 2*curParams.jointSizeKoef());
    gluDisk(q, 0, 0.5*curParams.jointSizeKoef(),curParams.jointResolution(), 1);

    gluDeleteQuadric(q);
    glPopMatrix();

}

void serialMan::ProjectVisualizator::drawLinearJoint(VisualizationParams& curParams)
{

    const static int modes[] {GL_QUADS, GL_LINE_LOOP};
    const static QColor colors[] {QColor(qRgb(200, 100, 0)), Qt::black};

    glPushMatrix();
    glLineWidth(2);
    glScalef(curParams.jointSizeKoef(),curParams.jointSizeKoef(),curParams.jointSizeKoef());
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

const VisualizationParams& serialMan::ProjectVisualizator::visualizationParams() const
{
    return this->_params;
}

void serialMan::ProjectVisualizator::setVisualizationParams(const VisualizationParams& p)
{
    _params = p;
}


void serialMan::ProjectVisualizator::drawCustomObjects()
{

}

