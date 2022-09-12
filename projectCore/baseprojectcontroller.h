#ifndef BASEPROJECTCONTROLLER_H
#define BASEPROJECTCONTROLLER_H

#include <QObject>

QT_FORWARD_DECLARE_CLASS(BaseDock)
QT_FORWARD_DECLARE_CLASS(BaseCentralDock)
QT_FORWARD_DECLARE_STRUCT(ProjectSource_t);
QT_FORWARD_DECLARE_STRUCT(QMenu);

class BaseProjectController : public QObject
{
    Q_OBJECT

    typedef QString Version_t;

public:
    explicit BaseProjectController();

    const QList<BaseDock*>& getAviableDocks() const;
    const QList<BaseCentralDock*>& getAviableCentralDocks() const;
    QMenu* getEditTitlebarMenu() const;
    QMenu* getViewTitlebarMenu() const;

    QString getName() const;
    void setName(QString&);

    Version_t getVersion() const;
    void setVersion(Version_t&);

    virtual ~BaseProjectController();

signals:

    void onRenamed(QString);
    void onVersionChanged(BaseProjectController::Version_t);

protected:

    ProjectSource_t* _projectSource;

    QList<BaseDock*> _avaiableDocks;
    QList<BaseCentralDock*> _avaiableCentralDocks;

    QMenu *_viewMenu, *_editMenu;

};

#endif // BASEPROJECTCONTROLLER_H
