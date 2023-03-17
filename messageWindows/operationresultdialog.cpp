#include "operationresultdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

OperationResultDialog::OperationResultDialog(const QString& message,QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("Info"));
    setMinimumSize(300, 200);

    setModal(true);

    setWindowFlags(windowFlags().setFlag(Qt::WindowContextHelpButtonHint, false));
    setFixedSize(minimumSize());


    QVBoxLayout * mainl = new QVBoxLayout(this);

    _confirmButt = new QPushButton();
    _confirmButt->setText(tr("accept"));
    _confirmButt->setDefault(true);
    connect(_confirmButt, &QPushButton::clicked, this, &OperationResultDialog::close);

    _message = new QLabel();
    _message->setText(message);
    _message->setAlignment(Qt::AlignmentFlag::AlignCenter);

    mainl->addWidget(_message);
    mainl->addWidget(_confirmButt);
    setLayout(mainl);
}

void OperationResultDialog::setMessage(const QString& message)
{
    _message->setText(message);
}
