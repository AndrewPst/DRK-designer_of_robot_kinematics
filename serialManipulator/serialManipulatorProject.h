#ifndef SERIALMANIPULATORPROJECTCONTROLLER_H
#define SERIALMANIPULATORPROJECTCONTROLLER_H

#include "projectCore/baseprojectcontroller.h"
#include "logic/manipulatorcontroller.h"

//Use the namespace to prevent conflicts
namespace serialMan {

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


private:

    ManipulatorController* _manipulatorController;

};

}
#endif // SERIALMANIPULATORPROJECTCONTROLLER_H
