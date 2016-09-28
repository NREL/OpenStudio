########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

#require "pathname"
#
#if RUBY_PLATFORM =~ /darwin/
#  mac_version = `/usr/bin/sw_vers -productVersion | tr -d "\n"`.split('.')
#  if mac_version[0].to_i < 10 || (mac_version[0].to_i == 10 && mac_version[1].to_i < 8)
#    raise LoadError, "OpenStudio is only compatible with OS 10.8 and later"
#  end
#end
#
## follow symlinks so that we find the original path to the so's 
#$OpenStudio_Dir = "#{File.expand_path(File.dirname(Pathname.new(__FILE__).realpath()))}/"
#if not $:.include?($OpenStudio_Dir)
#  $: << $OpenStudio_Dir
#end
#$: << "#{$OpenStudio_Dir}openstudio" # needed so dlls can find each other
#$: << "#{$OpenStudio_Dir}lib" # needed for gem case
#
## add OpenStudio shared library locations to system path
## we need to add these so that require can load the c++ shared libraries
#original_path = ENV['PATH']
#platform_specific_path = nil
#if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
#  front = []
#  back = []
#  original_path.split(';').each do |p|
#	if /SketchUp/.match(p)
#	  if /platform_specific/.match(p)
#	    platform_specific_path = p
#	  end
#	  front << p
#	else
#	  back << p
#	end
#  end
#
#  ENV['PATH'] = "#{front.join(';')};#{$OpenStudio_Dir};#{$OpenStudio_Dir}openstudio;#{back.join(';')}"
#  
#  # Pre-load our specific Qt Dll's on Windows to make sure we control which get loaded
#  require 'Win32API'
#  library = Win32API.new('kernel32','LoadLibrary', 'P', 'L')  
#  library.call("#{$OpenStudio_Dir}openstudio/icudt51.dll")
#  library.call("#{$OpenStudio_Dir}openstudio/icuin51.dll")
#  library.call("#{$OpenStudio_Dir}openstudio/icuuc51.dll")
#  library.call("#{$OpenStudio_Dir}openstudio/libEGL.dll")
#  library.call("#{$OpenStudio_Dir}openstudio/libGLESv2.dll")
#  library.call("#{$OpenStudio_Dir}openstudio/Qt5Core.dll")
#  library.call("#{$OpenStudio_Dir}openstudio/Qt5Gui.dll")
#  library.call("#{$OpenStudio_Dir}openstudio/Qt5Network.dll")
#  library.call("#{$OpenStudio_Dir}openstudio/Qt5Sql.dll")
#  library.call("#{$OpenStudio_Dir}openstudio/Qt5Widgets.dll")
#  library.call("#{$OpenStudio_Dir}openstudio/Qt5Xml.dll")
#  library.call("#{$OpenStudio_Dir}openstudio/qtwinmigrate.dll")
#else
#  # Do something here for Mac OSX environments
#  ENV['PATH'] = "#{$OpenStudio_Dir};#{$OpenStudio_Dir}openstudio;#{original_path}"
#end
#
#
## NOTE: The order of module loading is critically important. The SWIG Ruby module prefers the type
## information that is loaded *first* instead of the type information that can be considered *best*
##
## Example: OpenStudioGBXML references openstudio::model::Container, but does not define it
## OpenStudioModelCore does define openstudio::model::Container.
##
## If OpenStudioGBXML is loaded first, the *incomplete* type information that it contains for Container is loaded.
## When OpenStudioModelCore is subsequently loaded the *first* version of Container loaded (which is incomplete, from
## OpenStudioGBXML) is considered the authoritative source of type information.
##
## When the code is OpenStudioModelCore, at some point in the future, goes to use the type information for Container
## an odd, unchecked crash occurs inside of the SWIG generated code.
##
## To prevent this problem, make sure the module that defines the type is loaded before the module that uses the type.
## 
## True as of Ruby 3.0.1. There probably isn't any better way to handle this issue, so I'd be surprised if it changes
##
#
#
## load ruby extensions
#require 'openstudioutilitiescore'
#require 'openstudioutilitiestime'
#require 'openstudioutilitiesdata'
#require 'openstudioutilitiesplot'
#require 'openstudioutilitiesgeometry'
#require 'openstudioutilitiessql'
#require 'openstudioutilitiesbcl'
#require 'openstudioutilitiescloud'
#require 'openstudioutilitiesunits'
#require 'openstudioutilitiesidd'
#require 'openstudioutilitiesidf'
#require 'openstudioutilitiesfiletypes'
#require 'openstudioutilities'
#require 'openstudiomodel'
#require 'openstudiomodelcore'
#require 'openstudiomodelsimulation'
#require 'openstudiomodelresources'
#require 'openstudiomodelgeometry'
#require 'openstudiomodelhvac'
#require 'openstudiomodelrefrigeration'
#require 'openstudiomodelgenerators'
#require 'openstudioenergyplus'
#require 'openstudioradiance'
#require 'openstudiogbxml'
#require 'openstudioairflow'
#require 'openstudioosversion'
#require 'openstudiomeasure'
#require 'openstudiomodeleditor'
#require 'openstudiolib'
#require 'openstudioisomodel'
#require 'openstudiosdd'
#
## restore original path
#ENV['PATH'] = original_path
#
#have_open_ssl  = false
#if platform_specific_path
#  have_open_ssl = OpenStudio::RemoteBCL::initializeSSL(OpenStudio::Path.new(platform_specific_path))
#end
#if (!have_open_ssl)
#  if (!OpenStudio::RemoteBCL::initializeSSL(OpenStudio::Path.new("#{$OpenStudio_Dir}OpenStudio")))
#    if (!OpenStudio::RemoteBCL::initializeSSL())
#      raise "Unable to initialize OpenSSL: Verify that ruby can access the OpenSSL libraries"
#    end
#  end
#end
#
#if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
#  $OpenStudio_BinaryDir = "#{$OpenStudio_Dir}../bin/"
#elsif /darwin/.match(RUBY_PLATFORM)
#  $OpenStudio_BinaryDir = "#{$OpenStudio_Dir}../"
#else
#  $OpenStudio_BinaryDir = ""
#end
#
#$OpenStudio_LibPath = "#{$OpenStudio_Dir}"
#
#$OpenStudio_ResourcePath = "#{$OpenStudio_Dir}../Examples/"
#
## Find current ruby path, we may need this for launching ruby jobs later
#begin
#
#  # may not be defined, e.g for SketchUp plug-in
#  require 'rbconfig'  
#  
#  $OpenStudio_RubyExe = OpenStudio::Path.new(File.join(RbConfig::CONFIG['bindir'], RbConfig::CONFIG['ruby_install_name']).sub(/.*\s.*/m, '"\&"'))
#  $OpenStudio_RubyExeDir = $OpenStudio_RubyExe.parent_path()
#
#  # add packaged gems to GEM_PATH
#  # The gem path of Ruby that is running is listed first (can be non-OpenStudio Ruby v2.0.0 to override our gems), followed by the version of Ruby we package as the default
#  ENV['GEM_PATH'] = "#{ENV['GEM_PATH']};#{RbConfig::CONFIG['rubylibprefix']}/gems/2.0.0;#{OpenStudio::getOpenStudioAWSRubyPath.to_s}/lib/ruby/gems/2.0.0"
#  
#  # require packaged gems here
#  
#
#rescue Exception=>e
#
#  # fall back on system ruby
#  if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
#    begin
#      # which is not a standard command on Windows
#      #path_string = `where ruby`
#      #$OpenStudio_RubyExe = OpenStudio::Path.new(path_string.strip)
#      #$OpenStudio_RubyExeDir = $OpenStudio_RubyExe.parent_path()
#    rescue Exception=>e
#    end
#  else
#    begin
#      #path_string = `which ruby`
#      #$OpenStudio_RubyExe = OpenStudio::Path.new(path_string.strip)
#      #$OpenStudio_RubyExeDir = $OpenStudio_RubyExe.parent_path()
#    rescue Exception=>e
#    end
#  end
#  
#end

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
