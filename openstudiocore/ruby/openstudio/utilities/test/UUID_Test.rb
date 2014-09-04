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

class UUID_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_uuid_constuctors

    # created UUID is not null
    uuid = OpenStudio::UUID::create()
    assert(uuid)
  
    # copy UUID
    uuid2 = uuid
    assert(uuid2)
    assert_equal(uuid, uuid2)
  
    # to string
    s = uuid.to_s
    assert(s)
  end
  
  def test_uuid_big_set
    n = 10000
    nGroup = 10
    
    (0...nGroup).each do |group|
      assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/utilities/test/UUID_BigSet_Aux.rb' '#{n}'"))
    end
    
    File.delete("UUIDSet.txt")    
  end

end


