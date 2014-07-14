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

  class TestWorkspaceObjectWatcher < WorkspaceObjectWatcher
  
    def initialize(workspaceObject)
      super
    end

    def onChangeIdfObject
      super
      puts "onChangeIdfObject"    
    end
    
    def onDataFieldChange
      super
      puts "onDataFieldChange"    
    end
    
    def onNameChange
      super
      puts "onNameChange"    
    end

    def onBecomeDirty
      super
      puts "onBecomeDirty"    
    end

    def onBecomeClean
      super
      puts "onBecomeClean"    
    end

    def clearState
      super
      puts "clearState"    
    end
  
    def onRelationshipChange(index, newHandle, oldHandle)
      super
      puts "onRelationshipChange"    
    end
    
    def onRemoveFromWorkspace(handle)
      super
      puts "onRemoveFromWorkspace"    
    end
    
  end
end

class WorkspaceObjectWatcher_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_WorkspaceObjectWatcher
    workspace = OpenStudio::Workspace.new
    
    idfObject = OpenStudio::IdfObject.new("OS_Space".to_IddObjectType)
    workspaceObject = workspace.addObject(idfObject)
    assert((not workspaceObject.empty?))
    
    watcher = OpenStudio::TestWorkspaceObjectWatcher.new(workspaceObject.get)
    
    assert((not watcher.dirty))
    
    workspaceObject.get.setName("Name")
    
    assert(watcher.dirty)
    
    watcher.clearState
    
    assert((not watcher.dirty))
    
    workspaceObject.get.remove
    
    assert((not watcher.dirty))
        
  end
  
  def test_Change_NoWatcher
    workspace = OpenStudio::Workspace.new
    
    idfObject = OpenStudio::IdfObject.new("OS_Space".to_IddObjectType)
    workspaceObject = workspace.addObject(idfObject)
    assert((not workspaceObject.empty?))
    workspaceObject = workspaceObject.get
    
    for n in 0..20
      for i in 0..360
        #Direction of Relative North
        workspaceObject.setDouble(5, i.to_f)
      end
    end

  end
  
  def test_Change_DisabledWatcher
    workspace = OpenStudio::Workspace.new
    
    idfObject = OpenStudio::IdfObject.new("OS_Space".to_IddObjectType)
    workspaceObject = workspace.addObject(idfObject)
    assert((not workspaceObject.empty?))
    workspaceObject = workspaceObject.get
    
    watcher = OpenStudio::TestWorkspaceObjectWatcher.new(workspaceObject)
    watcher.disable
    
    for n in 0..20
      for i in 0..360
        #Direction of Relative North
        workspaceObject.setDouble(5, i.to_f)
      end
    end
  
  end
  
  def test_Change_EnabledWatcher
    workspace = OpenStudio::Workspace.new
    
    idfObject = OpenStudio::IdfObject.new("OS_Space".to_IddObjectType)
    workspaceObject = workspace.addObject(idfObject)
    assert((not workspaceObject.empty?))
    workspaceObject = workspaceObject.get
    
    watcher = OpenStudio::TestWorkspaceObjectWatcher.new(workspaceObject)
    
    for n in 0..20
      for i in 0..360
        #Direction of Relative North
        workspaceObject.setDouble(5, i.to_f)
      end
    end
    
  end
  
end


