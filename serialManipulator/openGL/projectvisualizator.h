#ifndef PROJECTVISUALIZATOR_H
#define PROJECTVISUALIZATOR_H

#include <QObject>


namespace serialMan
{

QT_FORWARD_DECLARE_CLASS(glVisualizatorWidget);

class ProjectVisualizator : public QObject
{
    Q_OBJECT

public:

    ProjectVisualizator() = default;

    void visualizate(serialMan::glVisualizatorWidget*);

public slots:



signals:

    void onUpdateNeeding();

private:

    void drawField();
    void drawAxis();
    void drawManipulator();

    void drawRotationJoint();
    void drawLinearJoint();

    void drawCustomObjects();

private:

    glVisualizatorWidget* _currentContext;

    float _fieldkKoef = 10;

};

}
#endif // PROJECTVISUALIZATOR_H
