#ifndef CENTRALWINDOW_H
#define CENTRALWINDOW_H

#include <QMainWindow>

QT_FORWARD_DECLARE_CLASS(BaseCentralDock);
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(BaseProjectController)

class CentralWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit CentralWindow(QWidget* parent = nullptr);

    void splitDockWidget(BaseCentralDock*, BaseCentralDock*, Qt::Orientation);
    void tabDockWidget(BaseCentralDock*, BaseCentralDock*);
    void addNewDockWidget(BaseCentralDock*);

public slots:

    void onProjectOpened(BaseProjectController* const);

private slots:



private:


};

#endif // CENTRALWINDOW_H
