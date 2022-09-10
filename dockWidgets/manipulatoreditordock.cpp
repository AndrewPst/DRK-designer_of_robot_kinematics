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
#include <QFormLayout>
#include <QComboBox>
#include <QScrollBar>
#include <QKeyEvent>


GetIntDialog::GetIntDialog(int min, int max, Qt::WindowFlags flags) : QDialog(NULL, flags)
{
    setFixedSize(160, 80);
    QWidget::setWindowTitle(tr("Input value"));
    setWindowFlags(windowFlags().setFlag(Qt::WindowContextHelpButtonHint, false));
    setTabletTracking(false);
    setModal(true);

    QVBoxLayout *vbl = new QVBoxLayout();

    _valueBox = new QSpinBox();
    _valueBox->setRange(min, max);
    _valueBox->setKeyboardTracking(true);

    _acceptBut = new QPushButton(tr("Accept"));
    _canselBut = new QPushButton(tr("Cansel"));
    connect(_acceptBut, &QPushButton::pressed, this, &GetIntDialog::onAcceptSlot);
    connect(_valueBox, &QSpinBox::editingFinished, this, &GetIntDialog::onAcceptSlot);

    QHBoxLayout *hbl = new QHBoxLayout();
    hbl->addWidget(_canselBut);
    hbl->addWidget(_acceptBut);

    vbl->addWidget(_valueBox);
    vbl->addLayout(hbl);

    setLayout(vbl);
}

void GetIntDialog::onAcceptSlot()
{
    setResult(QDialog::Accepted);
    close();
}


void GetIntDialog::getResult(int* out)
{
    *out = _valueBox->value();
}

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
    Q_FOREACH(auto c, _connections)
    {
        disconnect(c);
    }
}

void SelectedJointWidget::createConnections()
{
    _connections << connect(_joint, SIGNAL(onCurrentValueChanged(double)), _currentValue, SLOT(setValue(double)));
    _connections << connect(_joint, SIGNAL(onIdChanged(int)), _idSpin, SLOT(setValue(int)));
}

void SelectedJointWidget::initWidgets()
{
    _currentValue = new QDoubleSpinBox();
    _currentValue->setDecimals(2);
    _currentValue->setSingleStep(0.5);
    connect(_currentValue, SIGNAL(valueChanged(double)), this, SLOT(onCurrentValueChanged(double)));

    _idSpin = new QSpinBox();
    _idSpin->setRange(0, 255);
    connect(_idSpin, SIGNAL(valueChanged(int)), this, SLOT(onIdChanged(int)));

    _parentsList = new QListWidget();
    _parentsList->setFixedHeight(20);
    _parentsList->verticalScrollBar()->hide();
    _parentsList->setFlow(QListView::LeftToRight);
    _parentsList->addItem(tr("1"));
    _parentsList->addItem(tr("2"));
    _parentsList->addItem(tr("3"));
    _parentsList->addItem(tr("4"));

    _deleteParent = new QPushButton(tr("Delete"));
    _addParent = new QPushButton(tr("Add"));
    connect(_addParent, &QPushButton::clicked, this, []()
    {
        GetIntDialog _getIntDialog (0, 255);
        _getIntDialog.show();
        if(_getIntDialog.exec())
        {
            int out = 0;
            _getIntDialog.getResult(&out);
            qDebug() << out;
        }
    });

    QHBoxLayout* tempHbl = new QHBoxLayout;
    QVBoxLayout* tempVbl = new QVBoxLayout;
    tempHbl->addWidget(_deleteParent);
    tempHbl->addWidget(_addParent);
    tempVbl->addWidget(_parentsList);
    tempVbl->addLayout(tempHbl);

    QFormLayout *fl = new QFormLayout();
    fl->setFormAlignment(Qt::AlignHCenter | Qt::AlignTop);
    fl->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    fl->setVerticalSpacing(0);
    fl->addRow(tr("Id"), _idSpin);
    fl->addRow(tr("Current value:"), _currentValue);
    fl->addRow(tr("Parents"), tempVbl);
    fl->setSizeConstraint(QFormLayout::SizeConstraint::SetFixedSize);
    fl->setSpacing(4);
    setLayout(fl);
}


void SelectedJointWidget::onIdChanged(int id)
{
    if(id == _joint->getId()) return;
    int dir = id - _joint->getId();
    int newId = projectManager.getOpenedProject()->getNextFreeId(_joint->getId(), dir);
    projectManager.getOpenedProject()->changeJointId(_joint->getId(), newId);
    _idSpin->setValue(newId);
}

void SelectedJointWidget::updateWidgets()
{
    _currentValue->setValue(_joint->getCurrentValue());
    _currentValue->setRange(_joint->getMinValue(), _joint->getMaxValue());
    _currentValue->setSuffix(_joint->getJointType() == JointType_t::JOINT_LINEAR ? tr(" mm") : tr(" degrees"));

    _idSpin->setValue(_joint->getId());

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

    //_editButton = new QPushButton();
    //_editButton->setText(tr("Edit joint"));
    //connect(_editButton, &QPushButton::clicked, this, &ManipulatorEditorDock::editButtonClickedSlot);


    _detailedWidget = new SelectedJointWidget();

    vl->addLayout(bhl);
    vl->addWidget(_list);
    vl->addWidget(_detailedWidget);
    _detailedWidget->setEnabled(false);
    //vl->addWidget(_editButton);

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
                _detailedWidget->setEnabled(false);
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
    projectManager.getOpenedProject()->createJoint();
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
    _detailedWidget->setEnabled(true);
    _detailedWidget->setJoint(_selectedItem->getJoint());
}

//void ManipulatorEditorDock::editButtonClickedSlot()
//{
//    _detailedWidget->showNormal();
//}
