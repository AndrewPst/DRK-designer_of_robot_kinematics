#ifndef GCODESERIALIZATOR_H
#define GCODESERIALIZATOR_H


#include "actionscontroller.h"

#include "QTextStream"

namespace serialMan
{

class gCodeSerializator
{
private:

    ActionsController& _act;

    struct ptr_hash {
        std::size_t operator () (const actions::ArgDescription_t* ptr) const {
            std::size_t h = (size_t)ptr;
            return h;
        }
    };

    typedef std::unordered_map<actions::ArgDescription_t*, QVariant, ptr_hash> argsbuf_t;

public:

    enum SerializationResult
    {
        SERIALIZATION_SUCCESSFUL=0,
        SERIALIZATION_ERROR,
    };


    gCodeSerializator(ActionsController&);

    SerializationResult serializate(QTextStream&, bool);
    SerializationResult deserializate(QTextStream&, bool);

};

}

#endif // GCODESERIALIZATOR_H
