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
private:

    friend class serialMan::ActionsController
    ActionsLibrary();

public:

    typedef std::pair<char, uint16_t> actionIdentificator_t;

    typedef serialMan::actions::IAction*(*ActionGenerator)();
    typedef const QVector<ArgKey_t>* allowArgs_t;

    const std::map<const actionIdentificator_t, std::tuple<ActionGenerator, allowArgs_t, const QString>> actions
    {
        {actions::G1::static_key(), std::make_tuple(actions::G1::generate, actions::G1::allowArgs(), "Linear movement")},
        {actions::GTEST::static_key(), std::make_tuple(actions::GTEST::generate, actions::GTEST::allowArgs(), "Test")},
    };
};

}
}
#endif // ACTIONSLIBRARY_H
