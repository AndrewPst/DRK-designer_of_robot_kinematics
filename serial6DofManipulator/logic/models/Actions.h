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

    struct G1ArgsCollection : public IArgsCollection
    {
    public:

        G1ArgsCollection(){
            _args = {
                {'X', {0.0, false}},
                {'Y', {0.0, false}},
                {'Z', {0.0, false}},
                {'A', {0.0, false}},
                {'B', {0.0, false}},
                {'G', {0.0, false}},
                {'F', {50.0, false}},
            };
        }
        ~G1ArgsCollection(){}
    };

    static const QVector<std::shared_ptr<ArgDescription_t>> _keys;

    Effector_t startPos;
    Effector_t endPos;
    double _speed = 2;
    double _time = 0;

public:

    void serializate(std::ostream& out)override;
    ActionExectionResult startExecution(IArgsCollection&, const ExecutionEnivroment&) override;
    ActionExectionResult execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig config) override;
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

    const static QVector<std::shared_ptr<ArgDescription_t>>* allowArgs()
    {
        return &_keys;
    }

    static IArgsCollection* generateArgsCollection()
    {
        return new G1ArgsCollection;
    }
};

struct GTEST : public serialMan::actions::IAction
{
public:
    struct GTESTArgsCollection : public IArgsCollection
    {
    public:
        GTESTArgsCollection()
        {
            _args = {{'T', {0.0, false}}};;
        }
        ~GTESTArgsCollection(){}
    };

private:
    static const QVector<std::shared_ptr<ArgDescription_t>> _keys;
    double _param{1000};

public:

    void serializate(std::ostream& out)override;
    ActionExectionResult startExecution(IArgsCollection&, const ExecutionEnivroment& env) override;
    ActionExectionResult execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig config) override;
    void endExecution() override;

    static IAction* generate()
    {
        return new GTEST;
    }

    static constexpr const std::pair<char, uint16_t> static_key()
    {
        return {'G', 255};
    }

    const inline std::pair<char, uint16_t> key() override
    {
        return GTEST::static_key();
    }

    const static QVector<std::shared_ptr<ArgDescription_t>>* allowArgs()
    {
        return &_keys;
    }

    static IArgsCollection* generateArgsCollection()
    {
        return new GTESTArgsCollection;
    }
};
}
}
#endif // ACTIONS_H
