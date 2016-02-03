#ifndef MEASURE_I
#define MEASURE_I

#ifdef SWIGPYTHON
%module openstudioruleset
#endif


#define UTILITIES_API
#define MODEL_API
#define STANDARDSINTERFACE_API
#define MEASURE_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%ignore openstudio::detail;
%ignore openstudio::model::detail;
%ignore openstudio::measure::detail;

%{
  #include <measure/ModelOSMeasure.hpp>
  #include <measure/OSArgument.hpp>
  #include <measure/OSResult.hpp>
  #include <measure/OSRunner.hpp>
  #include <measure/ReportingOSMeasure.hpp>
  #include <measure/RubyUserScriptArgumentGetter.hpp>
  #include <measure/RubyMeasureInfoGetter.hpp>
  #include <measure/TranslationOSMeasure.hpp>
  #include <measure/OSMeasure.hpp>
  #include <measure/UtilityOSMeasure.hpp>
  #include <measure/WorkspaceOSMeasure.hpp>

  #include <model/Component.hpp>
  #include <model/ConcreteModelObjects.hpp>

  #include <utilities/core/Path.hpp>
  #include <utilities/bcl/BCLMeasure.hpp>

  using namespace openstudio;
  using namespace openstudio::model;
  using namespace openstudio::measure;
%}

//user scripts
%ignore std::vector<openstudio::measure::OSArgument>::vector(size_type);
%ignore std::vector<openstudio::measure::OSArgument>::resize(size_type);
%template(OSArgumentVector) std::vector<openstudio::measure::OSArgument>;
%template(OptionalOSArgument) boost::optional<openstudio::measure::OSArgument>;
%template(OptionalOSResult) boost::optional<openstudio::measure::OSResult>;
%template(OSArgumentMap) std::map<std::string, openstudio::measure::OSArgument>;

%template(UserScriptInfo) std::pair<openstudio::path,std::vector<openstudio::measure::OSArgument> >;
%template(UserScriptInfoVector) std::vector< std::pair<openstudio::path, std::vector<openstudio::measure::OSArgument> > >;
%template(FolderPathToUserScriptsInfoMap) std::map<openstudio::path,std::vector< std::pair<openstudio::path, std::vector<openstudio::measure::OSArgument> > > >;

%feature("director") UserScript;
%feature("director") ModelUserScript;
%feature("director") WorkspaceUserScript;
%feature("director") TranslatorUserScript;
%feature("director") UtilityUserScript;
%feature("director") ReportingUserScript;
%feature("director") OSRunner;

%include <measure/OSArgument.hpp>
%include <measure/OSResult.hpp>
%include <measure/OSRunner.hpp>
%include <measure/OSMeasure.hpp>
%include <measure/ModelMeasure.hpp>
%include <measure/EnergyPlusMeasure.hpp>
%include <measure/ReportingMeasure.hpp>

%extend openstudio::measure::OSArgument {
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

#if defined SWIGRUBY

  %ignore RubyUserScripInfoGetter;

  // Abstract class. Just need static method.
  %include <measure/RubyMeasureInfoGetter.hpp>

  %init %{
    rb_eval_string("OpenStudio::Ruleset.instance_eval { def getInfo(measure,model=OpenStudio::Model::OptionalModel.new,workspace=OpenStudio::OptionalWorkspace.new) eval(OpenStudio::Ruleset::infoExtractorRubyFunction); return infoExtractor(measure,OpenStudio::Model::OptionalModel.new(model),OpenStudio::OptionalWorkspace.new(workspace)); end }");
  %}

#endif

#endif // MEASURE_I
