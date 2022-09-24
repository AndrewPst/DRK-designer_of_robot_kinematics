#ifndef PROJECTSMANAGER_H
#define PROJECTSMANAGER_H

#include "projectCore/projectType.h"

//#include "projectGenerator.h"
#include "projectMetadata.h"

#include <QObject>
#include <QString>
#include <QMap>


QT_FORWARD_DECLARE_CLASS(BaseProjectController);


//This class manages projects (Amazing!)
class ProjectsManager : public QObject
{
    Q_OBJECT
private:

    //Singleton
    ProjectsManager();
    ProjectsManager(const ProjectsManager&) = delete;
    ProjectsManager& operator=(const ProjectsManager&) = delete;

    static QVector<ProjectMetadata> _projectsContollers;

public:
    //get instance of class
    static ProjectsManager& getInstance();

    BaseProjectController* getOpenedProject() const;

    void createNewProject(const ProjectMetadata&, const QString&);
    void closeProject();
    void saveProject();
    void saveProjectByPath(const QString&);

    //Map of project types and its names
    const QVector<ProjectMetadata>& getAvailablesControllers() const;

signals:

    void onProjectOpened(BaseProjectController* const);
    void onProjectClosed(BaseProjectController* const);


private:

    BaseProjectController* _openedProject;

};

extern ProjectsManager& projectsManager;

#endif // PROJECTSMANAGER_H
