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

# no handling of symlinks but that should be ok for developer build
require "#{File.dirname(__FILE__)}/config"

require "pathname"

if RUBY_PLATFORM =~ /darwin/
  mac_version = `/usr/bin/sw_vers -productVersion | tr -d "\n"`.split('.')
  if mac_version[0].to_i < 10 || (mac_version[0].to_i == 10 && mac_version[1].to_i < 8)
    raise LoadError, "OpenStudio is only compatible with OS 10.8 and later"
  end
end

# follow symlinks so that we find the original path to the so's 
$OpenStudio_Dir = "#{File.expand_path(File.dirname(Pathname.new(__FILE__).realpath()))}/"

# add binary dir to system path
original_path = ENV['PATH']
platform_specific_path = nil
if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
  front = []
  back = []
  original_path.split(';').each do |p|
	if /SketchUp/.match(p)
	  if /platform_specific/.match(p)
	    platform_specific_path = p
	  end
	  front << p
	else
	  back << p
	end
  end

  ENV['PATH'] = "#{front.join(';')};#{$OpenStudio_BinaryDir};#{$OpenStudio_RubyBinaryDir};#{back.join(';')}"
else
  ENV['PATH'] = "#{$OpenStudio_BinaryDir}:#{$OpenStudio_RubyBinaryDir}:#{original_path}"
end

# add to ruby path
$:.insert(0, $OpenStudio_Dir)
$:.insert(1, $OpenStudio_LibPath)
$:.insert(2, $OpenStudio_BinaryDir)
$:.insert(3, $OpenStudio_RubyBinaryDir)


# NOTE: The order of module loading is critically important. The SWIG Ruby module prefers the type
# information that is loaded *first* instead of the type information that can be considered *best*
#
# Example: OpenStudioGBXML references openstudio::model::Container, but does not define it
# OpenStudioModelCore does define openstudio::model::Container.
#
# If OpenStudioGBXML is loaded first, the *incomplete* type information that it contains for Container is loaded.
# When OpenStudioModelCore is subsequently loaded the *first* version of Container loaded (which is incomplete, from
# OpenStudioGBXML) is considered the authoritative source of type information.
#
# When the code is OpenStudioModelCore, at some point in the future, goes to use the type information for Container
# an odd, unchecked crash occurs inside of the SWIG generated code.
#
# To prevent this problem, make sure the module that defines the type is loaded before the module that uses the type.
# 
# True as of Ruby 3.0.1. There probably isn't any better way to handle this issue, so I'd be surprised if it changes
#

# load ruby extensions
require 'openstudioutilitiescore'
require 'openstudioutilitiestime'
require 'openstudioutilitiesdata'
require 'openstudioutilitiesplot'
require 'openstudioutilitiesgeometry'
require 'openstudioutilitiessql'
require 'openstudioutilitiesbcl'
require 'openstudioutilitiescloud'
require 'openstudioutilitiesunits'
require 'openstudioutilitiesidd'
require 'openstudioutilitiesidf'
require 'openstudioutilitiesfiletypes'
require 'openstudioutilities'
require 'openstudiomodel'
require 'openstudiomodelcore'
require 'openstudiomodelsimulation'
require 'openstudiomodelresources'
require 'openstudiomodelgeometry'
require 'openstudiomodelhvac'
require 'openstudiomodelrefrigeration'
require 'openstudiomodelgenerators'
require 'openstudioenergyplus'
require 'openstudioradiance'
require 'openstudiogbxml'
require 'openstudioairflow'
require 'openstudioosversion'
require 'openstudioruleset'
require 'openstudiorunmanager'
require 'openstudioproject'
require 'openstudioanalysisdriver'
require 'openstudiomodeleditor'
require 'openstudioanalysis'
require 'openstudiolib'
require 'openstudioisomodel'
require 'openstudiosdd'

# restore original path
ENV['PATH'] = original_path

have_open_ssl  = false
if platform_specific_path
  have_open_ssl = OpenStudio::RemoteBCL::initializeSSL(OpenStudio::Path.new(platform_specific_path))
end
if (!have_open_ssl)
  if (!OpenStudio::RemoteBCL::initializeSSL(OpenStudio::Path.new("#{$OpenStudio_RubyBinaryDir}")))
    if (!OpenStudio::RemoteBCL::initializeSSL())
      raise "Unable to initialize OpenSSL: Verify that ruby can access the OpenSSL libraries"
    end
  end
end

# Find current ruby path, we may need this for launching ruby jobs later
begin

  # may not be defined, e.g for SketchUp plug-in
  require 'rbconfig'  
  
  $OpenStudio_RubyExe = OpenStudio::Path.new(File.join(RbConfig::CONFIG['bindir'], RbConfig::CONFIG['ruby_install_name']).sub(/.*\s.*/m, '"\&"'))
  $OpenStudio_RubyExeDir = $OpenStudio_RubyExe.parent_path()

  # add packaged gems to GEM_PATH
  # The gem path of Ruby that is running is listed first (can be non-OpenStudio Ruby v2.0.0 to override our gems), followed by the version of Ruby we package as the default
  ENV['GEM_PATH'] = "#{ENV['GEM_PATH']};#{RbConfig::CONFIG['rubylibprefix']}/gems/2.0.0;#{OpenStudio::getOpenStudioAWSRubyPath.to_s}/lib/ruby/gems/2.0.0"
  
  # require packaged gems here
  

rescue Exception=>e

  # fall back on system ruby
  if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
    begin
      # which is not a standard command on Windows
      #path_string = `where ruby`
      #$OpenStudio_RubyExe = OpenStudio::Path.new(path_string.strip)
      #$OpenStudio_RubyExeDir = $OpenStudio_RubyExe.parent_path()
      
      # only do this for dev for running unit tests
      $OpenStudio_RubyExe = OpenStudio::Path.new($OpenStudio_RubyExe)
      $OpenStudio_RubyExeDir = $OpenStudio_RubyExe.parent_path()
  
    rescue Exception=>e
    end
  else
    begin
      #path_string = `which ruby`
      #$OpenStudio_RubyExe = OpenStudio::Path.new(path_string.strip)
      #$OpenStudio_RubyExeDir = $OpenStudio_RubyExe.parent_path()
      
      # only do this for dev for running unit tests
      $OpenStudio_RubyExe = OpenStudio::Path.new($OpenStudio_RubyExe)
      $OpenStudio_RubyExeDir = $OpenStudio_RubyExe.parent_path()
      
    rescue Exception=>e
    end
  end
  
end

# "typedefs" for backwards compatibility
module OpenStudio

# support for deprecated names
class IdfObject
  def to_Meter
    return to_OutputMeter
  end
end

module Model

# support for deprecated names
class Model
  def getMeter(handle)
    return getOutputMeter(handle)
  end
  def getMeters
    return getOutputMeters
  end
  def getMeterByName(name)
    return getOutputMeterByName(name)
  end
  def getMetersByName(name)
    return getOutputMetersByName(name)
  end
end

# support for name deprecated as of 1.12.1
class Meter < OutputMeter
end

end # module Model

module Ruleset

# support for name deprecated as of 0.10.1
class UserScriptArgument < OSArgument
end

# support for name deprecated as of 0.10.1
class OptionalUserScriptArgument < OptionalOSArgument
end

# support for name deprecated as of 0.10.1
class UserScriptArgumentVector < OSArgumentVector
end

# support for name deprecated as of 0.10.1
class UserScriptArgumentMap < OSArgumentMap
end

end # module Ruleset

module Analysis

# support for name deprecated as of 0.10.5
class OutputAttributeContinuousVariable < OutputAttributeVariable
end

# support for name deprecated as of 0.10.5
class OptionalOutputAttributeContinuousVariable < OptionalOutputAttributeVariable
end

# support for name deprecated as of 0.10.5
class OutputAttributeContinuousVariableVector < OutputAttributeVariableVector
end

# support for name deprecated as of 1.0.3
class DiscretePerturbation < Measure
end

# support for name deprecated as of 1.0.3
class OptionalDiscretePerturbation < OptionalMeasure
end

# support for name deprecated as of 1.0.3
class DiscretePerturbationVector < MeasureVector
end

# support for name deprecated as of 1.0.3
class NullPerturbation < NullMeasure
end

# support for name deprecated as of 1.0.3
class OptionalNullPerturbation < OptionalNullMeasure
end

# support for name deprecated as of 1.0.3
class NullPerturbationVector < NullMeasureVector
end

# support for name deprecated as of 1.0.3
class RubyPerturbation < RubyMeasure
end

# support for name deprecated as of 1.0.3
class OptionalRubyPerturbation < OptionalRubyMeasure
end

# support for name deprecated as of 1.0.3
class RubyPerturbationVector < RubyMeasureVector
end

end # module Analysis
end
