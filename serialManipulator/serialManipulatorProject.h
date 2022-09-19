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

    void init() override;

    ManipulatorController* getManipulatorController() const;


private:

    ManipulatorController* _manipulatorController;

};

}
#endif // SERIALMANIPULATORPROJECTCONTROLLER_H
