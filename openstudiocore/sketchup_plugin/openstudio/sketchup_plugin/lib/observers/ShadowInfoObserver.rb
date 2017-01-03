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

module OpenStudio

  class ShadowInfoObserver < Sketchup::ShadowInfoObserver

    def initialize(drawing_interface)
      @drawing_interface = drawing_interface  # This is the Building or Site drawing interface
      @shadow_info = @drawing_interface.model_interface.skp_model.shadow_info
      @north_angle = @shadow_info.north_angle
      @shadow_time = @shadow_info.time
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
      @shadow_info = nil
      @north_angle = nil
      @shadow_time = nil
      @enabled = false
    end  
    
    def onShadowInfoChanged(shadow_info, arg2)
    
      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
      
      return if not @enabled
      
      # arg2 is a flag that returns 1 when shadows are displayed.

      proc = Proc.new {

        # check if model has a site, there is no Site object in the default template
        # to avoid setting the location by default
        if not @drawing_interface.model_interface.site
          site = Site.new
          site.create_entity
          site.create_model_object
          site.update_model_object
          site.add_watcher
          site.add_observers
        end

        # Turn on Daylight Saving Time.  Appears that SketchUp does not automatically turn it on.
        if (@shadow_info.time.dst?)
          @shadow_info['DaylightSavings'] = true
        else
          @shadow_info['DaylightSavings'] = false
        end

        # does not call paint
        @drawing_interface.on_change_entity
        
        if (@drawing_interface.class == Site)
       
          # Only repaint if shadow_time has changed
          if (@shadow_time != @shadow_info.time)
            @shadow_time = @shadow_info.time 
            if (@drawing_interface.model_interface.materials_interface.rendering_mode == RenderByDataValue)
              @drawing_interface.model_interface.request_paint
            end
          end
        
        
        elsif (@drawing_interface.class == Building)
        
          # Only repaint if north_angle has changed
          if (@north_angle != @shadow_info.north_angle)
            @north_angle = @shadow_info.north_angle
            @drawing_interface.model_interface.request_paint
          end
        
        end

      } # Proc
      
      Plugin.add_event( proc )

    end

  end
  
end
