#ifndef SERIALMANIPULATORPROJECTCONTROLLER_H
#define SERIALMANIPULATORPROJECTCONTROLLER_H

#include "projectCore/baseprojectcontroller.h"
#include "logic/manipulatorcontroller.h"

namespace serialMan {


class SerialManipulatorProject : public BaseProjectController
{
    Q_OBJECT

public:
    SerialManipulatorProject();
    ~SerialManipulatorProject();


    ManipulatorController* getManipulatorController();


private:

    ManipulatorController* _manipulatorController;

};

}
#endif // SERIALMANIPULATORPROJECTCONTROLLER_H
