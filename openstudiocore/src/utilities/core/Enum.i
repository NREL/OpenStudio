#ifndef UTILITIES_CORE_ENUM_I
#define UTILITIES_CORE_ENUM_I

%{
  #include <boost/optional.hpp>
  #include <utilities/core/Enum.hpp>
%}

%include <utilities/core/EnumBase.hpp>

#if defined(SWIGRUBY)
%define ENUM_CONVERSION(_name) 
%init %{
  rb_eval_string("String.class_eval { define_method(:to_" #_name ") { " SWIG_name "::" #_name ".new(self); } }");
%}
%enddef

#else
#define ENUM_CONVERSION(_name)
#endif



// todo: we need to be able to get rid of default constructor below, however
// i can't instantiate optional class template without default constructor
#define OPENSTUDIO_ENUM(_name, ...) \
  class _name { \
  public: \
    _name(); \
    _name(int v); \
    _name(const std::string &n); \
    std::string valueName() const; \
    int value() const; \
    std::string valueDescription() const; \
    \
    bool operator==(const _name& other) const; \
    bool operator!=(const _name& other) const; \
    bool operator>(const _name& other) const; \
    bool operator>=(const _name& other) const; \
    bool operator<(const _name& other) const; \
    bool operator<=(const _name& other) const; \
    \
    static std::string enumName(); \
    static std::set<int> getValues(); \
  };  \
  ENUM_CONVERSION(_name);
  

#endif // UTILITIES_CORE_ENUM_I
