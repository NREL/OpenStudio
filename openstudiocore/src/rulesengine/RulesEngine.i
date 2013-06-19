#ifndef RULESENGINE_RULESENGINE_I
#define RULESENGINE_RULESENGINE_I

#ifdef SWIGPYTHON
%module openstudiorulesengine
#endif


#define UTILITIES_API
#define MODEL_API
#define STANDARDSINTERFACE_API
#define RULESET_API
#define RULESENGINE_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>
%import <standardsinterface/StandardsInterface.i>
%import <ruleset/Ruleset.i>
%import <project/Project.i>

%ignore CLIPS;

%{  
  #include <standardsinterface/Title24Interface.hpp>
  
  #include <ruleset/ConcreteRulesetObjects.hpp>
  
  #include <project/ProjectDatabase.hpp>

  #include <rulesengine/ApplyRulesetResult.hpp>
  #include <rulesengine/RulesEngine.hpp>
  #include <rulesengine/OpenStudioRulesEngine.hpp>
%}

%rename("OSRulesEngine") openstudio::rulesengine::OpenStudioRulesEngine;

%include <rulesengine/ApplyRulesetResult.hpp>
%include <rulesengine/RulesEngine.hpp>
%include <rulesengine/OpenStudioRulesEngine.hpp>

#endif // RULESENGINE_RULESENGINE_I
