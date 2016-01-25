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

  class AnimationManager

    attr_accessor :match_time_period, :start_month, :start_date, :start_hour, :end_month, :end_date, :end_hour, :repeat
    attr_accessor :start_marker, :end_marker
    attr_accessor :match_time_step, :day_only, :time_step, :multiplier, :delay

    def initialize

      @play = false
      @fast_forward = false
      @fast_reverse = false

      # store saved values on the model
      @start_marker = ::Time.utc(::Time.now.year, 1, 1, 0)
      @end_marker = ::Time.utc(::Time.now.year, 12, 31, 23)

      @match_time_period = false
      @match_time_step = false
      @repeat = false
      @day_only = false
      
      @time_step = 600.0
      @multiplier = 6
      @delay = 0.1
    end


    def play
      if (@play)
        stop_animation
      elsif (true) #(not $animation_lockout)
        @play = true

        time = Sketchup.active_model.shadow_info.time
        
        if (@end_marker > @start_marker)
          if (time < @start_marker or time > @end_marker)
            Sketchup.active_model.shadow_info.time = @start_marker
          end
        else
          if (time > @end_marker and time < @start_marker)
            Sketchup.active_model.shadow_info.time = @start_marker
          end
        end
        
        Plugin.menu_manager.rwd_anim_cmd.tooltip = "Fast Reverse"
        Plugin.menu_manager.play_anim_cmd.tooltip = "Stop"
        Plugin.menu_manager.fwd_anim_cmd.tooltip = "Fast Forward"

        @previous_time = ::Time.now - @delay - 1  # only used for measuring the delay
        Sketchup.active_model.active_view.animation = self
      end
    end


    def nextFrame(view)
      this_time = ::Time.now
      if ((this_time -  @previous_time) > @delay)
        @previous_time = this_time
        
        if (@fast_reverse)
          reverse_time
        else
          forward_time
        end
      end
      
      view.show_frame
    end
    

    def forward_time
      if (@fast_forward)
        time = Sketchup.active_model.shadow_info.time + @time_step * @multiplier
      else
        time = Sketchup.active_model.shadow_info.time + @time_step
      end
      
      if (@day_only)
        sunrise = Sketchup.active_model.shadow_info.sunrise
        sunset = Sketchup.active_model.shadow_info.sunset
        if (time > sunset)
          # Skip the night and restart at the next whole time step before sunrise
          night_duration = 86400 - (sunset - sunrise)
          time += (night_duration / @time_step).floor * @time_step
        elsif (time < sunrise)
          time = sunrise
        end
      end
      
      if (@end_marker > @start_marker)
        if (time > @end_marker)
          if (@repeat)
            time = @start_marker
          else
            stop_animation
            time = @end_marker
          end
        end
      else
        if (time > @end_marker and time < @start_marker)
          if (@repeat)
            time = @start_marker
          else
            stop_animation
            time = @end_marker
          end
        end
      end

      Sketchup.active_model.shadow_info.time = time
      update
    end


    def reverse_time
      if (@fast_reverse)
        time = Sketchup.active_model.shadow_info.time - @time_step * @multiplier
      else
        time = Sketchup.active_model.shadow_info.time - @time_step
      end
      
      if (@day_only)
        sunrise = Sketchup.active_model.shadow_info.sunrise
        sunset = Sketchup.active_model.shadow_info.sunset
        if (time < sunrise)
          # Skip the night and restart at the next whole time step after sunset
          night_duration = 86400 - (sunset - sunrise)
          time -= (night_duration / @time_step).floor * @time_step
        elsif time > sunset
          time = sunset
        end
      end
      
      if (@end_marker > @start_marker)
        if (time < @start_marker)
          if (@repeat)
            time = @end_marker
          else
            stop_animation
            time = @start_marker
          end
        end
      else
        if (time > @end_marker and time < @start_marker)
          if (@repeat)
            time = @end_marker
          else
            stop_animation
            time = @start_marker
          end
        end
      end

      Sketchup.active_model.shadow_info.time = time
      update
    end


    def forward
      if (@play)
        if (@fast_forward)
          @fast_forward = false
        else
          @fast_forward = true
          @fast_reverse = false
        end
      else
        @fast_forward = false
        forward_time
      end
    end


    def reverse
      if (@play)
        if (@fast_reverse)
          @fast_reverse = false
        else
          @fast_reverse = true
          @fast_forward = false
        end
      else
        @fast_reverse = false
        reverse_time
      end
    end


    def reverse_to_marker
      time = Sketchup.active_model.shadow_info.time
   
      if (@end_marker > @start_marker)
        if (time > @end_marker)
          Sketchup.active_model.shadow_info.time = @end_marker
        else
          Sketchup.active_model.shadow_info.time = @start_marker
        end
      else
        if (time < @end_marker)
          Sketchup.active_model.shadow_info.time = @start_marker
        elsif (time < @start_marker)
          Sketchup.active_model.shadow_info.time = @end_marker
        else
          Sketchup.active_model.shadow_info.time = @start_marker
        end
      end
      
      update
    end


    def forward_to_marker
      time = Sketchup.active_model.shadow_info.time
   
      if (@end_marker > @start_marker)
        if (time > @end_marker)
          Sketchup.active_model.shadow_info.time = @start_marker
        else
          Sketchup.active_model.shadow_info.time = @end_marker
        end
      else
        if (time < @end_marker)
          Sketchup.active_model.shadow_info.time = @end_marker
        elsif (time < @start_marker)
          Sketchup.active_model.shadow_info.time = @start_marker
        else
          Sketchup.active_model.shadow_info.time = @end_marker
        end
      end
      
      update
    end


    def update  # update_interface   update_dialogs
      Sketchup.set_status_text(Sketchup.active_model.shadow_info.time.strftime("%I:%M %p, %B %d"))
      
      # Also sets stuff in the control panel dialog, if open.
    end


    def stop_animation    

      # Can be called by SketchUp
      Sketchup.active_model.active_view.animation = nil

      @play = false
      @fast_forward = false
      @fast_reverse = false

      Sketchup.set_status_text(Sketchup.active_model.shadow_info.time.strftime("%I:%M %p, %B %d"))
      
      Plugin.menu_manager.rwd_anim_cmd.tooltip = "Reverse Frame"
      Plugin.menu_manager.play_anim_cmd.tooltip = "Play"
      Plugin.menu_manager.fwd_anim_cmd.tooltip = "Forward Frame"
    end


    def validate_play_animation
      return(@play ? MF_CHECKED : MF_UNCHECKED)
    end


    def validate_forward
      return(@fast_forward ? MF_CHECKED : MF_UNCHECKED)
    end


    def validate_reverse
      return(@fast_reverse ? MF_CHECKED : MF_UNCHECKED)
    end

  end

end
