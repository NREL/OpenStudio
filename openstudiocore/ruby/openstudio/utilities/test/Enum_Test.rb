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

class Enum_Test < MiniTest::Unit::TestCase
  
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


