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
%}

%include <utilities/core/Deprecated.hpp>
%include <utilities/core/Containers.hpp>

%template(BoolVector) std::vector<bool>;
%template(UnsignedVector) std::vector<unsigned>;
%template(IntVector) std::vector<int>;
%template(DoubleVector) std::vector<double>;
%template(StringVector) std::vector<std::string>;
%template(StringVectorVector) std::vector<std::vector<std::string> >;

%template(UnsignedSet) std::set<unsigned>;
%template(IntSet) std::set<int>;
%template(DoubleSet) std::set<double>;
%template(StringSet) std::set<std::string>;


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
    #if defined(SWIGRUBY) 
      %alias isNull "empty?";
    #endif

  public:
    
    optional();
    optional(const T& t);
    optional(const boost::optional<T>& t);
    void reset();
#ifndef SWIGPYTHON    
    operator bool() const;
#endif    
    //bool operator!() const; // SWIG ignores this
//    T get();
//    T* operator->();
    T operator*();
    bool is_initialized() const;
   
    %extend {
      bool isNull() {
        return !(self->is_initialized());
      }
      
      T get() {
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
    explicit regex(const char* p);
    regex(const regex&);
    explicit regex(const std::string& p);
    ~regex();

    regex& operator=(const regex&);
    regex& operator= (const char* ptr);

    regex& operator= (const std::string& p);

    // capacity:
    unsigned size() const;
    unsigned max_size() const;
    bool empty() const;
    unsigned mark_count()const;
    //
    // modifiers:
    regex& assign(const regex& that);
    regex& assign(const char* ptr);
    regex& assign(const std::string& s);

    // const operations:
    int status()const;
    std::string str() const;
    int compare(regex&) const;
    // swap
    void swap(regex&) throw();
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
