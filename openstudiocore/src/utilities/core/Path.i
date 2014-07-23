#ifndef UTILITIES_CORE_PATH_I
#define UTILITIES_CORE_PATH_I

// DLM@20100105: I would rather do conversions between our p and the native Ruby, Python, etc path classes
// same thing for Date/Time, etc

%template(OptionalPath) boost::optional<openstudio::path>;
%template(PathVector) std::vector<openstudio::path>;
%template(PathPair) std::pair<openstudio::path, openstudio::path>;
%template(PathPairVector) std::vector<std::pair<openstudio::path, openstudio::path> >;

%{
  #include <utilities/core/Path.hpp>
  #include <utilities/core/PathHelpers.hpp>
  #include <utilities/core/String.hpp>
  
  namespace openstudio{
//    #ifdef _WINDOWS
//      typedef wchar_t PathCharType;
//      typedef std::wstring PathStringType;
//    #else
//      typedef char PathCharType;
//      typedef std::string PathStringType;
//    #endif
    
    // does the path exist
    bool exists(const path& p){
      return boost::filesystem::exists(p);
    }
    
    // Attempts to create the directory dir resolves to
    bool create_directory(const path& dir){
      return boost::filesystem::create_directory(dir);
    }

    // Removes the file p
    bool remove(const path& p){
      return boost::filesystem::remove(p);
    }
    
    // Recursively deletes the contents of p if it exists, then deletes file p itself.
    // Returns the number of files removed.
    unsigned long remove_all(const path& p){
      return boost::filesystem::remove_all(p);
    }
    
    // The contents and attributes of the file from_fp resolves to are copied to the file to_fp resolves to.
    void copy_file(const path& from_path, const path& to_path){
      boost::filesystem::copy_file(from_path, to_path, boost::filesystem::copy_option::overwrite_if_exists);
    }
    
    // complete the path and return an absolute path, behavior is portable
    path complete(const path& p, const path& base=path()){
      return boost::filesystem::complete(p, base);
    }
    
    // complete the path according to system rules and return an absolute path, behavior is not portable
    path system_complete(const path& p){
      return boost::filesystem::system_complete(p);
    }
  
  }
%}

namespace openstudio {

//  #ifdef _WINDOWS
//    typedef wchar_t PathCharType;
//    typedef std::wstring PathStringType;
//  #else
//    typedef char PathCharType;
//    typedef std::string PathStringType;
//  #endif
  
  class path{
  public:
    path();
    //path(const PathStringType& p);
    //path(const PathCharType* p);
    path(const std::string& p);
    path(const char* p);
    path(const path& p);
    virtual ~path();
    
    //path& operator=(const PathStringType& s);
    path& operator=(const std::string& s);
    path& operator=(const path& p);
    
    //path& operator/=(const PathStringType& s);
    path& operator/=(const std::string& s);
    path& operator/=(const path& p);
    
    void clear();
    void swap(path& rhs);
    path& remove_filename();
    //path& replace_extension(const PathStringType& new_extension = PathStringType());
    path& replace_extension(const std::string& new_extension = "");
    
    //PathStringType string() const;
    //PathStringType file_string() const;
    //PathStringType directory_string() const;
    //PathStringType external_file_string() const;
    path string() const;

    path root_path() const;
    //PathStringType root_name() const;
    //PathStringType root_directory() const;

    path relative_path() const;
    path parent_path() const;
    //PathStringType filename() const;
    //PathStringType stem() const;
    //PathStringType extension() const;
    path filename() const;
    path stem() const;
    path extension() const;
    
    bool empty() const;
    bool is_complete() const;
    bool has_root_path() const;
    bool has_root_name() const;
    bool has_root_directory() const;
    bool has_relative_path() const;
    bool has_filename() const;
    bool has_parent_path() const;
  };
  
  // path to a temporary directory. 
  path tempDir();
  
  // path to std::string.
  // DLM: deprecate in favor of path.to_s
  std::string toString(const path& p);

  // path to QString.
  // DLM: deprecate 
  QString toQString(const path& p);

  // UTF-8 encoded char* to path
  // DLM: deprecate
  path toPath(const char* s);

  // UTF-8 encoded std::string to path
  // DLM: deprecate in favor of Path.new(string)
  path toPath(const std::string& s);

  // QString to path
  // DLM: deprecate
  path toPath(const QString& q);  
  
  // does the path exist
  bool exists(const path& p);
  
  // Attempts to create the directory dir resolves to
  bool create_directory(const path& dir);

  // Removes the file p
  bool remove(const path& p);
  
  // Recursively deletes the contents of p if it exists, then deletes file p itself.
  // Returns the number of files removed.
  unsigned long remove_all(const path& p);
  
  // The contents and attributes of the file from_fp resolves to are copied to the file to_fp resolves to.
  void copy_file(const path& from_path, const path& to_path);
  
  // complete the path and return an absolute path, behavior is portable
  path complete(const path& p, const path& base=path());
  
  // complete the path according to system rules and return an absolute path, behavior is not portable
  path system_complete(const path& p);

  %extend path{
  
    // append to path
    path __div__(const path& other) const{
      return (*self) / other;
    }
    
    // to std::string
    std::string __str__() const{
      return toString(*self);
    }
  
    //#ifdef SWIGRUBY
    //  #ifdef _WINDOWS
    //    // constructor from std::string 
    //    path(const std::string& s){
    //      path *p;
    //      p = new path(toPath(s));
    //      return p;
    //    }      
    //  #endif
    //#endif
   
    #ifdef SWIGJAVASCRIPT
        path append(const path &other) const {
          return (*self) / other;
        }

      #ifdef _WINDOWS
          // constructor from std::string 
          path(const std::string& s){
            path *p;
            p = new path(toPath(s));
            return p;
          }
      #endif
    #endif 
  };
  
  #ifdef SWIGRUBY

    %typemap(in) (path) {
    
      // check if input is a path already
      void *vptr = 0;
      int res = SWIG_ConvertPtr($input, &vptr, $&1_descriptor, 0);
      if (SWIG_IsOK(res)) {
        if (vptr) {
          // make a copy, no need to delete later
          openstudio::path * p = reinterpret_cast< openstudio::path * >(vptr);
          $1 = openstudio::path(*p);        
        }else{
          SWIG_exception_fail(SWIG_ValueError, Ruby_Format_TypeError("invalid null reference ", "openstudio::path", "$symname", 1, $input)); 
        }
      } else if (TYPE($input) == T_STRING) {
        // otherwise, if a string
        Check_Type($input, T_STRING);
        std::string s(StringValuePtr($input));
        $1 = openstudio::toPath(s);
      } else {
        SWIG_exception_fail(SWIG_ArgError(res), Ruby_Format_TypeError( "", "openstudio::path", "$symname", 1, $input)); 
      }
    }

    %typemap(typecheck, precedence=SWIG_TYPECHECK_STRING) (path) {
      bool stringType = (TYPE($input) == T_STRING);
      bool pathType = false;
      if (!stringType){
        void *vptr = 0;
        int res = SWIG_ConvertPtr($input, &vptr, $&1_descriptor, 0);
        pathType = (SWIG_IsOK(res) && (vptr != 0));
      }
      $1 = (stringType || pathType) ? 1 : 0;
    }    
    
    // no need for freearg typemap since new did not get called
    
    // handle const path like path
    %apply path { const path };

    // handle const path& separately
    %typemap(in) (const path&) {
      $1=NULL;

      // check if input is a path already
      void *vptr = 0;
      int res = SWIG_ConvertPtr($input, &vptr, $1_descriptor, 0);
      if (SWIG_IsOK(res)) {
        if (vptr) {
          // make a new copy, freearg typemap will call delete on this below
          openstudio::path * p = reinterpret_cast< openstudio::path * >(vptr);
          $1 = new openstudio::path(*(openstudio::path const *)p);        
        }else{
          SWIG_exception_fail(SWIG_ValueError, Ruby_Format_TypeError("invalid null reference ", "openstudio::path const &", "$symname", 1, $input)); 
        }
      } else if (TYPE($input) == T_STRING) {
        // otherwise if a string make a new copy, freearg typemap will call delete on this below
        Check_Type($input, T_STRING);
        std::string s(StringValuePtr($input));
        $1 = new openstudio::path(openstudio::toPath(s));
      } else {
        SWIG_exception_fail(SWIG_ArgError(res), Ruby_Format_TypeError( "", "openstudio::path const &", "$symname", 1, $input)); 
      }
    }    
    
    %typemap(typecheck, precedence=SWIG_TYPECHECK_STRING) (const path&) {
      bool stringType = (TYPE($input) == T_STRING);
      bool pathType = false;
      if (!stringType){
        void *vptr = 0;
        int res = SWIG_ConvertPtr($input, &vptr, $1_descriptor, 0);
        pathType = (SWIG_IsOK(res) && (vptr != 0));
      }
      $1 = (stringType || pathType) ? 1 : 0;
    }    
    
    %typemap(freearg) (const path&) {
      if ($1){
        delete $1;
      }
    }
  #endif

  #ifdef SWIGPYTHON    
    %typemap(in) (path) {
    
      // check if input is a path already
      void *vptr = 0;
      int res = SWIG_ConvertPtr($input, &vptr, $&1_descriptor, 0);
      if (SWIG_IsOK(res)) {
        if (vptr) {
          // make a copy, no need to delete later
          openstudio::path * p = reinterpret_cast< openstudio::path * >(vptr);
          $1 = openstudio::path(*p);        
        }else{
          SWIG_exception_fail(SWIG_ValueError, "Invalid null reference openstudio::path const &"); 
        }
      } else if(PyString_Check($input)) {
        std::string s(PyString_AsString($input));
        $1 = openstudio::toPath(s);
      } else {
        SWIG_exception_fail(SWIG_ArgError(res), "Wrong input type for openstudio::path const &"); 
      }
    }

    %typemap(typecheck, precedence=SWIG_TYPECHECK_STRING) (path) {
      bool stringType = PyString_Check($input);
      bool pathType = false;
      if (!stringType){
        void *vptr = 0;
        int res = SWIG_ConvertPtr($input, &vptr, $&1_descriptor, 0);
        pathType = (SWIG_IsOK(res) && (vptr != 0));
      }
      $1 = (stringType || pathType) ? 1 : 0;
    }    
    
    // no need for freearg typemap since new did not get called
    
    // handle const path like path
    %apply path { const path };

    // handle const path& separately
    %typemap(in) (const path&) {
      $1=NULL;

      // check if input is a path already
      void *vptr = 0;
      int res = SWIG_ConvertPtr($input, &vptr, $1_descriptor, 0);
      if (SWIG_IsOK(res)) {
        if (vptr) {
          // make a new copy, freearg typemap will call delete on this below
          openstudio::path * p = reinterpret_cast< openstudio::path * >(vptr);
          $1 = new openstudio::path(*(openstudio::path const *)p);        
        }else{
          SWIG_exception_fail(SWIG_ValueError, "Invalid null reference openstudio::path const &"); 
        }
      } else if(PyString_Check($input)) {
        std::string s(PyString_AsString($input));
        $1 = new openstudio::path(openstudio::toPath(s));
      } else {
        SWIG_exception_fail(SWIG_ArgError(res), "Wrong input type for openstudio::path const &"); 
      }
    }    
    
    %typemap(typecheck, precedence=SWIG_TYPECHECK_STRING) (const path&) {
      bool stringType = PyString_Check($input);
      bool pathType = false;
      if (!stringType){
        void *vptr = 0;
        int res = SWIG_ConvertPtr($input, &vptr, $1_descriptor, 0);
        pathType = (SWIG_IsOK(res) && (vptr != 0));
      }
      $1 = (stringType || pathType) ? 1 : 0;
    }    
    
    %typemap(freearg) (const path&) {
      if ($1){
        delete $1;
      }
    }
  #endif
  
} // openstudio


%include <utilities/core/PathHelpers.hpp>


// DLM@20100101: demo purposes only, should be able to automatically convert a string input, delete when working
%{
  openstudio::path funcOnlyTakesAConstPathRef(const openstudio::path& p)
  {
    openstudio::path copy(p);
    return copy;
  }
  openstudio::path funcOnlyTakesAConstPath(const openstudio::path p)
  {
    openstudio::path copy(p);
    return copy;
  }
  openstudio::path funcOnlyTakesAPath(openstudio::path p)
  {
    openstudio::path copy(p);
    return copy;
  }
  
  openstudio::path defaultArgFuncTakesAConstPathRef(const openstudio::path& p, bool copy = true)
  {
    openstudio::path result;
    if (copy){
      result = p;
    }
    return result;
  }
  openstudio::path defaultArgFuncTakesAConstPath(const openstudio::path p, bool copy = true)
  {
    openstudio::path result;
    if (copy){
      result = p;
    }
    return result;
  }
  openstudio::path defaultArgFuncTakesAPath(openstudio::path p, bool copy = true)
  {
    openstudio::path result;
    if (copy){
      result = p;
    }
    return result;
  } 
%}
openstudio::path funcOnlyTakesAConstPathRef(const openstudio::path& p);
openstudio::path funcOnlyTakesAConstPath(const openstudio::path p);
openstudio::path funcOnlyTakesAPath(openstudio::path p);

openstudio::path defaultArgFuncTakesAConstPathRef(const openstudio::path& p, bool copy = true);
openstudio::path defaultArgFuncTakesAConstPath(const openstudio::path p, bool copy = true);
openstudio::path defaultArgFuncTakesAPath(openstudio::path p, bool copy = true);


#endif // UTILITIES_CORE_PATH_I
