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

class Quantity_Test < MiniTest::Unit::TestCase
  def setup
    @tol = 1.0E-5
  end

  # def teardown
  # end
  
  def relative_near(expected,actual)
    rel_err = (expected - actual).abs/expected.abs
    if rel_err < @tol
      return true
    end
    return false
  end
  
  def absolute_near(expected,actual)
      abs_err = (expected - actual).abs
      if abs_err < @tol
        return true
      end
      return false
  end
  
  def test_arithmeticOperators
  
      q1 = OpenStudio::Quantity.new
      q2 = OpenStudio::Quantity.new(1.0)
      
      u3 = OpenStudio::ZUnit.new
      u3.setBaseUnitExponent("ft",1)
      q3 = OpenStudio::Quantity.new(34.2,u3)
    
      # addition
      q4 = q1 + q2
      q4 += q4
      assert(relative_near(2.0,q4.value()))
      assert("2" == q4.to_s)
      # ETH@20100819 Is there a way to test for a ruby raise?
      # ASSERT_THROW(q3 + q1,Exception)
    
      # subtraction;
      # ASSERT_THROW(q3 - q1,Exception)
      len = OpenStudio::Quantity.new(3.5,u3)
      len -= q3
      assert(relative_near(-30.7,len.value()))
    
      # multiplication
      q3 *= q4;
      assert(relative_near(68.4,q3.value()));
    
      u5 = OpenStudio::ZUnit.new;
      u5.setBaseUnitExponent("s",-2)
      u5.setBaseUnitExponent("ft",1)
      q5 = OpenStudio::Quantity.new(0.5,u5)
      q6 = q5 * q3
      assert(relative_near(34.2,q6.value()))
    
      # division
      len = OpenStudio::Quantity.new(1.1,u3)
      a = q6/len
      assert(relative_near(31.0909090909,a.value()))
      assert("ft/s^2" == a.standardUnitsString())
    
      # pow
      a.pow(6)
      assert("ft^6/s^12" == a.standardUnitsString())
      b = OpenStudio::pow(a,1,3)
      assert("ft^2/s^4" == b.standardUnitsString())
      assert(relative_near(966.644628099,b.value()))
      a = b
    
      # multiplication and division with double
      a *= 1.21
      assert(relative_near(1169.64,a.value()))
      b = a/2
      assert(relative_near(584.82,b.value()))
      assert("ft^2/s^4" == b.standardUnitsString())
      assert("" == b.prettyUnitsString())
    
      # Preferred syntax: c = 32.0/b.
      # Need to extend Ruby's Float class to enable preferred syntax.
      c = OpenStudio::Quantity.new(32.0)/b
      assert("s^4/ft^2" == c.standardUnitsString())
      assert(relative_near(0.054717690913,c.value()))
    
  end
  
end
