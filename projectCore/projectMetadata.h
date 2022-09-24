#ifndef PROJECTMETADATA_H
#define PROJECTMETADATA_H

#include "projectGenerator.h"
#include "baseprojectcontroller.h"

struct ProjectMetadata
{
public:

    explicit ProjectMetadata(const QString& name, ProjectGeneratorFunc gen)
        : Name(name), Generator(gen){}//, Generator(gen){}

    const QString Name;

private:

    BaseProjectController* generate() const { return Generator(); }

    const ProjectGeneratorFunc Generator;

    friend class ProjectsManager;
};

#endif // PROJECTMETADATA_H
