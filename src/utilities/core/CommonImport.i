#ifndef UTILITIES_COMMONIMPORT_I
#define UTILITIES_COMMONIMPORT_I

// first include, exception handler will be used for everything
%include <utilities/core/Exception.i>

%{
  #include <sstream>
  #include <utilities/core/Deprecated.hpp>
  #include <utilities/core/Containers.hpp>
  #include <boost/regex.hpp>
  #include <boost/variant.hpp>
  #include <pugixml.hpp>

#if defined SWIGRUBY
  extern "C" {
  #ifdef HAVE_RUBY_ENCODING_H
    #include "ruby/encoding.h"
  #endif

    rb_encoding *rb_utf8_encoding(void);
    ID rb_intern3(const char *name, long len, rb_encoding *enc);
  }
#endif
%}

%include <utilities/core/Deprecated.hpp>
%include <utilities/core/Containers.hpp>

%template(BoolVector) std::vector<bool>;
%template(UnsignedVector) std::vector<unsigned>;
%template(IntVector) std::vector<int>;
%template(LongVector) std::vector<long>;
%template(DoubleVector) std::vector<double>;
%template(StringVector) std::vector<std::string>;
%template(StringVectorVector) std::vector<std::vector<std::string> >;
%template(IntVectorVector) std::vector<std::vector<int> >;

%template(StringStringMap) std::map<std::string, std::string>;
%template(StringIntMap) std::map<std::string, int>;

%template(UnsignedSet) std::set<unsigned>;
%template(IntSet) std::set<int>;
%template(DoubleSet) std::set<double>;
%template(StringSet) std::set<std::string>;

%template(StringPair) std::pair<std::string, std::string>;
%template(StringPairVector) std::vector<std::pair<std::string, std::string> >;
%template(StringPairSet) std::set<std::pair<std::string, std::string> >;

%template(StringUIntPair) std::pair<std::string, unsigned int>;
%template(StringUIntPairVector) std::vector<std::pair<std::string, unsigned int> >;
%template(UIntPair) std::pair<unsigned int, unsigned int>;

// boost::optional wrapper
%{
  #include <boost/optional.hpp>
  #include <boost/any.hpp>
%}

%rename(Any) boost::any;

namespace boost {

  class any {
  public:
    %extend {
      std::string toString()
      {
        return boost::any_cast<std::string>(*self);
      }

      int toInt()
      {
        return boost::any_cast<int>(*self);
      }

      unsigned toUnsigned()
      {
        return boost::any_cast<unsigned>(*self);
      }

      float toFloat()
      {
        return boost::any_cast<float>(*self);
      }

      double toDouble()
      {
        return boost::any_cast<double>(*self);
      }

      std::string __str__() const{
        return boost::any_cast<std::string>(*self);
      }

      int __int__() const{
        return boost::any_cast<int>(*self);
      }

      float __float__() const{
        return boost::any_cast<float>(*self);
      }
    }
  };

  template <class T>
  class optional {
    #ifdef SWIGRUBY
      %alias isNull "empty?";
    #endif

  public:

    optional();
    optional(const T& t);
    optional(const boost::optional<T>& t);
    void reset();
    operator bool() const;


    // bool operator!() const; // SWIG ignores this
    // T get();
    // T* operator->();
    // T value_or(T& default_value);

    T operator*();
    bool is_initialized() const;

    %extend {
      bool isNull() const {
        return !(self->is_initialized());
      }

#ifdef SWIGPYTHON
      bool empty() const {
        return !(self->is_initialized());
      }
#endif

      T get() const {
        if (self->is_initialized())
        {
          return self->get();
        } else {
          throw std::runtime_error("Optional not initialized");
        }
      }

      void set(const T &t) {
        (*self) = t;
      }

      T value_or(const T& default_value) {
        if (self->is_initialized()) {
          return self->get();
        } else {
          return default_value;
        }
      }

    }
  };

  // Stubbing of std::function
  template<typename R>
  class function0
  {
    R operator()();
  };

  template<typename R, typename P1>
  class function1
  {
    R operator()(P1);
  };

  template<typename R, typename P1, typename P2>
  class function2
  {
    R operator()(P1, P2);
  };

};

// create instantiations of the optional classes
%template(OptionalInt) boost::optional<int>;
%template(OptionalBool) boost::optional<bool>;
%template(OptionalUnsigned) boost::optional<unsigned>;
%template(OptionalDouble) boost::optional<double>;
%template(OptionalString) boost::optional<std::string>;

// this is only for execAndReturnVectorOfString in SqlFile
// deprecate if possible
%template(OptionalStringVector) boost::optional<std::vector<std::string> >;
%template(OptionalDoubleVector) boost::optional<std::vector<double> >;
%template(OptionalIntVector) boost::optional<std::vector<int> >;

%extend boost::optional<int>{
  std::string __str__() const{
    std::ostringstream os;
    if (self->is_initialized()){
      os << self->get();
    }
    return os.str();
  }
  int __int__() const{
    int result = 0;
    if (self->is_initialized()){
      result = static_cast<int>(self->get());
    }
    return result;
  }
  float __float__() const{
    float result = 0;
    if (self->is_initialized()){
      result = static_cast<float>(self->get());
    }
    return result;
  }
};

%extend boost::optional<unsigned>{
  std::string __str__() const{
    std::ostringstream os;
    if (self->is_initialized()){
      os << self->get();
    }
    return os.str();
  }
  int __int__() const{
    int result = 0;
    if (self->is_initialized()){
      result = static_cast<int>(self->get());
    }
    return result;
  }
  float __float__() const{
    float result = 0;
    if (self->is_initialized()){
      result = static_cast<float>(self->get());
    }
    return result;
  }
};

%extend boost::optional<double>{
  std::string __str__() const{
    std::ostringstream os;
    if (self->is_initialized()){
      os << self->get();
    }
    return os.str();
  }
  int __int__() const{
    int result = 0;
    if (self->is_initialized()){
      result = static_cast<int>(self->get());
    }
    return result;
  }
  float __float__() const{
    float result = 0;
    if (self->is_initialized()){
      result = static_cast<float>(self->get());
    }
    return result;
  }
};

%extend boost::optional<std::string>{
  std::string __str__() const{
    if (self->is_initialized()){
      return self->get();
    }
    return "";
  }
};

%extend boost::optional<std::wstring>{
  std::wstring __str__() const{
    if (self->is_initialized()){
      return self->get();
    }
    return L"";
  }
};

// boost::regex wrapper
%{
  #include <boost/regex.hpp>
%}

%rename(Regex) boost::regex;

namespace boost
{
  class regex
  {
   public:
    explicit regex();
    // C# not happy about overloads, so just leave the std::string one
  #ifndef SWIGCSHARP
    explicit regex(const char* p);
  #endif
    regex(const regex&);
    explicit regex(const std::string& p);
    ~regex();

    regex& operator=(const regex&);
  #ifndef SWIGCSHARP
    regex& operator= (const char* ptr);
  #endif
    regex& operator= (const std::string& p);

    // capacity:
    unsigned size() const;
    unsigned max_size() const;
    bool empty() const;
    unsigned mark_count()const;
    //
    // modifiers:
    regex& assign(const regex& that);
  // C# not happy about overloads, so just leave the std::string one
  #ifndef SWIGCSHARP
    regex& assign(const char* ptr);
  #endif
    regex& assign(const std::string& s);

    // const operations:
    int status()const;
    std::string str() const;
    int compare(regex&) const;
    // swap
    void swap(regex&) noexcept;
  };
}

namespace boost
{
  template<typename T1, typename T2, typename T3, typename T4>
  class variant
  {
  };
}


%template(OptionalRegex) boost::optional<boost::regex>;

// pugi
namespace pugi
{
  class xml_node{
  public:
    xml_node();
  };

  class xml_document{
  public:
    xml_document();
  private:
    xml_document(const xml_document&);
  };
}


#ifdef SWIGCSHARP

%rename(OStream) std::ostream;
%rename(IStream) std::istream;

// Stub out streams placed here instead of LanguageSpecific.i because it needs to be
// in the set of imports.
%nodefaultctor std::ostream;
%nodefaultctor std::istream;
namespace std {
  class ostream
  {
  };

  class istream
  {
  };
}
#endif

#endif // UTILITIES_COMMONIMPORT_I
