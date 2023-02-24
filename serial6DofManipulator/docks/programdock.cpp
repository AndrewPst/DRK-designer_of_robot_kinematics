#include "programdock.h"
#include "../logic/models/executionEnivroment.h"

#include "../logic/models/unitsConverter.h"



using namespace serialMan;
using namespace docks;


//static uint32_t createMask(char k, uint16_t i)
//{
//    uint32_t r = k;
//    r <<= sizeof(i) * 8;
//    r |= i;
//    return r;
//}

//static actions::actionIdentificator_t getFromMask(uint32_t m)
//{
//    actions::actionIdentificator_t r;
//    r.second = m & 0xFF;
//    r.first = (m >> sizeof(r.second) * 8) & 0xFF;
//    return r;
//}

//------Action view model----------



ActionInProgramVievModel::ActionInProgramVievModel(EnivromentProgram::actionData_t& action, ActionsController& act)
    : _action(action), _act(act)
{
    _hl = new QHBoxLayout();
    QLabel *key = new QLabel(QString(action.first.first) + QString::number(action.first.second));
    _hl->addWidget(key);
    setLayout(_hl);
}

void ActionInProgramVievModel::setActive(bool){}
bool ActionInProgramVievModel::active() const{return true;}
EnivromentProgram::actionData_t& ActionInProgramVievModel::action(){return _action;}



//----------Args editor------------

ActionArgsEditor::ActionArgsEditor(ActionsController& act) : _lib(act.library()), _act(act)
{
    _saveBut = new QPushButton("Save");
    connect(_saveBut, SIGNAL(clicked(bool)), this, SLOT(onSaveButClicked()));

    _mainL = new QVBoxLayout();
    _mainL->addWidget(_saveBut);
    setLayout(_mainL);
}

void ActionArgsEditor::setAction(EnivromentProgram::actionData_t& data)
{
    _aw.clear(); //clear array of  action widgets
    _action = &data; //get pointer of current actions

    _allowArgs = _lib.allowArgs(_action->first); //get allow arguments of action
    if(_allowArgs == nullptr)
        return;

    if(_mainV)
    {
        delete _mainV;
    }

    _mainV = new QGroupBox();
    _mainV->setTitle("Arguments:");
    auto _formL = new QFormLayout();
    for(const auto &it : *_allowArgs)
    {
        QVariant *arg{nullptr}; //get arguments value
        _action->second->getArg(it->key, arg);
        if(!arg)
            continue;

        QWidget *v{nullptr};
        if(it->type == QVariant::Type::Double)
        {
            QDoubleSpinBox *ds = new QDoubleSpinBox();
            if(it->parameters & actions::ActionArgumentParameter::ARGPARAM_UNLIMITED)
            {
                ds->setMaximum(std::numeric_limits<double>::max());
                ds->setMinimum(std::numeric_limits<double>::lowest());
            }
            if(it->parameters & actions::ActionArgumentParameter::ARGPARAM_NUM_POSITIVE_ONLY)
            {
                ds->setMinimum(0);
            }
            if(it->parameters & actions::ActionArgumentParameter::ARGPARAM_IS_ANGLE)
            {
                ds->setValue(radToDeg(arg->toDouble()));
            } else
            {
                ds->setValue(arg->toDouble());
            }
            v = ds;
        }
        else if(it->type == QVariant::Type::String)
        {
            QLineEdit *le = new QLineEdit();
            le->setText(arg->toString());
            v = le;
        }
        else
        {
            continue;
        }

        QCheckBox *_enabledCB = new QCheckBox();
        _enabledCB->setFixedWidth(20);

        QHBoxLayout *_hbl = new QHBoxLayout();
        connect(_formL, SIGNAL(destroyed(QObject*)), _hbl, SLOT(deleteLater()));

        _hbl->addWidget(v);
        _hbl->addWidget(_enabledCB);
        connect(_enabledCB, SIGNAL(toggled(bool)), v, SLOT(setEnabled(bool)));
        if(it->parameters & actions::ActionArgumentParameter::ARGPARAM_IS_REQUIRED)
        {
            _enabledCB->setChecked(true);
            _enabledCB->setEnabled(false);
        }
        else
            _enabledCB->setChecked(_action->second->isArgUsable(it->key));

        _formL->addRow(QString(it->key) + " (" + it->name + ')', _hbl);
        _aw.push_back(ActionWidgets{it, arg, v});
    }
    _mainV->setLayout(_formL);
    _mainL->insertWidget(0, _mainV);
}

void ActionArgsEditor::onSaveButClicked()
{
    for(auto& it : _aw)
    {
        if(it.w->isEnabled() == false)
        {
            _action->second->setArgUsable(it.descr->key, false);
            continue;
        }
        if(it.descr->type == QVariant::Type::Double)
        {
            if(it.descr->parameters & actions::ActionArgumentParameter::ARGPARAM_IS_ANGLE)
                _action->second->setArg(it.descr->key, degToRad(((QDoubleSpinBox*)it.w)->value()));
            else
                _action->second->setArg(it.descr->key, (((QDoubleSpinBox*)it.w)->value()));
        }
        else if(it.descr->type == QVariant::Type::String)
        {
            _action->second->setArg(it.descr->key, (((QLineEdit*)it.w)->text()));
        }
    }
    _act.executeMomently(*_action);
}




//---------Program dock-------------

ProgramDock::ProgramDock(ActionsController& act,
                         const QString& title,
                         QWidget* parent,
                         Qt::WindowFlags flags) : BaseDock(title, parent, flags), _act(act)
{
    setMinimumSize(200, 200);

    _mainW = new QWidget();
    _mainL = new QVBoxLayout();

    _actEditor = new ActionArgsEditor(_act);

    //init buttons
    _startStopBut = new QPushButton("Start");
    connect(_startStopBut, SIGNAL(clicked(bool)), this, SLOT(onStartStopClick()));

    _pauseResumeBut = new QPushButton("Pause");
    connect(_pauseResumeBut, SIGNAL(clicked(bool)), this, SLOT(onPauseResumeClick()));

    _createNewActionBut = new QPushButton("Insert new action");
    connect(_createNewActionBut, SIGNAL(clicked(bool)), this, SLOT(onCreateNewActionClick()));

    _removeActionBut = new QPushButton("Remove action");
    connect(_removeActionBut, SIGNAL(clicked(bool)), this, SLOT(onRemoveActionClick()));

    _startWithSelectedActCBox = new QCheckBox("Start with selected action");

    //init labels
    _label = new QLabel();
    _label->setAlignment(Qt::AlignmentFlag::AlignCenter);
    connect(&act.enivroment(), SIGNAL(stateChanged(serialMan::ExecutionState)),
            this, SLOT(onStateChanged(serialMan::ExecutionState)), Qt::DirectConnection );


    //init combobox
    _actionsComboBox = new QComboBox();
    size_t size = _act.library().allowActions<typeof(_actionsList)>(_actionsList);
    std::sort(_actionsList.begin(), _actionsList.end(), [](const actions::actionIdentificator_t& f, const actions::actionIdentificator_t& s)
    {
        if(f.first < s.first)
            return true;
        else if(f.second < s.second)
            return true;
        else
            return false;
    });

    (void)size;
    for(auto it : qAsConst(_actionsList))
    {
        _actionsComboBox->addItem(QString(it.first) + QString::number(it.second));
    }
    connect(_actionsComboBox, SIGNAL(activated(int)), this, SLOT(onActionComboBoxActivated(int)));


    //init collections
    _actionsCollection = new QListWidget();
    _actionsCollection->setStyleSheet( "QListWidget::item { border-bottom: 1px solid black; } QListView::item:selected { background: gray }" );
    _actionsCollection->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    connect(_actionsCollection, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(onSelectedNewAction(QListWidgetItem*)));
    onProgramStructureChanged();
    connect(&_act.enivroment().program(), SIGNAL(structureChanged()), this, SLOT(onProgramStructureChanged()));

    //add widgets

    QVBoxLayout *actionsManageMenuL = new QVBoxLayout();
    actionsManageMenuL->addWidget(_actionsComboBox);
    actionsManageMenuL->addWidget(_createNewActionBut);
    actionsManageMenuL->addSpacing(5);
    actionsManageMenuL->addWidget(_removeActionBut);
    QGroupBox* actionsManageBox = new QGroupBox("Action management");
    actionsManageBox->setLayout(actionsManageMenuL);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(_startStopBut, 0, 0);
    grid->addWidget(_pauseResumeBut, 1, 0);
    grid->addWidget(_label, 0, 1, 2, 1);
    grid->addWidget(_startWithSelectedActCBox, 2, 0, 2, 2);

    _mainL->addWidget(_actionsCollection);
    _mainL->addWidget(_actEditor);
    _mainL->addWidget(actionsManageBox);
    _mainL->addLayout(grid);

    _mainW->setLayout(_mainL);
    setWidget(_mainW);

    onStateChanged(ExecutionState::STATE_FINISHED);
}

void ProgramDock::onStartStopClick()
{
    if(_act.enivroment().state() == ExecutionState::STATE_FINISHED)
    {
        if(_startWithSelectedActCBox->isChecked())
        {
            auto i = _actionsCollection->currentIndex().row();
            _act.enivroment().program().setPos(i);
        } else
        {
            _act.enivroment().manipulator().resetJoints();
            _act.enivroment().program().reset();
        }
        _act.startProgram();
    }
    else
        _act.stop();
}

void ProgramDock::onProgramStructureChanged()
{
    _actionsCollection->blockSignals(true);
    _actionsCollection->clear();
    auto& prog =_act.enivroment().program();
    prog.reset();
    QListWidgetItem* item{nullptr};
    for(size_t i = 0; i < prog.size(); ++i)
    {
        auto& action = prog.next();
        ActionInProgramVievModel* actvm = new ActionInProgramVievModel(action, _act);
        item = new QListWidgetItem(_actionsCollection);
        item->setSizeHint( actvm->sizeHint() );
        _actionsCollection->setItemWidget(item, actvm);
    }
    _actionsCollection->blockSignals(false);
    if(item)
        _actionsCollection->setCurrentItem(item);
    if(prog.size() ==0)
        _actEditor->setEnabled(false);
    else
        _actEditor->setEnabled(true);
}

void ProgramDock::onSelectedNewAction(QListWidgetItem* item)
{
    auto w = qobject_cast<ActionInProgramVievModel*>( _actionsCollection->itemWidget(item));
    _actEditor->setAction(w->action());
    _act.executeMomently(w->action());
}


void ProgramDock::onPauseResumeClick()
{
    if(_act.enivroment().state() == ExecutionState::STATE_IS_RUNNING)
        _act.pause();
    else if(_act.enivroment().state() == ExecutionState::STATE_SUSPENDED)
        _act.resume();
}

void ProgramDock::onActionComboBoxActivated(int i)
{
    (void)i;
    //    QVariant data = _actionsComboBox->itemData(i);
    //    actions::actionIdentificator_t ident = getFromMask(data.toUInt());
    //    qDebug() << ident.first << ' ' << ident.second << '\n';
}

void ProgramDock::onCreateNewActionClick()
{
    actions::actionIdentificator_t id = _actionsList[_actionsComboBox->currentIndex()];
    auto actParams = std::shared_ptr<actions::IArgsCollection>(_act.library().argsCollectionGenerator(id)());
    auto allowArgs = _act.library().allowArgs(id);
    for(const auto &it : *allowArgs)
    {
        if(it->manParameter == ManipulatorController::PARAMETER_NONE)
            continue;
        double v = _act.enivroment().manipulator().getParameter<double>(it->manParameter);
        actParams->setArg(it->key, v);
    }
    if(_act.enivroment().program().size() == 0)
    {
        _act.enivroment().program().add(EnivromentProgram::actionData_t(id, actParams));
    }
    else
    {
        size_t cItem = _actionsCollection->currentIndex().row();
        _act.enivroment().program().insert(EnivromentProgram::actionData_t(id, actParams), cItem);
    }
}

void ProgramDock::onRemoveActionClick()
{
    auto cItem = _actionsCollection->currentItem();
    auto actionW = (qobject_cast<ActionInProgramVievModel*>(_actionsCollection->itemWidget(cItem)))->action();
    _act.enivroment().program().remove(actionW);
}

void ProgramDock::onStateChanged(serialMan::ExecutionState state)
{
    switch(state)
    {
    case serialMan::ExecutionState::STATE_IS_RUNNING:
        _label->setText("is running");
        _label->setStyleSheet("QLabel { background-color : green; font-size: 16pt;}");
        _startStopBut->setText("Stop");
        _pauseResumeBut->setText("Pause");
        break;
    case serialMan::ExecutionState::STATE_SUSPENDED:
        _label->setText("suspended");
        _label->setStyleSheet("QLabel { background-color : orange; font-size: 16pt;}");
        _startStopBut->setText("Stop");
        _pauseResumeBut->setText("Resume");
        break;
    default:
        _label->setText("finished");
        _label->setStyleSheet("QLabel { background-color : red; font-size: 16pt;}");
        _startStopBut->setText("Start");
        _pauseResumeBut->setText("Pause");
        break;
    }
}
