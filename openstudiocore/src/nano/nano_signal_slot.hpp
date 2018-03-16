/**********************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********************************************************************/

#ifndef MODEL_NANO_SIGNAL_SLOT_HPP
#define MODEL_NANO_SIGNAL_SLOT_HPP

#include "nano_function.hpp"
#include "nano_observer.hpp"

namespace Nano
{

template <typename RT> class Signal;
template <typename RT, typename... Args>
class Signal<RT(Args...)> : private Observer
{
    template <typename T>
    void insert_sfinae(DelegateKey const& key, typename T::Observer* instance)
    {
        Observer::insert(key, instance);
        instance->insert(key, this);
    }
    template <typename T>
    void remove_sfinae(DelegateKey const& key, typename T::Observer* instance)
    {
        Observer::remove(key, instance);
        instance->remove(key, this);
    }
    template <typename T>
    void insert_sfinae(DelegateKey const& key, ...)
    {
        Observer::insert(key, this);
    }
    template <typename T>
    void remove_sfinae(DelegateKey const& key, ...)
    {
        Observer::remove(key, this);
    }

    public:

    using Delegate = Function<RT(Args...)>;

    //-------------------------------------------------------------------CONNECT

    template <typename L>
    void connect(L* instance)
    {
        Observer::insert(Delegate::template bind (instance), this);
    }
    template <typename L>
    void connect(L& instance)
    {
        connect(std::addressof(instance));
    }

    template <RT (* fun_ptr)(Args...)>
    void connect()
    {
        Observer::insert(Delegate::template bind<fun_ptr>(), this);
    }

    template <typename T, RT (T::* mem_ptr)(Args...)>
    void connect(T* instance)
    {
        insert_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }
    template <typename T, RT (T::* mem_ptr)(Args...) const>
    void connect(T* instance)
    {
        insert_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }

    template <typename T, RT (T::* mem_ptr)(Args...)>
    void connect(T& instance)
    {
        connect<T, mem_ptr>(std::addressof(instance));
    }
    template <typename T, RT (T::* mem_ptr)(Args...) const>
    void connect(T& instance)
    {
        connect<T, mem_ptr>(std::addressof(instance));
    }

    //----------------------------------------------------------------DISCONNECT

    template <typename L>
    void disconnect(L* instance)
    {
        Observer::remove(Delegate::template bind (instance), this);
    }
    template <typename L>
    void disconnect(L& instance)
    {
        disconnect(std::addressof(instance));
    }

    template <RT (* fun_ptr)(Args...)>
    void disconnect()
    {
        Observer::remove(Delegate::template bind<fun_ptr>(), this);
    }

    template <typename T, RT (T::* mem_ptr)(Args...)>
    void disconnect(T* instance)
    {
        remove_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }
    template <typename T, RT (T::* mem_ptr)(Args...) const>
    void disconnect(T* instance)
    {
        remove_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }

    template <typename T, RT (T::* mem_ptr)(Args...)>
    void disconnect(T& instance)
    {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }
    template <typename T, RT (T::* mem_ptr)(Args...) const>
    void disconnect(T& instance)
    {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }

    //----------------------------------------------------EMIT / EMIT ACCUMULATE

    #ifdef NANO_USE_DEPRECATED

    /// Will not benefit from perfect forwarding
    /// TODO [[deprecated]] when c++14 is comfortably supported

    void operator() (Args... args)
    {
        emit(std::forward<Args>(args)...);
    }
    template <typename Accumulate>
    void operator() (Args... args, Accumulate&& accumulate)
    {
        emit_accumulate<Accumulate>
            (std::forward<Accumulate>(accumulate), std::forward<Args>(args)...);
    }

    #endif

    template <typename... Uref>
    void nano_emit(Uref&&... args)
    {
        Observer::onEach<Delegate>(std::forward<Uref>(args)...);
    }

    template <typename Accumulate, typename... Uref>
    void emit_accumulate(Accumulate&& accumulate, Uref&&... args)
    {
        Observer::onEach_Accumulate<Delegate, Accumulate>
            (std::forward<Accumulate>(accumulate), std::forward<Uref>(args)...);
    }

    //-------------------------------------------------------------------UTILITY

    bool empty() const
    {
        return Observer::ss_isEmpty();
    }

    void removeAll()
    {
        Observer::removeAll();
    }

};

} // namespace Nano ------------------------------------------------------------

#endif // MODEL_NANO_SIGNAL_SLOT_HPP
