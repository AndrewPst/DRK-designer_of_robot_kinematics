#ifndef CENTRALWINDOW_H
#define CENTRALWINDOW_H

#include <QMainWindow>

class BaseCentralDock;
QT_FORWARD_DECLARE_CLASS(QMenu)

class CentralWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit CentralWindow(QWidget* parent = nullptr);

    //---Get menu for toolbar---
    QMenu* getMenu();

public slots:

    void createNewDockSlot(bool); //Create new dock widget
    void splitDockSlot(BaseCentralDock* w, Qt::Orientation o); //Create new dock widget and split widget in arg
    void addTabDockSlot(BaseCentralDock* w); //Create new dock widget and adds it to tab

private slots:



private:
    void initMenu();


    BaseCentralDock *getNewDock(Qt::DockWidgetArea area); //Return new dock widget
    //BaseCentralDock *createDockByType(const QString&, const CentralDockTypes_t&, Qt::WindowFlags); //Creates a new widget by its type

    void splitDock(BaseCentralDock* w, Qt::Orientation o);//Divides the widget by orientation

private:

    QMenu* _menu;

    QAction *_createNewDock;
    QAction *_pinAllWidgets;
};

#endif // CENTRALWINDOW_H
