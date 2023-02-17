#ifndef PROJECTVISUALIZATOR_H
#define PROJECTVISUALIZATOR_H

#include <QObject>


namespace serialMan
{



namespace gl
{

QT_FORWARD_DECLARE_CLASS(glVisualizatorWidget);

struct VisualizationParams
{
private:

    float _step{10};
    int _fieldWidth{500}, _fieldHeight{500};
    int _jointResolution{16};
    float _jointSizeKoef{10};
    float _textSize{5};

public:

    void setFieldWidth(const int);
    int fieldWidth() const;

    void setFieldHeight(const int);
    int fieldHeight() const;

    void setFieldStep(const float);
    float fieldStep() const;

    void setJointResolution(const int);
    int jointResolution() const;

    void setJointSizeKoef(const float);
    float jointSizeKoef() const;

    void setTestSize(const float);
    float testSize() const;

};

class ProjectVisualizator : public QObject
{
    Q_OBJECT

public:

    ProjectVisualizator() = default;

    void visualizate(serialMan::gl::glVisualizatorWidget*);
    const VisualizationParams& visualizationParams() const;

    void setVisualizationParams(const VisualizationParams&);

public slots:



signals:

    void onUpdateNeeding();

private:

    void drawField(VisualizationParams&);
    void drawAxis();
    void drawManipulator(VisualizationParams&);

    void drawRotationJoint(VisualizationParams&);
    void drawLinearJoint(VisualizationParams&);

    void drawCustomObjects();


private:

    VisualizationParams _params;

    glVisualizatorWidget* _currentContext;

};

}

}
#endif // PROJECTVISUALIZATOR_H
