#ifndef BASEPROJECTCONTROLLER_H
#define BASEPROJECTCONTROLLER_H

#include <QObject>

QT_FORWARD_DECLARE_CLASS(BaseDock)
QT_FORWARD_DECLARE_CLASS(BaseCentralDock)
QT_FORWARD_DECLARE_STRUCT(ProjectSource_t);

class BaseProjectController : public QObject
{
    Q_OBJECT

    typedef QString Version_t;

public:
    explicit BaseProjectController();

    virtual QList<BaseDock*> getAviableCentralDocks();
    virtual QList<BaseCentralDock*> getAviableDocks();

    QString getProjectName() const;
    void setProjectName(QString&);

    Version_t getVersion() const;
    void setVersion(Version_t&);

signals:

    void onRenamed(QString);
    void onVersionChanged(BaseProjectController::Version_t);

protected:

    ProjectSource_t* _projectSource;

};

#endif // BASEPROJECTCONTROLLER_H
