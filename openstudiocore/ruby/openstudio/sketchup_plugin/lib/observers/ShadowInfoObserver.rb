######################################################################
#  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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
