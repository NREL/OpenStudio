#ifndef UTILITIES_CORE_APPLICATION_I
#define UTILITIES_CORE_APPLICATION_I


%{
  #include <utilities/core/Application.hpp>
%}


%template(Application) openstudio::Singleton<openstudio::ApplicationSingleton>;


%include <utilities/core/Application.hpp>

#endif //UTILITIES_CORE_APPLICATION_I
