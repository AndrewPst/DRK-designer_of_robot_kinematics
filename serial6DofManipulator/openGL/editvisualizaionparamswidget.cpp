#include "editvisualizaionparamswidget.h"

#include "QLabel"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include <QFormLayout>
#include <QSpinBox>
#include <QPushButton>

using namespace serialMan;

EditVisualizaionParamsWidget::EditVisualizaionParamsWidget(ProjectVisualizator& vis) : QDialog()
{
    setMinimumSize(400, 400);

    QFormLayout *fl = new QFormLayout();

    _vis = &vis;
    _params = vis.visualizationParams();

    spinFieldW = new QSpinBox();
    spinFieldW->setRange(1, 99999);
    fl->addRow("Field width", spinFieldW);

    spinFieldH = new QSpinBox();
    spinFieldH->setRange(1, 99999);
    fl->addRow("Field height", spinFieldH);

    spinTextSize = new QDoubleSpinBox();
    spinTextSize->setRange(1, 10);
    spinTextSize->setDecimals(1);
    fl->addRow("Text size", spinTextSize);

    spinStep = new QDoubleSpinBox();
    spinStep->setRange(0.5, 100);
    spinStep->setDecimals(1);
    fl->addRow("Step", spinStep);

    QPushButton *confirmBut = new QPushButton(tr("Confirm"));
    connect(confirmBut, &QPushButton::clicked, this, &serialMan::EditVisualizaionParamsWidget::confirmValues);
    QPushButton *resetBut = new QPushButton(tr("Reset"));
    connect(resetBut, &QPushButton::clicked, this, &serialMan::EditVisualizaionParamsWidget::resetValues);
    QPushButton *cancelBut = new QPushButton(tr("Cancel"));
    connect(cancelBut, &QPushButton::clicked, this, &serialMan::EditVisualizaionParamsWidget::close);


    QHBoxLayout* butl = new QHBoxLayout();
    butl->addWidget(cancelBut);
    butl->addWidget(resetBut);
    butl->addWidget(confirmBut);

    _mainl = new QVBoxLayout();
    _mainl->addLayout(fl);
    _mainl->addLayout(butl);
    setLayout(_mainl);

    resetValues();
}

void EditVisualizaionParamsWidget::resetValues()
{
    spinFieldH->setValue(_params.fieldHeight());
    spinFieldW->setValue(_params.fieldWidth());
    spinStep->setValue(_params.fieldStep());
    spinTextSize->setValue(_params.testSize());
}


void EditVisualizaionParamsWidget::confirmValues()
{
    _params.setFieldHeight(spinFieldH->value());
    _params.setFieldWidth(spinFieldW->value());
    _params.setFieldStep(spinStep->value());
    _params.setTestSize(spinTextSize->value());

    _vis->setVisualizationParams(_params);
    close();
}
