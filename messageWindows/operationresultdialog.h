#ifndef OPERATIONRESULTDIALOG_H
#define OPERATIONRESULTDIALOG_H

#include <QDialog>
#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QPushButton);
QT_FORWARD_DECLARE_CLASS(QComboBox);
QT_FORWARD_DECLARE_CLASS(QLineEdit);
QT_FORWARD_DECLARE_CLASS(QLabel);


class OperationResultDialog : public QDialog
{
    Q_OBJECT

public:
    OperationResultDialog(const QString& = QString(), QWidget* parent = nullptr);

    void setMessage(const QString&);

private:
    QLabel* _message;
    QPushButton* _confirmButt;

};

#endif // OPERATIONRESULTDIALOG_H
