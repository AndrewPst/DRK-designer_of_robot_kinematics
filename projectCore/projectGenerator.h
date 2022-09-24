#ifndef PROJECTGENERATOR_H
#define PROJECTGENERATOR_H

#include "baseprojectcontroller.h"

#define GENERATE(type) []() -> BaseProjectController* { return new type; }

typedef std::function<BaseProjectController*()> ProjectGeneratorFunc;

//template <typename T>
//class ProjectGenerator
//{
//public:

//    explicit ProjectGenerator()
//    {

//    }

//private:

//    inline BaseProjectController* create() {return new T;}


//    friend class ProjectsManager;
//};

#endif // PROJECTGENERATOR_H
