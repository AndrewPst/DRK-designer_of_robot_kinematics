#ifndef GCODESERIALIZATORWIDGET_H
#define GCODESERIALIZATORWIDGET_H

#include "../logic/actionscontroller.h"

#include <QWidget>
#include <QDialog>
#include <QMainWindow>

#include <QFileDialog>
#include <QFile>
#include <QFileDevice>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QFormLayout>

namespace serialMan
{

namespace widgets
{

class gcodeSerializatorWidget : public QDialog
{
    Q_OBJECT
public:
    gcodeSerializatorWidget(QMainWindow* mainWindow);

    void setProgram(serialMan::ActionsController&);

private slots:

    void onBrowsePathButtonClicked();

    void onExportButClicked();

private:

    serialMan::ActionsController* _act;

    QMainWindow* _mainWindow;

    QPushButton* _browsePathBut;
    QLineEdit* _pathLine;

    QPushButton* _canselBut;
    QPushButton* _exportBut;

    QCheckBox* _restoreLostArgCheckB;

    QTextEdit* _startGCodeText, *_endGCodeText;

    QVBoxLayout* _mainL;

};

}

}
#endif // GCODESERIALIZATORWIDGET_H
