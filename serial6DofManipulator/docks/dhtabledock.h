#ifndef DHTABLEDOCK_H
#define DHTABLEDOCK_H

#include "basedock.h"

QT_FORWARD_DECLARE_CLASS(QTableWidget)
QT_FORWARD_DECLARE_CLASS(QVBoxLayout)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QDoubleSpinBox)

namespace serialMan {

QT_FORWARD_DECLARE_CLASS(ManipulatorController)

class DhTableDock : public BaseDock
{
    Q_OBJECT

private:

    void initTable();

public:

    explicit DhTableDock(const QString& title, ManipulatorController* man,
                         QWidget* parent = nullptr,
                         Qt::WindowFlags flags = {});

    Qt::DockWidgetArea getDefaultArea() const override;

private slots:

    void tableChanged();
    void changesApplied();

private:

    QWidget* _mainW;

    QVBoxLayout *_mainLayout;
    QTableWidget *_table;
    ManipulatorController* _man;

    QPushButton* _updateParamsButton;
};

}
#endif // DHTABLEDOCK_H
