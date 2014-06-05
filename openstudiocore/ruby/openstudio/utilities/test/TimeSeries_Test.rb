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

require 'openstudio'

require 'minitest/autorun'

class TimeSeries_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_IntervalConstructor
 
    # fill vector with 3 days worth of hourly data
    values = OpenStudio::Vector.new(3*24, 0.000001)
    for i in (0..3*24-1)
      values[i] = i
    end
  
    # create timeSeries with hourly interval
    date = OpenStudio::Date.new(OpenStudio::MonthOfYear.new("Jan"), 1, 2009)
    time = OpenStudio::Time.new(0,1,0,0)
    timeSeries = OpenStudio::TimeSeries.new(date, time, values, "W")
  
    assert_equal(72, timeSeries.daysFromFirstReport().length())
    assert_equal(72, timeSeries.values().length())
    assert_in_delta(0.0, timeSeries.daysFromFirstReport()[0], 0.000001)
    assert_in_delta(2.0+23.0/24.0, timeSeries.daysFromFirstReport()[71], 0.000001)
  end
  
  def test_ValueAt
 
    # fill vector with 3 hours of data
    values = OpenStudio::Vector.new(3)
    for i in (0..3-1)
      values[i] = i
    end
  
    # create timeSeries with hourly interval
    date = OpenStudio::Date.new(OpenStudio::MonthOfYear.new("Jan"), 1, 2009)
    time = OpenStudio::Time.new(0,1,0,0)
    timeSeries = OpenStudio::TimeSeries.new(date, time, values, "W")
    
    assert_in_delta(0.0, timeSeries.value(OpenStudio::Time.new(0,0,0,0)), 0.000001)
    assert_in_delta(1.0, timeSeries.value(OpenStudio::Time.new(0,0,0,1)), 0.000001)
    assert_in_delta(1.0, timeSeries.value(OpenStudio::Time.new(0,0,1,0)), 0.000001)
    assert_in_delta(1.0, timeSeries.value(OpenStudio::Time.new(0,0,59,0)), 0.000001)
    assert_in_delta(1.0, timeSeries.value(OpenStudio::Time.new(0,1,0,0)), 0.000001)
    assert_in_delta(2.0, timeSeries.value(OpenStudio::Time.new(0,1,0,1)), 0.000001)
    assert_in_delta(2.0, timeSeries.value(OpenStudio::Time.new(0,1,1,0)), 0.000001)
    assert_in_delta(2.0, timeSeries.value(OpenStudio::Time.new(0,1,59,0)), 0.000001)
    assert_in_delta(2.0, timeSeries.value(OpenStudio::Time.new(0,2,0,0)), 0.000001)
  end

end


