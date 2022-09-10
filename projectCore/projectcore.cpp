#include "projectCore/projectcore.h"
#include "projectCore/projectStructure.h"

#include <QListIterator>

ProjectController::ProjectController(ProjectStructure* structure)
    : QObject(), _structure(structure)
{
}

Joint_t*  ProjectController::createJoint()
{
    int id = getFirstFreeId();
    if(id == -1)
        return nullptr;

    Joint_t* joint = new Joint_t();
    joint->setId(id);
    _structure->manipulator.joints[id] = joint;
    emit onNewJointAdded(joint);
    return joint;
}

Joint_t* ProjectController::getJoint(int i, bool* result)
{
    Joint_t* vm = nullptr;
    bool r = getJoint(i, &vm);
    if(result)
        *result = r;
    return vm;
}

bool ProjectController::changeJointId(int from, int to)
{
    if(from == to) return true;
    if(_structure->manipulator.joints.contains(to)) return false;
    Joint_t* j = _structure->manipulator.joints[from];
    _structure->manipulator.joints.remove(from);
    _structure->manipulator.joints[to] = j;
    j->setId(to);
    return true;
}

bool ProjectController::getJoint(int i, Joint_t** out)
{
    *out = nullptr;
    if(_structure->manipulator.joints.contains(i) == false)
        return false;
    *out = _structure->manipulator.joints[i];
    return true;
}

QMap<int, Joint_t*>::ConstIterator ProjectController::jointsBegin()
{
    return _structure->manipulator.joints.constBegin();
}

QMap<int, Joint_t*>::ConstIterator ProjectController::jointsEnd()
{
    return _structure->manipulator.joints.constEnd();
}

int ProjectController::getFirstFreeId()
{
    return getNextFreeId(0, +1);
}

int ProjectController::getNextFreeId(int cur, int dir)
{
    for(int i = cur+dir; i > 0 && i < 255; i+=dir)
    {
        if(_structure->manipulator.joints.contains(i) == false)
            return i;
    }
    return -1;
}

bool ProjectController::removeJoint(Joint_t* joint)
{
    if(!joint)
        return false;
    return removeJoint(joint->getId());
}

bool ProjectController::removeJoint(int id)
{
    Joint_t* joint = _structure->manipulator.joints[id];
    bool result = _structure->manipulator.joints.remove(id);
    if(result)
    {
        emit onJointRemoved(joint);
        delete joint;
    }
    return result;
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


