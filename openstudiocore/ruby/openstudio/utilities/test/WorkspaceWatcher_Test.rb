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

  class TestWorkspaceWatcher < WorkspaceWatcher
  
    attr_accessor :addedObjectHandle, :removedObjectHandle
  
    def initialize(workspace)
      @addedObjectHandle = nil 
      @removedObjectHandle = nil    
      super
    end

    def clearState
      super
      @addedObjectHandle = nil 
      @removedObjectHandle = nil
      puts "clearState"
    end
    
    def onChangeWorkspace
      super
      puts "onChangeWorkspace"
    end

    def onBecomeDirty
      super
      puts "onBecomeDirty"
    end

    def onBecomeClean
      super
      puts "onBecomeClean"
    end

    def onObjectAdd(addedObject)
      super
      @addedObjectHandle = addedObject.handle
      puts "onObjectAdd"
    end

    def onObjectRemove(removedObject)
      super
      @removedObjectHandle = removedObject.handle
      puts "removedObject"
    end

  end

end


class WorkspaceWatcher_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_WorkspaceWatcher
  
    workspace = OpenStudio::Workspace.new
    watcher = OpenStudio::TestWorkspaceWatcher.new(workspace)
    
    assert((not watcher.dirty))
    assert((not watcher.addedObjectHandle))
    assert((not watcher.removedObjectHandle))
    
    idfObject = OpenStudio::IdfObject.new("OS_Space".to_IddObjectType)
    addedObject = workspace.addObject(idfObject)
    assert((not addedObject.empty?))
    addedObjectHandle = addedObject.get.handle
    
    assert_equal(1, workspace.numObjects)
    assert(watcher.dirty)
    assert(watcher.objectAdded)
    assert(watcher.enabled)
    OpenStudio::Application::instance.processEvents
    assert(watcher.addedObjectHandle)
    assert((not watcher.removedObjectHandle))
    assert_equal(addedObjectHandle.to_s, watcher.addedObjectHandle.to_s)
    
    watcher.clearState
      
    assert((not watcher.dirty))
    assert((not watcher.addedObjectHandle))
    assert((not watcher.removedObjectHandle))
    
    addedObject.get.remove
    
    assert_equal(0, workspace.numObjects)
    assert(watcher.dirty)
    assert((not watcher.addedObjectHandle))
    assert(watcher.removedObjectHandle)
    assert_equal(addedObjectHandle.to_s, watcher.removedObjectHandle.to_s)
    
  end

end


