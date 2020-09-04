Proposed Interface Changes
==========================

This is a list of interface changes that may be generally desirable, but cannot be done at just any old time because they would break the API.

```c++
// in
namespace openstudio {
class Attribute {
// change
  boost::optional<std::string> displayName() const;
// to
  std::string displayName() const;
};
}
```

