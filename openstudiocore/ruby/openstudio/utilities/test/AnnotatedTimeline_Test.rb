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

class AnnotatedTimeline_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_stockPrices
    # make two time series with random data
    n = 1000
    t1 = OpenStudio::TimeSeries.new(OpenStudio::Date::currentDate(), OpenStudio::Time.new(0,1), OpenStudio::cumsum(OpenStudio::randVector(-1,1.1,n), 100), "pesos")
    t2 = OpenStudio::TimeSeries.new(OpenStudio::Date::currentDate(), OpenStudio::Time.new(0,1), OpenStudio::cumsum(OpenStudio::randVector(-1,1.1,n)), "dollars")

    timeline = OpenStudio::AnnotatedTimeline::create().get
    timeline.addTimeSeries("Mexican Stocks", t1)
    timeline.addTimeSeries("US Stocks", t2)
    timeline.save(OpenStudio::Path.new("./StockPrices.html"))
  end

end


