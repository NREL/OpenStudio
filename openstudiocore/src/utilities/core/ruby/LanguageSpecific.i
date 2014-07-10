#ifndef UTILITIES_RUBY_LANGUAGESPECIFIC_I
#define UTILITIES_RUBY_LANGUAGESPECIFIC_I

%include <std_set.i>

%header %{

// The ruby SWIG wrappers have lots of unused variables which makes GCC rather chatty.
// This cleans it up.
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

%}

// always define this for Ruby, undoes some crummy defines in ruby.h
%{
  #ifdef accept
  #undef accept
  #endif
  #ifdef close
  #undef close
  #endif
  #ifdef times
  #undef times
  #endif
  #ifdef Sleep
  #undef Sleep
  #endif
  #ifdef sleep
  #undef sleep
  #endif
  #ifdef bind
  #undef bind
  #endif
  #ifdef rename
  #undef rename
  #endif
  #ifdef connect
  #undef connect
  #endif
  #ifdef shutdown
  #undef shutdown
  #endif
  #ifdef mkdir
  #undef mkdir
  #endif
  #ifdef select
  #undef select
  #endif
  #ifdef ALLOC
  #undef ALLOC
  #endif
  #ifdef _
  #undef _
  #endif
  #ifdef StringValue
  #undef StringValue
  #endif
%}

// wrap wstring
%fragment("SWIG_AsWCharPtrAndSize","header",fragment="<wchar.h>",fragment="SWIG_pwchar_descriptor") {
SWIGINTERN int
SWIG_AsWCharPtrAndSize(VALUE obj, wchar_t **cptr, size_t *psize, int *alloc)
{
  if (TYPE(obj) == T_STRING) {
    %#if defined(StringValuePtr)
    char *cstr = StringValuePtr(obj);
    %#else
    char *cstr = STR2CSTR(obj);
    %#endif

  std::string tempStr1(cstr);
    std::wstring tempStr(tempStr1.begin(), tempStr1.end());

    size_t size = tempStr.size() + 1;
    if (cptr)  {
      if (alloc) {
                *cptr = %new_copy_array(tempStr.c_str(), size, wchar_t);
                *alloc = SWIG_NEWOBJ;
      }
    }
    if (psize) *psize = size;
    return SWIG_OK;
  } else {
    swig_type_info* pwchar_descriptor = SWIG_pwchar_descriptor();
    if (pwchar_descriptor) {
      void* vptr = 0;
      if (SWIG_ConvertPtr(obj, &vptr, pwchar_descriptor, 0) == SWIG_OK) {
        if (cptr) *cptr = (wchar_t *)vptr;
        if (psize) *psize = vptr ? (wcslen((wchar_t*)vptr) + 1) : 0;
        if (alloc) *alloc = SWIG_OLDOBJ;
        return SWIG_OK;
      }
    }
  }
  return SWIG_TypeError;
}
}

%fragment("SWIG_FromWCharPtrAndSize","header",fragment="<wchar.h>",fragment="SWIG_pwchar_descriptor") {
SWIGINTERNINLINE VALUE
SWIG_FromWCharPtrAndSize(const wchar_t * carray, size_t size)
{
  if (carray) {

  std::wstring tempStr1(carray);
    std::string tempStr(tempStr1.begin(), tempStr1.end());

    if (tempStr.size() > LONG_MAX) {
      swig_type_info* pwchar_descriptor = SWIG_pwchar_descriptor();
      return pwchar_descriptor ?
                SWIG_NewPointerObj(%const_cast(carray,wchar_t *), pwchar_descriptor, 0) : Qnil;
    } else {
      return rb_str_new(tempStr.c_str(), %numeric_cast(tempStr.size(),long));
    }
  } else {
    return Qnil;
  }
}
}


// std::shared_ptr wrapper, the current macros in shared_ptr.i are not defined for Ruby
// this has to be missing a lot but seems to work
namespace std {

    template <class T>
    class shared_ptr {
        #if defined(SWIGRUBY) || defined(SWIGMZSCHEME) || defined(SWIGGUILE)
          %rename("nil?") isNull;
        #endif

      public:

    // constructor from pointer
    %apply SWIGTYPE *DISOWN {T* t}; // the shared_ptr now owns the object
    shared_ptr (T* t);
    void reset();
    %clear T *t; // clear apply type

    // dereference
//        T* operator->();
        T* get() const;

        #if defined(SWIGPYTHON)
        %extend {
            bool __nonzero__() {
                return !!(*self);
            }
        }
        #else
        %extend {
            bool isNull() {
                return !(*self);
            }
        }
        #endif
    };
};


// boost::shared_ptr wrapper, the current macros in shared_ptr.i are not defined for Ruby
// this has to be missing a lot but seems to work
%{
  #include <boost/smart_ptr.hpp>
%}
namespace boost {

    template <class T>
    class shared_ptr {
        #if defined(SWIGRUBY) || defined(SWIGMZSCHEME) || defined(SWIGGUILE)
          %rename("nil?") isNull;
        #endif

      public:

    // constructor from pointer
    %apply SWIGTYPE *DISOWN {T* t}; // the shared_ptr now owns the object
    shared_ptr (T* t);
    void reset();
    %clear T *t; // clear apply type

    // dereference
//        T* operator->();
        T* get() const;

        #if defined(SWIGPYTHON)
        %extend {
            bool __nonzero__() {
                return !!(*self);
            }
        }
        #else
        %extend {
            bool isNull() {
                return !(*self);
            }
        }
        #endif
    };
};

%rename(multiplyEqual) operator *=;



#endif // UTILITIES_RUBY_LANGUAGESPECIFIC_I
