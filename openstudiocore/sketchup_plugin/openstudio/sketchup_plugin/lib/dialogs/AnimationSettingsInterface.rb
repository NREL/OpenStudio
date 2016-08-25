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

require("openstudio/sketchup_plugin/lib/dialogs/DialogInterface")
require("openstudio/sketchup_plugin/lib/dialogs/AnimationSettingsDialog")


module OpenStudio

  class AnimationSettingsInterface < DialogInterface

    def initialize
      super
      
      @dialog = AnimationSettingsDialog.new(nil, self, @hash)
    end


    def populate_hash
      
      @hash['MATCH_TIME_PERIOD'] = Plugin.animation_manager.match_time_period.to_s
      @hash['REPEAT'] = Plugin.animation_manager.repeat

      @hash['START_MONTH'] = Plugin.animation_manager.start_marker.month
      @hash['START_DATE'] = Plugin.animation_manager.start_marker.day
      @hash['START_HOUR'] = Plugin.animation_manager.start_marker.hour
      
      @hash['END_MONTH'] = Plugin.animation_manager.end_marker.month
      @hash['END_DATE'] = Plugin.animation_manager.end_marker.day
      @hash['END_HOUR'] = Plugin.animation_manager.end_marker.hour

      @hash['MATCH_TIME_STEP'] = Plugin.animation_manager.match_time_step.to_s
      @hash['DAY_ONLY'] = Plugin.animation_manager.day_only
      
      @hash['TIME_STEP'] = (Plugin.animation_manager.time_step / 60.0).to_s
      @hash['MULTIPLIER'] = Plugin.animation_manager.multiplier.to_s
      @hash['DELAY'] = Plugin.animation_manager.delay.to_s

    end


    def report

      # check values

      Plugin.animation_manager.match_time_period = @hash['MATCH_TIME_PERIOD'] 
      Plugin.animation_manager.repeat = @hash['REPEAT']
 
      Plugin.animation_manager.start_marker = ::Time.utc(::Time.now.year, @hash['START_MONTH'].to_i, @hash['START_DATE'].to_i, @hash['START_HOUR'].to_i)
      Plugin.animation_manager.end_marker = ::Time.utc(::Time.now.year, @hash['END_MONTH'].to_i, @hash['END_DATE'].to_i, @hash['END_HOUR'].to_i)
 
      Plugin.animation_manager.match_time_step = @hash['MATCH_TIME_STEP']
      Plugin.animation_manager.day_only = @hash['DAY_ONLY']

      Plugin.animation_manager.time_step = @hash['TIME_STEP'].to_i * 60.0
      Plugin.animation_manager.multiplier = @hash['MULTIPLIER'].to_i
      Plugin.animation_manager.delay = @hash['DELAY'].to_f
 
      return(true)
    end

  end

end
