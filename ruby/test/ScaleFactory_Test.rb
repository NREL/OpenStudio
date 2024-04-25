########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class ScaleFactory_Test < Minitest::Test
  # def setup
  # end

  # def teardown
  # end

  def test_printScales
    str = OpenStudio::printScales
    assert(str != "")
    puts str
  end
end
