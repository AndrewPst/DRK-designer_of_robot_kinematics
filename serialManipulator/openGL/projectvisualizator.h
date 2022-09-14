#ifndef PROJECTVISUALIZATOR_H
#define PROJECTVISUALIZATOR_H

#include <QObject>
#include "../openGL/glvisualizatorwidget.h"

namespace serialMan
{

class ProjectVisualizator : public ::QObject
{

    Q_OBJECT
private:
    ProjectVisualizator() = default;
    ProjectVisualizator(const ProjectVisualizator&) = delete;
    ProjectVisualizator& operator=(const ProjectVisualizator&) =  delete;

public:

    static ProjectVisualizator& getInstance();

    void visualizate(serialMan::glVisualizatorWidget*);

public slots:



signals:

    void onUpdateNeeding();

private:

    void drawField();
    void drawAxis();

    void drawCustomObjects();

private:

    glVisualizatorWidget* _currentContext;

};

extern ProjectVisualizator& projectVisualizator;


}
#endif // PROJECTVISUALIZATOR_H
