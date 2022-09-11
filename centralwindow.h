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

    //---Get menu for toolbar---
    QMenu* getMenu();

public slots:

    void onProjectOpened(BaseProjectController*);

private slots:



private:
    void initMenu();

private:

    QMenu* _menu;

};

#endif // CENTRALWINDOW_H
