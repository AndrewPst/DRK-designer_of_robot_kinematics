#ifndef PROJECTCORE_H
#define PROJECTCORE_H

#include <QList>
#include <QObject>

struct Joint_t;
struct ProjectStructure;

class ProjectController : public QObject
{
    Q_OBJECT
public:

    explicit ProjectController(ProjectStructure*);

    void addJoint(Joint_t*);

    bool getJoint(int, Joint_t**);
    Joint_t* getJoint(int, bool* = nullptr);

    bool removeJoint(Joint_t*);
    bool removeJoint(int id);

    QString getProjectName();
    void setProjectName(QString&);

    QString getProjectVersion();
    void setProjectVersion(QString&);

    QList<Joint_t*>::ConstIterator jointsBegin();
    QList<Joint_t*>::ConstIterator jointsEnd();

signals:

    void onNewJointAdded(Joint_t*);
    void onJointRemoved(Joint_t*);
    void onRenamed(QString);
    void onVerionUpdated(QString);

private:
    ProjectStructure *_structure;
};

class ProjectsManager : public QObject
{
    Q_OBJECT
private:
    ProjectsManager();
    ProjectsManager(const ProjectsManager&) = delete;
    ProjectsManager& operator=(const ProjectsManager&) = delete;
public:
    static ProjectsManager& getInstance();

    void setOpenedProject(ProjectController*);
    void closeOpenedProject();
    void saveOpenedProject(QString&);

    bool isProjectOpened();
    ProjectController* getOpenedProject();

signals:

    void onProjectOpened(ProjectController*);
    void onProjectClosed(ProjectController*);
    void onProjectSaved(ProjectController*);

private:

    ProjectController *_openedProject;
};

extern ProjectsManager& projectManager;

#endif // PROJECTCORE_H
