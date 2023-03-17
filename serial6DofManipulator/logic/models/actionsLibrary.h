#ifndef ACTIONSLIBRARY_H
#define ACTIONSLIBRARY_H

#include "iaction.h"
#include "Actions.h"

namespace serialMan
{
namespace actions
{

struct ActionsLibrary
{
    friend class serialMan::ActionsController;
public:

    enum ActionsFunctions : uint8_t
    {
        FUNC_ACTION_GENERATOR = 0,
        FUNC_ARGS_COLLECTION_GENERATOR = 1,
        FUNC_ALLOW_ARGS_GETTER = 2
    };

    typedef serialMan::actions::IAction*(*ActionGenerator)();
    typedef serialMan::actions::IArgsCollection*(*ArgsCollectionsGenerator)();
    typedef const QVector<ArgDescription_t*>* allowArgs_t;

    typedef std::tuple<ActionGenerator, ArgsCollectionsGenerator, allowArgs_t> ActionsFuncs_t;

private:

    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator () (const std::pair<T1,T2> &p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ h2;
        }
    };

    const std::unordered_map<const actionIdentificator_t, ActionsFuncs_t, pair_hash> _actions
    {
        {actions::G0::static_key(), std::make_tuple(actions::G0::generate, actions::G0::generateArgsCollection, actions::G0::allowArgs())},
        {actions::G1::static_key(), std::make_tuple(actions::G1::generate, actions::G1::generateArgsCollection, actions::G1::allowArgs())},
        {actions::G5::static_key(), std::make_tuple(actions::G5::generate, actions::G5::generateArgsCollection, actions::G5::allowArgs())},
        {actions::G6::static_key(), std::make_tuple(actions::G6::generate, actions::G6::generateArgsCollection, actions::G6::allowArgs())},
        {actions::GTEST::static_key(), std::make_tuple(actions::GTEST::generate, actions::GTEST::generateArgsCollection, actions::GTEST::allowArgs())},
    };

public:

    ActionGenerator actionGenerator(const actionIdentificator_t&) const;
    ArgsCollectionsGenerator argsCollectionGenerator(const actionIdentificator_t&) const;
    const QVector<ArgDescription_t*>* allowArgs(const actionIdentificator_t&) const;
    const ActionsFuncs_t& allActionsFuncs(const actionIdentificator_t&) const;
    bool hasAction(const actionIdentificator_t&) const;

    template <typename _Conteiner>
    size_t allowActions(_Conteiner& out) const
    {
        auto it = _actions.begin();
        for(;it != _actions.end(); it++)
        {
            out.push_back(it->first);
        }
        return _actions.size();
    }

};

}
}
#endif // ACTIONSLIBRARY_H
