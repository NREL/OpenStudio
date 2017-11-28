########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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


