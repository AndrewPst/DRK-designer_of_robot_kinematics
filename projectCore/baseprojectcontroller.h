#ifndef BASEPROJECTCONTROLLER_H
#define BASEPROJECTCONTROLLER_H

#include "basecentraldock.h"
#include "version_t.h"

#include <QObject>


QT_FORWARD_DECLARE_CLASS(BaseDock)
QT_FORWARD_DECLARE_STRUCT(ProjectSource_t);

//#include "projectCore/templateTisBase.h"


class BaseProjectController : public QObject
{
    Q_OBJECT

public:
    explicit BaseProjectController();

    const QList<BaseDock*>& getAviableDocks() const;
    const QList<BaseCentralDock*>& getAviableCentralDocks() const;
    QMenu* getEditTitlebarMenu() const;
    QMenu* getViewTitlebarMenu() const;

    template <typename T,
              typename = typename std::enable_if_t<std::is_base_of_v<BaseCentralDock, T>>>
    T * getNewCentralDock()
    {
        T* result = new T();
        _avaiableCentralDocks.append(result);
        return result;
    }

    void deleteCentralDock(BaseCentralDock*);


    QString getName() const;
    void setName(const QString&);

    Version_t getVersion() const;
    void setVersion(const Version_t&);

    virtual ~BaseProjectController();

signals:

    void onRenamed(QString);
    void onVersionChanged(Version_t);

protected:

    ProjectSource_t* _projectSource;

    //TEMP
    QString _projectName;
    Version_t _projectVersion;

    QList<BaseDock*> _avaiableDocks;
    QList<BaseCentralDock*> _avaiableCentralDocks;

    QMenu *_viewMenu, *_editMenu;

};

#endif // BASEPROJECTCONTROLLER_H
