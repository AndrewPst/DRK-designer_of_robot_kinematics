#include "gcodedeserializatiorwidget.h"
#include "../logic/gcodeserializator.h"
#include "QMessageBox"

using namespace serialMan;
using namespace widgets;

gcodeDeserializatiorWidget::gcodeDeserializatiorWidget(QMainWindow* mainWindow) : _mainWindow(mainWindow)
{
    setWindowTitle("Export gcode file");

    setModal(true);

    setWindowFlags(windowFlags().setFlag(Qt::WindowContextHelpButtonHint, false));
    setMinimumSize(480, 320);

    _mainL = new QVBoxLayout();

    QHBoxLayout* _editPathL = new QHBoxLayout();
    _pathLine = new QLineEdit();
    _browsePathBut = new QPushButton("Browse");
    _editPathL->addWidget(_pathLine);
    _editPathL->addWidget(_browsePathBut);

    connect(_browsePathBut, SIGNAL(clicked(bool)), this, SLOT(onBrowsePathButtonClicked()));

    _restoreLostArgCheckB = new QCheckBox("Restore lost arguments");


    _canselBut = new QPushButton("Cansel");
    connect(_canselBut, SIGNAL(clicked(bool)), this, SLOT(close()));
    _exportBut = new QPushButton("Import");
    connect(_exportBut, SIGNAL(clicked(bool)), this, SLOT(onExportButClicked()));
    QHBoxLayout *butL = new QHBoxLayout();
    butL->addWidget(_canselBut);
    butL->addWidget(_exportBut);

    _mainL->addLayout(_editPathL);
    _mainL->addWidget(_restoreLostArgCheckB);
    _mainL->addLayout(butL);

    setLayout(_mainL);
}

void gcodeDeserializatiorWidget::setProgram(serialMan::ActionsController& act)
{
    _act = &act;
}

void gcodeDeserializatiorWidget::onBrowsePathButtonClicked()
{
    QFileDialog fDialog;
    QString name = fDialog.getOpenFileName(_mainWindow, "Open file", "", "GCODE file(*.gcode)");
    _pathLine->setText(name);
}

void gcodeDeserializatiorWidget::onExportButClicked()
{
    QMessageBox message(this);
    message.addButton("Close", QMessageBox::ButtonRole::AcceptRole);
    QFile file(_pathLine->text());
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        gCodeSerializator ser(*_act);
        ser.deserializate(stream, _restoreLostArgCheckB->isChecked());
        message.setText("Deserialization successfull");
    } else
    {
        message.setText("Deserialization error! (File not found)");
        qDebug() << "Error opening file";
    }
    file.close();
    message.show();
    message.exec();
}
