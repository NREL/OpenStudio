########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

module OpenStudio

  # This class is used for both Space Groups and Detached Shading Groups.
  class SurfaceGroupObserver < Sketchup::EntityObserver

    def initialize(drawing_interface)
      @drawing_interface = drawing_interface
      @enabled = false
    end
    
    def disable
      was_enabled = @enabled
      @enabled = false
      return was_enabled
    end
    
    def enable
      @enabled = true
    end
    
    def destroy
      @drawing_interface = nil
      @enabled = false
    end 
    
    # Group was moved, scaled, or rotated.
    # Note that onChangeEntity is NOT triggered for the enclosed entities!
    # Vertices of the enclosed entities DO NOT change...instead a transformation is applied to the Group.
    def onChangeEntity(entity)
      
      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
      
      return if not @enabled

      # http://www.thomthom.net/software/sketchup/observers/#note_EntityObserver
      # EntityObserver.onChangeEntity mistriggers right before EntityObserver.onEraseEntity, referencing a non-existant entity. 
      # EntityObserver.onEraseEntity reference a non-existant entity. 
      
      proc = Proc.new {

        # When a group is erased, one last call to 'onChangeEntity' is still issued.
        if (@drawing_interface.valid_entity?)
          @drawing_interface.on_change_entity
        end
      }
      
      Plugin.add_event( proc )
      
    end


    # Group was erased or otherwise deleted.
    # onEraseEntity IS subsequently triggered for the enclosed entities.
    def onEraseEntity(entity)
      
      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
      
      return if not @enabled
      
      # http://www.thomthom.net/software/sketchup/observers/#note_EntityObserver
      # EntityObserver.onChangeEntity mistriggers right before EntityObserver.onEraseEntity, referencing a non-existant entity. 
      # EntityObserver.onEraseEntity reference a non-existant entity. 
      
      proc = Proc.new {
        @drawing_interface.on_erase_entity
      }
      
      Plugin.add_event( proc )
      
    end

  end


end
