#ifndef MANIPULATORSTRUCTUREEDITORDOCK_H
#define MANIPULATORSTRUCTUREEDITORDOCK_H

#include "basedock.h"

QT_FORWARD_DECLARE_CLASS(QSpinBox);
QT_FORWARD_DECLARE_CLASS(QPushButton);

namespace serialMan {

QT_FORWARD_DECLARE_CLASS(ManipulatorController);

class ManipulatorStructureEditorDock : public BaseDock
{
    Q_OBJECT

public:
    explicit ManipulatorStructureEditorDock(const QString&);


private slots:

    void onDofSpinValueChanged(int);

private:

    QSpinBox *_dofSpin;

    QPushButton *_rebuildProject;

    serialMan::ManipulatorController* _manipulator;


};

}

#endif // MANIPULATORSTRUCTUREEDITORDOCK_H
