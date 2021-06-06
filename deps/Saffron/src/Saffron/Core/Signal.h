﻿#pragma once

#include "Saffron/Core/UUID.h"
#include "Saffron/Core/TypeDefs.h"
#include "Saffron/Debug/Debug.h"

namespace Se
{
class BasicSignal
{
public:
    using TypeID = UUID;

public:
    BasicSignal() = default;

    virtual    ~BasicSignal() = default;

    /** Generate a UUID for a signal.
     * @return Signal UUID.
     */
    static TypeID GenerateTypeID();
};

template<typename t_ParamType>
class Signal : public BasicSignal
{
public:
    Signal() = default;

    Signal(const Signal &) = delete;

    Signal &operator=(const Signal &) = delete;

    Signal(Signal &&other)
            : _delagates(std::move(other._delagates))
    {
    }

    Signal &operator=(Signal &&other)
    {
        _delagates = std::move(other._delagates);
        return *this;
    }

    /** Connect to free function.
     * @param delegate Free function.
     * @return Connection serial, use for disconnecting.
     */
    unsigned int Connect(Function<void(t_ParamType)> delegate)
    {
        if (!_delagates)
        {
            _delagates.reset(new DelegateMap);
        }

        (*_delagates)[_serial] = delegate;
        return _serial++;
    }

    /** Disconnect signal.
     * @param serial Serial generated by Connect().
     */
    void Disconnect(unsigned int serial)
    {
        if (!_delagates)
        {
            return;
        }

        _delagates->erase(serial);

        if (_delagates->empty())
        {
            _delagates.reset();
        }
    }

    /** Emit.
     */
    void operator()(t_ParamType param) const
    {
        if (!_delagates)
        {
            return;
        }

        for (const auto &delegate : *_delagates)
        {
            delegate.second(param);
        }
    }


private:
    typedef Map<unsigned int, Function<void(t_ParamType)>> DelegateMap;
    std::unique_ptr<DelegateMap> _delagates;
    static Uint32 _serial;
};

template<>
class Signal<void> : public BasicSignal
{
public:
    Signal() = default;

    Signal(const Signal &) = delete;

    Signal &operator=(const Signal &) = delete;

    Signal(Signal &&other)
            : _delagates(std::move(other._delagates))
    {
    }

    Signal &operator=(Signal &&other)
    {
        _delagates = std::move(other._delagates);
        return *this;
    }

    /** Connect to free function.
     * @param delegate Free function.
     * @return Connection serial, use for disconnecting.
     */
    unsigned int Connect(Function<void()> delegate)
    {
        if (!_delagates)
        {
            _delagates.reset(new DelegateMap);
        }

        (*_delagates)[_serial] = delegate;
        return _serial++;
    }

    /** Disconnect signal.
     * @param serial Serial generated by Connect().
     */
    void Disconnect(unsigned int serial)
    {
        if (!_delagates)
        {
            return;
        }

        _delagates->erase(serial);

        if (_delagates->empty())
        {
            _delagates.reset();
        }
    }

    void operator()() const
    {
        if (!_delagates)
        {
            return;
        }

        for (const auto &delegate : *_delagates)
        {
            delegate.second();
        }
    }


private:
    typedef Map<unsigned int, Function<void()>> DelegateMap;
    std::unique_ptr<DelegateMap> _delagates;
    static Uint32 _serial;
};

template<typename t_ParamType>
Uint32 Signal<t_ParamType>::_serial = 0;


template<typename t_ParamType>
struct SignalAggregate
{
    using ParamType = t_ParamType;
    BasicSignal::TypeID TypeID;

    SignalAggregate() : TypeID(0ull)
    {}
};


/** SignalContainer
* Should only be used internally to store signals.
* Needed to reduce memory consumption
*/
class SignalContainer
{
public:
    /** Access signal.
     * @param signalAggregate
     * @return Reference to the requested signal.
     */
    template<typename t_ParamType>
    BasicSignal &operator[](SignalAggregate<t_ParamType> &signalAggregate)
    {
        if (!_signals)
        {
            _signals.reset(new SignalMap);
        }

        auto signalIter = _signals->find(signalAggregate.TypeID);

        if (signalIter == _signals->end())
        {
            signalAggregate.TypeID = BasicSignal::GenerateTypeID();
            signalIter = _signals->emplace(signalAggregate.TypeID, new Signal<t_ParamType>()).first;
        }

        return *signalIter->second;
    }

    /** Emit signal.
     * @param signalAggregate
     * @param param
     */
    template<typename t_ParamType, typename... Param>
    void Emit(const SignalAggregate<t_ParamType> &signalAggregate, Param &&... param)
    {

        if (!_signals || !signalAggregate.TypeID)
        {
            return;
        }

        const auto signalIter = _signals->find(signalAggregate.TypeID);

        if (signalIter != _signals->end())
        {
            auto *dyncastSignal = dynamic_cast<Signal<t_ParamType> *>(signalIter->second);
            Debug::Assert(dyncastSignal);
            (*dyncastSignal)(std::forward<Param>(param)...);
        }
    }


    void Clear();

private:
    typedef Map<BasicSignal::TypeID, BasicSignal *> SignalMap;
    std::unique_ptr<SignalMap> _signals;
};

/** Emit signal.
 * @param signalAggregate
 */
template<>
inline void SignalContainer::Emit(const SignalAggregate<void> &signalAggregate)
{
    if (!_signals || !signalAggregate.TypeID)
    {
        return;
    }

    const auto signalIter = _signals->find(signalAggregate.TypeID);

    if (signalIter != _signals->end())
    {
        auto *dyncastSignal = dynamic_cast<Signal<void> *>(signalIter->second);
        Debug::Assert(dyncastSignal);
        (*dyncastSignal)();
    }
}


}
