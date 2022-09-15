#include "messageWindows/ConfirmActionDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>


ConfirmActionDialog::ConfirmActionDialog(const QString& message,QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Confirm action"));

    setModal(true);

    setWindowFlags(windowFlags().setFlag(Qt::WindowContextHelpButtonHint, false));
    setFixedSize(minimumSize());


    QVBoxLayout * mainl = new QVBoxLayout(this);

    QHBoxLayout *bl = new QHBoxLayout();

    _yesBut = new QPushButton();
    _yesBut->setText(tr("Create"));
    _yesBut->setDefault(true);
    connect(_yesBut, &QPushButton::clicked, this, &ConfirmActionDialog::onCreateSelected);

    _noBut = new QPushButton();
    _noBut->setText(tr("Cansel"));
    connect(_noBut, &QPushButton::clicked, this, &ConfirmActionDialog::close);

    bl->addWidget(_noBut);
    bl->addWidget(_yesBut);

    _message = new QLabel();
    _message->setText(message);
    _message->setAlignment(Qt::AlignmentFlag::AlignCenter);

    mainl->addWidget(_message);
    mainl->addLayout(bl);
    setLayout(mainl);
}

void ConfirmActionDialog::setMessage(const QString& message)
{
    _message->setText(message);
}

void ConfirmActionDialog::setYesButtonText(const QString& message)
{
    _yesBut->setText(message);
}
void ConfirmActionDialog::setNoButtonText(const QString& message)
{
    _noBut->setText(message);
}

void ConfirmActionDialog::onCreateSelected()
{
    setResult(true);
    hide();
}
