#ifndef PROGRAMDOCK_H
#define PROGRAMDOCK_H

#include "basedock.h"

#include <QVBoxLayout>
#include <QPushButton>

class ProgramDock  : public BaseDock
{
    Q_OBJECT
public:

    explicit ProgramDock(const QString& title,
                             QWidget* parent = nullptr,
                             Qt::WindowFlags flags = {});
private:

    QWidget *_mainW;
    QVBoxLayout* _mainL;

    QPushButton *_startBut;

};

#endif // PROGRAMDOCK_H
