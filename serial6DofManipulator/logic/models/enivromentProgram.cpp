#include "enivromentProgram.h"


#include "QMutexLocker"

using namespace serialMan;


EnivromentProgram::EnivromentProgram(const actionsBuf_t& input)
    : _prog(input)
{
}

EnivromentProgram::EnivromentProgram(actionsBuf_t&& input)
    : _prog(std::move(input))
{
}

size_t EnivromentProgram::size() const
{
    QMutexLocker lock(&_progMutex);
    return _prog.size();
}

EnivromentProgram::actionType_t EnivromentProgram::at(size_t i)
{
    QMutexLocker lock(&_progMutex);
    return _prog.at(i);
}

void EnivromentProgram::add(EnivromentProgram::actionType_t act)
{
    QMutexLocker lock(&_progMutex);
    _prog.push_back(act);
}

void EnivromentProgram::insert(EnivromentProgram::actionType_t act, size_t i)
{
    QMutexLocker lock(&_progMutex);
    _prog.insert(_prog.begin()+i, act);
}

void EnivromentProgram::remove(EnivromentProgram::actionType_t act)
{
    QMutexLocker lock(&_progMutex);
    _prog.erase(std::find(_prog.begin(), _prog.end(), act));
}

EnivromentProgram::actionType_t EnivromentProgram::reset()
{
    QMutexLocker lock(&_progMutex);
    _executePos = _prog.begin();
    return *_executePos;
}

EnivromentProgram::actionType_t EnivromentProgram::next()
{
    QMutexLocker lock(&_progMutex);
    return *_executePos++;
}

uint8_t EnivromentProgram::setPos(size_t i)
{
    QMutexLocker lock(&_progMutex);
    if(i >= _prog.size())
        return 1;
    _executePos = _prog.begin() + i;
    return 0;
}

uint8_t EnivromentProgram::setPos(actionType_t act)
{
    QMutexLocker lock(&_progMutex);
    auto temp = std::find(_prog.begin(), _prog.end(), act);
    if (temp == _prog.end())
        return 1;
    _executePos = temp;
    return 0;
}
