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

# DLM@what are the options to distinuguish between OpenStudio::Date and built in Date?

class Calendar_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_daylight_savings
    calendar = OpenStudio::Calendar.new(2009)
  
    jan1 = OpenStudio::Date.new("Jan".to_MonthOfYear, 1, 2009)
    mar7 = OpenStudio::Date.new("Mar".to_MonthOfYear, 7, 2009)
    mar8 = OpenStudio::Date.new("Mar".to_MonthOfYear, 8, 2009)
    mar9 = OpenStudio::Date.new("Mar".to_MonthOfYear, 9, 2009)
    jun12 = OpenStudio::Date.new("Jun".to_MonthOfYear, 12, 2009)
    oct31 = OpenStudio::Date.new("Oct".to_MonthOfYear, 31, 2009)
    nov1 = OpenStudio::Date.new("Nov".to_MonthOfYear, 1, 2009)
    nov2 = OpenStudio::Date.new("Nov".to_MonthOfYear, 2, 2009)
    dec31 = OpenStudio::Date.new("Dec".to_MonthOfYear, 31, 2009)
  
    # nothing is in daylight savings yet
    assert(!calendar.isDaylightSavings(jan1))
    assert(!calendar.isDaylightSavings(mar7))
    assert(!calendar.isDaylightSavings(mar8))
    assert(!calendar.isDaylightSavings(mar9))
    assert(!calendar.isDaylightSavings(jun12))
    assert(!calendar.isDaylightSavings(oct31))
    assert(!calendar.isDaylightSavings(nov1))
    assert(!calendar.isDaylightSavings(nov2))
    assert(!calendar.isDaylightSavings(dec31))
  
    # turn on standard daylight savings
    calendar.standardDaylightSavings()
  
    # mar8-oct31 are in daylight savings for 2009
    assert(!calendar.isDaylightSavings(jan1))
    assert(!calendar.isDaylightSavings(mar7))
    assert(calendar.isDaylightSavings(mar8))
    assert(calendar.isDaylightSavings(mar9))
    assert(calendar.isDaylightSavings(jun12))
    assert(calendar.isDaylightSavings(oct31))
    assert(!calendar.isDaylightSavings(nov1))
    assert(!calendar.isDaylightSavings(nov2))
    assert(!calendar.isDaylightSavings(dec31))

  end
  
  
  def test_holidays
    calendar = OpenStudio::Calendar.new(2009)
    
    # turn on standard holidays
    calendar.standardHolidays()
  
    jan1 = OpenStudio::Date.new("Jan".to_MonthOfYear, 1, 2009)
    assert(calendar.isNamedDay(jan1))
    assert(calendar.isHoliday(jan1))
    assert_equal("New Years Day", calendar.getName(jan1))
  
    jan2 = OpenStudio::Date.new("Jan".to_MonthOfYear, 2, 2009)
    assert(!calendar.isNamedDay(jan2))
    assert(!calendar.isHoliday(jan2))
    assert_equal("", calendar.getName(jan2))
  
    nov26 = OpenStudio::Date.new("Nov".to_MonthOfYear, 26, 2009)
    assert(calendar.isNamedDay(nov26))
    assert(calendar.isHoliday(nov26))
    assert_equal("Thanksgiving", calendar.getName(nov26))
  
    nov27 = OpenStudio::Date.new("Nov".to_MonthOfYear, 27, 2009)
    assert(calendar.isNamedDay(nov27))
    assert(calendar.isHoliday(nov27))
    assert_equal("Day After Thanksgiving", calendar.getName(nov27), 2009)
    
    nov28 = OpenStudio::Date.new("Nov".to_MonthOfYear, 28, 2009)
    assert(!calendar.isNamedDay(nov28))
    assert(!calendar.isHoliday(nov28))
    assert_equal("", calendar.getName(nov28))
  end
  
  def test_custom_days
    calendar = OpenStudio::Calendar.new(2009)
  
    apr12 = OpenStudio::Date.new("Apr".to_MonthOfYear, 12, 2009)
    assert(!calendar.isNamedDay(apr12))
    assert(!calendar.isHoliday(apr12))
    assert_equal("", calendar.getName(apr12))
  
    jun12 = OpenStudio::Date.new("Jun".to_MonthOfYear, 12, 2009)
    assert(!calendar.isNamedDay(jun12))
    assert(!calendar.isHoliday(jun12))
    assert_equal("", calendar.getName(jun12))
  
    calendar.addHoliday(apr12, "Easter")
    calendar.addNamedDay(jun12, "Dan's Birthday")
  
    assert(calendar.isNamedDay(apr12))
    assert(calendar.isHoliday(apr12))
    assert_equal("Easter", calendar.getName(apr12))
  
    assert(calendar.isNamedDay(jun12))
    assert(!calendar.isHoliday(jun12))
    assert_equal("Dan's Birthday", calendar.getName(jun12))
  end


end


