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

