#ifndef UTILITIES_CORE_APPLICATION_I
#define UTILITIES_CORE_APPLICATION_I


%{
  #include <model_editor/Application.hpp>
%}


%template(Application) openstudio::Singleton<openstudio::ApplicationSingleton>;

%include <model_editor/Application.hpp>

#endif //UTILITIES_CORE_APPLICATION_I
