#ifndef VERSION_T_H
#define VERSION_T_H

class QString;

enum class VersionStage_t
{
    VERSION_ALFA = 0,
    VERSION_BETA,
    VERSION_RELEASE_CANDIDATE,
    VERSION_PRODUCTION
};

struct Version_t
{
    int major, minor, micro;
    VersionStage_t stage;


    QString toStr();
    bool operator==(const Version_t&);
};

#endif // VERSION_T_H
