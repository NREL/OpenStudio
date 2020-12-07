nano-signal-slot
================

Pure C++17 Signals and Slots

#### Include
```
// #include "nano_function.hpp"         // Nano::Function, Nano::Delegate_Key
// #include "nano_mutex.hpp"            // Nano::Spin_Mutex, all threading policies
// #include "nano_observer.hpp"         // Nano::Observer
#include "nano_signal_slot.hpp"         // Nano::Signal
```

#### Declare
```
// Declare Nano::Signals using function signature syntax
Nano::Signal<bool(const char*)> signal_one;
Nano::Signal<bool(const char*, std::size_t)> signal_two;
```

#### Connect
```
// Connect member functions to Nano::Signals
signal_one.connect<&Foo::slot_member_one>(foo);
signal_two.connect<&Foo::slot_member_two>(foo);

// Connect overloaded member functions (required template syntax)
signal_one.connect<Foo, &Foo::slot_overloaded_member>(foo);
signal_two.connect<Foo, &Foo::slot_overloaded_member>(foo);

// Connect a static member function
signal_one.connect<&Foo::slot_static_member_one>();

// Connect a free function
signal_two.connect<&slot_free_function_one>();
```

#### Fire / Fire Accumulate

_Slot emission is not guaranteed to be in the order of connection._

```
// Fire Signals
signal_one.fire("we get signal");
signal_two.fire("main screen turn on", __LINE__);

std::vector<bool> statuses;
auto accumulator = [&](bool srv)
{
    statuses.push_back(srv);
};

// Fire Signals and accumulate SRVs (signal return values)
signal_one.fire_accumulate(accumulator, "how are you gentlemen");
```

#### Disconnect
```
// Disconnect member functions from Nano::Signals
signal_one.disconnect<&Foo::slot_member_one>(foo);
signal_two.disconnect<&Foo::slot_member_two>(foo);

// Disconnect overloaded member functions (required template syntax)
signal_one.disconnect<Foo, &Foo::slot_overloaded_member>(foo);

// Disconnect a static member function
signal_one.disconnect<&Foo::slot_static_member_one>();

// Disconnect a free function
signal_two.disconnect<&slot_free_function_one>();

// Disconnect all slots
signal_two.disconnect_all();
```

#### Connection Management

_Automatic connection management requires public inheritance from Nano::Observer<>._

```
struct Foo : public Nano::Observer<>
{
    bool slot_member_one(const char* sl) const
    {
        std::cout << sl << std::endl;
        return true;
    }
	...
```

#### Function Objects

**_Connected function objects must live longer than the connected signal._**
<br/>
_(be sure to disconnect the function object prior to it destructing)_

```
auto fo = [&](const char* sl)
{
    std::cout << sl << std::endl;
    return true;
};

...

// Connect any object that defines a suitable operator()
signal_one.connect(fo);

...

// Disconnect that same functor instance
signal_one.disconnect(fo);
```

#### Threading Policies

Nano-signal-slot has the following threading policies available for use:

| &nbsp; | ST_Policy | TS_Policy | ST_Policy_Safe | TS_Policy_Safe |
|:-------|:---------:|:---------:|:--------------:|:--------------:|
| Single threading only | X | - | X | - |
| Thread safe using mutex | - | X | - | X |
| Reentrant safe* | - | - | X | X |

_* Reentrant safety achieved using emission copying and reference counting._

#### Threading Policies - Aliases

When integrating nano-signal-slot, it is recommended to alias the Nano::Signal and Nano::Observer template classes.
<br/>
**When using a non-default Policy you must make sure that both Signal and Observer use the same policy.**

```
namespace Your_Namespace
{

// Creating aliases when using nano-signal-slot will increase the maintainability of your code
// especially if you are choosing to use the alternative policies.
template <typename Signatue>
using Your_Signal<Signature> = Nano::Signal<Signature, Nano::TS_Policy_Safe<>>;
using Your_Observer = Nano::Observer<Nano::TS_Policy_Safe<>>;

}
```

#### Threading Policies - Mutex Policy

Both the TS_Policy and TS_Policy_Safe allow for a provided mutex type if non-trivial locking is desired.
<br/>
_The provided mutex type only needs to implement the BasicLockable requirement._

```
using Your_Policy = Nano::TS_Policy<Your_Mutex>;
```

## Deadlock Disclaimer

The TS_Policy does not mitigate any deadlocks that could occur due to slot emissions fiddling with their signals.
Additionally, when using this policy, it is not safe to destruct connected Nano::Observers from different threads.
Generally if the threading posture in your application allows for it then use the TS_Policy for the performance.
If the lifetimes of Signals vs Observers is unknown or if the slots could be hostile then use the TS_Policy_Safe.

#### Links

| [Benchmark Results](https://github.com/NoAvailableAlias/signal-slot-benchmarks/tree/master/#signal-slot-benchmarks) | [Benchmark Algorithms](https://github.com/NoAvailableAlias/signal-slot-benchmarks/tree/master/#benchmark-algorithms) | [Unit Tests](/tests/#unit-tests) |
|:-------------------------------------------------------------------------------------------------------------------:|:--------------------------------------------------------------------------------------------------------------------:|:--------------------------------:|
<br/>
