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

class FloodPlot_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  
  def test_LinearScaleColorLevelsContourLevels
  
    matrix = OpenStudio::Matrix.new(10,10)
    for i in (0..9)
      for j in (0..9)
        matrix[i,j] = 10*i+j
      end
    end
  
    fp = OpenStudio::FloodPlot::create(matrix).get
    fp.generateImage(OpenStudio::Path.new("./testMatrixConstructor.png"))
    
    fp.colorMapRange(20,60)
    ls = OpenStudio::linspace( 20,60,20 )
    fp.colorLevels(ls)
    fp.generateImage(OpenStudio::Path.new("./testLinearScaleColorLevels.png"))
    
    fp.contourLevels(ls)
    fp.showContour(true)
    fp.generateImage(OpenStudio::Path.new("./testLinearScaleColorLevelsContourLevels.png"))

    assert(fp)  
  end

end


