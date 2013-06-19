######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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
require("openstudio/sketchup_plugin/lib/dialogs/RenderingSettingsDialog")


module OpenStudio

  class RenderingSettingsInterface < DialogInterface

    def initialize
      super
      @dialog = RenderingSettingsDialog.new(nil, self, @hash)
    end


    def populate_hash
      @hash['OUTPUT_FILE_PATH'] = Plugin.model_manager.model_interface.results_interface.output_file_path
      @hash['RUN_PERIOD'] = Plugin.model_manager.model_interface.results_interface.run_period
      @hash['VARIABLE_TYPE'] = Plugin.model_manager.model_interface.results_interface.variable_type
      @hash['NORMALIZE'] = Plugin.model_manager.model_interface.results_interface.normalize
      @hash['OUTSIDE_VARIABLE'] = Plugin.model_manager.model_interface.results_interface.outside_variable_name
      @hash['INSIDE_VARIABLE'] = Plugin.model_manager.model_interface.results_interface.inside_variable_name
      @hash['APPEARANCE'] = Plugin.model_manager.model_interface.results_interface.rendering_appearance
      @hash['MATCH_RANGE'] = Plugin.model_manager.model_interface.results_interface.match_range
      @hash['RANGE_MINIMUM'] = Plugin.model_manager.model_interface.results_interface.range_minimum
      @hash['RANGE_MAXIMUM'] = Plugin.model_manager.model_interface.results_interface.range_maximum
      @hash['RANGE_UNITS'] = Plugin.model_manager.model_interface.results_interface.range_units
    end


    def report

      # Check the output file path
      output_file_path = @hash['OUTPUT_FILE_PATH']
      if (not output_file_path.empty? and not File.exists?(output_file_path))
        UI.messagebox("Cannot locate the output file.  Correct the output file path and try again.")
        return(false)
      end

      Plugin.model_manager.model_interface.results_interface.output_file_path = @hash['OUTPUT_FILE_PATH']
      Plugin.model_manager.model_interface.results_interface.run_period = @hash['RUN_PERIOD']
      Plugin.model_manager.model_interface.results_interface.variable_type = @hash['VARIABLE_TYPE']
      Plugin.model_manager.model_interface.results_interface.normalize = @hash['NORMALIZE']
      Plugin.model_manager.model_interface.results_interface.outside_variable_name = @hash['OUTSIDE_VARIABLE']
      Plugin.model_manager.model_interface.results_interface.inside_variable_name = @hash['INSIDE_VARIABLE']
      Plugin.model_manager.model_interface.results_interface.rendering_appearance = @hash['APPEARANCE']
      Plugin.model_manager.model_interface.results_interface.match_range = @hash['MATCH_RANGE']
      Plugin.model_manager.model_interface.results_interface.range_minimum = @hash['RANGE_MINIMUM']
      Plugin.model_manager.model_interface.results_interface.range_maximum = @hash['RANGE_MAXIMUM']
      Plugin.model_manager.model_interface.results_interface.range_units = @hash['RANGE_UNITS']
      
      Plugin.model_manager.model_interface.results_interface.update
      Plugin.dialog_manager.update(ColorScaleInterface)
      Plugin.model_manager.model_interface.request_paint
 
      return(true)
    end

  end

end
