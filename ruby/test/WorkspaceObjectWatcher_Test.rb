########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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


