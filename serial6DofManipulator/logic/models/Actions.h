#ifndef ACTIONS_H
#define ACTIONS_H

#include "iaction.h"
#include <QThreadStorage>
#include <QDebug>
#include <QCache>

#include "executionEnivroment.h"

#include <vector>

namespace serialMan {

namespace actions
{

struct G0 : public serialMan::actions::IAction
{
    static const QVector<ArgDescription_t*> _keys;

    double _time = 1000;

    QVector<double> _startJointsPos;
    QVector<double> _endJointsPos;

public:

    ActionExectionResult startExecution(IArgsCollection&, const ExecutionEnivroment&) override;
    ActionExectionResult execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig config) override;
    void endExecution() override;

    static IAction* generate()
    {
        return new G0;
    }

    static constexpr const std::pair<char, uint16_t> static_key()
    {
        return {'G', 0};
    }

    const inline std::pair<char, uint16_t> key() override
    {
        return G0::static_key();
    }

    const static QVector<ArgDescription_t*>* allowArgs()
    {
        return &_keys;
    }

    static IArgsCollection* generateArgsCollection()
    {
        return new IArgsCollection({
                                       {'X', {0.0, false}},
                                       {'Y', {0.0, false}},
                                       {'Z', {0.0, false}},
                                       {'A', {0.0, false}},
                                       {'B', {0.0, false}},
                                       {'G', {0.0, false}},
                                       {'T', {1000.0, false}},
                                       {'P', {0.0, false}},
                                   });
    }
};

struct G1 : public serialMan::actions::IAction
{
    static const QVector<ArgDescription_t*> _keys;

    Position_t _startPos;
    Position_t _endPos;
    double _speed = 50;
    double _time = 0;

    double _effStartPos;
    double _effEndPos;

public:

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

    const static QVector<ArgDescription_t*>* allowArgs()
    {
        return &_keys;
    }

    static IArgsCollection* generateArgsCollection()
    {
        return new IArgsCollection({
                                       {'X', {0.0, false}},
                                       {'Y', {0.0, false}},
                                       {'Z', {0.0, false}},
                                       {'A', {0.0, false}},
                                       {'B', {0.0, false}},
                                       {'G', {0.0, false}},
                                       {'F', {50.0, false}},
                                       {'P', {0.0, false}},
                                   });
    }
};

struct G5 : public serialMan::actions::IAction
{
    static const QVector<ArgDescription_t*> _keys;

    QVector<double> _startJointsPos;
    QVector<double> _endJointsPos;

    double _speed {50};

public:

    ActionExectionResult startExecution(IArgsCollection&, const ExecutionEnivroment&) override;
    ActionExectionResult execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig config) override;
    void endExecution() override;

    static IAction* generate()
    {
        return new G5;
    }

    static constexpr const std::pair<char, uint16_t> static_key()
    {
        return {'G', 5};
    }

    const inline std::pair<char, uint16_t> key() override
    {
        return G5::static_key();
    }

    const static QVector<ArgDescription_t*>* allowArgs()
    {
        return &_keys;
    }

    static IArgsCollection* generateArgsCollection()
    {
        return new IArgsCollection({
                                       {'A', {0.0, false}},
                                       {'B', {0.0, false}},
                                       {'C', {0.0, false}},
                                       {'D', {0.0, false}},
                                       {'E', {0.0, false}},
                                       {'F', {0.0, false}},
                                       {'S', {50.0, false}},
                                       {'P', {0.0, false}},
                                   });
    }
};

struct G6 : public serialMan::actions::IAction
{
    static const QVector<ArgDescription_t*> _keys;

    double _time = 1000;

    QVector<double> _startJointsPos;
    QVector<double> _endJointsPos;

public:

    ActionExectionResult startExecution(IArgsCollection&, const ExecutionEnivroment&) override;
    ActionExectionResult execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig config) override;
    void endExecution() override;

    static IAction* generate()
    {
        return new G6;
    }

    static constexpr const std::pair<char, uint16_t> static_key()
    {
        return {'G', 6};
    }

    const inline std::pair<char, uint16_t> key() override
    {
        return G5::static_key();
    }

    const static QVector<ArgDescription_t*>* allowArgs()
    {
        return &_keys;
    }

    static IArgsCollection* generateArgsCollection()
    {
        return new IArgsCollection({
                                       {'A', {0.0, false}},
                                       {'B', {0.0, false}},
                                       {'C', {0.0, false}},
                                       {'D', {0.0, false}},
                                       {'E', {0.0, false}},
                                       {'F', {0.0, false}},
                                       {'T', {1000.0, false}},
                                       {'P', {0.0, false}},
                                   });
    }
};



struct G28 : public serialMan::actions::IAction
{
    static const QVector<ArgDescription_t*> _keys;

    double _time = 1000;

    QVector<double> _startJointsPos;
    QVector<double> _endJointsPos;

public:

    ActionExectionResult startExecution(IArgsCollection&, const ExecutionEnivroment&) override;
    ActionExectionResult execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig config) override;
    void endExecution() override;

    static IAction* generate()
    {
        return new G28;
    }

    static constexpr const std::pair<char, uint16_t> static_key()
    {
        return {'G', 28};
    }

    const inline std::pair<char, uint16_t> key() override
    {
        return G28::static_key();
    }

    const static QVector<ArgDescription_t*>* allowArgs()
    {
        return &_keys;
    }

    static IArgsCollection* generateArgsCollection()
    {
        return new IArgsCollection({
                                       {'X', {0.0, false}},
                                       {'Y', {0.0, false}},
                                       {'Z', {0.0, false}},
                                       {'A', {0.0, false}},
                                       {'B', {0.0, false}},
                                       {'G', {0.0, false}},
                                       {'T', {1000.0, false}},
                                   });
    }
};




struct GTEST : public serialMan::actions::IAction
{
private:
    static const QVector<ArgDescription_t*> _keys;
    double _param{1000};

public:

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

    const static QVector<ArgDescription_t*>* allowArgs()
    {
        return &_keys;
    }

    static IArgsCollection* generateArgsCollection()
    {
        return new IArgsCollection({{'T', {0.0, false}}});
    }
};
}
}
#endif // ACTIONS_H
