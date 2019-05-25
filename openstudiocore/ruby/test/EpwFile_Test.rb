########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class EpwFile_Test < MiniTest::Unit::TestCase

  def setup
    @epwDir = OpenStudio::Path.new(File.join(File.dirname(__FILE__), "../../resources/utilities/Filetypes/"))
  end

  # def teardown
  # end

  def test_file
    path = @epwDir / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")
    epw = OpenStudio::EpwFile::load(path.to_s).get
    assert_equal("Denver Centennial  Golden   Nr", epw.city);
    assert_equal("CO", epw.stateProvinceRegion);
    assert_equal("USA", epw.country);
    assert_equal("TMY3", epw.dataSource);
    assert_equal("724666", epw.wmoNumber);
    assert_equal(39.74, epw.latitude);
    assert_equal(-105.18, epw.longitude);
    assert_equal(-7, epw.timeZone);
    assert_equal(1829, epw.elevation);
    startDate = epw.startDate
    assert(startDate.is_a? OpenStudio::Date)
    assert_equal(1, startDate.dayOfMonth)
    assert_equal(1, startDate.monthOfYear.value)
    endDate = epw.endDate
    assert(endDate.is_a? OpenStudio::Date)
    assert_equal(31, endDate.dayOfMonth)
    assert_equal(12, endDate.monthOfYear.value)
  end

  def test_data
    path = @epwDir / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")
    epw = OpenStudio::EpwFile::load(path.to_s).get
    data = epw.data
    assert_equal(8760, data.size)
    dataPoint = data[8759];
    assert(dataPoint.is_a? OpenStudio::EpwDataPoint)
    assert_equal(4, dataPoint.dryBulbTemperature.get)
    assert_equal(81100, dataPoint.atmosphericStationPressure.get)
    assert_equal(-1.0,dataPoint.getFieldByName("Dew Point Temperature").get)
    field = "Dew Point Temperature".to_EpwDataField
    assert_equal(-1.0,dataPoint.getField(field).get)
    known = [ "1996", "12", "31", "24", "0",
      "?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9", "4.0", "-1.0",
      "69", "81100", "0", "0", "294", "0.000000", "0", "0", "0", "0", "0",
      "0", "130", "6.200000", "9", "9", "48.3", "7500", "9", "999999999",
      "60", "0.0310", "0", "88", "0.210", "999", "99" ]
    epwStrings = dataPoint.toEpwStrings
    assert_equal(35, epwStrings.size)
    (0..34).each do |i|
        assert_equal(known[i], epwStrings[i])
    end
  end

  def test_time_series
    path = @epwDir / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")
    epw = OpenStudio::EpwFile::load(path.to_s).get
    windSpeedOpt = epw.getTimeSeries("Wind Speed")
    assert(windSpeedOpt.is_a? OpenStudio::OptionalTimeSeries)
    assert(windSpeedOpt.is_initialized)
    windSpeed = windSpeedOpt.get
    assert(windSpeed.is_a? OpenStudio::TimeSeries)
    first = windSpeed.firstReportDateTime
    assert(first.is_a? OpenStudio::DateTime)
    assert_equal(1, first.date.dayOfMonth)
    assert_equal(1, first.date.monthOfYear.value)
    assert_equal(1, first.time.hours)
    assert_equal(0, first.time.minutes)
    assert_equal(0, first.time.seconds)
    secs = windSpeed.secondsFromFirstReport
    assert(secs.is_a? Array)
    assert_equal(8760, secs.size)
    assert_equal(8759*60*60, secs[8759])
  end

end


