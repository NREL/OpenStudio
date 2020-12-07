#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

namespace Nano
{

class Spin_Mutex
{
    std::atomic_bool locked = { false };

    public:

    inline void lock() noexcept
    {
        do
        {
            while (locked.load(std::memory_order_relaxed))
            {
                std::this_thread::yield();
            }
        }
        while (locked.exchange(true, std::memory_order_acquire));
    }

    inline void unlock() noexcept
    {
        locked.store(false, std::memory_order_release);
    }

    Spin_Mutex() noexcept = default;
    Spin_Mutex(Spin_Mutex const&) = delete;
    Spin_Mutex& operator= (Spin_Mutex const&) = delete;
};

//------------------------------------------------------------------------------

// Single Thread Policy
// Use this policy when you DO want performance but NO thread-safety!

class ST_Policy
{
    public:

    template <typename T, typename L>
    inline T const& copy_or_ref(T const& param, L&&) const
    {
        // Return a ref of param
        return param;
    }

    constexpr auto lock_guard() const
    {
        return false;
    }

    protected:

    using Weak_Ptr = ST_Policy*;

    constexpr auto weak_ptr()
    {
        return this;
    }

    constexpr auto observed(Weak_Ptr) const
    {
        return true;
    }

    constexpr auto visiting(Weak_Ptr observer) const
    {
        return (observer == this ? nullptr : observer);
    }

    constexpr auto unmask(Weak_Ptr observer) const
    {
        return observer;
    }

    constexpr void before_disconnect_all() const
    {

    }
};

//------------------------------------------------------------------------------

// Thread Safe Policy
// Use this policy when you DO want thread-safety but NO reentrancy!

template <typename Mutex = Spin_Mutex>
class TS_Policy
{
    mutable Mutex mutex;

    public:

    template <typename T, typename L>
    inline T const& copy_or_ref(T const& param, L&&) const
    {
        // Return a ref of param
        return param;
    }

    inline auto lock_guard() const
    {
        // All policies must implement the BasicLockable requirement
        return std::lock_guard<TS_Policy>(*const_cast<TS_Policy*>(this));
    }

    inline void lock() const
    {
        mutex.lock();
    }

    inline void unlock() noexcept
    {
        mutex.unlock();
    }

    protected:

    using Weak_Ptr = TS_Policy*;

    constexpr auto weak_ptr()
    {
        return this;
    }

    constexpr auto observed(Weak_Ptr) const
    {
        return true;
    }

    constexpr auto visiting(Weak_Ptr observer) const
    {
        return (observer == this ? nullptr : observer);
    }

    constexpr auto unmask(Weak_Ptr observer) const
    {
        return observer;
    }

    constexpr void before_disconnect_all() const
    {

    }
};

//------------------------------------------------------------------------------

// Single Thread Policy "Safe"
// Use this policy when you DO want reentrancy but NO thread-safety!

class ST_Policy_Safe
{
    public:

    template <typename T, typename L>
    inline T copy_or_ref(T const& param, L&&) const
    {
        // Return a copy of param
        return param;
    }

    constexpr auto lock_guard() const
    {
        return false;
    }

    protected:

    using Weak_Ptr = ST_Policy_Safe*;

    constexpr auto weak_ptr()
    {
        return this;
    }

    constexpr auto observed(Weak_Ptr) const
    {
        return true;
    }

    constexpr auto visiting(Weak_Ptr observer) const
    {
        return (observer == this ? nullptr : observer);
    }

    constexpr auto unmask(Weak_Ptr observer) const
    {
        return observer;
    }

    constexpr void before_disconnect_all() const
    {

    }
};

//------------------------------------------------------------------------------

// Thread Safe Policy "Safe"
// Use this policy when you DO want thread-safety AND reentrancy!

template <typename Mutex = Spin_Mutex>
class TS_Policy_Safe
{
    using Shared_Ptr = std::shared_ptr<TS_Policy_Safe>;

    Shared_Ptr tracker { this, [](...){} };
    mutable Mutex mutex;

    public:

    template <typename T, typename L>
    inline T copy_or_ref(T const& param, L&& lock) const
    {
        std::unique_lock<TS_Policy_Safe> unlock_after_copy = std::move(lock);
        // Return a copy of param and then unlock the now "sunk" lock
        return param;
    }

    inline auto lock_guard() const
    {
        // Unique_lock must be used in order to "sink" the lock into copy_or_ref
        return std::unique_lock<TS_Policy_Safe>(*const_cast<TS_Policy_Safe*>(this));
    }

    inline void lock() const
    {
        mutex.lock();
    }

    inline void unlock() noexcept
    {
        mutex.unlock();
    }

    protected:

    TS_Policy_Safe() noexcept = default;

    using Weak_Ptr = std::weak_ptr<TS_Policy_Safe>;

    inline Weak_Ptr weak_ptr() const
    {
        return tracker;
    }

    inline Shared_Ptr observed(Weak_Ptr const& observer) const
    {
        return std::move(observer.lock());
    }

    inline Shared_Ptr visiting(Weak_Ptr const& observer) const
    {
        // Lock the observer if the observer isn't tracker
        return observer.owner_before(tracker)
            || tracker.owner_before(observer) ? std::move(observer.lock()) : nullptr;
    }

    inline auto unmask(Shared_Ptr& observer) const
    {
        return observer.get();
    }

    inline void before_disconnect_all()
    {
        // Immediately create a weak ptr so we can "ping" for expiration
        auto ping = weak_ptr();
        // Reset the tracker and then ping for any lingering refs
        tracker.reset();
        // Wait for all visitors to finish their emissions
        do
        {
            while (!ping.expired())
            {
                std::this_thread::yield();
            }
        }
        while (ping.lock());
    }
};

} // namespace Nano ------------------------------------------------------------
