#ifndef RULESET_I
#define RULESET_I

#ifdef SWIGPYTHON
%module openstudioruleset
#endif


#define UTILITIES_API
#define MODEL_API
#define STANDARDSINTERFACE_API
#define RULESET_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%ignore openstudio::detail;
%ignore openstudio::model::detail;
%ignore openstudio::ruleset::detail;

%{
  #include <ruleset/ModelUserScript.hpp>
  #include <ruleset/OSArgument.hpp>
  #include <ruleset/OSResult.hpp>
  #include <ruleset/OSRunner.hpp>
  #include <ruleset/ReportingUserScript.hpp>
  #include <ruleset/RubyUserScriptArgumentGetter.hpp>
  #include <ruleset/TranslationUserScript.hpp>
  #include <ruleset/UserScript.hpp>
  #include <ruleset/UtilityUserScript.hpp>
  #include <ruleset/WorkspaceUserScript.hpp>

  #include <model/Component.hpp>
  #include <model/ConcreteModelObjects.hpp>

  #include <utilities/core/Path.hpp>

  using namespace openstudio;
  using namespace openstudio::model;
  using namespace openstudio::ruleset;
%}

//user scripts
%ignore std::vector<openstudio::ruleset::OSArgument>::vector(size_type);
%ignore std::vector<openstudio::ruleset::OSArgument>::resize(size_type);
%template(OSArgumentVector) std::vector<openstudio::ruleset::OSArgument>;
%template(OptionalOSArgument) boost::optional<openstudio::ruleset::OSArgument>;
%template(OptionalOSResult) boost::optional<openstudio::ruleset::OSResult>;
%template(OSArgumentMap) std::map<std::string, openstudio::ruleset::OSArgument>;

%template(UserScriptInfo) std::pair<openstudio::path,std::vector<openstudio::ruleset::OSArgument> >;
%template(UserScriptInfoVector) std::vector< std::pair<openstudio::path, std::vector<openstudio::ruleset::OSArgument> > >;
%template(FolderPathToUserScriptsInfoMap) std::map<openstudio::path,std::vector< std::pair<openstudio::path, std::vector<openstudio::ruleset::OSArgument> > > >;

%feature("director") UserScript;
%feature("director") ModelUserScript;
%feature("director") WorkspaceUserScript;
%feature("director") TranslatorUserScript;
%feature("director") UtilityUserScript;
%feature("director") ReportingUserScript;
%feature("director") OSRunner;

%include <ruleset/OSArgument.hpp>
%include <ruleset/OSResult.hpp>
%include <ruleset/OSRunner.hpp>
%include <ruleset/UserScript.hpp>
%include <ruleset/ModelUserScript.hpp>
%include <ruleset/WorkspaceUserScript.hpp>
%include <ruleset/TranslationUserScript.hpp>
%include <ruleset/UtilityUserScript.hpp>
%include <ruleset/ReportingUserScript.hpp>

%extend openstudio::ruleset::OSArgument {
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

#if defined SWIGRUBY

  %ignore RubyUserScriptArgumentGetter;

  // Abstract class. Just need static method.
  %include <ruleset/RubyUserScriptArgumentGetter.hpp>

  %init %{
    rb_eval_string("OpenStudio::Ruleset.instance_eval { def getArguments(measure,model=OpenStudio::Model::OptionalModel.new,workspace=OpenStudio::OptionalWorkspace.new) eval(OpenStudio::Ruleset::argumentExtractorRubyFunction); return argumentExtractor(measure,OpenStudio::Model::OptionalModel.new(model),OpenStudio::OptionalWorkspace.new(workspace)); end }");
  %}

#endif

#endif // RULESET_I
