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

    ManipulatorController& getManipulatorController() const;
    ProjectVisualizator& getVisualizator() const;
    ActionsController& getActionsController() const;

private:

    std::unique_ptr<ManipulatorController> _manipulatorController;
    std::unique_ptr<ProjectVisualizator> _projectVisualizator;
    std::unique_ptr<ActionsController> _actionsController;
};

}

#endif // SERIAL6DOFMANIPULATOR_H
