#include "gcodeserializatorwidget.h"
#include "../logic/gcodeserializator.h"

using namespace serialMan;
using namespace widgets;

gcodeSerializatorWidget::gcodeSerializatorWidget(QMainWindow* mainWindow) : _mainWindow(mainWindow)
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

    _startGCodeText = new QTextEdit();
    _startGCodeText->setPlaceholderText("Initial gcode commands");
    _endGCodeText = new QTextEdit();
    _endGCodeText ->setPlaceholderText("Final gcode commands");

    QHBoxLayout* _startEndTextL = new QHBoxLayout();
    _startEndTextL->addWidget(_startGCodeText);
    _startEndTextL->addWidget(_endGCodeText);

    _canselBut = new QPushButton("Cansel");
    connect(_canselBut, SIGNAL(clicked(bool)), this, SLOT(close()));
    _exportBut = new QPushButton("Export");
    connect(_exportBut, SIGNAL(clicked(bool)), this, SLOT(onExportButClicked()));
    QHBoxLayout *butL = new QHBoxLayout();
    butL->addWidget(_canselBut);
    butL->addWidget(_exportBut);

    _mainL->addLayout(_editPathL);
    _mainL->addWidget(_restoreLostArgCheckB);
    _mainL->addLayout(_startEndTextL);
    _mainL->addLayout(butL);

    setLayout(_mainL);
}

void gcodeSerializatorWidget::setProgram(serialMan::ActionsController& act)
{
    _act = &act;
}

void gcodeSerializatorWidget::onBrowsePathButtonClicked()
{
    QFileDialog fDialog;
    QString name = fDialog.getSaveFileName(_mainWindow, "Open file", "/program", "GCODE file(*.gcode)");
    _pathLine->setText(name);
}

void gcodeSerializatorWidget::onExportButClicked()
{
    QFile file(_pathLine->text());
    if(file.open(QIODevice::WriteOnly | QIODevice::Text) == false)
    {
        qDebug() << "error opening file";
        return;
    }
    QTextStream out(&file);

    QString gcode = _startGCodeText->toPlainText();
    if(gcode.isEmpty() == false)
    {
        out << gcode;
        if(gcode.endsWith('\n') == false)
            out << '\n';
    }

    gCodeSerializator ser(*_act);
    ser.serializate(out, _restoreLostArgCheckB->isChecked());

    gcode = _endGCodeText->toPlainText();
    if(gcode.isEmpty() == false)
        out << gcode;

    file.close();
}



