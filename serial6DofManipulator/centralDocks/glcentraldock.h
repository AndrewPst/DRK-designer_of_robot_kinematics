#ifndef GLCENTRALDOCK_H
#define GLCENTRALDOCK_H

#include "basecentraldock.h"
#include "../openGL/glvisualizatorwidget.h"

namespace serialMan {

QT_FORWARD_DECLARE_CLASS(ManipulatorController)

namespace centralDocks
{

class glCentralDock : public BaseCentralDock
{
public:
    glCentralDock(ManipulatorController& man,const QString& title = "Visualization",
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
    void onResetPos();

    void onOpenVisualizationSettings();

private:

    QWidget *_mainWidget;
    gl::glVisualizatorWidget* _glWidget;

    QAction *_pModeOrtho, *_pModePers;
    QAction *_lookFromX, *_lookFromY, *_lookFromZ;
    QAction *_reverseDirection;
    QAction *_openVisualizateSettings;

    ManipulatorController& _man;
};

}

}
#endif // GLCENTRALDOCK_H
