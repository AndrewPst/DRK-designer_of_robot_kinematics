#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>

//----forward declaration classes----
class ToolBar;
class CentralWindow;
class BaseDock;
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QActionGroup)


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = {});
    ~MainWindow();

//----Actions slots----
public slots:

    //----File actions----
    void actionNewProjectSlot(); //Create new project
    void actionOpenProjectSlot(); //Open project

    void actionSaveAllSlot(); //Save all
    void actionSaveAllAsSlot(); //Save all as user the user says

    void actionReturnToLastSaveSlot(); //return project to last backup

    void actionCloseProjectSlot(); //close opened project

    //----Edit actions----
    void actionUndoSlot();
    void actionRedoSlot();
    void actionOpenPreferencesSlot();

    //----View action----
    void actionSetDockOptionsSlot();

private:
    //void setupToolBar();
    void setupMenuBar();
    void setupDockWidgets();

    //QList<ToolBar*> _toolBars;
    QMenu *_fileMenu;
    QMenu *_editMenu;
    QMenu *_viewMenu;

    QMenu *_dockParametersMenu;

    CentralWindow* _centralWindow;

    QList<BaseDock*> _dockWidgets;
};
#endif // MAINWINDOW_H
