#ifndef MANIPULATOREDITORDOCK_H
#define MANIPULATOREDITORDOCK_H

#include "basedock.h"

QT_FORWARD_DECLARE_CLASS(QListWidget);
QT_FORWARD_DECLARE_CLASS(QListWidgetItem);
QT_FORWARD_DECLARE_CLASS(QPushButton);
QT_FORWARD_DECLARE_CLASS(QLabel);
QT_FORWARD_DECLARE_CLASS(Joint_t);
QT_FORWARD_DECLARE_CLASS(ProjectController);
QT_FORWARD_DECLARE_CLASS(QDoubleSpinBox);
QT_FORWARD_DECLARE_CLASS(QSpinBox);

//-----------Joint widget for the selected widget----------

class SelectedJointWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SelectedJointWidget(QWidget* = nullptr);

    void setJoint(Joint_t*);

private:

    void initWidgets();
    void updateWidgets();

    void resetSignalsSlots();
    void createConnections();

private slots:

    void onCurrentValueChanged(double);


private:

    Joint_t* _joint  = NULL;

    QDoubleSpinBox* _currentValue;

};

//-----------Joint widget for the QWidgetList----------

class JointListWidgetItem : public QWidget
{
    Q_OBJECT

public:
    explicit JointListWidgetItem(Joint_t* joint, QWidget *parent = nullptr);

    void setJoint(Joint_t*);
    Joint_t* getJoint();

    void setParentWidgetItem(QListWidgetItem*);
    QListWidgetItem* getParentWidgetItem();

public slots:

    void jointChanged();

    void onDataUpdated();


private:
    void initWidgets();

private:

    Joint_t* _joint = NULL;
    QListWidgetItem* _parentWidgetItem;
    QLabel *_info;

};

//------------Dock class-------------

class ManipulatorEditorDock : public BaseDock
{
    Q_OBJECT

public:
    explicit ManipulatorEditorDock(const QString& title,
                                   QWidget* parent = nullptr,
                                   Qt::WindowFlags flags = {});

public slots:

    void onJointCreated(Joint_t*);
    void onJointDeleted(Joint_t*);
    void onProjectOpened(ProjectController*);

private slots:

    void addButtonClickedSlot();
    void rmButtonClickedSlot();

    void jointSelectedSlot(QListWidgetItem*);

private:
    void initWidgets();

    void createNewListItem(Joint_t*);
    void deleteListItem(Joint_t*);

private:

    QWidget *_mainWidget;

    QListWidget *_list;

    QPushButton *_addJoint, *_rmJoint, *_upJoint, *_downJoint;

    SelectedJointWidget *_detailedWidget = nullptr;
    JointListWidgetItem *_selectedItem = nullptr;
};

#endif // MANIPULATOREDITORDOCK_H
