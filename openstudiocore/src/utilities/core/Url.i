#ifndef UTILITIES_CORE_URL_I
#define UTILITIES_CORE_URL_I

// DLM@200921221: reinclude the ruby undef stuff, ruby define clashes with accept in Qt
%include <utilities/core/CommonInclude.i>
%include <utilities/core/CommonImport.i>

%{
  #include <utilities/core/Url.hpp>
  #include <utilities/core/URLHelpers.hpp>
%}

namespace openstudio{

  #ifdef SWIGRUBY
    %rename("nil?") isNull();
  #endif

  class Url {
  public:

    ~Url();

  protected:
     Url();
  };


  %extend Url{
    Url(const std::string& s){
      Url *p;
      p = new Url(toQString(s));
      return p;
    }

    std::string __str__() const{
      return toString(*self);
    }
  };

} // openstudio

%template(OptionalUrl) boost::optional<openstudio::Url>;
%template(UrlVector) std::vector<openstudio::Url>;

%include <utilities/core/URLHelpers.hpp>

#endif //UTILITIES_CORE_URL_I
