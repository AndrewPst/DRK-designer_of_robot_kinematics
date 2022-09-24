#ifndef NEWPROJECTCONSTRUCTORWIDGET_H
#define NEWPROJECTCONSTRUCTORWIDGET_H

//#include "projectCore/projectType.h"

#include <QDialog>
#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QPushButton);
QT_FORWARD_DECLARE_CLASS(QComboBox);
QT_FORWARD_DECLARE_CLASS(QLineEdit);

class NewProjectConstructorWidget : public QDialog
{
    Q_OBJECT
public:
    explicit NewProjectConstructorWidget(QWidget* parent = nullptr);

    QString getName() const;
    int getProjectType() const;

private slots:

    void onCreateSelected();

private:

    QComboBox *_availabledProjects;
    QLineEdit *_projectName;

    QPushButton* _createBut, *_canselBut;

};

#endif // NEWPROJECTCONSTRUCTORWIDGET_H
