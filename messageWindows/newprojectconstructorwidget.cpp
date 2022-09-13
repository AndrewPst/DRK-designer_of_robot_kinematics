#include "newprojectconstructorwidget.h"
#include "projectCore/projectsmanager.h"

#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

NewProjectConstructorWidget::NewProjectConstructorWidget(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Create new project"));

    setModal(true);

    setWindowFlags(windowFlags().setFlag(Qt::WindowContextHelpButtonHint, false));
    setFixedSize(minimumSize());


    QVBoxLayout * mainl = new QVBoxLayout(this);

    _projectName = new QLineEdit();
    _projectName->setWindowTitle(tr("Project name"));

    _projectName->setMaxLength(30);
    connect(_projectName, &QLineEdit::returnPressed, this, &NewProjectConstructorWidget::onCreateSelected);

    _availabledProjects = new QComboBox();
    auto map = projectsManager.getAvailablesControllers();
    Q_FOREACH(QString i, map)
    {
        _availabledProjects->addItem(i);
    }

    QHBoxLayout *bl = new QHBoxLayout();

    _createBut = new QPushButton();
    _createBut->setText(tr("Create"));
    _createBut->setDefault(true);
    connect(_createBut, &QPushButton::clicked, this, &NewProjectConstructorWidget::onCreateSelected);

    _canselBut = new QPushButton();
    _canselBut->setText(tr("Cansel"));
    connect(_canselBut, &QPushButton::clicked, this, &NewProjectConstructorWidget::close);


    bl->addWidget(_canselBut);
    bl->addWidget(_createBut);

    mainl->addWidget(_projectName);
    mainl->addWidget(_availabledProjects);
    mainl->addLayout(bl);

    setLayout(mainl);
}

void NewProjectConstructorWidget::onCreateSelected()
{
    setResult(true);
    hide();
}

QString NewProjectConstructorWidget::getName() const
{
    return _projectName->text();
}

ProjectType_t NewProjectConstructorWidget::getProjectType() const
{
    return projectsManager.getAvailablesControllers().key(_availabledProjects->currentText());
}
