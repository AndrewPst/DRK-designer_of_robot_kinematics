#ifndef SERIAL6DOFMANIPULATOR_H
#define SERIAL6DOFMANIPULATOR_H

#include "projectCore/baseprojectcontroller.h"

namespace serialMan {

QT_FORWARD_DECLARE_CLASS(ManipulatorController);
QT_FORWARD_DECLARE_CLASS(ProjectVisualizator);
QT_FORWARD_DECLARE_CLASS(ActionsController);

class Serial6DofManipulator : public BaseProjectController
{
    Q_OBJECT

public:
    Serial6DofManipulator();
    ~Serial6DofManipulator();

    void init() override;

    ManipulatorController* getManipulatorController() const;
    ProjectVisualizator* getVisualizator() const;
    ActionsController* getActionsController() const;

private:

    ManipulatorController* _manipulatorController;
    ProjectVisualizator* _projectVisualizator;
    ActionsController* _actionsController;
};

}

#endif // SERIAL6DOFMANIPULATOR_H
