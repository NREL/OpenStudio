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

class DateTime_Test < MiniTest::Unit::TestCase
  
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


