#ifndef SERIALMANIPULATORPROJECTCONTROLLER_H
#define SERIALMANIPULATORPROJECTCONTROLLER_H

#include "projectCore/baseprojectcontroller.h"
#include "logic/manipulatorcontroller.h"

namespace serialMan {


class SerialManipulatorProject : public BaseProjectController
{
public:
    explicit SerialManipulatorProject();

    ManipulatorController& getManipulatorController() const;


private:

    ManipulatorController _manipulatorController;

};

}
#endif // SERIALMANIPULATORPROJECTCONTROLLER_H
