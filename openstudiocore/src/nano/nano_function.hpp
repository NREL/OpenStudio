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

#ifndef MODEL_NANO_FUNCTION_HPP
#define MODEL_NANO_FUNCTION_HPP

#include <cstdint>
#include <array>

namespace Nano
{

using DelegateKey = std::array<std::uintptr_t, 2>;

template <typename RT> class Function;
template <typename RT, typename... Args>
class Function<RT(Args...)>
{
    using Thunk = RT (*)(void*, Args...);

    friend class Observer;

    void* m_this_ptr; // instance pointer
    Thunk m_stub_ptr; // free function pointer

    Function(void* this_ptr, Thunk stub_ptr) :
        m_this_ptr(this_ptr), m_stub_ptr(stub_ptr) {}

    Function(DelegateKey delegate_key) :
        m_this_ptr(reinterpret_cast<void*>(delegate_key[0])),
        m_stub_ptr(reinterpret_cast<Thunk>(delegate_key[1])) {}

    public:

    template <RT (*fun_ptr) (Args...)>
    static inline Function bind()
    {
        return { nullptr, [](void* /*NULL*/, Args... args)
            { return (*fun_ptr)(std::forward<Args>(args)...); } };
    }
    template <typename T, RT (T::* mem_ptr) (Args...)>
    static inline Function bind(T* pointer)
    {
        return { pointer, [](void* this_ptr, Args... args)
            { return (static_cast<T*>(this_ptr)->*mem_ptr) (std::forward<Args>(args)...); } };
    }
    template <typename T, RT (T::* mem_ptr) (Args...) const>
    static inline Function bind(T* pointer)
    {
        return { pointer, [](void* this_ptr, Args... args)
            { return (static_cast<T*>(this_ptr)->*mem_ptr) (std::forward<Args>(args)...); } };
    }
    template <typename L>
    static inline Function bind(L* pointer)
    {
        return { pointer, [](void *this_ptr, Args... args)
            { return (static_cast<L*>(this_ptr)->operator()(std::forward<Args>(args)...)); }};
    }
    inline operator DelegateKey() const
    {
        return
        {{
            reinterpret_cast<std::uintptr_t>(m_this_ptr),
            reinterpret_cast<std::uintptr_t>(m_stub_ptr)
        }};
    }
    template <typename... Uref>
    inline RT operator() (Uref&&... args)
    {
        return (*m_stub_ptr)(m_this_ptr, std::forward<Uref>(args)...);
    }
};

} // namespace Nano ------------------------------------------------------------

#endif // MODEL_NANO_FUNCTION_HPP
