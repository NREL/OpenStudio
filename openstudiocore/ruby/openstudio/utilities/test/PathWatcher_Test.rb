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


