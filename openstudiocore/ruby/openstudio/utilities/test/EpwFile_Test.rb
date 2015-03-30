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

require 'openstudio'

require 'minitest/autorun'

class EpwFile_Test < MiniTest::Unit::TestCase
  
  def setup
    @epwDir = OpenStudio::Path.new($OpenStudio_ResourcePath + "utilities/Filetypes/")
  end

  # def teardown
  # end

  def test_file
    path = @epwDir / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")
    epw = OpenStudio::EpwFile::load(path.to_s).get
    #assert_equal("F188656D", epw.checksum);
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
  end

  def test_data
    path = @epwDir / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")
    epw = OpenStudio::EpwFile::load(path.to_s).get
    data = epw.data
    assert_equal(8760, data.size)
  end

  def test_time_series
    path = @epwDir / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")
    epw = OpenStudio::EpwFile::load(path.to_s).get
    windSpeed = epw.getTimeSeries("Wind Speed")
    assert(windSpeed.is_a? OpenStudio::OptionalTimeSeries)
    assert(windSpeed.is_initialized)
  end

#  def test_errors
#    assert_raises(RuntimeError){OpenStudio::Date.new("Jan".to_MonthOfYear, 0, 2008)}
#  end

#  def test_ymd_constructor
#    assert(!OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008).nil?)
#  end
  
#  def test_ydoy_constructor
#    assert_equal(OpenStudio::Date::fromDayOfYear(1,2008), OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008))
#    assert_equal(OpenStudio::Date::fromDayOfYear(59,2008), OpenStudio::Date.new("Feb".to_MonthOfYear,28,2008))
#    assert_equal(OpenStudio::Date::fromDayOfYear(60,2008), OpenStudio::Date.new("Feb".to_MonthOfYear,29,2008))
#    assert_equal(OpenStudio::Date::fromDayOfYear(61,2008), OpenStudio::Date.new("Mar".to_MonthOfYear,1,2008))
#    assert_equal(OpenStudio::Date::fromDayOfYear(366,2008), OpenStudio::Date.new("Dec".to_MonthOfYear,31,2008))
#    assert_equal(OpenStudio::Date::fromDayOfYear(365,2009), OpenStudio::Date.new("Dec".to_MonthOfYear,31,2009))
#    
#    assert_raises(RuntimeError){OpenStudio::Date::fromDayOfYear(0,2008)} 
#    assert_raises(RuntimeError){OpenStudio::Date::fromDayOfYear(367, 2008)}
#    assert_raises(RuntimeError){OpenStudio::Date::fromDayOfYear(366, 2009)}
#  end

#  def test_equality
#  
#    d1a = OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008)
#    d1b = OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008)
#    d2a = OpenStudio::Date.new("Jan".to_MonthOfYear,2,2008)
#
#    assert( (d1a == d1b))
#    assert(!(d1a == d2a))
#    assert( (d1a != d2a))
#    assert(!(d1b == d2a))
#  end
  
#  def test_comparison
  
#    d1a = OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008)
#    d1b = OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008)
#    d2a = OpenStudio::Date.new("Jan".to_MonthOfYear,2,2008)

#    assert(!(d1a < d1b))
#    assert( (d1a <= d1b))
#    assert(!(d1a > d1b))
#    assert( (d1a <= d1b))

#    assert( (d1a < d2a))
#    assert( (d1a <= d2a))
#    assert(!(d1a > d2a))
#    assert(!(d1a >= d2a))

#    assert(!(d2a < d1a))
#    assert(!(d2a <= d1a))
#    assert( (d2a > d1a))
#    assert( (d2a >= d1a))
#  end

end


