#ifndef BASECENTRALDOCK_H
#define BASECENTRALDOCK_H

#include <QObject>
#include <QDockWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenu>

//Base class for dock widget in central window
class BaseCentralDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit BaseCentralDock(const QString& title,
                                   QMainWindow* parent = nullptr,
                                   Qt::WindowFlags flags = {});

    virtual Qt::DockWidgetArea getDefaultArea() const;

    QMenu *getMenu();
//---Translate signals from Titlebar to parent window
signals:

    void onCreateDock(bool); //Create new independent widget
    void onSplitDock(BaseCentralDock*, Qt::Orientation); //Split this widget
    void onTabDock(BaseCentralDock*); //Add a new widget to a tab



public slots:

    void splitHInto(bool);
    void splitVInto(bool);
    void addTabinto(bool);

//---Events---
private slots:

    void closeWidgetSlot(bool);
    void onTopLevelChanged(bool);

//---Private methods
private:


//---Events
private:
    void closeEvent(QCloseEvent*) override;
    void changeEvent(QEvent*) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

protected:
    QWidget *_titleWidget;
    Qt::WindowFlags _flags;
    QMainWindow *_parentWindow;

    QMenu *_menu;
};

#endif // BASECENTRALDOCK_H
