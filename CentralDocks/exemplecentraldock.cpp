#include "CentralDocks/exemplecentraldock.h"
#include "CentralDocks/titleBars/splittertitlebar.h"

#include <QCheckBox>
#include <QTextEdit>

ExempleCentralDock::ExempleCentralDock(const QString& title,
                                       QMainWindow *parent,
                                       Qt::WindowFlags flags)
    : BaseCentralDock(title, parent, flags)
{
    setMinimumSize(200, 200);
    _tEdit = new QTextEdit();

    _titlebar= new SplitterTitleBar(this);
    setTitleBarWidget(_titlebar);

    _topView = new QPushButton(tr("TOP"));
    _rightView = new QPushButton(tr("RIGHT"));
    _frontView = new QPushButton(tr("FRONT"));
    _usePerspectiveProj = new QCheckBox(tr("Use perspective"));

    _hLayout = new QHBoxLayout();
    _hLayout->addWidget(_topView);
    _hLayout->addWidget(_rightView);
    _hLayout->addWidget(_frontView);
    _hLayout->addWidget(_usePerspectiveProj);

    _vLayout = new QVBoxLayout();
    _vLayout->setMargin(0);
    _vLayout->addLayout(_hLayout);
    _vLayout->addWidget(_tEdit);

    mainWidget = new QWidget(this);
    mainWidget->setLayout(_vLayout);
    setWidget(mainWidget);
}
