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


