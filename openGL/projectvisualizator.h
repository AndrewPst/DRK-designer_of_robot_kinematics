#ifndef PROJECTVISUALIZATOR_H
#define PROJECTVISUALIZATOR_H

#include <QObject>

QT_FORWARD_DECLARE_CLASS(glVisualizatorWidget);
QT_FORWARD_DECLARE_CLASS(BaseGLObject);

class ProjectVisualizator : public QObject
{

    Q_OBJECT
private:
    ProjectVisualizator() = default;
    ProjectVisualizator(const ProjectVisualizator&) = delete;
    ProjectVisualizator& operator=(const ProjectVisualizator&) =  delete;

public:

    static ProjectVisualizator& getInstance();

    void visualizate(glVisualizatorWidget*);

public slots:



signals:

    void onUpdateNeeding();

private:

    void drawField();
    void drawAxis();

    void drawCustomObjects();

private:

    QList<BaseGLObject*> _glObjects;
    glVisualizatorWidget* _currentContext;

};

extern ProjectVisualizator& projectVisualizator;

#endif // PROJECTVISUALIZATOR_H
