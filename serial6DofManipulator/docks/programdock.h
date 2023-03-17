#ifndef PROGRAMDOCK_H
#define PROGRAMDOCK_H

#include "basedock.h"
#include "../logic/actionscontroller.h"
#include "../logic/models/enivromentProgram.h"
#include "../logic/models/iaction.h"
//#include "../logic/models/Actions.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QCheckBox>
#include <QLabel>
#include <QStringBuilder>
#include <QGroupBox>

namespace serialMan
{

namespace docks
{

class ActionInProgramVievModel : public QWidget
{
    Q_OBJECT

private:

    EnivromentProgram::actionData_t& _action;
    ActionsController& _act;

    QHBoxLayout *_hl;
    QLabel *_key;
    bool _isActive{false};

public:
    ActionInProgramVievModel(EnivromentProgram::actionData_t&, ActionsController&);
    EnivromentProgram::actionData_t& action();
    void setActive(bool);
    bool active() const;

public slots:

    void onExecutableActionChanged(EnivromentProgram::actionData_t*);

};

class ActionArgsEditor : public QWidget
{
    Q_OBJECT
private:

    const actions::ActionsLibrary& _lib;

    ActionsController& _act;

    const QVector<actions::ArgDescription_t*>* _allowArgs;
    EnivromentProgram::actionData_t* _action;

    struct ActionWidgets
    {
        const actions::ArgDescription_t* descr;
        QVariant* arg;
        QWidget* w;
    };

    QVector<ActionWidgets> _aw;

    QGroupBox *_mainV{nullptr};
    QVBoxLayout *_mainL;
    QPushButton *_saveBut;
public:

    ActionArgsEditor(ActionsController&);
    void setAction(EnivromentProgram::actionData_t&);

private slots:

    void onSaveButClicked();

};


class ProgramDock  : public BaseDock
{
    Q_OBJECT
public:

    explicit ProgramDock(ActionsController& act, const QString& title = "Program",
                         QWidget* parent = nullptr,
                         Qt::WindowFlags flags = {});

private slots:

    void onPauseResumeClick();
    void onStartStopClick();
    void onCreateNewActionClick();
    void onRemoveActionClick();

    void onStateChanged(serialMan::ExecutionState);
    void onProgramStructureChanged();
    void onSelectedNewAction(QListWidgetItem*);
    void onActionComboBoxActivated(int);
    void onExecutionActionChanged(EnivromentProgram::actionData_t*, int);

private:

    ActionsController& _act;

    ActionInProgramVievModel* _currentExecutableAction;

    QWidget *_mainW;
    QVBoxLayout* _mainL;

    ActionArgsEditor* _actEditor;
    QListWidget *_actionsCollection;

    QLabel* _label;
    QPushButton *_startStopBut, *_pauseResumeBut;
    QPushButton *_createNewActionBut;
    QPushButton *_removeActionBut;
    QCheckBox *_startWithSelectedActCBox;

    QComboBox *_actionsComboBox;
    QList<actions::actionIdentificator_t> _actionsList;


    EnivromentProgram::actionData_t* _currentEditableAction;


};

}

}
#endif // PROGRAMDOCK_H
