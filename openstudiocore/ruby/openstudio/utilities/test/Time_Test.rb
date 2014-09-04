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

class Time_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def numberOfSeconds(days, hours, minutes, seconds)
    return 24*60*60*days + 60*60*hours + 60*minutes + seconds
  end
  
  def numberOfDays(days, hours, minutes, seconds)
    return days + 1.0/(24.0)*hours + 1.0/(24.0*60.0)*minutes + 1.0/(24.0*60.0*60.0)*seconds
  end
  
  def test_fracDaysConstructor
  
    testTime = OpenStudio::Time.new(0.0)
    assert_equal(testTime.totalSeconds(), 0)
  
    testTime = OpenStudio::Time.new(0.5)
    assert_equal(testTime.totalSeconds(), 60*60*12)
  
    testTime = OpenStudio::Time.new(1.0)
    assert_equal(testTime.totalSeconds(), 60*60*24)
  end
  
  
  def test_daysHoursMinutesSecondsConstructor
 
    testTime = OpenStudio::Time.new(0,0,0,0)
    assert_in_delta(testTime.totalDays(), 0.0/(24*60*60), 0.00001)
    assert_in_delta(testTime.totalHours(), 0.0/(60*60), 0.00001)
    assert_in_delta(testTime.totalMinutes(), 0.0/60, 0.00001)
    assert_in_delta(testTime.totalSeconds(), 0, 0.00001)
    assert_in_delta(testTime.days(), 0, 0.00001)
    assert_in_delta(testTime.hours(), 0, 0.00001)
    assert_in_delta(testTime.minutes(), 0, 0.00001)
    assert_in_delta(testTime.seconds(), 0, 0.00001)
  
    testTime = OpenStudio::Time.new(0,0,0,1)
    assert_in_delta(testTime.totalDays(), 1.0/(24*60*60), 0.00001)
    assert_in_delta(testTime.totalHours(), 1.0/(60*60), 0.00001)
    assert_in_delta(testTime.totalMinutes(), 1.0/60, 0.00001)
    assert_in_delta(testTime.totalSeconds(), 1, 0.00001)
    assert_in_delta(testTime.days(), 0, 0.00001)
    assert_in_delta(testTime.hours(), 0, 0.00001)
    assert_in_delta(testTime.minutes(), 0, 0.00001)
    assert_in_delta(testTime.seconds(), 1, 0.00001)
  
    testTime = OpenStudio::Time.new(0,0,1,0)
    assert_in_delta(testTime.totalDays(), 1.0/(24*60), 0.00001)
    assert_in_delta(testTime.totalHours(), 1.0/(60), 0.00001)
    assert_in_delta(testTime.totalMinutes(), 1.0, 0.00001)
    assert_in_delta(testTime.totalSeconds(), 60, 0.00001)
    assert_in_delta(testTime.days(), 0, 0.00001)
    assert_in_delta(testTime.hours(), 0, 0.00001)
    assert_in_delta(testTime.minutes(), 1, 0.00001)
    assert_in_delta(testTime.seconds(), 0, 0.00001)
  
    testTime = OpenStudio::Time.new(0,1,0,0)
    assert_in_delta(testTime.totalDays(), 1.0/24, 0.00001)
    assert_in_delta(testTime.totalHours(), 1.0, 0.00001)
    assert_in_delta(testTime.totalMinutes(), 60.0, 0.00001)
    assert_in_delta(testTime.totalSeconds(), 60*60, 0.00001)
    assert_in_delta(testTime.days(), 0, 0.00001)
    assert_in_delta(testTime.hours(), 1, 0.00001)
    assert_in_delta(testTime.minutes(), 0, 0.00001)
    assert_in_delta(testTime.seconds(), 0, 0.00001)
 
    testTime = OpenStudio::Time.new(1,0,0,0)
    assert_in_delta(testTime.totalDays(), 1.0, 0.00001)
    assert_in_delta(testTime.totalHours(), 24.0, 0.00001)
    assert_in_delta(testTime.totalMinutes(), 24.0*60.0, 0.00001)
    assert_in_delta(testTime.totalSeconds(), 24.0*60*60, 0.00001)
    assert_in_delta(testTime.days(), 1, 0.00001)
    assert_in_delta(testTime.hours(), 0, 0.00001)
    assert_in_delta(testTime.minutes(), 0, 0.00001)
    assert_in_delta(testTime.seconds(), 0, 0.00001)
 
    testTime = OpenStudio::Time.new(1,1,1,1)
    assert_in_delta(testTime.totalDays(), 1.0+1.0/24+1.0/(24*60)+1.0/(24*60*60), 0.00001)
    assert_in_delta(testTime.totalHours(), 24+1.0+1.0/60+1.0/(60*60), 0.00001)
    assert_in_delta(testTime.totalMinutes(), 24*60+60+1+1.0/60, 0.00001)
    assert_in_delta(testTime.totalSeconds(), 24*60*60+60*60+60+1, 0.00001)
    assert_in_delta(testTime.days(), 1, 0.00001)
    assert_in_delta(testTime.hours(), 1, 0.00001)
    assert_in_delta(testTime.minutes(), 1, 0.00001)
    assert_in_delta(testTime.seconds(), 1, 0.00001)
  end
  
  def test_to_s
    assert(!OpenStudio::Time.new(-1.0).to_s.nil?)
    assert(!OpenStudio::Time.new(0.0).to_s.nil?)
    assert(!OpenStudio::Time.new(0.5).to_s.nil?)
    assert(!OpenStudio::Time.new(1.0).to_s.nil?)
    assert(!OpenStudio::Time.new(2.0).to_s.nil?)
  end

end


