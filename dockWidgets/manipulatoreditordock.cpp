#include "manipulatoreditordock.h"
#include "projectCore/projectStructure.h"
#include "projectCore/projectcore.h"

#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QWidget>
#include <QLabel>
#include <QListIterator>
#include <QDoubleSpinBox>

//-----------Joint widget for the selected widget----------

SelectedJointWidget::SelectedJointWidget(QWidget *parent)
    : QWidget(parent)
{
    initWidgets();
}

void SelectedJointWidget::setJoint(Joint_t* j)
{
    if(j == _joint)
        return;
    resetSignalsSlots();
    _joint = j;
    updateWidgets();
    createConnections();
}

void SelectedJointWidget::resetSignalsSlots()
{
    if(_joint == NULL) return;
    disconnect(_joint, SIGNAL(onCurrentValueChanged(double)), _currentValue, SLOT(setValue(double)));

}

void SelectedJointWidget::createConnections()
{
    connect(_joint, SIGNAL(onCurrentValueChanged(double)), _currentValue, SLOT(setValue(double)));

}

void SelectedJointWidget::initWidgets()
{
    _currentValue = new QDoubleSpinBox();
    _currentValue->setDecimals(2);
    connect(_currentValue, SIGNAL(valueChanged(double)), this, SLOT(onCurrentValueChanged(double)));


    QVBoxLayout *vbl = new QVBoxLayout();

    vbl->addWidget(_currentValue);
    setLayout(vbl);
}

void SelectedJointWidget::updateWidgets()
{
    _currentValue->setValue(_joint->getCurrentValue());
    _currentValue->setSingleStep(0.5);
    _currentValue->setRange(_joint->getMinValue(), _joint->getMaxValue());
    _currentValue->setPrefix(_joint->getJointType() == JointType_t::JOINT_LINEAR ? tr("mm ") : tr("angle "));

}

void SelectedJointWidget::onCurrentValueChanged(double value)
{
    if(value == _joint->getCurrentValue()) return;
    _joint->setCurrentValue(value);
    _currentValue->setValue(value);
}



//-----------Joint widget for the QWidgetList----------

JointListWidgetItem::JointListWidgetItem(Joint_t* joint, QWidget *parent)
    : QWidget(parent), _joint(joint)
{
    connect(_joint, &Joint_t::onTypeChanged, this, &JointListWidgetItem::onDataUpdated);
    initWidgets();
}

void JointListWidgetItem::initWidgets()
{
    _info = new QLabel();
    onDataUpdated();

    QHBoxLayout *l = new QHBoxLayout();
    l->addWidget(_info);

    setLayout(l);
}

void JointListWidgetItem::onDataUpdated()
{
    _info->setText(tr("Type = %3")
                   .arg(_joint->getJointType() == JointType_t::JOINT_ROTATION ? "Rotation" : "Linear"));
}

void JointListWidgetItem::setJoint(Joint_t* joint)
{
    _joint = joint;
    jointChanged();
}

void JointListWidgetItem::setParentWidgetItem(QListWidgetItem* p)
{
    _parentWidgetItem = p;
}

QListWidgetItem* JointListWidgetItem::getParentWidgetItem()
{
    return _parentWidgetItem;
}


Joint_t* JointListWidgetItem::getJoint()
{
    return _joint;
}

void JointListWidgetItem::jointChanged()
{
    //TODO make slot logic
}

//------------Dock class-------------

ManipulatorEditorDock::ManipulatorEditorDock(const QString& title,
                                             QWidget* parent,
                                             Qt::WindowFlags flags) : BaseDock(title, parent, flags)
{
    drawAsWindow(true);

    connect(&projectManager, SIGNAL(onProjectOpened(ProjectController*)), this, SLOT(onProjectOpened(ProjectController*)));
    if(projectManager.getOpenedProject())
        onProjectOpened(projectManager.getOpenedProject());
    initWidgets();
    _rmJoint->setEnabled(false);
}

void ManipulatorEditorDock::onProjectOpened(ProjectController* pc)
{
    connect(pc, &ProjectController::onJointRemoved, this, &ManipulatorEditorDock::onJointDeleted);
    connect(pc, &ProjectController::onNewJointAdded, this, &ManipulatorEditorDock::onJointCreated);
}

void ManipulatorEditorDock::initWidgets()
{
    _mainWidget = new QWidget();

    QVBoxLayout *vl = new QVBoxLayout();

    _addJoint = new QPushButton(this);
    _addJoint->setText(tr("Add joint"));
    _rmJoint = new QPushButton(this);
    _rmJoint->setText(tr("Remove joint"));

    connect(_addJoint, &QPushButton::clicked, this, &ManipulatorEditorDock::addButtonClickedSlot);
    connect(_rmJoint, &QPushButton::clicked, this, &ManipulatorEditorDock::rmButtonClickedSlot);

    QHBoxLayout *bhl = new QHBoxLayout();
    bhl->addWidget(_addJoint);
    bhl->addWidget(_rmJoint);


    _list = new QListWidget();
    _list->setSpacing(0);
    ProjectController* project = projectManager.getOpenedProject();
    for (auto it = project->jointsBegin(); it != project->jointsEnd(); ++it)
    {
        createNewListItem(*it);
    }
    connect(_list, &QListWidget::itemClicked, this, &ManipulatorEditorDock::jointSelectedSlot);

    _upJoint = new QPushButton();
    _upJoint->setText(tr("Up"));
    _downJoint = new QPushButton();
    _downJoint->setText(tr("Down"));

    QHBoxLayout *hblForButt = new QHBoxLayout();
    hblForButt->addWidget(_downJoint);
    hblForButt->addWidget(_upJoint);

    _detailedWidget = new SelectedJointWidget();

    vl->addLayout(bhl);
    vl->addWidget(_list);
    vl->addLayout(hblForButt);
    vl->addWidget(_detailedWidget);

    _mainWidget->setLayout(vl);
    setWidget(_mainWidget);
}

void ManipulatorEditorDock::createNewListItem(Joint_t* jvm)
{
    JointListWidgetItem *element = new JointListWidgetItem(jvm);
    QListWidgetItem *item  = new QListWidgetItem(_list);
    element->setParentWidgetItem(item);
    item->setSizeHint(element->sizeHint());
    _list->setItemWidget(item, element);
}

void ManipulatorEditorDock::deleteListItem(Joint_t* jvm)
{
    for(qsizetype i = 0; i < _list->count(); i++)
    {
        auto item = _list->item(i);//get QListWidgetIten by id
        auto itemW = (JointListWidgetItem*)_list->itemWidget(item);//get its widget
        if(itemW->getJoint() == jvm)//if it is the needing widget
        {
            _list->removeItemWidget(item);
            if(itemW == _selectedItem) //set to NULL for correct work rmJoint button
            {
                _rmJoint->setEnabled(false);
                _upJoint->setEnabled(false);
                _downJoint->setEnabled(false);
                _list->setCurrentRow(-1);
            }
            itemW->deleteLater();//this object will be deleted after exiting the method
            delete item;
            return;
        }
    }
}

void ManipulatorEditorDock::onJointCreated(Joint_t* jvm)
{
    createNewListItem(jvm);
}

void ManipulatorEditorDock::onJointDeleted(Joint_t* jvm)
{
    deleteListItem(jvm);
}

void ManipulatorEditorDock::addButtonClickedSlot()
{
    projectManager.getOpenedProject()->addJoint(new Joint_t{});
}

void ManipulatorEditorDock::rmButtonClickedSlot()
{
    if(_selectedItem == NULL)
        return;
    projectManager.getOpenedProject()->removeJoint(_selectedItem->getJoint());
}

void ManipulatorEditorDock::jointSelectedSlot(QListWidgetItem* item)
{
    _selectedItem = qobject_cast<JointListWidgetItem*>(_list->itemWidget(item));
    _rmJoint->setEnabled(true);
    int cRow = _list->currentRow();
    _upJoint->setEnabled(cRow > 0);
    _downJoint->setEnabled(cRow < _list->count()-1);
    _detailedWidget->setJoint(_selectedItem->getJoint());
}

