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

class Optional_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end

  def test_Optional
  
    # test can assign a value
    optional = OpenStudio::OptionalDouble.new
    assert_equal(false, optional.is_initialized)
    optional = 10.0
    if optional
      assert(true) 
      assert(optional == 10.0)
      assert_equal(10.0, optional)
    else
      assert(false) # should not get here
    end
    
    # this syntax works but is not as nice
    optional = OpenStudio::OptionalDouble.new
    if optional.is_initialized
      assert(false) # should not get here
    else
      assert(true)
    end
  
    # would like to use this syntax to test if set
    optional = OpenStudio::OptionalDouble.new
    if !optional.empty?
      assert(false) # should not get here
    else
      assert(true)
    end

    optional = OpenStudio::OptionalDouble.new
    assert(!optional.is_initialized);
    assert_raises(RuntimeError) {optional.get}
    optional.set(10.0);
    assert(optional.is_initialized);
    assert_nothing_raised { optional.get }


    
  end
  
  
end


