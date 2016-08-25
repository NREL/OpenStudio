#include "init_openstudio.hpp"
#include <ruby.h>

extern "C" {
  void Init_openstudioairflow(void);
  void Init_openstudiomodelcore(void);
  void Init_openstudiomodelsimulation(void);
  void Init_openstudioutilitiescore(void);
  void Init_openstudioutilitiesplot(void);
  void Init_openstudioenergyplus(void);
  void Init_openstudioosversion(void);
  void Init_openstudioutilitiesdata(void);
  void Init_openstudioutilitiessql(void);
  void Init_openstudiogbxml(void);
  void Init_openstudiomodelgenerators(void);
  void Init_openstudioradiance(void);
  void Init_openstudioutilitiestime(void);
  void Init_openstudioisomodel(void);
  void Init_openstudiomodelgeometry(void);
  void Init_openstudiosdd(void);
  void Init_openstudioutilitiesfiletypes(void);
  void Init_openstudioutilitiesunits(void);
  void Init_openstudiomodelhvac(void);
  void Init_openstudioutilities(void);
  void Init_openstudioutilitiesgeometry(void);
  void Init_openstudiomeasure(void);
  void Init_openstudiomodelrefrigeration(void);
  void Init_openstudioutilitiesbcl(void);
  void Init_openstudioutilitiesidd(void);
  void Init_openstudiomodel(void);
  void Init_openstudiomodelresources(void);
  void Init_openstudioutilitiescloud(void);
  void Init_openstudioutilitiesidf(void);

  ////void Init_openstudiomodeleditor(void); # happens separately in openstudio.so only, for SketchUp plug-in
}

void init_openstudio_internal() {
  Init_openstudioutilitiescore();
  rb_provide("openstudioutilitiescore");
  Init_openstudioutilitiestime();
  rb_provide("openstudioutilitiestime");
  Init_openstudioutilitiesdata();
  rb_provide("openstudioutilitiesdata");
  Init_openstudioutilitiesplot();
  rb_provide("openstudioutilitiesplot");
  Init_openstudioutilitiesgeometry();
  rb_provide("openstudioutilitiesgeometry");
  Init_openstudioutilitiessql();
  rb_provide("openstudioutilitiessql");
  Init_openstudioutilitiesbcl();
  rb_provide("openstudioutilitiesbcl");
  Init_openstudioutilitiescloud();
  rb_provide("openstudioutilitiescloud");
  Init_openstudioutilitiesunits();
  rb_provide("openstudioutilitiesunits");
  Init_openstudioutilitiesidd();
  rb_provide("openstudioutilitiesidd");
  Init_openstudioutilitiesidf();
  rb_provide("openstudioutilitiesidf");
  Init_openstudioutilitiesfiletypes();
  rb_provide("openstudioutilitiesfiletypes");
  Init_openstudioutilities();
  rb_provide("openstudioutilities");
  Init_openstudiomodel();
  rb_provide("openstudiomodel");
  Init_openstudiomodelcore();
  rb_provide("openstudiomodelcore");
  Init_openstudiomodelsimulation();
  rb_provide("openstudiomodelsimulation");
  Init_openstudiomodelresources();
  rb_provide("openstudiomodelresources");
  Init_openstudiomodelgeometry();
  rb_provide("openstudiomodelgeometry");
  Init_openstudiomodelhvac();
  rb_provide("openstudiomodelhvac");
  Init_openstudiomodelrefrigeration();
  rb_provide("openstudiomodelrefrigeration");
  Init_openstudiomodelgenerators();
  rb_provide("openstudiomodelgenerators");
  Init_openstudioenergyplus();
  rb_provide("openstudioenergyplus");
  Init_openstudioradiance();
  rb_provide("openstudioradiance");
  Init_openstudiogbxml();
  rb_provide("openstudiogbxml");
  Init_openstudioairflow();
  rb_provide("openstudioairflow");
  Init_openstudioosversion();
  rb_provide("openstudioversion");
  Init_openstudiomeasure();
  rb_provide("openstudiomeasure");
  Init_openstudioisomodel();
  rb_provide("openstudioisomodel");
  Init_openstudiosdd();
  rb_provide("openstudiosdd");

  //Init_openstudiomodeleditor(); # happens separately in openstudio.so only, for SketchUp plug-in
  //rb_provide("openstudiomodeleditor");

  // "typedefs" for backwards compatibility
  std::string ruby_typedef_script = R"END(
module OpenStudio
module Ruleset

  # support for name deprecated as of 0.10.1
  class UserScriptArgument < OpenStudio::Measure::OSArgument
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScriptArgument is deprecated, use OpenStudio::Measure::Argument instead.")
      super
    end
  end

  # support for name deprecated as of 0.10.1
  class OptionalUserScriptArgument < OpenStudio::Measure::OptionalOSArgument
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OptionalUserScriptArgument is deprecated, use OpenStudio::Measure::OptionalOSArgument instead.")
      super
    end  
  end

  # support for name deprecated as of 0.10.1
  class UserScriptArgumentVector < OpenStudio::Measure::OSArgumentVector
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScriptArgumentVector is deprecated, use OpenStudio::Measure::OSArgumentVector instead.")
      super
    end    
  end

  # support for name deprecated as of 0.10.1
  class UserScriptArgumentMap < OpenStudio::Measure::OSArgumentMap
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScriptArgumentMap is deprecated, use OpenStudio::Measure::OSArgumentMap instead.")
      super
    end      
  end
  
  # support for name deprecated as of 2.0.0
  class UserScript < OpenStudio::Measure::OSMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScript is deprecated, use OpenStudio::Measure::OSMeasure instead.")
      super
    end      
  end
  
  # support for name deprecated as of 2.0.0
  class ModelUserScript < OpenStudio::Measure::ModelMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "ModelUserScript is deprecated, use OpenStudio::Measure::ModelMeasure instead.")
      super
    end      
  end
  
  # support for name deprecated as of 2.0.0
  class WorkspaceUserScript < OpenStudio::Measure::EnergyPlusMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "WorkspaceUserScript is deprecated, use OpenStudio::Measure::EnergyPlusMeasure instead.")
      super
    end      
  end
  
  # support for name deprecated as of 2.0.0
  class ReportingUserScript < OpenStudio::Measure::ReportingMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "ReportingUserScript is deprecated, use OpenStudio::Measure::ReportingMeasure instead.")
      super
    end      
  end
  
  # support for name deprecated as of 2.0.0
  class OSArgument < OpenStudio::Measure::OSArgument
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSArgument is deprecated, use OpenStudio::Measure::OSArgument instead.")
      super
    end      
  end
  
  # support for name deprecated as of 2.0.0
  def self.convertOSArgumentVectorToMap(argument_vector)
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OpenStudio::Ruleset::convertOSArgumentVectorToMap is deprecated, use OpenStudio::Measure::convertOSArgumentVectorToMap instead.")
    return OpenStudio::Measure::convertOSArgumentVectorToMap(argument_vector)
  end      

  # support for name deprecated as of 2.0.0
  class OSArgumentVector < OpenStudio::Measure::OSArgumentVector
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSArgumentVector is deprecated, use OpenStudio::Measure::OSArgumentVector instead.")
      super
    end      
  end
  
  # support for name deprecated as of 2.0.0
  class OSArgumentMap < OpenStudio::Measure::OSArgumentMap
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSArgumentMap is deprecated, use OpenStudio::Measure::OSArgumentMap instead.")
      super
    end      
  end
  
  # class was replaced by OpenStudio::WorkflowStepResult
#  # support for name deprecated as of 2.0.0
#  class OSResult < OpenStudio::Measure::OSResult
#    def initialize
#      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSResult is deprecated, use OpenStudio::Measure::OSResult instead.")
#      super
#    end      
#  end
#  
#  # support for name deprecated as of 2.0.0
#  class OSResultVector < OpenStudio::Measure::OSResultVector
#    def initialize
#      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSResultVector is deprecated, use OpenStudio::Measure::OSResultVector instead.")
#      super
#    end      
#  end
  
  # support for name deprecated as of 2.0.0
  class OSRunner < OpenStudio::Measure::OSRunner
    def initialize(workflow_json = nil)
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSRunner is deprecated, use OpenStudio::Measure::OSRunner instead.")
      if workflow_json.nil?
        workflow_json = OpenStudio::WorkflowJSON.new
        OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "No workflow provided, using empty WorkflowJSON.")
      end
      super(workflow_json)
    end       
  end
  
  # support for name deprecated as of 2.0.0
  class RubyUserScriptInfo < OpenStudio::Measure::OSMeasureInfo
    def initialize(*args)
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "RubyUserScriptInfo is deprecated, use OpenStudio::Measure::OSMeasureInfo instead.")
      if args.size == 1
        super(args[0])
      elsif args.size == 8
        super(args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],args[8])
      end
    end       
  end
  
  # support for name deprecated as of 2.0.0
  def self.infoExtractorRubyFunction
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "Ruleset is deprecated, use OpenStudio::Measure::infoExtractorRubyFunction instead.")
    return OpenStudio::Measure.infoExtractorRubyFunction
  end
  
  # support for name deprecated as of 2.0.0
  def self.getInfo(measure, model, workspace)
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "Ruleset is deprecated, use OpenStudio::Measure::getInfo instead.")
    return OpenStudio::Measure.getInfo(measure, model, workspace)
  end
  
end # module Ruleset
end # module OpenStudio

)END";

  evalString(ruby_typedef_script);

}

class RubyException : public std::runtime_error
{
  public:

    RubyException(const std::string& msg, const std::string& location)
      : std::runtime_error(msg), m_location(location)
    {}

    virtual ~RubyException() throw() {}

    std::string location() const {return m_location;}

  private:

    std::string m_location;

};

static VALUE evaluateSimpleImpl(VALUE arg) 
{
  return rb_eval_string(StringValuePtr(arg));
}

void evalString(const std::string &t_str)
{

  VALUE val = rb_str_new2(t_str.c_str());
  int error;

  rb_protect(evaluateSimpleImpl,val,&error);


  if (error != 0)
  {
    VALUE errval = rb_eval_string("$!.to_s");
    char *str = StringValuePtr(errval);
    std::string err(str);

    VALUE locval = rb_eval_string("$@.to_s");
    str = StringValuePtr(locval);
    std::string loc(str);

    throw RubyException(err, loc);
  } 
}



