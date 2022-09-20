#ifndef MANIPULATORSTRUCTUREEDITORDOCK_H
#define MANIPULATORSTRUCTUREEDITORDOCK_H

#include "basedock.h"

QT_FORWARD_DECLARE_CLASS(QSpinBox);
QT_FORWARD_DECLARE_CLASS(QPushButton);
QT_FORWARD_DECLARE_CLASS(QListWidget);
QT_FORWARD_DECLARE_CLASS(QLabel);



namespace serialMan {

QT_FORWARD_DECLARE_CLASS(Joint_t);
QT_FORWARD_DECLARE_CLASS(ManipulatorController);

//Class for list of joints
class JointListElement : public QWidget
{
    Q_OBJECT

public:

    explicit JointListElement(serialMan::Joint_t*);
    Joint_t* getJoint() const;

private slots:



private:

    void updateValues();

private:

    QLabel *_value;
    Joint_t* _joint;
};


//Dock widget for editing and monitoring manipulators structure
class ManipulatorStructureEditorDock : public BaseDock
{
    Q_OBJECT

public:
    explicit ManipulatorStructureEditorDock(const QString&);


private slots:

    //Slots for internal widgets
    void onDofSpinValueChanged(int);
    void onRebuildClicked();

public slots:

    //Slots for external signals
    void onDofChanged(int);
    void onJointAdded(serialMan::Joint_t*);
    void onJointRemoved(serialMan::Joint_t*);

private:

    void updateJointsList();

private:

    QSpinBox *_dofSpin;
    QPushButton *_rebuildProject;
    QListWidget *_jointsList;

    serialMan::ManipulatorController* _manipulator;
};

}

#endif // MANIPULATORSTRUCTUREEDITORDOCK_H
