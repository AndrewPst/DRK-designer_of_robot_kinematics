#include "dhparameterscalculator.h"
#include "manipulatorcontroller.h"

#include <QGenericMatrix>
#include <QTransform>
#include <QMatrix4x4>
#include <QQuaternion>
#include <cmath>

using namespace serialMan;

dhParametersCalculator::dhParametersCalculator(SerialManipulatorProject* pr)
    : QObject(), _proj(pr)
{

}

dhParametersCalculator::~dhParametersCalculator()
{

}

void dhParametersCalculator::correctRotateJoint(Joint_t* , Joint_t* )
{

}

void dhParametersCalculator::calculate()
{
    auto j = _proj->getManipulatorController()->getJoints();

    for(int i = 0; i < j.size(); i++)
    {
        //get Joint[N]  or effector
        Joint_t* secondJ;
        if(i == j.size()-1)
            secondJ = _proj->getManipulatorController()->getEffector();
        else
            secondJ = j[i+1];

        QVector3D newRot = secondJ->getRotation();
        float newZRot = 0;

        bool isCorrect = false;
        int iterCount = 0;
        while(isCorrect == false)
        {
            if(iterCount >= 4)
                break;
            newRot.setZ(newRot.z() + newZRot);
            QVector3D vecX;
            QVector3D vecY;
            QVector3D vecZ;
            QQuaternion::fromEulerAngles(newRot).getAxes(&vecX, &vecY, &vecZ);
            QVector3D z1{0, 0, 1};
            float i = z1.x() * vecX.x() + z1.y() * vecX.y() + z1.z() * vecX.z();
            float angle = std::acos(i / (z1.length() * vecX.length()));
            if(angle != M_PI_2)
                newZRot += 90;
            else
                isCorrect = true;
            iterCount++;
        }
        secondJ->setRotation(newRot);
        if(i+2 <= j.size())
        {
            Joint_t* nextJ = NULL;
            if(i + 2 == j.size())
                nextJ = _proj->getManipulatorController()->getEffector();
            else
                nextJ = j[i+2];
            QVector3D lpos = nextJ->getPosition();
            lpos.setX(lpos.x() * cos(newZRot * M_PI / 180.0));
            lpos.setY(lpos.x() * sin(newZRot * M_PI / 180.0));
            nextJ->setPosition(lpos);
            QVector3D lrot = nextJ->getRotation();
            lrot.setZ(lrot.z() - newZRot);
        }
        //Check, that X axis perpendiculare

    }

}

