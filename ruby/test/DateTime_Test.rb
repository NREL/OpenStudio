########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class DateTime_Test < Minitest::Test

  # def setup
  # end

  # def teardown
  # end

  def test_normalization

    baseDate = OpenStudio::Date.new("Jan".to_MonthOfYear, 1, 2008)

    testDateTime = OpenStudio::DateTime.new(baseDate, OpenStudio::Time.new(0,0,0,0))
    assert(testDateTime.date() == OpenStudio::Date.new("Jan".to_MonthOfYear, 1, 2008))

    testDateTime = OpenStudio::DateTime.new(baseDate, OpenStudio::Time.new(0,0,0,1))
    assert(testDateTime.date() == OpenStudio::Date.new("Jan".to_MonthOfYear, 1, 2008))

    testDateTime = OpenStudio::DateTime.new(baseDate, OpenStudio::Time.new(0,0,0,-1))
    assert(testDateTime.date() == OpenStudio::Date.new("Dec".to_MonthOfYear, 31, 2007))

    testDateTime = OpenStudio::DateTime.new(baseDate, OpenStudio::Time.new(0,23,59,59))
    assert(testDateTime.date() == OpenStudio::Date.new("Jan".to_MonthOfYear, 1, 2008))

    testDateTime = OpenStudio::DateTime.new(baseDate, OpenStudio::Time.new(0,-23,-59,-59))
    assert(testDateTime.date() == OpenStudio::Date.new("Dec".to_MonthOfYear, 31, 2007))

    testDateTime = OpenStudio::DateTime.new(baseDate, OpenStudio::Time.new(1,0,0,0))
    assert(testDateTime.date() == OpenStudio::Date.new("Jan".to_MonthOfYear, 2, 2008))

    testDateTime = OpenStudio::DateTime.new(baseDate, OpenStudio::Time.new(-1,0,0,0))
    assert(testDateTime.date() == OpenStudio::Date.new("Dec".to_MonthOfYear, 31, 2007))

    testDateTime = OpenStudio::DateTime.new(baseDate, OpenStudio::Time.new(1,23,59,59))
    assert(testDateTime.date() == OpenStudio::Date.new("Jan".to_MonthOfYear, 2, 2008))

    testDateTime = OpenStudio::DateTime.new(baseDate, OpenStudio::Time.new(-1,-23,-59,-59))
    assert(testDateTime.date() == OpenStudio::Date.new("Dec".to_MonthOfYear, 30, 2007))
  end

  def test_normalization2
    baseDate = OpenStudio::Date.new("Jan".to_MonthOfYear, 1, 2008)

    testDateTime = OpenStudio::DateTime.new(baseDate)
    testDateTime += OpenStudio::Time.new(0,0,0,0)
    assert(testDateTime.date() == OpenStudio::Date.new("Jan".to_MonthOfYear, 1, 2008))

    testDateTime = OpenStudio::DateTime.new(baseDate)
    testDateTime -= OpenStudio::Time.new(0,0,0,0)
    assert(testDateTime.date() == OpenStudio::Date.new("Jan".to_MonthOfYear, 1, 2008))

    testDateTime = OpenStudio::DateTime.new(baseDate)
    testDateTime += OpenStudio::Time.new(0,0,0,1)
    assert(testDateTime.date() == OpenStudio::Date.new("Jan".to_MonthOfYear, 1, 2008))

    testDateTime = OpenStudio::DateTime.new(baseDate)
    testDateTime -= OpenStudio::Time.new(0,0,0,1)
    assert(testDateTime.date() == OpenStudio::Date.new("Dec".to_MonthOfYear, 31, 2007))

    testDateTime = OpenStudio::DateTime.new(baseDate)
    testDateTime += OpenStudio::Time.new(0,23,59,59)
    assert(testDateTime.date() == OpenStudio::Date.new("Jan".to_MonthOfYear, 1, 2008))

    testDateTime = OpenStudio::DateTime.new(baseDate)
    testDateTime -= OpenStudio::Time.new(0,23,59,59)
    assert(testDateTime.date() == OpenStudio::Date.new("Dec".to_MonthOfYear, 31, 2007))

    testDateTime = OpenStudio::DateTime.new(baseDate)
    testDateTime += OpenStudio::Time.new(1,0,0,0)
    assert(testDateTime.date() == OpenStudio::Date.new("Jan".to_MonthOfYear, 2, 2008))

    testDateTime = OpenStudio::DateTime.new(baseDate)
    testDateTime -= OpenStudio::Time.new(1,0,0,0)
    assert(testDateTime.date() == OpenStudio::Date.new("Dec".to_MonthOfYear, 31, 2007))

    testDateTime = OpenStudio::DateTime.new(baseDate)
    testDateTime += OpenStudio::Time.new(1,23,59,59)
    assert(testDateTime.date() == OpenStudio::Date.new("Jan".to_MonthOfYear, 2, 2008))

    testDateTime = OpenStudio::DateTime.new(baseDate)
    testDateTime -= OpenStudio::Time.new(1,23,59,59)
    assert(testDateTime.date() == OpenStudio::Date.new("Dec".to_MonthOfYear, 30, 2007))
  end

  def test_to_s
    baseDate = OpenStudio::Date.new("Jan".to_MonthOfYear, 1, 2008)

    testDateTime = OpenStudio::DateTime.new(baseDate)
    assert(!testDateTime.to_s.nil?)

    testDateTime += OpenStudio::Time.new(1,1,1,1)
    assert(!testDateTime.to_s.nil?)
  end

end


