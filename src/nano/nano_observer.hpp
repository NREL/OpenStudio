#pragma once

#include <algorithm>
#include <vector>

#include "nano_function.hpp"
#include "nano_mutex.hpp"

namespace Nano
{

template <typename MT_Policy = ST_Policy>
class Observer : private MT_Policy
{
    // Only Nano::Signal is allowed private access
    template <typename, typename> friend class Signal;

    struct Connection
    {
        Delegate_Key delegate;
        typename MT_Policy::Weak_Ptr observer;

        Connection() noexcept = default;
        Connection(Delegate_Key const& key) : delegate(key), observer() {}
        Connection(Delegate_Key const& key, Observer* obs) : delegate(key), observer(obs->weak_ptr()) {}
    };

    struct Z_Order
    {
        inline bool operator()(Delegate_Key const& lhs, Delegate_Key const& rhs) const
        {
            std::size_t x = lhs[0] ^ rhs[0];
            std::size_t y = lhs[1] ^ rhs[1];
            auto k = (x < y) && x < (x ^ y);
            return lhs[k] < rhs[k];
        }

        inline bool operator()(Connection const& lhs, Connection const& rhs) const
        {
            return operator()(lhs.delegate, rhs.delegate);
        }
    };

    std::vector<Connection> connections;

    //--------------------------------------------------------------------------

    void insert(Delegate_Key const& key, Observer* obs)
    {
        // auto lock = MT_Policy::lock_guard();

        auto begin = std::begin(connections);
        auto end = std::end(connections);

        connections.emplace(std::upper_bound(begin, end, key, Z_Order()), key, obs);
    }

    void remove(Delegate_Key const& key) noexcept
    {
        // auto lock = MT_Policy::lock_guard();

        auto begin = std::begin(connections);
        auto end = std::end(connections);

        auto slots = std::equal_range(begin, end, key, Z_Order());
        connections.erase(slots.first, slots.second);
    }

    //--------------------------------------------------------------------------

    template <typename Function, typename... Uref>
    void for_each(Uref&&... args)
    {
        auto lock = MT_Policy::lock_guard();

        for (auto const& slot : MT_Policy::copy_or_ref(connections, lock))
        {
            if (auto observer = MT_Policy::observed(slot.observer))
            {
                Function::bind(slot.delegate)(args...);
            }
        }
    }

    template <typename Function, typename Accumulate, typename... Uref>
    void for_each_accumulate(Accumulate&& accumulate, Uref&&... args)
    {
        auto lock = MT_Policy::lock_guard();

        for (auto const& slot : MT_Policy::copy_or_ref(connections, lock))
        {
            if (auto observer = MT_Policy::observed(slot.observer))
            {
                accumulate(Function::bind(slot.delegate)(args...));
            }
        }
    }

    //--------------------------------------------------------------------------

    public:

    void disconnect_all() noexcept
    {
        // auto lock = MT_Policy::lock_guard();

        for (auto const& slot : connections)
        {
            if (auto observer = MT_Policy::visiting(slot.observer))
            {
                static_cast<Observer*>(MT_Policy::unmask(observer))->remove(slot.delegate);
            }
        }
        connections.clear();
    }

    bool is_empty() const noexcept
    {
        auto lock = MT_Policy::lock_guard();

        return connections.empty();
    }

    protected:

    // Guideline #4: A base class destructor should be
    // either public and virtual, or protected and non-virtual.
    ~Observer()
    {
        MT_Policy::before_disconnect_all();

        disconnect_all();
    }

    Observer() noexcept = default;
    Observer(Observer const&) = delete;
    Observer& operator= (Observer const&) = delete;
};

} // namespace Nano ------------------------------------------------------------
