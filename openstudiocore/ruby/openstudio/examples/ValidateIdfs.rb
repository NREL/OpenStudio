######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

######################################################################
# == Synopsis 
#
#   Validate all EnergyPlus Idf files in a directory.
#
# == Usage
#
#   ruby ValidateIdfs.rb ARGV[0] 
# 
#   ARGV[0] - Path to directory with EnergyPlus Idf files directory.
#   ARGV[1] - Path to create temporary files in during execution
#
# == Examples
#
#   ruby ValidateIdfs.rb 'C:\path\to\energyplus\dir\'
#
######################################################################

require 'openstudio'
require 'fileutils'

Dir.chdir(ARGV[0])

if not File.exists?(ARGV[1])
  Dir.mkdir(ARGV[1])
end

if File.exists?("#{ARGV[1]}/unfiltered")
  FileUtils.rm_rf("#{ARGV[1]}/unfiltered")
end
Dir.mkdir("#{ARGV[1]}/unfiltered")

if File.exists?("#{ARGV[1]}/filtered")
  FileUtils.rm_rf("#{ARGV[1]}/filtered")
end
Dir.mkdir("#{ARGV[1]}/filtered")

Dir.glob("*.idf").each do |file|

  # we don't handle parametric preprocessor
  next if /1ZoneParameterAspect.idf/.match(file)
  
  FileUtils.cp(file, "#{ARGV[1]}/unfiltered/#{File.basename(file)}")
  
  puts "Processing idf file #{file}"
  STDOUT.flush

  # load IdfFile
  idfPath = OpenStudio::Path.new(file)
  oIdfFile = OpenStudio::IdfFile.load(idfPath,"EnergyPlus".to_IddFileType)

  # Construct workspace
  workspace = OpenStudio::Workspace.new(oIdfFile.get(),"None".to_StrictnessLevel)
  STDOUT.flush
  
  File.open("#{ARGV[1]}/filtered/#{File.basename(file)}", 'w') do |f|
    f << workspace.to_s
  end
  
  if not workspace.isValid("Draft".to_StrictnessLevel)
    puts "#{file} is not valid at Draft level"
    result = false
    report = workspace.validityReport("Draft".to_StrictnessLevel)
    puts report.to_s
  end

  if not workspace.isValid("Final".to_StrictnessLevel)
    puts "#{file} is not valid at Final level"
    result = false
    report = workspace.validityReport("Final".to_StrictnessLevel)
    puts report.to_s
  end
  
  puts ""
  STDOUT.flush

end
