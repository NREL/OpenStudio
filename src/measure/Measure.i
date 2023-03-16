#ifndef MEASURE_I
#define MEASURE_I

#ifdef SWIGPYTHON
%module(directors="1") openstudiomeasure
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

#if defined(SWIGCSHARP)
  // Help in overload resolution preferring std::string over char const *
  %ignore openstudio::measure::OSArgument::setValue(char const*);
  %ignore openstudio::measure::OSArgument::setDefaultValue(char const*);
#elif defined(SWIGPYTHON)
  // Avoid triggering a SWIG warning: 'print' is a python keyword
  %rename(toString) openstudio::measure::OSArgument::print;
  %rename(toString) openstudio::measure::OSOutput::print;
#endif

%{
  #include <measure/OSArgument.hpp>
  #include <measure/OSOutput.hpp>
  #include <measure/OSRunner.hpp>
  #include <measure/OSMeasure.hpp>
  #include <measure/OSMeasureInfoGetter.hpp>

  #include <measure/EnergyPlusMeasure.hpp>
  #include <measure/ModelMeasure.hpp>
  #include <measure/ReportingMeasure.hpp>

  #include <model/Component.hpp>
  #include <model/ConcreteModelObjects.hpp>

  #include <utilities/core/Path.hpp>
  #include <utilities/bcl/BCLMeasure.hpp>

  using namespace openstudio;
  using namespace openstudio::model;
  using namespace openstudio::measure;
%}


%ignore std::vector<openstudio::measure::OSOutput>::vector(size_type);
%ignore std::vector<openstudio::measure::OSOutput>::resize(size_type);
%template(OSOutputVector) std::vector<openstudio::measure::OSOutput>;
%template(OptionalOSOutput) boost::optional<openstudio::measure::OSOutput>;

%feature("director") OSMeasure;
%feature("director") ModelMeasure;
%feature("director") EnergyPlusMeasure;
%feature("director") ReportingMeasure;
%feature("director") OSRunner;
%feature("director") OSArgument;

//user scripts
%template(OSArgumentVector) std::vector<openstudio::measure::OSArgument>;
%template(OptionalOSArgument) boost::optional<openstudio::measure::OSArgument>;
%template(OSArgumentMap) std::map<std::string, openstudio::measure::OSArgument>;

%include <measure/OSArgument.hpp>
%include <measure/OSOutput.hpp>
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

%extend openstudio::measure::OSOutput {
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

#if defined SWIGRUBY

  %ignore OSMeasureInfoGetter;

  %include <measure/OSMeasureInfoGetter.hpp>

  %init %{
    rb_eval_string("OpenStudio::Measure.instance_eval { def getInfo(measure,model=OpenStudio::Model::OptionalModel.new,workspace=OpenStudio::OptionalWorkspace.new) eval(OpenStudio::Measure::infoExtractorRubyFunction); return infoExtractor(measure,OpenStudio::Model::OptionalModel.new(model),OpenStudio::OptionalWorkspace.new(workspace)); end }");
  %}

#endif

#endif // MEASURE_I
