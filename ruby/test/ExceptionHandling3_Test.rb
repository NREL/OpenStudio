########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Exceptions3_Test < Minitest::Test

  # def setup
  # end

  # def teardown
  # end

  def test_exceptions3

    # all it has to do is finish
    OpenStudio::System::testExceptions1
    OpenStudio::System::testExceptions2
    OpenStudio::System::testExceptions3
    OpenStudio::System::testExceptions4
    OpenStudio::System::testExceptions5
  end
end


