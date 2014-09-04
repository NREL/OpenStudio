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

class Vector_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end

  def test_constructor
  
    v = OpenStudio::Vector.new(0)
    assert_equal(0, v.length())
  
    v = OpenStudio::Vector.new(2)
    assert_equal(2, v.length())
    
    v = OpenStudio::Vector.new(2,-1)
    assert_equal(2, v.length())
    assert_equal(-1, v[0])
    assert_equal(-1, v[1])
    
    v[0] = 1
    v[1] = 2
    
    assert_equal(1, v[0])
    assert_equal(2, v[1])
    
  end
  
  def test_interp
  
    x = OpenStudio::Vector.new(2)
    y = OpenStudio::Vector.new(2)
  
    x[0] = 0.0
    x[1] = 1.0
    y[0] = 1.0
    y[1] = 2.0
  
    assert_in_delta(1.0, OpenStudio::interp(x, y, 0.0, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(2.0, OpenStudio::interp(x, y, 1.0, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(1.5, OpenStudio::interp(x, y, 0.5, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(1.25, OpenStudio::interp(x, y, 0.25, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(1.75, OpenStudio::interp(x, y, 0.75, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(0.0, OpenStudio::interp(x, y, -1.0, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(0.0, OpenStudio::interp(x, y, -2.0, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
  
    assert_in_delta(1.0, OpenStudio::interp(x, y, 0.0, OpenStudio::NearestInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(2.0, OpenStudio::interp(x, y, 1.0, OpenStudio::NearestInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(2.0, OpenStudio::interp(x, y, 0.5, OpenStudio::NearestInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(1.0, OpenStudio::interp(x, y, 0.25, OpenStudio::NearestInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(2.0, OpenStudio::interp(x, y, 0.75, OpenStudio::NearestInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(0.0, OpenStudio::interp(x, y, -1.0, OpenStudio::NearestInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(0.0, OpenStudio::interp(x, y, -2.0, OpenStudio::NearestInterp, OpenStudio::NoneExtrap), 0.000001)
  
    assert_in_delta(1.0, OpenStudio::interp(x, y, 0.0, OpenStudio::HoldLastInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(2.0, OpenStudio::interp(x, y, 1.0, OpenStudio::HoldLastInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(1.0, OpenStudio::interp(x, y, 0.5, OpenStudio::HoldLastInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(1.0, OpenStudio::interp(x, y, 0.25, OpenStudio::HoldLastInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(1.0, OpenStudio::interp(x, y, 0.75, OpenStudio::HoldLastInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(0.0, OpenStudio::interp(x, y, -1.0, OpenStudio::HoldLastInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(0.0, OpenStudio::interp(x, y, -2.0, OpenStudio::HoldLastInterp, OpenStudio::NoneExtrap), 0.000001)
  
    assert_in_delta(1.0, OpenStudio::interp(x, y, 0.0, OpenStudio::LinearInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(2.0, OpenStudio::interp(x, y, 1.0, OpenStudio::LinearInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(1.5, OpenStudio::interp(x, y, 0.5, OpenStudio::LinearInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(1.25, OpenStudio::interp(x, y, 0.25, OpenStudio::LinearInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(1.75, OpenStudio::interp(x, y, 0.75, OpenStudio::LinearInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(1.0, OpenStudio::interp(x, y, -1.0, OpenStudio::LinearInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(2.0, OpenStudio::interp(x, y, 2.0, OpenStudio::LinearInterp, OpenStudio::NearestExtrap), 0.000001)
  end
  
  def test_linspace
 
    vector = OpenStudio::linspace(1, 3, 3)
  
    assert_equal(3, vector.length())
    assert_in_delta(1, vector[0], 0.000001)
    assert_in_delta(2, vector[1], 0.000001)
    assert_in_delta(3, vector[2], 0.000001)
  end
  
  def test_logspace
  
    vector = OpenStudio::logspace(0, 2, 11)
  
    assert_equal(11, vector.length())
    assert_in_delta(10.0**0.0, vector[0], 0.000001)
    assert_in_delta(10.0**0.2, vector[1], 0.000001)
    assert_in_delta(10.0**0.4, vector[2], 0.000001)
    assert_in_delta(10.0**0.6, vector[3], 0.000001)
    assert_in_delta(10.0**0.8, vector[4], 0.000001)
    assert_in_delta(10.0**1.0, vector[5], 0.000001)
    assert_in_delta(10.0**1.2, vector[6], 0.000001)
    assert_in_delta(10.0**1.4, vector[7], 0.000001)
    assert_in_delta(10.0**1.6, vector[8], 0.000001)
    assert_in_delta(10.0**1.8, vector[9], 0.000001)
    assert_in_delta(10.0**2.0, vector[10], 0.000001)
  end
  
  def test_log
    vector = OpenStudio::Vector.new(3)
    vector[0] = 1
    vector[1] = 10
    vector[2] = 100

    logVector = OpenStudio::log(vector, 10.0)

    assert_equal(vector.length(), logVector.length())
    assert_in_delta(0, logVector[0], 0.000001)
    assert_in_delta(1, logVector[1], 0.000001)
    assert_in_delta(2, logVector[2], 0.000001)
  end


  def test_statistics

    vector = OpenStudio::Vector.new(8)

    # [2, 4, 4, 4, 5, 5, 7, 9]
    vector[0] = 2
    vector[1] = 4
    vector[2] = 4
    vector[3] = 4
    vector[4] = 5
    vector[5] = 5
    vector[6] = 7
    vector[7] = 9

    assert_in_delta(40.0, OpenStudio::sum(vector), 0.000001)
    assert_in_delta(9.0, OpenStudio::maximum(vector), 0.000001)
    assert_in_delta(2.0, OpenStudio::minimum(vector), 0.000001)
    assert_in_delta(5.0, OpenStudio::mean(vector), 0.000001)
    assert_in_delta(4.0, OpenStudio::variance(vector), 0.000001)
    assert_in_delta(2.0, OpenStudio::stdDev(vector), 0.000001)
  end

  def test_functors
    vector = OpenStudio::Vector.new(8)

    # [2, 4, 4, 4, 5, 5, 7, 9]
    vector[0] = 2
    vector[1] = 4
    vector[2] = 4
    vector[3] = 4
    vector[4] = 5
    vector[5] = 5
    vector[6] = 7
    vector[7] = 9

    functor = OpenStudio::sumVectorFunctor()
    assert_in_delta(40.0, OpenStudio::evaluateDoubleFromVectorFunctor(functor,vector), 0.000001)
    functor = OpenStudio::maximumVectorFunctor()
    assert_in_delta(9.0, OpenStudio::evaluateDoubleFromVectorFunctor(functor,vector), 0.000001)
    functor = OpenStudio::minimumVectorFunctor()
    assert_in_delta(2.0, OpenStudio::evaluateDoubleFromVectorFunctor(functor,vector), 0.000001)
    functor = OpenStudio::meanVectorFunctor()
    assert_in_delta(5.0, OpenStudio::evaluateDoubleFromVectorFunctor(functor,vector), 0.000001)
    functor = OpenStudio::varianceVectorFunctor()
    assert_in_delta(4.0, OpenStudio::evaluateDoubleFromVectorFunctor(functor,vector), 0.000001)
    functor = OpenStudio::stdDevVectorFunctor()
    assert_in_delta(2.0, OpenStudio::evaluateDoubleFromVectorFunctor(functor,vector), 0.000001)
    
  end
  
end


