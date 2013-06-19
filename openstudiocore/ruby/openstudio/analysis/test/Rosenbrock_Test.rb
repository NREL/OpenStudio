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
require $OpenStudio_LibPath + 'openstudio/analysis/problems/Rosenbrock.rb'

require 'test/unit'

class Rosenbrock_Test < Test::Unit::TestCase

  def setup
    @tol = 1.0E-5
  end

  # def teardown
  # end
  
  def test_rosenbrock
    p = Rosenbrock.new
    assert(p.nx == 2)
    assert(p.nh == 0)
    assert(p.ng == 0)
         
    x = []
    x.push(1.0)
    x.push(1.0)
    val = p.f_eval(x)
    puts val
    assert(val.abs < @tol)
    
    x[0] = 5.0
    x[1] = -1.0
    val = p.f_eval(x)
    assert((val - 67616.0).abs/67616.0 < @tol)

    assert(p.h_eval(x).empty?)
    assert(p.g_eval(x).empty?)
        
  end

end
