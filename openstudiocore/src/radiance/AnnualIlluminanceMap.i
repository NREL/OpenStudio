#ifndef RADIANCE_ANNUALILLUMINANCEMAP_I
#define RADIANCE_ANNUALILLUMINANCEMAP_I

#define UTILITIES_API
#define RADIANCE_API

%include <utilities/core/CommonInclude.i>
%import <utilities/Utilities.i>

%{
  namespace openstudio{ namespace radiance{} }
  using namespace openstudio::radiance;

  #include <radiance/AnnualIlluminanceMap.hpp>
%}

// create an instantiation of the smart ptr class
%template(AnnualIlluminanceMapPtr) std::shared_ptr<openstudio::radiance::AnnualIlluminanceMap>;
 
// create an instantiation of the vector class
%template(AnnualIlluminanceMapVector) std::vector< std::shared_ptr<openstudio::radiance::AnnualIlluminanceMap> >;

%ignore openstudio::radiance::AnnualIlluminanceMap::AnnualIlluminanceMap(const openstudio::Path&);

%include <radiance/AnnualIlluminanceMap.hpp>

#endif //RADIANCE_ANNUALILLUMINANCEMAP_I 
