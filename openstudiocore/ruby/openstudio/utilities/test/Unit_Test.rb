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

class Unit_Test < MiniTest::Unit::TestCase
  # def setup
  # end

  # def teardown
  # end

  def test_UnitConstructors
    u1 = OpenStudio::ZUnit.new("k","J")
    u1.setBaseUnitExponent("kg",1)
    u1.setBaseUnitExponent("m",2)
    u1.setBaseUnitExponent("s",-2)
    assert_equal("kJ",u1.to_s)
    assert_equal(-2,u1.baseUnitExponent("s"))
    assert_equal(0,u1.baseUnitExponent("ft"))

    u2 = OpenStudio::ZUnit.new
    assert_equal("",u2.to_s)

    u3 = OpenStudio::ZUnit.new(-2)
    u3.setBaseUnitExponent("m",1)
    assert_equal("cm",u3.standardString)
    assert_equal("m",u3.standardString(false))
    assert_equal("",u3.prettyString)
  end
end
