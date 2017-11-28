nano-signal-slot
================

Pure C++11 Signals and Slots

#### Include
```
// #include "nano_function.hpp" // Nano::Function, Nano::DelegateKey
// #include "nano_observer.hpp" // Nano::Observer
#include "nano_signal_slot.hpp" // Nano::Signal / All the above
```

#### Declare
```
// Declare Nano::Signals using function signature syntax
Nano::Signal<bool(const char*)> signal_one;
Nano::Signal<bool(const char*, std::size_t)> signal_two;
```

#### Connect

```
// Connect member functions to Nano::signals;
signal_one.connect<Foo, &Foo::handler_a>(&foo);
signal_two.connect<Foo, &Foo::handler_b>(&foo);

// Connect a static member function
signal_one.connect<Foo::handler_c>();

// Connect a free function
signal_two.connect<handler_d>();
```

#### Emit / Emit Accumulate

```
// Emit Signals
signal_one.emit("we get signal");
signal_two.emit("main screen turn on", __LINE__);

std::vector<bool> status;

// Emit Signals and accumulate SRVs (signal return values)
signal_one.emit_accumulate([&](bool srv)
{
    status.push_back(srv);
}
,"how are you gentlemen");
```

#### Disconnect

_Additionally test convenience overloads for references._

```
// Disconnect member functions from Nano::Signals
signal_one.disconnect<Foo, &Foo::handler_a>(foo);
signal_two.disconnect<Foo, &Foo::handler_b>(foo);

// Disconnect a static member function
signal_one.disconnect<Foo::handler_c>();

// Disconnect a free function
signal_two.disconnect<handler_d>();
```

#### Connection Management

_To utilize automatic connection management you must inherit from Nano::Observer._

```
struct Foo : public Nano::Observer
{
    bool handler_a(const char* sl) const
    {
        std::cout << sl << std::endl;
        return true;
    }
	...
```

#### Function Objects

_*Must guarantee that object lifetimes are compatible.*_

```
// Test using function objects
auto fo = [&](const char* sl)
{
    std::cout << sl << std::endl;
    return true;
};

...

// Connecting function objects (or any object defining a suitable operator())
signal_one.connect(&fo);

...

// Disconnecting function objects (convenience overload is used here)
signal_one.disconnect(fo);
```

#### Links

| [Performance](https://github.com/NoAvailableAlias/signal-slot-benchmarks/tree/master/#performance) | [Metrics](https://github.com/NoAvailableAlias/signal-slot-benchmarks/tree/master/#metrics) | [Benchmark Algorithms](https://github.com/NoAvailableAlias/signal-slot-benchmarks/tree/master/#benchmark-algorithms) |
|:-------------------------------------------------------------------------------------------------- |:------------------------------------------------------------------------------------------:|:--------------------------------------------------------------------------------------------------------------------:|
