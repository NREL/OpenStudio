#ifndef UTILITIES_CLOUD_CLOUD_I
#define UTILITIES_CLOUD_CLOUD_I

// does not turn on directors for all classes, just enables them
%module(directors="1") OpenStudio

%ignore openstudio::detail;

%{
  #include <utilities/cloud/OSServer.hpp>
  #include <utilities/cloud/CloudProvider.hpp>
%}

%include <utilities/cloud/OSServer.hpp>
%include <utilities/cloud/CloudProvider.hpp>

#endif // UTILITIES_CLOUD_CLOUD_I
