#include "gcodeserializator.h"

//#include "models/unitsConverter.h"

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
                    value = act.second->getArg(arg->key);
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
                        value = act.second->getArg(arg->key);
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

gCodeSerializator::SerializationResult gCodeSerializator::deserializate(QTextStream& input, bool restoreArgs)
{
    QString buf;
    _act.enivroment().program().clear();
    std::shared_ptr<argsbuf_t> _argsBuf(new argsbuf_t());
    _act.enivroment().program().blockSignals(true);
    while(!input.atEnd())
    {
        input.readLineInto(&buf);
        if(buf.size() == 0)
            continue;
        buf = buf.simplified();
        auto list = buf.split(' ');
        actions::actionIdentificator_t id;
        id.first =  list[0][0].toLatin1();
        id.second = list[0].midRef(1).toUInt();

        if(_act.library().hasAction(id) == false)
            continue;

        auto actionArgs = std::shared_ptr<actions::IArgsCollection>(_act.library().argsCollectionGenerator(id)());
        auto allowArgs = _act.library().allowArgs(id);
        std::unordered_map<actions::ArgKey_t, uint16_t> argsMap;
        for(auto i = list.begin()+1; i != list.end(); i++)
        {
            actions::ArgKey_t key = i->at(0).toLatin1();
            if(argsMap.count(key) != 0)
                argsMap.at(key) = i - list.begin();
            else
                argsMap.insert({key, i - list.begin()});
        }
        for(const auto &i : *allowArgs)
        {
            QVariant value;
            if(argsMap.count(i->key) == 0)
            {
                if(_argsBuf->find(i) != _argsBuf->end() && restoreArgs)
                {
                    actionArgs->setArg(i->key, _argsBuf->at(i));
                }

            } else
            {
                if(i->type == QVariant::Double)
                    value = list[argsMap.at(i->key)].midRef(1).toDouble();
                else if(i->type == QVariant::String)
                    value = list[argsMap.at(i->key)].midRef(1).string();
                if(restoreArgs)
                {
                    if(_argsBuf->find(i) == _argsBuf->end())
                    {
                        _argsBuf->insert({i, value});
                    }else
                        _argsBuf->at(i) = value;
                }
                actionArgs->setArg(i->key, value);
            }
        }
        _act.enivroment().program().add({id, actionArgs});
    }
    _act.enivroment().program().blockSignals(false);
    emit _act.enivroment().program().structureChanged();
    return SerializationResult::SERIALIZATION_SUCCESSFUL;
}
