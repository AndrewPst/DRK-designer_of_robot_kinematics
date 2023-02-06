//#ifndef PROGRAMDOCK_H
//#define PROGRAMDOCK_H

//#include "basedock.h"
//#include "../logic/actionscontroller.h"
//#include "../logic/models/iaction.h"
////#include "../logic/models/Actions.h"

//#include <QVBoxLayout>
//#include <QPushButton>
//#include <QFormLayout>
//#include <QDoubleSpinBox>
//#include <QLineEdit>

//#include <QLabel>

//namespace serialMan
//{

//class SetupElementWidget : public QWidget
//{
//    Q_OBJECT;
//private:

//    actions::ArgKey_t* _argKey;

//public:

//    explicit SetupElementWidget(actions::ArgKey_t& arg) : _argKey(&arg)
//    {
//        QFormLayout * _formL = new QFormLayout();
//        QWidget* _control;
//        if(arg.type == actions::ActionArgumentType_t::ARGTYPE_DOUBLE)
//        {
//            _control = new QDoubleSpinBox();
//        } else if(arg.type == actions::ActionArgumentType_t::ARGTYPE_STRING)
//        {
//            _control = new QLineEdit();
//        } else {
//            _control = new QWidget();
//        }
//        _formL->addRow(arg.name, _control);
//        setLayout(_formL);
//    }

//private:



//};

//class SetupWidget : public QWidget
//{
//    Q_OBJECT
//public:

//    explicit SetupWidget();

//    void setAction(actions::IAction& act)
//    {
//        _action = &act;
//    }

//    actions::IAction& action()
//    {
//        return *_action;
//    }
//private:

//    void updateLayout()
//    {
//        if(_mainL)
//            delete _mainL;
//        _mainL = new QVBoxLayout();

////        auto list = _action->argsKeys();
////        if(!list)
////            return;

////        for(int i = 0; i < list->size(); ++i)
////        {

////        }

//    }

//private:

//    actions::IAction* _action;

//    QVBoxLayout *_mainL;
//};

//class ProgramDock  : public BaseDock
//{
//    Q_OBJECT
//public:

//    explicit ProgramDock(ActionsController& act, const QString& title = "Program",
//                             QWidget* parent = nullptr,
//                             Qt::WindowFlags flags = {});

//private slots:

//    void onPauseClick();
//    void onResumeClick();
//    void onStartClick();
//    void onStopClick();

//    void stateChanged(serialMan::ProgramState_t);

//private:

//    ActionsController& _act;

//    QWidget *_mainW;
//    QVBoxLayout* _mainL;

//    QPushButton *_startBut, *_stop, *_pause, *_resume;

//    QPushButton *_startStopBut, *_pauseResumeBut;

//    QLabel* _label;

//};

//}
//#endif // PROGRAMDOCK_H
