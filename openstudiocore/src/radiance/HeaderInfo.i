#ifndef RADIANCE_HEADERINFO_I
#define RADIANCE_HEADERINFO_I

#define UTILITIES_API
#define RADIANCE_API

%include <utilities/core/CommonInclude.i>
%import <utilities/Utilities.i>

%{
  namespace openstudio{ namespace radiance{} }
  using namespace openstudio::radiance;

  #include <radiance/HeaderInfo.hpp>
%}

// create an instantiation of the smart ptr class
%template(HeaderInfoPtr) std::shared_ptr<openstudio::radiance::HeaderInfo>;
 
// create an instantiation of the vector class
%template(HeaderInfoVector) std::vector< std::shared_ptr<openstudio::radiance::HeaderInfo> >;

%include <radiance/HeaderInfo.hpp>

#endif //RADIANCE_HEADERINFO_I 
