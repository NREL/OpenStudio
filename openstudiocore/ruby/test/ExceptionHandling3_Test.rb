
require 'openstudio'

require 'minitest/autorun'

class Exceptions3_Test < MiniTest::Unit::TestCase

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


