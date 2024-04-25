########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Enum_Test < Minitest::Test

  # def setup
  # end

  # def teardown
  # end

  def test_idd_object_type
    e = OpenStudio::IddObjectType.new("LeadInput")
    assert_equal(e.value(), 2)
    assert_equal("LeadInput", e.valueName())
    assert_equal("Lead Input", e.valueDescription())

    e = "Site_Location".to_IddObjectType
    assert_equal("Site_Location", e.valueName())
    assert_equal("Site:Location", e.valueDescription())

    assert_raises(RuntimeError){"Bob".to_IddObjectType}
  end
end


