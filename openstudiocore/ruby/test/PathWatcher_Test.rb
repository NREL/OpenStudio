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

class TestPathWatcher < OpenStudio::PathWatcher

  attr_accessor :changed
  
  def initialize(path)
    super
    @changed = false
  end

  def onPathChanged
    @changed = true
  end
  
  def onPathRemoved
    @changed = true
  end
  
end

class PathWatcher_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_watchFile
    p = OpenStudio::Path.new("./path_watcher_file")
    if OpenStudio::exists(p)
      OpenStudio::remove(p)
    end
    assert((not OpenStudio::exists(p)))
    
    File.open(p.to_s, 'w') do |f|
      f << "test 1"
    end
    assert(OpenStudio::exists(p))
    
    watcher = TestPathWatcher.new(p)
    assert((not watcher.changed))
    
    # write to the file
    system("echo 'test 2' > #{p.to_s}")
    
    # calls processEvents
    OpenStudio::System::msleep(1000)
    
    assert(watcher.changed)
    watcher.changed = false
    assert((not watcher.changed))
    
    OpenStudio::remove(p)
    assert((not OpenStudio::exists(p)))
    
    # calls processEvents
    OpenStudio::System::msleep(1000)
    
    assert(watcher.changed)
    
    
  end

  def test_watchDir
    p = OpenStudio::Path.new("./")
    assert(OpenStudio::exists(p))
    
    filePath = OpenStudio::Path.new("./path_watcher_dir")
    if OpenStudio::exists(filePath)
      OpenStudio::remove(filePath)
    end
    assert((not OpenStudio::exists(filePath)))
    
    watcher = TestPathWatcher.new(p)
    assert((not watcher.changed))
    
    filePath = OpenStudio::Path.new("./path_watcher_dir")
    File.open(filePath.to_s, 'w') do |f|
      f << "test 1"
    end
    assert(OpenStudio::exists(filePath))
    
    # calls processEvents
    OpenStudio::System::msleep(1000)
    
    assert(watcher.changed)
    watcher.changed = false
    assert((not watcher.changed))

    OpenStudio::remove(filePath)
    assert((not OpenStudio::exists(filePath)))
    
    # calls processEvents
    OpenStudio::System::msleep(1000)
    
    assert(watcher.changed)
    
    # try non-existent dir
    assert_raises(RuntimeError){TestPathWatcher.new(OpenStudio::Path.new("./I do not exist/"))}
  end
  
end


