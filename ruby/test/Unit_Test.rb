########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Unit_Test < Minitest::Test
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
