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
#   Make a Report after running EnergyPlus simulation
#
# == Usage
#
#  ruby MakeReport.rb ARGV[0] 
#
#  ARGV[0] - Path to EnergyPlus simulation directory for base file. 
#
# == Examples
#
#   ruby MakeReport.rb 'C:\path\to\energyplus\dir\' 
#
######################################################################

require 'openstudio'

# provide path to EnergyPlus simulation directory including file 'in.idf'
epDir = OpenStudio::Path.new(ARGV[0]) 

# load and translate idf file
idfFilepath = epDir / OpenStudio::Path.new("in.idf")
test_model = OpenStudio::EnergyPlus::loadAndTranslateIdf(idfFilepath)

if test_model.empty?
  puts "Loading and translation of EnergyPlus file failed"
  return false
end

# get model
model = test_model.get

# load sql file into the model
sqlFile = OpenStudio::SqlFile.new(epDir / OpenStudio::Path.new("eplusout.sql"))
model.setSqlFile(sqlFile)

facility = model.getFacility
  
# make report
reportVector = OpenStudio::AttributeVector.new
facilityVector = OpenStudio::AttributeVector.new

attribute = facility.getAttribute("totalSiteEnergy")
facilityVector << attribute.get

attribute = facility.getAttribute("netSiteEnergy")
facilityVector << attribute.get

attribute = facility.getAttribute("totalSourceEnergy")
facilityVector << attribute.get

attribute = facility.getAttribute("netSourceEnergy")
facilityVector << attribute.get

facilityAttribute = OpenStudio::Attribute.new("Facility", facilityVector)
reportVector << facilityAttribute

report = OpenStudio::Attribute.new("Report", reportVector)

File.open('./report.xml', 'w') do |f|
  f.puts report.to_s
end

