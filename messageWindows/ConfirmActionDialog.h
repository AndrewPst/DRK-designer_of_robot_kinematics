#ifndef CONFIRMACTIONDIALOG_H
#define CONFIRMACTIONDIALOG_H

#include <QDialog>
#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QPushButton);
QT_FORWARD_DECLARE_CLASS(QComboBox);
QT_FORWARD_DECLARE_CLASS(QLineEdit);
QT_FORWARD_DECLARE_CLASS(QLabel);

class ConfirmActionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfirmActionDialog(const QString& = QString(), QWidget* parent = nullptr);

    void setMessage(const QString&);
    void setYesButtonText(const QString&);
    void setNoButtonText(const QString&);

private slots:

    void onCreateSelected();

private:

    QLabel *_message;

    QPushButton *_noBut, *_yesBut;

};

#endif // CONFIRMACTIONDIALOG_H
