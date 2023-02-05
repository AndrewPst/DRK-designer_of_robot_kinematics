#include "dhtabledock.h"
#include "../logic/manipulatorcontroller.h"
#include "../logic/models/units_t.h"

#include <math.h>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>
#include <QLineEdit>
#include <QDoubleSpinBox>

using namespace serialMan;


DhTableDock::DhTableDock(ManipulatorController& man, const QString& title ,
                         QWidget* parent,
                         Qt::WindowFlags flags)
    : BaseDock(title, parent, flags),  _man(man)
{
    setMinimumSize(200, 200);

    drawAsWindow(true);
    _updateParamsButton = new QPushButton();

    initTable();

    _updateParamsButton->setText(tr("Apply changes"));
    _updateParamsButton->setEnabled(false);
    connect(_updateParamsButton, SIGNAL(clicked(bool)), this, SLOT(changesApplied()));

    _mainW = new QWidget();
    _mainLayout = new QVBoxLayout();
    _mainLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    _mainW->setLayout(_mainLayout);

    _mainLayout->addWidget(_table);
    _mainLayout->addWidget(_updateParamsButton);

    setWidget(_mainW);
}

void DhTableDock::initTable()
{
    _table = new QTableWidget();
    _table->setRowCount(ManipulatorController::DEFAULT_DOF);
    _table->setColumnCount(4);
    _table->setSelectionMode(QAbstractItemView::NoSelection);

    QStringList horHeader;
    horHeader << "Theta" << "Alfa" << "R" << "D";
    _table->setHorizontalHeaderLabels(horHeader);
    _table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    auto& dh = _man.getDHTable();
    for(int i = 0; i < ManipulatorController::DEFAULT_DOF; i++)
    {
        QDoubleSpinBox* thetaSpin = new QDoubleSpinBox();
        thetaSpin->setSuffix(" Degrees");
        connect(thetaSpin, SIGNAL(valueChanged(double)), this, SLOT(tableChanged()));
        thetaSpin->setRange(-180, 180);
        thetaSpin->setSingleStep(15);
        thetaSpin->setValue(radToDeg(dh.theta[i]));
        thetaSpin->setAlignment(Qt::AlignmentFlag::AlignCenter);
        _table->setCellWidget(i, 0, thetaSpin);

        QDoubleSpinBox* alfaSpin = new QDoubleSpinBox();
        alfaSpin->setSuffix(" Degrees");
        connect(alfaSpin, SIGNAL(valueChanged(double)), this, SLOT(tableChanged()));
        alfaSpin->setRange(-180, 180);
        alfaSpin->setSingleStep(15);
        alfaSpin->setValue(radToDeg(dh.alfa[i]));
        alfaSpin->setAlignment(Qt::AlignmentFlag::AlignCenter);
        _table->setCellWidget(i, 1, alfaSpin);

        QDoubleSpinBox* rSpin = new QDoubleSpinBox();
        rSpin->setSuffix(" mm");
        connect(rSpin, SIGNAL(valueChanged(double)), this, SLOT(tableChanged()));
        rSpin->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
        rSpin->setValue(dh.r[i]);
        rSpin->setAlignment(Qt::AlignmentFlag::AlignCenter);
        _table->setCellWidget(i, 2, rSpin);

        QDoubleSpinBox* dSpin = new QDoubleSpinBox();
        dSpin->setSuffix(" mm");
        connect(dSpin, SIGNAL(valueChanged(double)), this, SLOT(tableChanged()));
        dSpin->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
        dSpin->setValue(dh.d[i]);
        dSpin->setAlignment(Qt::AlignmentFlag::AlignCenter);
        _table->setCellWidget(i, 3, dSpin);
    }
}

void DhTableDock::tableChanged()
{
    _updateParamsButton->setEnabled(true);
}

void DhTableDock::changesApplied()
{
    DHTable_t<ManipulatorController::DEFAULT_DOF> newDH;
    for(int i = 0; i < ManipulatorController::DEFAULT_DOF; i++)
    {
        newDH.theta[i] = degToRad(qobject_cast<QDoubleSpinBox*>(_table->cellWidget(i, 0))->value());
        newDH.alfa[i] = degToRad(qobject_cast<QDoubleSpinBox*>(_table->cellWidget(i, 1))->value());
        newDH.r[i] = qobject_cast<QDoubleSpinBox*>(_table->cellWidget(i, 2))->value();
        newDH.d[i] = qobject_cast<QDoubleSpinBox*>(_table->cellWidget(i, 3))->value();
    }
    _man.setDHTable(std::move(newDH));
    _updateParamsButton->setEnabled(false);
}



Qt::DockWidgetArea DhTableDock::getDefaultArea() const
{
    return Qt::DockWidgetArea::BottomDockWidgetArea;
}
