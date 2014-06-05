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

class Plot_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_colors
    red = OpenStudio::Color.new(OpenStudio::Red)
    assert_equal(255, red.red())
    assert_equal(0, red.green())
    assert_equal(0, red.blue())
  
    green = OpenStudio::Color.new(OpenStudio::Green)
    assert_equal(0, green.red())
    assert_equal(255, green.green())
    assert_equal(0, green.blue())
    
    blue = OpenStudio::Color.new(OpenStudio::Blue)
    assert_equal(0, blue.red())
    assert_equal(0, blue.green())
    assert_equal(255, blue.blue())
  end
end


