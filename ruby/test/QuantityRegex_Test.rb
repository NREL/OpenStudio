########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class QuantityRegex_Test < Minitest::Test
  # def setup
  # end

  # def teardown
  # end

  def test_stringsAre
    assert(OpenStudio::isFixedPrecisionValue("3.012"))
  end

  def test_stringsContain
    assert(OpenStudio::containsScientificNotationValue(" .1D-1 "))
  end
end
