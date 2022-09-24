#ifndef GLVISUALIZATORWIDGET_H
#define GLVISUALIZATORWIDGET_H

#include <QGLWidget>
#include <QtMath>
#include <QtGui>


namespace serialMan
{

QT_FORWARD_DECLARE_CLASS(ProjectVisualizator);

enum class ProjectionMode_t : char
{
    UNKNOWN = 0,
    PR_PERSPECTIVE,
    PR_ORTHOGONAL
};

class glVisualizatorWidget : public QGLWidget
{
    Q_OBJECT

public:
    glVisualizatorWidget(ProjectVisualizator* visualizator, QWidget* parent = nullptr);


    //---Getters---
    ProjectionMode_t getProjectionMode();
    float getDistance();
    float getAngleX();
    float getAngleY();

    //---Setters---

    void setProjectionMode(const ProjectionMode_t&);
    void setDistance(float);
    void setAngleX(float);
    void setAngleY(float);

    //---GL methods---
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:

    void setupCamera();
    void setupProjection();

protected:
    //---Events---
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent *) override;

    //bool eventFilter( QObject *o, QEvent *e ) override;

private:

    ProjectionMode_t _projectionMode = ProjectionMode_t::PR_ORTHOGONAL;

    ProjectVisualizator* _visualizator;

    float _distance = 60;
    float _angleX = M_PI/5.0, _angleY = 0;

    //current viewport width and height
    float _cWidth, _cHeight;

    //last mause position
    int _mauseXOrigin = -1, _mauseYOrigin = -1;

    bool _isRightPressed = false;
    bool _isMiddlePressed = false;

    float _cameraZPoint = 4;
    //Split places
    const float NEAR_PLANE = 0.01f;
    const float FAR_PLANE = 1000.0;

    //Max and min distance
    const float MIN_DISTANCE = 0.1;
    const float MAX_DISTANCE = 500;

    //Mause wheel rotation koef
    const int MOUSE_WHEEL_KOEF = -1;

    //Display resolution
    int _displayWidth = 1920;
    int _displayHeight = 1080;
};

}
#endif // GLVISUALIZATORWIDGET_H
