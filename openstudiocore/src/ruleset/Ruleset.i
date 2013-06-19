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
%import <standardsinterface/StandardsInterface.i>

%ignore openstudio::detail;
%ignore openstudio::model::detail;
%ignore openstudio::standardsinterface::detail;
%ignore openstudio::ruleset::detail;

%{
  #include <ruleset/RulesetEnums.hpp>
  #include <ruleset/StandardsRulesetEnums.hpp>

  #include <ruleset/ConcreteRulesetObjects.hpp>

  #include <ruleset/UserScript.hpp>
  #include <ruleset/OSArgument.hpp>
  #include <ruleset/OSResult.hpp>
  #include <ruleset/OSRunner.hpp>
  #include <ruleset/ModelUserScript.hpp>
  #include <ruleset/WorkspaceUserScript.hpp>
  #include <ruleset/TranslationUserScript.hpp>
  #include <ruleset/UtilityUserScript.hpp>
  #include <ruleset/RubyUserScriptArgumentGetter.hpp>

  #include <standardsinterface/DataDictionary.hpp>

  #include <model/Component.hpp>
  #include <model/ConcreteModelObjects.hpp>

  #include <utilities/core/Path.hpp>

  using namespace openstudio;
  using namespace openstudio::model;
  using namespace openstudio::standardsinterface;
  using namespace openstudio::ruleset;
%}

%include <ruleset/RulesetEnums.hpp>
%include <ruleset/StandardsRulesetEnums.hpp>

%template(OptionalStandardsRulesetModelType) boost::optional<openstudio::ruleset::StandardsRulesetModelType>;

%define SWIG_RULESETOBJECT(_name)
  %ignore std::vector<openstudio::ruleset::_name>::vector(size_type);
  %ignore std::vector<openstudio::ruleset::_name>::resize(size_type);
  %template(_name##Vector) std::vector<openstudio::ruleset::_name>;
  %template(Optional##_name) boost::optional<openstudio::ruleset::_name>;

  %include <ruleset/##_name##.hpp>

  %extend openstudio::ruleset::RulesetObject {
    %template(to_##_name) optionalCast<openstudio::ruleset::##_name>;
  }
%enddef

%define SWIG_FILTERCLAUSE(_name)
  SWIG_RULESETOBJECT(_name)
  %extend openstudio::ruleset::Rule {
    %template(get##_name##s) getFilters<openstudio::ruleset::##_name>;
  }
%enddef

%define SWIG_ACTIONCLAUSE(_name)
  SWIG_RULESETOBJECT(_name)
  %extend openstudio::ruleset::Rule {
    %template(get##_name##s) getActions<openstudio::ruleset::##_name>;
  }
%enddef

// uber-base class
SWIG_RULESETOBJECT(RulesetObject);

// options
SWIG_RULESETOBJECT(RulesetOption);
SWIG_RULESETOBJECT(StandardsRulesetOption);
SWIG_RULESETOBJECT(StandardsEnumRulesetOption);

// rule base class
SWIG_RULESETOBJECT(Rule);

// clauses
SWIG_RULESETOBJECT(Clause);

SWIG_FILTERCLAUSE(FilterClause);
SWIG_FILTERCLAUSE(ModelObjectFilterClause);
SWIG_FILTERCLAUSE(ModelObjectFilterAttribute);
SWIG_FILTERCLAUSE(ModelObjectFilterBooleanAttribute);
SWIG_FILTERCLAUSE(ModelObjectFilterNumericAttribute);
SWIG_FILTERCLAUSE(ModelObjectFilterStringAttribute);
SWIG_FILTERCLAUSE(ModelObjectFilterRelationship);
SWIG_FILTERCLAUSE(ModelObjectFilterType);
SWIG_FILTERCLAUSE(StandardsFilterClause);
SWIG_FILTERCLAUSE(StandardsFilterAvailableModel);
SWIG_FILTERCLAUSE(StandardsFilterAvailableObject);
SWIG_FILTERCLAUSE(StandardsFilterObjectAttribute);
SWIG_FILTERCLAUSE(StandardsFilterObjectNumericalAttribute);
SWIG_FILTERCLAUSE(StandardsFilterObjectBooleanAttribute);
SWIG_FILTERCLAUSE(StandardsFilterObjectDoubleAttribute);
SWIG_FILTERCLAUSE(StandardsFilterObjectEnumAttribute);
SWIG_FILTERCLAUSE(StandardsFilterObjectIntegerAttribute);
SWIG_FILTERCLAUSE(StandardsFilterRulesetOption);

SWIG_ACTIONCLAUSE(ActionClause);
SWIG_ACTIONCLAUSE(ModelObjectActionClause);
SWIG_ACTIONCLAUSE(ModelObjectActionSetAttribute);
SWIG_ACTIONCLAUSE(ModelObjectActionSetRelationship);
SWIG_ACTIONCLAUSE(StandardsActionClause);
SWIG_ACTIONCLAUSE(StandardsActionCloneModel);
SWIG_ACTIONCLAUSE(StandardsActionRuleResult);
SWIG_ACTIONCLAUSE(StandardsActionRuleSatisfied);
SWIG_ACTIONCLAUSE(StandardsActionRuleViolated);

// rules
SWIG_RULESETOBJECT(ModelRule);
SWIG_RULESETOBJECT(StandardsRule);
SWIG_RULESETOBJECT(StandardsAttributeCheckRule);

// rulesets
SWIG_RULESETOBJECT(Ruleset);
SWIG_RULESETOBJECT(ModelRuleset);
SWIG_RULESETOBJECT(StandardsRuleset);

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
%feature("director") OSRunner;

%include <ruleset/OSArgument.hpp>
%include <ruleset/OSResult.hpp>
%include <ruleset/OSRunner.hpp>
%include <ruleset/UserScript.hpp>
%include <ruleset/ModelUserScript.hpp>
%include <ruleset/WorkspaceUserScript.hpp>
%include <ruleset/TranslationUserScript.hpp>
%include <ruleset/UtilityUserScript.hpp>

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
