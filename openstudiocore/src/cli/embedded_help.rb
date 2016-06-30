
module RbConfig
  def RbConfig.ruby
    EmbeddedScripting::applicationFilePath;
  end
end

module OpenStudio
  def self.openstudio_path
    RbConfig.ruby
  end
end

BINDING = Kernel::binding()
Encoding.default_external = Encoding::ASCII

module Kernel
  # ":" is our root path to the embedded file system
  # make sure it is in the ruby load path
  $LOAD_PATH << ':'
  $LOAD_PATH << ':/ruby/2.2.0'
  # TODO configure this in a better way
  # these hardcoded platform paths are brain dead
  $LOAD_PATH << ':/ruby/2.2.0/x86_64-darwin13'
  $LOAD_PATH << ':/ruby/2.2.0/x64-mswin64_120'
  $LOAD_PATH << ':/openstudio-workflow-1.0.0.alpha.0/lib'
  $LOADED = []

  alias :original_require_relative :require_relative
  alias :original_require :require

  def require path
    rb_path = path

    jsonparser = 'json/ext/parser' 
    if path == jsonparser
      if $LOADED.include?(jsonparser) then
        return true
      else
        EmbeddedScripting::initJSONParser()
        $LOADED << jsonparser
        return true
      end
    end

    jsongenerator = 'json/ext/generator'
    if path == jsongenerator
      if $LOADED.include?(jsongenerator) then
        return true
      else
        EmbeddedScripting::initJSONGenerator()
        $LOADED << jsongenerator
        return true
      end
    end

    if File.extname(path).empty? then
      rb_path = path + '.rb'
    end 

    if rb_path.chars.first == ':'
       if $LOADED.include?(rb_path) then
         return true
      else
        return require_embedded_absolute(rb_path)
      end
    #elsif rb_path == 'openstudio.rb'
    #  return true
    else
      $LOAD_PATH.each do |p|
        if p.chars.first == ':' then
          embedded_path = p + '/' + rb_path
          if $LOADED.include?(embedded_path) then
            return true
          elsif EmbeddedScripting::hasFile(embedded_path) then
            return require_embedded_absolute(embedded_path)
          end
        end
      end
    end

    return original_require path
  end

  def require_embedded_absolute path
    $LOADED << path
    result = eval(EmbeddedScripting::getFileAsString(path),BINDING,path)
    return result
  end

  def require_relative path
    absolute_path = File.dirname(caller_locations(1,1)[0].path) + '/' + path
    return require absolute_path
  end

end



# "typedefs" for backwards compatibility
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
