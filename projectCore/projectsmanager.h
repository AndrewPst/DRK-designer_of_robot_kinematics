#ifndef PROJECTSMANAGER_H
#define PROJECTSMANAGER_H

#include "projectCore/projectType.h"

#include <QObject>
#include <QString>
#include <QMap>



QT_FORWARD_DECLARE_CLASS(BaseProjectController);

class ProjectsManager : public QObject
{
    Q_OBJECT
private:
    ProjectsManager();
    ProjectsManager(const ProjectsManager&) = delete;
    ProjectsManager& operator=(const ProjectsManager&) = delete;

    QMap<ProjectType_t, QString> _projectsDictionary;

    void initAvailableControllers();

public:

    static ProjectsManager& getInstance();

    BaseProjectController* getOpenedProject() const;

    void createNewProject(const ProjectType_t);
    void closeProject();

    BaseProjectController* getProjectByType(const ProjectType_t);

    const QMap<ProjectType_t, QString>& getAvailablesControllers() const;

signals:

    void onProjectOpened(BaseProjectController* const);
    void onProjectClosed(BaseProjectController* const);


private:

    BaseProjectController* _openedProject;

};

extern ProjectsManager& projectsManager;

#endif // PROJECTSMANAGER_H
