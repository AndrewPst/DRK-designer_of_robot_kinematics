#ifndef GLCENTRALDOCK_H
#define GLCENTRALDOCK_H

#include "basecentraldock.h"
#include "../openGL/glvisualizatorwidget.h"

namespace serialMan {

QT_FORWARD_DECLARE_CLASS(ManipulatorController)

class glCentralDock : public BaseCentralDock
{
public:
    glCentralDock(const QString& title, ManipulatorController* man,
                  QMainWindow *parent = nullptr,
                  Qt::WindowFlags flags = {});

public slots:

    void setProjectionModeSlot(QAction*);
    void setLookDirectionSlot(QAction*);
    void reverseDirecionSlot();

private slots:

    void onHSplit();
    void onVSplit();
    void onTabSplit();
    void onUpdate();

private:

    QWidget *_mainWidget;
    glVisualizatorWidget* _glWidget;

    QAction *_pModeOrtho, *_pModePers;
    QAction *_lookFromX, *_lookFromY, *_lookFromZ;
    QAction *_reverseDirection;

    ManipulatorController* _man;
};

}
#endif // GLCENTRALDOCK_H
