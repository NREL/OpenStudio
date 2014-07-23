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

class Checksum_Test < MiniTest::Unit::TestCase
  
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


