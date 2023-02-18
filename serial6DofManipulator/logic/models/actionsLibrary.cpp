#include "actionsLibrary.h"

using namespace serialMan;
using namespace actions;

ActionsLibrary::ActionGenerator ActionsLibrary::actionGenerator(const actionIdentificator_t& i) const
{
    return std::get<ActionsFunctions::FUNC_ACTION_GENERATOR>(_actions.at(i));
}

ActionsLibrary::ArgsCollectionsGenerator ActionsLibrary::argsCollectionGenerator(const actionIdentificator_t& i) const
{
    return std::get<ActionsFunctions::FUNC_ARGS_COLLECTION_GENERATOR>(_actions.at(i));
}

const QVector<ArgDescription_t>* ActionsLibrary::allowArgs(const actionIdentificator_t& i) const
{
    return std::get<ActionsFunctions::FUNC_ALLOW_ARGS_GETTER>(_actions.at(i));
}

const ActionsLibrary::ActionsFuncs_t& ActionsLibrary::allActionsFuncs(const actionIdentificator_t& i) const
{
    return _actions.at(i);
}
