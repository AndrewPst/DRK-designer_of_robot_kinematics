#include "version_t.h"

bool Version_t::operator==(const Version_t& v)
{
    return major == v.major && minor == v.minor && v.micro == micro && stage == v.stage;
}

QString Version_t::toStr()
{
    QString strStage;
    if(stage == VersionStage_t::VERSION_ALFA)
        strStage = "alfa";
    else if(stage == VersionStage_t::VERSION_BETA)
        strStage = "beta";
    else if(stage == VersionStage_t::VERSION_RELEASE_CANDIDATE)
        strStage = "Release";
    else if(stage == VersionStage_t::VERSION_PRODUCTION)
        strStage = "production";
    QString result = "%1.%2.%3 %4";
    result = result.arg(major);
    result = result.arg(minor);
    result = result.arg(micro);
    result = result.arg(strStage);
    return result;
}


