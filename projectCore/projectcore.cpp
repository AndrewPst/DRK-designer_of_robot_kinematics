#include "projectCore/projectcore.h"
#include "projectCore/projectStructure.h"

#include <QListIterator>

ProjectController::ProjectController(ProjectStructure* structure)
    : QObject(), _structure(structure)
{
}

void ProjectController::addJoint(Joint_t* joint)
{
    if(_structure->manipulator.joints.contains(joint))
        return;

    _structure->manipulator.joints.append(joint);
    emit onNewJointAdded(joint);
}

bool ProjectController::getJoint(int i, Joint_t** out)
{
    *out = nullptr;
    if(i >= _structure->manipulator.joints.size() || i < 0)
        return false;
    *out = _structure->manipulator.joints[i];
    return false;
}

QList<Joint_t*>::ConstIterator ProjectController::jointsBegin()
{
    return _structure->manipulator.joints.constBegin();
}

QList<Joint_t*>::ConstIterator ProjectController::jointsEnd()
{
    return _structure->manipulator.joints.constEnd();
}

Joint_t* ProjectController::getJoint(int i, bool* result)
{
    Joint_t* vm = nullptr;
    bool r = getJoint(i, &vm);
    if(result)
        *result = r;
    return vm;
}


bool ProjectController::removeJoint(Joint_t* joint)
{
    if(_structure->manipulator.joints.contains(joint) == false)
        return false;
    emit onJointRemoved(joint);
    return _structure->manipulator.joints.removeOne(joint);
}

bool ProjectController::removeJoint(int id)
{
    if(id >= _structure->manipulator.joints.size() || id < 0)
        return false;
    emit onJointRemoved(_structure->manipulator.joints.at(id));
    _structure->manipulator.joints.removeAt(id);
    return false;
}

QString ProjectController::getProjectName()
{
    return _structure->projectName;
}

void ProjectController::setProjectName(QString& str)
{
    _structure->projectName = str;
}

//-----------Project Controller----------

ProjectsManager::ProjectsManager() : QObject()
{
    //_openedProject = new ProjectController(s);ProjectController
}

void ProjectsManager::setOpenedProject(ProjectController* pc)
{
    if(_openedProject)
    {
        emit onProjectClosed(_openedProject);
    }
    _openedProject = pc;
    emit onProjectOpened(pc);

}

ProjectController* ProjectsManager::getOpenedProject()
{
    return _openedProject;
}

ProjectsManager& ProjectsManager::getInstance()
{
    static ProjectsManager pm;
    return pm;
}

ProjectsManager& projectManager = ProjectsManager::getInstance();


