#ifndef ACTIONS_H
#define ACTIONS_H

#include "iaction.h"
#include <QThreadStorage>
#include <QDebug>
#include <QCache>

#include "executionEnivroment.h"


namespace serialMan {

namespace actions
{

struct G1 : public serialMan::actions::IAction
{

    constexpr static const char* _cmd1 = "G1";
    constexpr static const char _separator = ' ';

    static const QVector<ArgKey_t> _keys;

    Effector_t endPos;
    Effector_t argsSpeed;

    double _speed = 2;
    double _dist, _time = 0;

public:

    void serializate(std::ostream& out)override;
    void startExecution(const ExecutionEnivroment& env) override;
    ActionExectionResult_t execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig_t config) override;
    void endExecution() override;

    static IAction* generate()
    {
        return new G1;
    }

    static constexpr const std::pair<char, uint16_t> static_key()
    {
        return {'G', 1};
    }

    const inline std::pair<char, uint16_t> key() override
    {
        return G1::static_key();
    }

    const static QVector<ArgKey_t>* allowArgs()
    {
        return &_keys;
    }
};

}

}
#endif // ACTIONS_H
