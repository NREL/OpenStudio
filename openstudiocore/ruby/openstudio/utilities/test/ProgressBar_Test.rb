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

module OpenStudio
  class MyProgressBar < ProgressBar
    
    def initialize
      super
      @@called = false
    end
    
    def self.called
      return @@called
    end 

    def onPercentageUpdated(percentage)
      self.setWindowTitle(percentage.to_s)
      @@called = true
      super
    end
  end
end

class ProgressBar_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_basic
    pb = OpenStudio::ProgressBar::new()
    for i in (0..100)
      pb.setValue(i)
      assert_equal(i, pb.value())
      sleep(0.01)
    end
  end
  
  def test_advanced
    pb1 = OpenStudio::ProgressBar::new()
    pb2 = OpenStudio::ProgressBar::new()
    pb3 = OpenStudio::ProgressBar::new()
    for i in (0..100)
      pb1.setValue(i)
      pb2.setValue(100-i)
      pb3.setValue((10*i)%100)
      sleep(0.01)
    end
  end
  
  def test_idf
    pb = OpenStudio::ProgressBar::new()
    pb.setWindowTitle("Loading Idf")

    # load IdfFile
    idfPath = OpenStudio::Path.new($OpenStudio_ResourcePath + "resultsviewer/SmallOffice/SmallOffice.idf")
    assert(OpenStudio::exists(idfPath))

    oIdfFile = OpenStudio::IdfFile.load(idfPath,"EnergyPlus".to_IddFileType, pb)
    assert(oIdfFile.empty? == false)
    
    ws = OpenStudio::Workspace.new()
    ws.connectProgressBar(pb)
    ws.addObjects(oIdfFile.get.objects)
  end
  
  def test_idf_derived
    pb = OpenStudio::MyProgressBar::new()
    assert((not OpenStudio::MyProgressBar::called))

    # load IdfFile
    idfPath = OpenStudio::Path.new($OpenStudio_ResourcePath + "resultsviewer/SmallOffice/SmallOffice.idf")
    assert(OpenStudio::exists(idfPath))

    oIdfFile = OpenStudio::IdfFile.load(idfPath,"EnergyPlus".to_IddFileType, pb)
    assert(oIdfFile.empty? == false)
    
    ws = OpenStudio::Workspace.new()
    ws.connectProgressBar(pb)
    
    ws.addObjects(oIdfFile.get.objects)
    assert(OpenStudio::MyProgressBar::called)
    
    pb = nil
    GC.start
    
    ws.addObjects(oIdfFile.get.objects)
  end
  
  def test_idf_derived_destructor
    # load IdfFile
    idfPath = OpenStudio::Path.new($OpenStudio_ResourcePath + "resultsviewer/SmallOffice/SmallOffice.idf")
    assert(OpenStudio::exists(idfPath))

    oIdfFile = OpenStudio::IdfFile.load(idfPath,"EnergyPlus".to_IddFileType)
    assert(oIdfFile.empty? == false)
    
    ws = OpenStudio::Workspace.new()

    pb = OpenStudio::MyProgressBar::new()
    assert((not OpenStudio::MyProgressBar::called))
    ws.connectProgressBar(pb)
    ws.disconnectProgressBar(pb) 


    assert((not OpenStudio::MyProgressBar::called))
    ws.addObjects(oIdfFile.get.objects)
    assert((not OpenStudio::MyProgressBar::called))
   
  end
  
end


