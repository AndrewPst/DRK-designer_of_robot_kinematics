#ifndef MANYBUTTONSDOCK_H
#define MANYBUTTONSDOCK_H

#include "basedock.h"

#include <QVBoxLayout>
#include <QPushButton>

namespace serialMan {


class ManyButtonsDock : public BaseDock
{
public:
    explicit ManyButtonsDock(const QString& title,
                             QWidget* parent = nullptr,
                             Qt::WindowFlags flags = {});

private slots:

    void actionAddButSlot();
    void actionRmButSlot();

private:

    QWidget *_mainW;
    QVBoxLayout* _mainL;

    QAction *addBut;
    QAction *rmBut;

    QList<QPushButton*> _buttons;
};

}
#endif // MANYBUTTONSDOCK_H
