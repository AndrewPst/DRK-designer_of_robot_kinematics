#ifndef SERIALMANIPULATORPROJECTCONTROLLER_H
#define SERIALMANIPULATORPROJECTCONTROLLER_H

#include "projectCore/baseprojectcontroller.h"




//Use the namespace to prevent conflicts
namespace serialMan {

QT_FORWARD_DECLARE_CLASS(ManipulatorController);
QT_FORWARD_DECLARE_CLASS(ProjectVisualizator);
QT_FORWARD_DECLARE_CLASS(dhParametersCalculator);

//Project controller for a serial manipulator with rotation and linear joints
class SerialManipulatorProject : public BaseProjectController
{
    Q_OBJECT

public:
    SerialManipulatorProject();
    ~SerialManipulatorProject();

    void init() override;

    //get controller of joints
    ManipulatorController* getManipulatorController() const;
    ProjectVisualizator* getVisualizator() const;
    dhParametersCalculator* getDhCalculator() const;

private:

    ManipulatorController* _manipulatorController;
    ProjectVisualizator* _projectVisualizator;
    dhParametersCalculator* _dhCalculator;

};

}
#endif // SERIALMANIPULATORPROJECTCONTROLLER_H
