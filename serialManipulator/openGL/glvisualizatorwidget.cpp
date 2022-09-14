#include "../openGL/glvisualizatorwidget.h"

#include "../openGL/projectvisualizator.h"

#include <gl/gl.h>
#include <gl/glu.h>

#include <QApplication>
#include <QDesktopWidget>
#include <QContextMenuEvent>

glVisualizatorWidget::glVisualizatorWidget(QWidget* parent) : QGLWidget(parent)
{
    //Get the display resolution to display the model correctly
    _displayWidth = QApplication::desktop()->geometry().width();
    _displayHeight = QApplication::desktop()->geometry().height();

    //installEventFilter(this);
    //Pass the context menu event to the parent widget
    setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);

}

//bool glVisualizatorWidget::eventFilter( QObject *o, QEvent *e )
//{
//    if(QGLWidget::eventFilter(o, e) == TRUE)
//        return TRUE;
//    if ( e->type() == QEvent::KeyPress  || e->type() == QEvent::KeyRelease || e->type() == QEvent::FocusOut) {
//        return TRUE; // eat event
//    } else {
//        // standard event processing
//        return FALSE;
//    }
//}

//------------------GL methods--------------------

void glVisualizatorWidget::initializeGL() {
    qglClearColor(Qt::white); // заполняем экран белым цветом
    glEnable(GL_DEPTH_TEST); // задаем глубину проверки пикселей
    glEnable(GL_MULTISAMPLE); //режим antialiazing
    glShadeModel(GL_FLAT); // убираем режим сглаживания цветов
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // фигуры будут закрашены с обеих сторон

}

//изменение размера экрана
void glVisualizatorWidget::resizeGL(int w, int h) {
    if (h == 0)
        h = 1;
    _cWidth = w;
    _cHeight = h;
    glMatrixMode(GL_PROJECTION); // используем матрицу проекции
    glLoadIdentity(); // обнуляем матрицу
    //glViewport(0, 0, w, h);
    glViewport(-(_displayWidth-w)/2.0, -(_displayHeight-h)/2.0, _displayWidth, _displayHeight);
    setupProjection();
    glMatrixMode(GL_MODELVIEW);// вернуться к матрице проекции
}

void glVisualizatorWidget::paintGL() // рисование
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очистка экрана
    //setupProjection();
    glMatrixMode(GL_MODELVIEW); // задаем модельно-видовую матрицу
    glLoadIdentity(); // загрузка единичную матрицу
    setupCamera();

    //Конвертирование системы координат из xzy в xyz
    glRotatef(-90, 1, 0, 0);
    glTranslatef(0, 0, -_cameraZPoint);

    projectVisualizator.visualizate(this);

}


//-------------Private methods--------------

//высчет расположения камеры по углам
void glVisualizatorWidget::setupCamera() {

    //раскрытое перемножение матрицы поворота
    double _x = 0, _z = _distance, _y = 0;
    double _nx, _ny, _nz;
    _ny = _y * cos(_angleX) + _z * sin(_angleX);
    _nz = -_y * sin(_angleX) + _z * cos(_angleX);

    _nx = _x * cos(_angleY) - _nz * sin(_angleY);
    _nz = _x * sin(_angleY) + _nz * cos(_angleY);

    // установка камеры
    gluLookAt(_nx, _ny, _nz, 0.0f, 0.0f, 0.0f, 0.0f, cos(_angleX), 0.0f);
}

void glVisualizatorWidget::setupProjection()
{
    glMatrixMode(GL_PROJECTION); // используем матрицу проекции
    glLoadIdentity(); // обнуляем матрицу
    //float ratio = _cWidth / _cHeight;
    float ratio = (float)_displayWidth/(float)_displayHeight;
    if(_projectionMode == ProjectionMode_t::PR_ORTHOGONAL)
    {
        glOrtho(-ratio, ratio, -1, 1, -FAR_PLANE, FAR_PLANE);
        float skaleFoef = 1.0 / (_distance * 60.0 / 180.0);
        glScalef(skaleFoef, skaleFoef, skaleFoef);
    }
    else if(_projectionMode == ProjectionMode_t::PR_PERSPECTIVE)
    {
        gluPerspective(60, ratio, NEAR_PLANE, FAR_PLANE);
    }
    glMatrixMode(GL_MODELVIEW);// вернуться к матрице проекции

}

//--------------Getters--------------

ProjectionMode_t glVisualizatorWidget::getProjectionMode()
{
    return _projectionMode;
}

float glVisualizatorWidget::getDistance()
{
    return _distance;
}

float glVisualizatorWidget::getAngleX()
{
    return _angleX;
}

float glVisualizatorWidget::getAngleY()
{
    return _angleY;
}


//-------------Setters---------------

void glVisualizatorWidget::setProjectionMode(const ProjectionMode_t& mode)
{
    _projectionMode = mode;
    setupProjection();
    updateGL();
}

void glVisualizatorWidget::setDistance(float d)
{
    _distance = d;
    updateGL();
}

void glVisualizatorWidget::setAngleX(float a)
{
    _angleX = a;
    updateGL();
}

void glVisualizatorWidget::setAngleY(float a)
{
    _angleY = a;
    updateGL();
}


//----------------Events---------------

// нажатие на клавишу мыши
void glVisualizatorWidget::mousePressEvent(QMouseEvent *pe)
{
    makeCurrent();

    if (pe->button() == Qt::MouseButton::MiddleButton) {
        _mauseXOrigin = pe->x();
        _mauseYOrigin = pe->y();
        _isMiddlePressed = true;
    } else if(pe->button() == Qt::MouseButton::LeftButton){
        _isRightPressed = true;
        _mauseXOrigin = pe->x();
        _mauseYOrigin = pe->y();
    }
}


// отжатие клавиши мыши
void glVisualizatorWidget::mouseReleaseEvent(QMouseEvent *pe)
{
    makeCurrent();

    if (pe->button() == Qt::MouseButton::MiddleButton) {
        _isMiddlePressed = false;
    } else if(pe->button() == Qt::MouseButton::LeftButton){
        _isRightPressed = false;
    }
}

// перемещение мыши
void glVisualizatorWidget::mouseMoveEvent(QMouseEvent *pe)
{
    makeCurrent();

    if (_isMiddlePressed) {
        // update deltaAngle
        _angleY += (pe->x() - _mauseXOrigin) * 0.005f;
        _mauseXOrigin = pe->x();

        _angleX += (pe->y() - _mauseYOrigin) * 0.005f;
        _mauseYOrigin = pe->y();

        //        if (angle_x > M_PI / 2 - 0.01)
        //            angle_x = M_PI / 2 - 0.01;
        //        if (angle_x < -M_PI / 2 + 0.01)
        //            angle_x = -M_PI / 2 + 0.01;

        updateGL();
    } else if(_isRightPressed){
        _cameraZPoint += (pe->y() - _mauseYOrigin) * (0.0005 *_distance);
        _mauseYOrigin = pe->y();
        _mauseXOrigin = pe->x();
        updateGL();
    }
}

void glVisualizatorWidget::wheelEvent(QWheelEvent *pe)
{
    makeCurrent();
    if (pe->angleDelta().y() * MOUSE_WHEEL_KOEF > 0)
        _distance = qMin<float>(MAX_DISTANCE, _distance * 1.1);
    else if (pe->angleDelta().y() * MOUSE_WHEEL_KOEF < 0)
        _distance = qMax<float>(MIN_DISTANCE, _distance / 1.1);
    setupProjection();
    updateGL();
}


