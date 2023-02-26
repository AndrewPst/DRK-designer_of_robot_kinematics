#include "gcodeserializator.h"

#include "models/unitsConverter.h"

using namespace serialMan;

gCodeSerializator::gCodeSerializator(ActionsController& prog) : _act(prog)
{

}

gCodeSerializator::SerializationResult gCodeSerializator::serializate(QTextStream& out, bool restoreArgs)
{
    auto& prog = _act.enivroment().program();
    if(prog.size() == 0)
        return SerializationResult::SERIALIZATION_ERROR;
    std::shared_ptr<argsbuf_t> _argsBuf(new argsbuf_t());
    prog.reset();
    for(size_t i = 0; i < prog.size(); i++)
    {
        auto& act = prog.next();
        QString actKey = QString(act.first.first) + QString::number(act.first.second);
        out << actKey;
        auto allowArgs = _act.library().allowArgs(act.first);
        if(!allowArgs)
            continue;
        for(auto& arg : *allowArgs)
        {
            QVariant *value{nullptr};
            if(act.second->isArgUsable(arg->key) || restoreArgs)
            {
                out << ' ' << arg->key;
                if(act.second->isArgUsable(arg->key))
                {
                    act.second->getArg(arg->key, value);
                    if(!value)
                        return SerializationResult::SERIALIZATION_ERROR;
                    if(restoreArgs)
                    {
                        if(_argsBuf->find(arg) == _argsBuf->end())
                            _argsBuf->insert({arg, *value});
                        else
                            _argsBuf->at(arg) = *value;
                    }
                }
                else if(restoreArgs)
                {
                    if(_argsBuf->find(arg) == _argsBuf->end())
                    {
                        act.second->getArg(arg->key, value);
                        if(!value)
                            return SerializationResult::SERIALIZATION_ERROR;
                        _argsBuf->insert({arg, *value});
                    }else
                        value = &_argsBuf->at(arg);
                }

                if(!value)
                    return SerializationResult::SERIALIZATION_ERROR;
                if(arg->type == QVariant::Double)
                {
                    if(arg->parameters & actions::ActionArgumentParameter::ARGPARAM_IS_ANGLE)
                        out << radToDeg(value->toDouble());
                    else
                        out << value->toDouble();
                }
                else if(arg->type == QVariant::String)
                    out << value->toString();
            }
        }
        out << '\n';
    }
    return SerializationResult::SERIALIZATION_SUCCESSFUL;
}

gCodeSerializator::SerializationResult gCodeSerializator::deserializate(QTextStream& input, bool)
{
    QString buf;
    while(!input.atEnd())
    {
        input.readLineInto(&buf);
        qDebug() << buf;
    }
    return SerializationResult::SERIALIZATION_SUCCESSFUL;
}
