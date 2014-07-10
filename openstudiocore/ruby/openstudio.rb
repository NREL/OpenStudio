######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

if (defined? Sketchup) && (Sketchup.version_number <= 14000000)
  require File.expand_path(File.dirname(__FILE__)) + '/openstudio/sketchup_plugin/stdruby/pathname'
else
  require 'pathname'
end

if RUBY_PLATFORM =~ /darwin/
  mac_version = `/usr/bin/sw_vers -productVersion | tr -d "\n"`.split('.')
  if mac_version[0].to_i <= 10
    if mac_version[1].to_i < 8
      raise LoadError, "OpenStudio is only compatible with OS 10.8 and later"
    end
  end
end

# follow symlinks so that we find the original path to the so's 
$OpenStudio_Dir = "#{File.expand_path(File.dirname(Pathname.new(__FILE__).realpath()))}/"
if not $:.include?($OpenStudio_Dir)
  $: << $OpenStudio_Dir
end
$: << "#{$OpenStudio_Dir}openstudio" # needed so dlls can find each other
$: << "#{$OpenStudio_Dir}lib" # needed for gem case

# add OpenStudio shared library locations to system path
# we need to add these so that require can load the c++ shared libraries
original_path = ENV['PATH']
if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
  ENV['PATH'] = "#{$OpenStudio_Dir};#{$OpenStudio_Dir}openstudio;#{original_path}"
  
  # Pre-load our specific Qt Dll's on Windows to make sure we control which get loaded
  require 'Win32API'
  library = Win32API.new('kernel32','LoadLibrary', 'P', 'L')  
  library.call("#{$OpenStudio_Dir}openstudio/icudt51.dll")
  library.call("#{$OpenStudio_Dir}openstudio/icuin51.dll")
  library.call("#{$OpenStudio_Dir}openstudio/icuuc51.dll")
  library.call("#{$OpenStudio_Dir}openstudio/libEGL.dll")
  library.call("#{$OpenStudio_Dir}openstudio/libGLESv2.dll")
  library.call("#{$OpenStudio_Dir}openstudio/Qt5Core.dll")
  library.call("#{$OpenStudio_Dir}openstudio/Qt5Gui.dll")
  library.call("#{$OpenStudio_Dir}openstudio/Qt5Network.dll")
  library.call("#{$OpenStudio_Dir}openstudio/Qt5Sql.dll")
  library.call("#{$OpenStudio_Dir}openstudio/Qt5Widgets.dll")
  library.call("#{$OpenStudio_Dir}openstudio/Qt5Xml.dll")
  library.call("#{$OpenStudio_Dir}openstudio/qtwinmigrate.dll")
else
  # Do something here for Mac OSX environments
  ENV['PATH'] = "#{$OpenStudio_Dir};#{$OpenStudio_Dir}openstudio;#{original_path}"
end

# load ruby extensions
require 'openstudioutilitiescore'
require 'openstudioutilitieseconomics'
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
require 'openstudioenergyplus'
require 'openstudioradiance'
require 'openstudiogbxml'
require 'openstudiocontam'
require 'openstudiomodel'
require 'openstudiomodelcore'
require 'openstudiomodelsimulation'
require 'openstudiomodelresources'
require 'openstudiomodelgeometry'
require 'openstudiomodelhvac'
require 'openstudiomodelrefrigeration'
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

if (!OpenStudio::RemoteBCL::initializeSSL(OpenStudio::Path.new("#{$OpenStudio_Dir}OpenStudio")))
  if (!OpenStudio::RemoteBCL::initializeSSL())
    raise "Unable to initialize OpenSSL: Verify that ruby can access the OpenSSL libraries"
  end
end


if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
  $OpenStudio_BinaryDir = "#{$OpenStudio_Dir}../bin/"
elsif /darwin/.match(RUBY_PLATFORM)
  $OpenStudio_BinaryDir = "#{$OpenStudio_Dir}../"
else
  $OpenStudio_BinaryDir = ""
end

$OpenStudio_LibPath = "#{$OpenStudio_Dir}"

$OpenStudio_ResourcePath = "#{$OpenStudio_Dir}../Examples/"

# Find current ruby path, we may need this for launching ruby jobs later
begin

  # may not be defined, e.g for SketchUp plug-in
  require 'rbconfig'  
  
  $OpenStudio_RubyExe = OpenStudio::Path.new(File.join(RbConfig::CONFIG['bindir'], RbConfig::CONFIG['ruby_install_name']).sub(/.*\s.*/m, '"\&"'))
  $OpenStudio_RubyExeDir = $OpenStudio_RubyExe.parent_path()

rescue Exception=>e

  # fall back on system ruby
  if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
    begin
      # which is not a standard command on Windows
      #path_string = `where ruby`
      #$OpenStudio_RubyExe = OpenStudio::Path.new(path_string.strip)
      #$OpenStudio_RubyExeDir = $OpenStudio_RubyExe.parent_path()
    rescue Exception=>e
    end
  else
    begin
      #path_string = `which ruby`
      #$OpenStudio_RubyExe = OpenStudio::Path.new(path_string.strip)
      #$OpenStudio_RubyExeDir = $OpenStudio_RubyExe.parent_path()
    rescue Exception=>e
    end
  end
  
end

# "typedefs" for backwards compatibility
module OpenStudio
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
