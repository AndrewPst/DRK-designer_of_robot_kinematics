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

//struct G1 : public serialMan::actions::IAction
//{

//    constexpr static const char* _cmd1 = "G1";
//    constexpr static const char _separator = ' ';

//    static const QVector<ArgKey_t> _keys;

//    Effector_t endPos;
//    Effector_t argsSpeed;

//    double _speed = 2;
//    double _dist, _time = 0;

//public:

//    void serializate(std::ostream& out)override;
//    ActionExectionResult_t startExecution(const ExecutionEnivroment& env) override;
//    ActionExectionResult_t execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig_t config) override;
//    void endExecution() override;

//    static IAction* generate()
//    {
//        return new G1;
//    }

//    static constexpr const std::pair<char, uint16_t> static_key()
//    {
//        return {'G', 1};
//    }

//    const inline std::pair<char, uint16_t> key() override
//    {
//        return G1::static_key();
//    }

//    const static QVector<ArgKey_t>* allowArgs()
//    {
//        return &_keys;
//    }
//};

struct GTEST : public serialMan::actions::IAction
{
public:
    struct GTESTArgsCollection : public IArgsCollection
    {
    private:
        std::unordered_map<ArgKey_t, Arg_t> _args
        {{'T', Arg_t(QVariant(0.0))}};
    public:

        GTESTArgsCollection(){}

        bool getValue(ArgKey_t key, Arg_t& out) const override
        {
            (void)out;
            if(_args.find(key) == _args.end())
                return false;
            auto a = _args.at(key);
//            Arg_t<double> t = a;
            //auto t = static_cast<Arg_t<double&>>(a);
            if(a.isUseble() == false)
                return false;
            out = a;
            return true;

        }
        void setValue(ArgKey_t key, const Arg_t& value) override
        {
            if(_args.find(key) == _args.end())
                return;
            _args[key] = value;
        }
        ~GTESTArgsCollection(){}
    };

private:
    static const QVector<ArgDescription_t> _keys;
    double _param{1000};

public:

    void serializate(std::ostream& out)override;
    ActionExectionResult_t startExecution(const IArgsCollection&, const ExecutionEnivroment& env) override;
    ActionExectionResult_t execute(const ExecutionEnivroment& env, qint64 t, ExecuteConfig_t config) override;
    void endExecution() override;

//    void setArg(const ArgKey_t& key, const QVariant) override;
//    bool getArg(const ArgKey_t& key, QVariant& result) const override;

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

    const static QVector<ArgDescription_t>* allowArgs()
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
