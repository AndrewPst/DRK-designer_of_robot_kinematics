#ifndef BASEDOCK_H
#define BASEDOCK_H

#include <QDockWidget>
#include <QObject>
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QString>

//this category of widgets is not deleted after closing

//Base class for  dock widgets in the peripheral screen
class BaseDock : public QDockWidget
{
    Q_OBJECT
public:
    explicit BaseDock(const QString& title,
                            QWidget* parent = nullptr,
                            Qt::WindowFlags flags = {}); //flags for window parameters

    //returns the menu to display in menubar
    QMenu* getMenu();

    //Set parameter '_drawAsWindow' for opening widget as window

    virtual Qt::DockWidgetArea getDefaultArea() const;

protected:
    void drawAsWindow(bool);
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void onTopLevelChanged(bool);


protected:
    bool _drawAsWindow;

    Qt::WindowFlags _flags;
    QMenu *_menu;


};

#endif // BASEDOCK_H
