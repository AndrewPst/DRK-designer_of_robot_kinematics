#include "CentralDocks/titleBars/splittertitlebar.h"

#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QStyle>
#include <QSizePolicy>

SplitterTitleBar::SplitterTitleBar(QWidget *parent)
    : QWidget(parent)
{
    _parent = qobject_cast<QDockWidget*>(parent);

    setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed, QSizePolicy::TabWidget));

    _closeBut = new QPushButton();
    _minBut = new QPushButton();
    _splitHBut = new QPushButton(tr("-"));

    _splitHBut->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
                                          QSizePolicy::Fixed,
                                          QSizePolicy::ToolButton));
    _splitVBut = new QPushButton(tr("|"));
    _splitVBut->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
                                          QSizePolicy::Fixed,
                                          QSizePolicy::ToolButton));

    _addTabBut = new QPushButton(tr("TAB"));
    _addTabBut->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
                                          QSizePolicy::Fixed,
                                          QSizePolicy::ToolButton));

    connect(_closeBut, SIGNAL(clicked(bool)), this, SLOT(onCloseButPressed(bool)));
    connect(_minBut, SIGNAL(clicked(bool)), this, SLOT(onMinButPressed(bool)));

    connect(_splitHBut, SIGNAL(clicked(bool)), _parent, SLOT(splitHInto(bool)));
    connect(_splitVBut, SIGNAL(clicked(bool)), _parent, SLOT(splitVInto(bool)));
    connect(_addTabBut, SIGNAL(clicked(bool)), _parent, SLOT(addTabinto(bool)));

    minPix = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    _minBut->setIcon(QIcon(minPix));
    _minBut->setIconSize(minPix.size());
    _minBut->setFixedSize(20, 20);

    closePix  = style()->standardPixmap(QStyle::SP_DockWidgetCloseButton);
    _closeBut->setIcon(QIcon(closePix));
    _closeBut->setIconSize(closePix.size());
    _closeBut->setFixedSize(20, 20);

    _mainL = new QHBoxLayout();
    _mainL->setAlignment(Qt::AlignmentFlag::AlignRight);
    _mainL->setMargin(0);
    _mainL->setSpacing(0);
    _mainL->setDirection(QHBoxLayout::RightToLeft);
    _mainL->addWidget(_closeBut);
    _mainL->addWidget(_minBut);
    _mainL->addWidget(_splitHBut);
    _mainL->addWidget(_splitVBut);
    _mainL->addWidget(_addTabBut);

    setStyleSheet("background-color: red");

    setLayout(_mainL);
}


void SplitterTitleBar::onCloseButPressed(bool)
{
    _parent->close();
}

void SplitterTitleBar::onMinButPressed(bool)
{
    _parent->setFloating(!_parent->isFloating());
}


QSize SplitterTitleBar::minimumSizeHint() const
{
    Q_ASSERT(_parent != 0);
    QSize result(200, 20);
    if  (_parent->features() & QDockWidget::DockWidgetVerticalTitleBar)
        result.transpose();

    return  result;
}
