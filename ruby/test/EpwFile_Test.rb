########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class EpwFile_Test < Minitest::Test

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


