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

class IddObject_Test < MiniTest::Unit::TestCase

  # def setup
  # end

  # def teardown
  # end
  
  def test_ExtensibleIndex
    factory = OpenStudio::IddFileAndFactoryWrapper.new("EnergyPlus".to_IddFileType)
    oio = factory.getObject("BuildingSurface:Detailed".to_IddObjectType)
    assert((not oio.empty?))
    iddObj = oio.get()
   
    extInd = OpenStudio::ExtensibleIndex.new(0,1)
    ind = iddObj.index(extInd)
    assert_equal(11,ind)
    
    extInd.group = 2
    extInd.field = 2
    ind = iddObj.index(extInd)
    assert_equal(18,ind)
    
    ind = 26
    extInd = iddObj.extensibleIndex(ind)
    assert_equal(5,extInd.group)
    assert_equal(1,extInd.field)
  
  end

end
