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

#ifndef MODEL_NANO_OBSERVER_HPP
#define MODEL_NANO_OBSERVER_HPP

#include "nano_function.hpp"

namespace Nano
{

class Observer
{

    template <typename T> friend class Signal;

    struct DelegateKeyObserver { DelegateKey delegate; Observer* observer; };
    struct SSNode { DelegateKeyObserver data; SSNode* next; } *head = nullptr;

    //-----------------------------------------------------------PRIVATE METHODS

    void insert(DelegateKey const& key, Observer* obs)
    {
        head = new SSNode { { key, obs }, head };
    }

    void remove(DelegateKey const& key, Observer* obs)
    {
        SSNode* node = head;
        SSNode* prev = nullptr;
        // Only delete the first occurrence
        for ( ; node; prev = node, node = node->next)
        {
            if (node->data.delegate == key && node->data.observer == obs)
            {
                if (prev)
                {
                    prev->next = node->next;
                }
                else
                {
                    head = head->next;
                }
                delete node;
                break;
            }
        }
    }

    void removeAll()
    {
        for (auto node = head; node;)
        {
            auto temp = node;
            // If this is us we only need to delete
            if (this != node->data.observer)
            {
                // Remove this slot from this listening Observer
                node->data.observer->remove(node->data.delegate, this);
            }
            node = node->next;
            delete temp;
        }
        head = nullptr;
    }

    bool ss_isEmpty() const
    {
        return head == nullptr;
    }

    template <typename Delegate, typename... Uref>
    void onEach(Uref&&... args)
    {
        for (auto node = head, next = head; node; node = next)
        {
            next = node->next;
            // Perfect forward and emit
            Delegate(node->data.delegate)(std::forward<Uref>(args)...);
        }
    }

    template <typename Delegate, typename Accumulate, typename... Uref>
    void onEach_Accumulate(Accumulate&& accumulate, Uref&&... args)
    {
        for (auto node = head, next = head; node; node = next)
        {
            next = node->next;
            // Perfect forward, emit, and accumulate the return value
            accumulate(Delegate(node->data.delegate)(std::forward<Uref>(args)...));
        }
    }

    //-----------------------------------------------------------------PROTECTED

    protected:

    ~Observer()
    {
        removeAll();
    }

    //--------------------------------------------------------------------PUBLIC

    public:

    Observer() = default;
    Observer(const Observer& other) = delete; // non construction-copyable
    Observer& operator=(const Observer&) = delete; // non copyable

};

} // namespace Nano ------------------------------------------------------------

#endif // MODEL_NANO_OBSERVER_HPP
