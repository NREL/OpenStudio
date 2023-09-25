########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Date_Test < MiniTest::Unit::TestCase

  # def setup
  # end

  # def teardown
  # end

  def test_errors
    assert_raises(RuntimeError){OpenStudio::Date.new("Jan".to_MonthOfYear, 0, 2008)}
  end

  def test_ymd_constructor
    assert(!OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008).nil?)
    assert(!OpenStudio::Date.new("Jan".to_MonthOfYear,31,2008).nil?)
    assert(!OpenStudio::Date.new("Feb".to_MonthOfYear,28,2008).nil?)
    assert(!OpenStudio::Date.new("Feb".to_MonthOfYear,29,2008).nil?)
    assert(!OpenStudio::Date.new("Dec".to_MonthOfYear,1,2008).nil?)
    assert(!OpenStudio::Date.new("Dec".to_MonthOfYear,31,2008).nil?)

    assert_raises(RuntimeError){OpenStudio::Date.new("Jan".to_MonthOfYear,0,2008)}
    assert_raises(RuntimeError){OpenStudio::Date.new("Dec".to_MonthOfYear,0,2008)}

    # TC 2021-05-26  After the upgrade to ruby 2.7.2, Ruby on Windows segfaults running the below tests. 
    # These throw a RuntimeError when ran outside the minitest. It seems isolated to ruby 2.7.2, minitest on Windows
    #assert_raises(RuntimeError){OpenStudio::Date.new("Jan".to_MonthOfYear,32,2008)}
    #assert_raises(RuntimeError){OpenStudio::Date.new("Feb".to_MonthOfYear,30,2008)}
    #assert_raises(RuntimeError){OpenStudio::Date.new("Dec".to_MonthOfYear,32,2008)}
    #assert_raises(RuntimeError){OpenStudio::Date.new("NotAMonth".to_MonthOfYear,1,2008)}
    #assert_raises(RuntimeError){OpenStudio::Date.new("NumMonths".to_MonthOfYear,1,2008)}
  end

  def test_ydoy_constructor
    assert_equal(OpenStudio::Date::fromDayOfYear(1,2008), OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008))
    assert_equal(OpenStudio::Date::fromDayOfYear(59,2008), OpenStudio::Date.new("Feb".to_MonthOfYear,28,2008))
    assert_equal(OpenStudio::Date::fromDayOfYear(60,2008), OpenStudio::Date.new("Feb".to_MonthOfYear,29,2008))
    assert_equal(OpenStudio::Date::fromDayOfYear(61,2008), OpenStudio::Date.new("Mar".to_MonthOfYear,1,2008))
    assert_equal(OpenStudio::Date::fromDayOfYear(366,2008), OpenStudio::Date.new("Dec".to_MonthOfYear,31,2008))
    assert_equal(OpenStudio::Date::fromDayOfYear(365,2009), OpenStudio::Date.new("Dec".to_MonthOfYear,31,2009))

    assert_raises(RuntimeError){OpenStudio::Date::fromDayOfYear(0,2008)}
    assert_raises(RuntimeError){OpenStudio::Date::fromDayOfYear(367, 2008)}
    assert_raises(RuntimeError){OpenStudio::Date::fromDayOfYear(366, 2009)}
  end

  def test_leap_year

    testDate = OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008)
    assert_equal(testDate.year(), 2008)
    assert_equal(testDate.monthOfYear(), "Jan".to_MonthOfYear)
    assert_equal(testDate.dayOfMonth(), 1)

    testDate = OpenStudio::Date.new("Jan".to_MonthOfYear,31,2008)
    assert_equal(testDate.year(), 2008)
    assert_equal(OpenStudio::month(testDate.monthOfYear()), 1)
    assert_equal(testDate.dayOfMonth(), 31)

    testDate = OpenStudio::Date.new("Feb".to_MonthOfYear,28,2008)
    assert_equal(testDate.year(), 2008)
    assert_equal(OpenStudio::month(testDate.monthOfYear()), 2)
    assert_equal(testDate.dayOfMonth(), 28)

    testDate = OpenStudio::Date.new("Feb".to_MonthOfYear,29,2008)
    assert_equal(testDate.year(), 2008)
    assert_equal(OpenStudio::month(testDate.monthOfYear()), 2)
    assert_equal(testDate.dayOfMonth(), 29)

    testDate = OpenStudio::Date.new("Dec".to_MonthOfYear,31,2008)
    assert_equal(testDate.year(), 2008)
    assert_equal(OpenStudio::month(testDate.monthOfYear()), 12)
    assert_equal(testDate.dayOfMonth(), 31)

    assert_raises(RuntimeError){OpenStudio::Date.new("Feb".to_MonthOfYear,30,2008)}
  end

  def test_non_leap_year

    testDate = OpenStudio::Date.new("Jan".to_MonthOfYear,1,2009)
    assert_equal(testDate.year(), 2009)
    assert_equal(OpenStudio::month(testDate.monthOfYear()), 1)
    assert_equal(testDate.dayOfMonth(), 1)

    testDate = OpenStudio::Date.new("Jan".to_MonthOfYear,31,2009)
    assert_equal(testDate.year(), 2009)
    assert_equal(OpenStudio::month(testDate.monthOfYear()), 1)
    assert_equal(testDate.dayOfMonth(), 31)

    testDate = OpenStudio::Date.new("Feb".to_MonthOfYear,28,2009)
    assert_equal(testDate.year(), 2009)
    assert_equal(OpenStudio::month(testDate.monthOfYear()), 2)
    assert_equal(testDate.dayOfMonth(), 28)

    testDate = OpenStudio::Date.new("Dec".to_MonthOfYear,31,2009)
    assert_equal(testDate.year(), 2009)
    assert_equal(OpenStudio::month(testDate.monthOfYear()), 12)
    assert_equal(testDate.dayOfMonth(), 31)

    assert_raises(RuntimeError){OpenStudio::Date.new("Feb".to_MonthOfYear,29,2009)}
  end

  def test_equality

    d1a = OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008)
    d1b = OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008)
    d2a = OpenStudio::Date.new("Jan".to_MonthOfYear,2,2008)

    assert( (d1a == d1b))
    assert(!(d1a == d2a))
    assert( (d1a != d2a))
    assert(!(d1b == d2a))
  end

  def test_comparison

    d1a = OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008)
    d1b = OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008)
    d2a = OpenStudio::Date.new("Jan".to_MonthOfYear,2,2008)

    assert(!(d1a < d1b))
    assert( (d1a <= d1b))
    assert(!(d1a > d1b))
    assert( (d1a <= d1b))

    assert( (d1a < d2a))
    assert( (d1a <= d2a))
    assert(!(d1a > d2a))
    assert(!(d1a >= d2a))

    assert(!(d2a < d1a))
    assert(!(d2a <= d1a))
    assert( (d2a > d1a))
    assert( (d2a >= d1a))
  end

  def test_NthDayOfWeekInMonthAssumedYear

    firstSundayInJanuary = OpenStudio::Date::fromNthDayOfMonth("first".to_NthDayOfWeekInMonth,
                                                        "Sunday".to_DayOfWeek, "Jan".to_MonthOfYear)
    assert_equal("Sunday".to_DayOfWeek, firstSundayInJanuary.dayOfWeek())
    assert_equal("Jan".to_MonthOfYear, firstSundayInJanuary.monthOfYear())
    assert(firstSundayInJanuary <= OpenStudio::Date.new("Jan".to_MonthOfYear, 7))

    secondSundayInJanuary = OpenStudio::Date::fromNthDayOfMonth("second".to_NthDayOfWeekInMonth, "Sunday".to_DayOfWeek, "Jan".to_MonthOfYear)
    assert_equal("Sunday".to_DayOfWeek, secondSundayInJanuary.dayOfWeek())
    assert_equal("Jan".to_MonthOfYear, secondSundayInJanuary.monthOfYear())
    assert(secondSundayInJanuary <= OpenStudio::Date.new("Jan".to_MonthOfYear, 14))
    assert(firstSundayInJanuary < secondSundayInJanuary)
  end

  def test_NthDayOfWeekInMonthQualifiedYear

    firstSundayInJanuary = OpenStudio::Date::fromNthDayOfMonth("first".to_NthDayOfWeekInMonth, "Sunday".to_DayOfWeek, "Jan".to_MonthOfYear, 2008)
    assert_equal("Sunday".to_DayOfWeek, firstSundayInJanuary.dayOfWeek())
    assert_equal("Jan".to_MonthOfYear, firstSundayInJanuary.monthOfYear())
    assert(firstSundayInJanuary == OpenStudio::Date.new("Jan".to_MonthOfYear, 6, 2008))

    secondSundayInJanuary = OpenStudio::Date::fromNthDayOfMonth("second".to_NthDayOfWeekInMonth, "Sunday".to_DayOfWeek, "Jan".to_MonthOfYear, 2008)
    assert_equal("Sunday".to_DayOfWeek, secondSundayInJanuary.dayOfWeek())
    assert_equal("Jan".to_MonthOfYear, secondSundayInJanuary.monthOfYear())
    assert(secondSundayInJanuary == OpenStudio::Date.new("Jan".to_MonthOfYear, 13, 2008))
  end

  def test_to_s
    assert(!OpenStudio::Date.new("Jan".to_MonthOfYear,1,2008).to_s.nil?)
    assert(!OpenStudio::Date::fromDayOfYear(1,2008).to_s.nil?)
  end

  def test_enums
    monday = "Monday".to_DayOfWeek
    assert_equal("Monday",monday.valueName)

    june = "Jun".to_MonthOfYear
    assert_equal("June",june.valueDescription)
  end

end


