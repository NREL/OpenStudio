########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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


