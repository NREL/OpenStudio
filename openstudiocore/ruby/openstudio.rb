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
#####################################################################

$OpenStudio_Dir = "#{File.expand_path(File.dirname(__FILE__))}/"
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
  library.call("#{$OpenStudio_Dir}openstudio/QtCore4.dll")
  library.call("#{$OpenStudio_Dir}openstudio/QtGui4.dll")
  library.call("#{$OpenStudio_Dir}openstudio/QtNetwork4.dll")
  library.call("#{$OpenStudio_Dir}openstudio/QtSql4.dll")
  library.call("#{$OpenStudio_Dir}openstudio/QtXml4.dll")
  library.call("#{$OpenStudio_Dir}openstudio/QtXmlPatterns4.dll")
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
require 'openstudioutilitiesdocument'
require 'openstudioutilitiesidd'
require 'openstudioutilitiesidf'
require 'openstudioutilitiesfiletypes'
require 'openstudioutilities'
require 'openstudioenergyplus'
require 'openstudioradiance'
require 'openstudiogbxml'
require 'openstudiomodel'
require 'openstudiomodelcore'
require 'openstudiomodelsimulation'
require 'openstudiomodelresources'
require 'openstudiomodelgeometry'
require 'openstudiomodelhvac'
require 'openstudioosversion'
require 'openstudioruleset'
require 'openstudiorunmanager'
require 'openstudioproject'
require 'openstudioanalysisdriver'
require 'openstudiomodeleditor'
require 'openstudioanalysis'
require 'openstudiolib'
require 'openstudioplugin'

# optional extensions
begin
  require 'openstudiosimxml'
rescue Exception=>e
end

begin
  require 'openstudiosdd'
rescue Exception=>e
end

# restore original path
ENV['PATH'] = original_path

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
