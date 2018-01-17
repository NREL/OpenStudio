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
#   Translate an EnergyPlus Simulation to a OpenStudio Model, load results,
#   and check user defined tests.  We will later create a job type that
#   takes one of these scripts as input, scrapes the screen for any 
#   predefined output (e.g. out of range values ResultsViewer might want to 
#   know about), and reports pass/fail back to the project similar to other high 
#   level results.
#
# == Usage
#
#  ruby QCTest.rb ARGV[0] 
#
#  ARGV[0] - Path to EnergyPlus simulation directory. EnergyPlus simulation 
#            must be pre-run with the SQLite output including tabular data 
#            and any required tables requested.
#
# == Examples
#
#   ruby QCTest.rb 'C:\path\to\energyplus\dir\'
#
######################################################################

require 'openstudio'
require 'minitest/autorun'
require 'minitest/autorun/ui/console/testrunner'

# special function that prints report in format that job can scrape from screen and report to ResultsViewer
def check_timeseries(timeseries, min, max)
  result = true
  values = timeseries.values
  if min
    result = result and (OpenStudio::min(values) > min)
  end
  if max
    result = result and (OpenStudio::maximum(values) < max)
  end
  if not result
    puts "Timeseries out of range" # more formating so we can tell ResultsViewer what to plot to show the error
  end
  return result
end

# define the tests that the user cares about in this test case
# this class can be configured for each project
class QCTestCase < MiniTest::Unit::TestCase

  @@model = nil
  
  def self.model=(m)
    @@model = m
  end
  
  def self.model
    return @@model
  end
  
  def test_hours_simulated
    query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' AND ReportForString='Entire Facility' AND TableName='General' AND RowName='Hours Simulated' AND Units='hrs'"
    result = @@model.sqlFile.get.execAndReturnFirstDouble(query)
    assert(result)
    assert_equal(8760, result.get)
  end
  
  def test_hours_setpoint_not_met_during_occupied_heating
    query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' AND ReportForString='Entire Facility' AND RowName='Time Setpoint Not Met During Occupied Heating' AND ColumnName='Facility' AND Units='Hours'"
    result = @@model.sqlFile.get.execAndReturnFirstDouble(query)
    assert((not result.empty?))
    assert(result.get < 300)
  end
  
  def test_hours_setpoint_not_met_during_occupied_cooling
    query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' AND ReportForString='Entire Facility' AND RowName='Time Setpoint Not Met During Occupied Cooling' AND ColumnName='Facility' AND Units='Hours'"
    result = @@model.sqlFile.get.execAndReturnFirstDouble(query)
    assert((not result.empty?))
    assert(result.get < 300)
  end
  
  def test_glare_timeseries
    weatherFile = @@model.getWeatherFile
    environmentName = weatherFile.environmentName
    assert(environmentName)
    environmentName = environmentName.get
    
    building = @@model.getBuilding
    assert(building.spaces.size > 0)
    building.spaces.each do |space|
      space_name = space.name.get.upcase
      timeseries = @@model.sqlFile.get.timeSeries(environmentName, "Hourly", "Daylighting Reference Point 1 Glare Index", space_name)
      assert((not timeseries.empty?))
      assert(check_timeseries(timeseries.get, nil, 30))
    end
  end  

end


# argument to script is path to EnergyPlus simulation directory including file 'in.idf'
epDir = OpenStudio::Path.new(ARGV[0])
idfFilepath = epDir / OpenStudio::Path.new("in.idf")
test_model = OpenStudio::EnergyPlus::loadAndTranslateIdf(idfFilepath)
if test_model.empty?
  puts "Translation from EnergyPlus to Model failed."
  return false
end

# get model
model = test_model.get

# load weather file
epwFile = OpenStudio::EpwFile.new(epDir / OpenStudio::Path.new("in.epw"))
OpenStudio::Model::WeatherFile::setWeatherFile(model, epwFile)
weatherFile = model.getWeatherFile

# load results
sqlFile = OpenStudio::SqlFile.new(epDir / OpenStudio::Path.new("eplusout.sql"))
errFile = OpenStudio::EnergyPlus::ErrorFile.new(epDir / OpenStudio::Path.new("eplusout.err"))

# check error file
if not errFile.completedSuccessfully
  puts "Simulation did not complete successfully"
  return false
end

# set sqlFile on the mode
model.setSqlFile(sqlFile)

# set model for the test suite
QCTestCase::model = model

# run the test suite
MiniTest::Unit::UI::Console::TestRunner.run(QCTestCase.suite)
