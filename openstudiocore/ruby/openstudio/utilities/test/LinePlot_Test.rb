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

class LinePlot_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_constructor
    lp = OpenStudio::LinePlot::create()
  end
  
  def test_repeatedConstructor
    lp = OpenStudio::LinePlot::create()
    lp = OpenStudio::LinePlot::create()
    lp = OpenStudio::LinePlot::create()
    lp = OpenStudio::LinePlot::create()
  end
  
  def test_VectorLinePlotData
  
    xVector = OpenStudio::Vector.new(11)
    yVector = OpenStudio::Vector.new(11)
    for i in (0..10)
      xVector[i] = i
      yVector[i] = 10 - i
    end
  
    data = OpenStudio::VectorLinePlotData::create(xVector, yVector)
  
    lp = OpenStudio::LinePlot::create().get
    lp.linePlotData(data, "Vector Test", OpenStudio::Color.new(OpenStudio::Red))
    lp.axesFontSize(12)
    lp.tickFontSize(10)
    lp.bottomAxisTitle("X axis")
    lp.leftAxisTitle("Y axis")
    lp.generateImage(OpenStudio::Path.new("./testVectorLinePlotData.png"))
  end
  

end


