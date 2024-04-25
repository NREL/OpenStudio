########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Checksum_Test < Minitest::Test

  # def setup
  # end

  # def teardown
  # end

  def test_strings
    assert_equal("00000000",  OpenStudio::checksum(""))
    assert_equal("1AD514BA",  OpenStudio::checksum("Hi there"))
    assert_equal("D5682D26",  OpenStudio::checksum("HI there"))
  end

  def test_paths

    # write a file
    File.open("./Checksum.txt", 'w') do |f|
      f.write("Hi there")
    end

    p = OpenStudio::Path.new("./Checksum.txt")
    assert_equal("1AD514BA",  OpenStudio::checksum(p))

    p = OpenStudio::Path.new("./NotAFile.txt")
    assert_equal("00000000",  OpenStudio::checksum(p));
  end

end


