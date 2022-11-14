#ifndef KINEMATICSDOCK_H
#define KINEMATICSDOCK_H

#include "basedock.h"

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QDoubleSpinBox);
QT_FORWARD_DECLARE_CLASS(QVBoxLayout);
QT_FORWARD_DECLARE_CLASS(QListWidget);
QT_FORWARD_DECLARE_CLASS(QGroupBox);
QT_FORWARD_DECLARE_CLASS(QCheckBox)

namespace serialMan
{


QT_FORWARD_DECLARE_CLASS(ManipulatorController);
QT_FORWARD_DECLARE_CLASS(Joint_t)

class JointViewModelWidget : public QWidget
{
  Q_OBJECT
public:
    JointViewModelWidget(Joint_t* joint, ManipulatorController* man);

private slots:

    void onJointValueChanged();
    void onJointMinChanged();
    void onJointMaxChanged();

    void onSpinsChanged();

private:

    Joint_t* _joint;
    QVBoxLayout* _mainL;

    QDoubleSpinBox *_value, *_min, *_max;
};

class KinematicsDock : public BaseDock
{
    Q_OBJECT

private:

    void initList();

public:
    explicit KinematicsDock(ManipulatorController* man, const QString& title = "Kinematics",
                            QWidget* parent = nullptr,
                            Qt::WindowFlags flags = {});

public slots:

    void onStructureChanged();

    void onPositionChanged();
    void onConfigChanged();

private:

    QWidget *_mainW;
    QVBoxLayout* _mainL;

    QListWidget* _list;

    QDoubleSpinBox *_posX, *_posY, *_posZ, *_rotX, *_rotY, *_rotZ;

    ManipulatorController* _man;

    QCheckBox* _v1, *_v2;
};

}
#endif // KINEMATICSDOCK_H
