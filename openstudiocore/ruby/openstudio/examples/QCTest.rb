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
