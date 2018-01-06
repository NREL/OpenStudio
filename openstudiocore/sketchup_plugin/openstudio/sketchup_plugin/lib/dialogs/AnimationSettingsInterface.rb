########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
