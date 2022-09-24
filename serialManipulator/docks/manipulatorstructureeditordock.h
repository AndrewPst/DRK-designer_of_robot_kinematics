#ifndef MANIPULATORSTRUCTUREEDITORDOCK_H
#define MANIPULATORSTRUCTUREEDITORDOCK_H

#include "basedock.h"
#include <QWidget>

#include "../logic/models/joint_t.h"

QT_FORWARD_DECLARE_CLASS(QSpinBox);
QT_FORWARD_DECLARE_CLASS(QPushButton);
QT_FORWARD_DECLARE_CLASS(QListWidget);
QT_FORWARD_DECLARE_CLASS(QLabel);
QT_FORWARD_DECLARE_CLASS(QDoubleSpinBox);
QT_FORWARD_DECLARE_CLASS(QComboBox);
QT_FORWARD_DECLARE_CLASS(QVector3D);



namespace serialMan {

QT_FORWARD_DECLARE_CLASS(Joint_t);
QT_FORWARD_DECLARE_CLASS(ManipulatorController);

class JointDetailedWidget : public ::QWidget
{
    Q_OBJECT

public:

    explicit JointDetailedWidget();

    Joint_t* getJoint() const;
    void setJoint(Joint_t*);

private:

    void initWidgets();
    void updateWidgets();

    void resetConnections();
    void createConnections();

public slots:

    void onCurrentValueChanged(double);
    void onMinValueChanged(double);
    void onMaxValueChanged(double);
    void onTypeChanged(serialMan::JointType_t);
    void onPositionChanged(QVector3D);
    void onRotationChanged(QVector3D);

    //For widgets:
    void onPosUpdated();
    void onRotUpdated();
    void onTypeUpdated(int);

private:

    Joint_t* _joint  = NULL;
    QDoubleSpinBox* _currentValue;
    QDoubleSpinBox* _minValue;
    QDoubleSpinBox* _maxValue;
    QComboBox* _typeJointBox;

    //Position
    QDoubleSpinBox* _posX;
    QDoubleSpinBox* _posY;
    QDoubleSpinBox* _posZ;

    //Rotation
    QDoubleSpinBox* _rotX;
    QDoubleSpinBox* _rotY;
    QDoubleSpinBox* _rotZ;

    QList<QMetaObject::Connection> _connections;
};

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

    Qt::DockWidgetArea getDefaultArea() const override;

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
    void jointSelected();

private:

    QSpinBox *_dofSpin;
    QPushButton *_rebuildProject;
    QListWidget *_jointsList;

    JointDetailedWidget* _detailed;

    serialMan::ManipulatorController* _manipulator;
};

}

#endif // MANIPULATORSTRUCTUREEDITORDOCK_H
