######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

require 'test/unit'

def changeTestPath(p)
  p  = OpenStudio::Path.new("./there")
  return p
end

class Path_Test < Test::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_path
    p = OpenStudio::Path.new("./here")
    assert_equal("./here", p.to_s())
  end

  def test_funcOnlyTakesAPath
    p = OpenStudio::Path.new("./here")
    OpenStudio::funcOnlyTakesAPath(p)
    
    # would like to be able to call with string and Pathname
    #s = "./here"
    #OpenStudio::funcOnlyTakesAPath(s)
    #p = Pathname.new("./here")
    #OpenStudio::funcOnlyTakesAPath(p)
  end
  
  def test_PathCopy
    p = OpenStudio::Path.new("./here")
    assert_equal("./here", p.to_s())
    
    q = p
    assert_equal("./here", q.to_s())
    assert_equal("./here", p.to_s())
    
    q = OpenStudio::Path.new("./there")
    assert_equal("./there", q.to_s())
    assert_equal("./here", p.to_s())
    
    q = changeTestPath(p)
    assert_equal("./there", q.to_s())
    assert_equal("./here", p.to_s())

  end
end


