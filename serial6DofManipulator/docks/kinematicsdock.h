#ifndef KINEMATICSDOCK_H
#define KINEMATICSDOCK_H

#include "basedock.h"

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QDoubleSpinBox);
QT_FORWARD_DECLARE_CLASS(QVBoxLayout);
QT_FORWARD_DECLARE_CLASS(QListWidget);
QT_FORWARD_DECLARE_CLASS(QGroupBox);
QT_FORWARD_DECLARE_CLASS(QCheckBox);
QT_FORWARD_DECLARE_CLASS(QPalette);

namespace serialMan
{

QT_FORWARD_DECLARE_CLASS(ManipulatorController);
QT_FORWARD_DECLARE_CLASS(Joint_t)

class JointViewModelWidget : public QWidget
{
    Q_OBJECT
public:
    JointViewModelWidget(Joint_t* joint, ManipulatorController& man);

public slots:

    void onStepChanged(double);

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
    explicit KinematicsDock(ManipulatorController& man, const QString& title = "Kinematics",
                            QWidget* parent = nullptr,
                            Qt::WindowFlags flags = {});

    Qt::DockWidgetArea getDefaultArea() const override;

public slots:

    void onStructureChanged();

    void onPositionChanged();
    void onConfigChanged();

    void controlParamsChanged();

signals:

    void stepChanged(double);

private:

    QWidget *_mainW;
    QVBoxLayout* _mainL;

    QListWidget* _list;

    QDoubleSpinBox *_posX, *_posY, *_posZ, *_rotX, *_rotY, *_rotZ;

    QDoubleSpinBox *posStep;
    QDoubleSpinBox *rotStep;
    QDoubleSpinBox *jointStep;

    double _stepMove{1}, _stepRot{1}, _stepJointMove{1};

    ManipulatorController& _man;

    QCheckBox* _v1, *_v2;
};

}
#endif // KINEMATICSDOCK_H
