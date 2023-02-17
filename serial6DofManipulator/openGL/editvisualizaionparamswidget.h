#ifndef EDITVISUALIZAIONPARAMSWIDGET_H
#define EDITVISUALIZAIONPARAMSWIDGET_H

#include "../openGL/projectvisualizator.h"

#include <QObject>
#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>


class QVBoxLayout;


namespace serialMan
{

class ProjectVisualizator;

namespace gl
{


class EditVisualizaionParamsWidget : public QDialog
{
    Q_OBJECT

public:
    explicit EditVisualizaionParamsWidget(ProjectVisualizator&);

private slots:

    void resetValues();
    void confirmValues();

private:

    QVBoxLayout* _mainl;
    ProjectVisualizator* _vis;
    VisualizationParams _params;

    QSpinBox *spinFieldW;
    QSpinBox *spinFieldH;
    QDoubleSpinBox *spinTextSize;
    QDoubleSpinBox *spinStep;
};

}

}
#endif // EDITVISUALIZAIONPARAMSWIDGET_H
