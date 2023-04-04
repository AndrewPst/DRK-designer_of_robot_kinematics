#ifndef SERIAL6DOFMANIPULATOR_H
#define SERIAL6DOFMANIPULATOR_H

#include "projectCore/baseprojectcontroller.h"

namespace serialMan {

QT_FORWARD_DECLARE_CLASS(ManipulatorController);
QT_FORWARD_DECLARE_CLASS(ActionsController);

namespace gl
{
QT_FORWARD_DECLARE_CLASS(ProjectVisualizator);
}

class Serial6DofManipulator : public BaseProjectController
{
    Q_OBJECT

public:
    Serial6DofManipulator();
    ~Serial6DofManipulator();

    void init() override;

    ManipulatorController& getManipulatorController() const;
    gl::ProjectVisualizator& getVisualizator() const;
    ActionsController& getActionsController() const;

public slots:

    void onExportGCodeCall();
    void onExportJsonConfigCall();

    void onImportGcodeCall();

    void onNewProgramCall();

private:

    std::unique_ptr<ManipulatorController> _manipulatorController;
    std::unique_ptr<gl::ProjectVisualizator> _projectVisualizator;
    std::unique_ptr<ActionsController> _actionsController;
};

}

#endif // SERIAL6DOFMANIPULATOR_H
