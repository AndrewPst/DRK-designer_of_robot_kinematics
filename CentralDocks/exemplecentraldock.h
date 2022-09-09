#ifndef EXEMPLECENTRALDOCK_H
#define EXEMPLECENTRALDOCK_H

#include "basecentraldock.h"
#include "CentralDocks/titleBars/splittertitlebar.h"

class QCheckBox;
class QTextEdit;

#include <QMainWindow>

class ExempleCentralDock : public BaseCentralDock
{
public:
    explicit ExempleCentralDock(const QString& title,
                                QMainWindow *parent = nullptr,
                                Qt::WindowFlags flags = {});
private:

    QWidget *mainWidget;

    QTextEdit* _tEdit;
    QHBoxLayout* _hLayout;
    QVBoxLayout* _vLayout;

    QCheckBox *_usePerspectiveProj;
    QPushButton *_topView, *_rightView, *_frontView;

    SplitterTitleBar *_titlebar;

};

#endif // EXEMPLECENTRALDOCK_H
