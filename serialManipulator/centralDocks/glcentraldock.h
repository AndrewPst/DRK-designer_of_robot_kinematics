#ifndef GLCENTRALDOCK_H
#define GLCENTRALDOCK_H

#include "basecentraldock.h"
#include "../openGL/glvisualizatorwidget.h"

namespace serialMan {

class glCentralDock : public BaseCentralDock
{
public:
    glCentralDock(const QString& title = "Visualization",
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

private:

    QWidget *_mainWidget;
    glVisualizatorWidget* _glWidget;

    QAction *_pModeOrtho, *_pModePers;
    QAction *_lookFromX, *_lookFromY, *_lookFromZ;
    QAction *_reverseDirection;
};

}
#endif // GLCENTRALDOCK_H
