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

require("openstudio/sketchup_plugin/lib/dialogs/Dialogs")
require("openstudio/sketchup_plugin/lib/dialogs/DialogContainers")


module OpenStudio

  class RenderingSettingsDialog < PropertiesDialog

    def initialize(container, interface, hash)
      super
      w = Plugin.platform_select(489, 560)
      h = Plugin.platform_select(560, 640)
      @container = WindowContainer.new("Rendering Settings", w, h, 150, 150)
      @container.set_file(Plugin.dir + "/lib/dialogs/html/RenderingSettings.html")

      @run_period = ""
      @surface_variables = []
      @thermal_zone_variables = []
      
      @outside_units = ""
      @outside_min = ""
      @outside_max = ""
      @inside_units = ""
      @inside_min = ""
      @inside_max = ""

      add_callbacks
    end

    def add_callbacks
      super
      @container.web_dialog.add_action_callback("on_browse") { on_browse }
      @container.web_dialog.add_action_callback("on_change_run_period") { on_change_run_period }
      @container.web_dialog.add_action_callback("on_click_variable_type") { on_click_variable_type }
      @container.web_dialog.add_action_callback("on_click_normalize") { on_click_normalize }
      @container.web_dialog.add_action_callback("on_change_outside_variable") { on_change_outside_variable }
      @container.web_dialog.add_action_callback("on_change_inside_variable") { on_change_inside_variable }
      @container.web_dialog.add_action_callback("on_click_match_range") { on_click_match_range }
      @container.web_dialog.add_action_callback("on_change_appearance") { on_change_appearance }
      @container.web_dialog.add_action_callback("on_load_results") { on_load_results }
      @container.web_dialog.add_action_callback("on_close") { on_close }
      @container.web_dialog.add_action_callback("on_resultsviewer") { on_resultsviewer }
    end

    def on_load
      super
      on_change_output_file 
      on_click_variable_type
      on_click_match_range
      on_change_appearance
      update
      report

      if (Plugin.platform == Platform_Mac)
        @container.execute_function("invalidate()")  # Force the WebDialog to redraw
      end
    end
    
    def update
      super
      
      sql_file = Plugin.model_manager.model_interface.openstudio_model.sqlFile
      if @hash['OUTPUT_FILE_PATH'] and not @hash['OUTPUT_FILE_PATH'].empty? and sql_file.empty?
        enable_element('LOAD')  
      else
        disable_element('LOAD')  
      end

    end
    
    def on_browse
      if (@hash['OUTPUT_FILE_PATH'].empty?)
        dir = Plugin.model_manager.model_interface.openstudio_dir
        file_name = "*.sql"      
      else
        dir = File.dirname(@hash['OUTPUT_FILE_PATH'])
        file_name = File.basename(@hash['OUTPUT_FILE_PATH'])
      end

      if (output_file_path = UI.open_panel("Locate Output File", dir, file_name))

        # could replace with a single method called here and in report
        if (File.exists?(output_file_path))
          output_file_path = output_file_path.split("\\").join("/")
          @hash['OUTPUT_FILE_PATH'] = output_file_path
          Plugin.model_manager.model_interface.results_interface.attach_output_file(output_file_path) 
          @hash['RUN_PERIOD'] = ""
          @run_period = @hash['RUN_PERIOD']
          Plugin.model_manager.model_interface.results_interface.run_period = @hash['RUN_PERIOD']
          on_change_output_file
          update
          report
        end
      end
    end

    def on_change_output_file
      sql_file = Plugin.model_manager.model_interface.openstudio_model.sqlFile
      if sql_file.empty?
        disable_element('CLOSE')
        
        set_select_options("RUN_PERIOD", [])
        @hash['RUN_PERIOD'] = ""
      else
        enable_element('CLOSE')  
        
        run_period_names = []
        sql_file.get.availableEnvPeriods.each { |run_period|
          run_period_names << run_period
        }

        set_select_options("RUN_PERIOD", run_period_names)
        
        if run_period_names.empty?
          @hash['RUN_PERIOD'] = ""
          @run_period = @hash['RUN_PERIOD']
          Plugin.model_manager.model_interface.results_interface.run_period = @hash['RUN_PERIOD']
        else
          @hash['RUN_PERIOD'] = run_period_names[0]
          @run_period = @hash['RUN_PERIOD']
          Plugin.model_manager.model_interface.results_interface.run_period = @hash['RUN_PERIOD']
        end
        
        on_change_run_period
        on_click_variable_type
      end
    end

    def on_change_run_period
      sql_file = Plugin.model_manager.model_interface.openstudio_model.sqlFile
      if (not sql_file.empty?)
        @run_period = @hash['RUN_PERIOD']
        Plugin.model_manager.model_interface.results_interface.run_period = @hash['RUN_PERIOD']
        on_change_outside_variable
        on_change_inside_variable
      end
    end

    def on_click_variable_type
    
      if (@hash['VARIABLE_TYPE'] == "SURFACE")
        set_element_value("NORMALIZE_VARIABLE_BY", "net surface area")
        variable_names = get_surface_variables.to_a
      else
        set_element_value("NORMALIZE_VARIABLE_BY", "thermal zone floor area")
        variable_names = get_thermal_zone_variables.to_a
      end

      if (not variable_names.nil?)
        variable_names.sort! { |a, b| a <=> b }

        set_select_options("OUTSIDE_VARIABLE", variable_names)
        set_select_options("INSIDE_VARIABLE", variable_names)
        
        if (variable_names.index(@hash['OUTSIDE_VARIABLE']))
          set_element_value("OUTSIDE_VARIABLE", @hash['OUTSIDE_VARIABLE'])
        else
          @hash['OUTSIDE_VARIABLE'] = variable_names[0]
          set_element_value("OUTSIDE_VARIABLE", variable_names[0])
        end
        on_change_outside_variable

        if (variable_names.index(@hash['INSIDE_VARIABLE']))
          set_element_value("INSIDE_VARIABLE", @hash['INSIDE_VARIABLE'])
        else
          @hash['INSIDE_VARIABLE'] = variable_names[0]
          set_element_value("INSIDE_VARIABLE", variable_names[0])
        end
        on_change_inside_variable

      end 
      
    end
    
    def on_click_normalize

      if @hash['NORMALIZE']
        # disable match range
        @hash['MATCH_RANGE'] = false
        set_element_value("MATCH_RANGE", @hash['MATCH_RANGE'])
        disable_element('MATCH_RANGE')
        enable_element('RANGE_MINIMUM')
        enable_element('RANGE_MAXIMUM')
        
        set_element_value("OUTSIDE_UNITS", @outside_units + "/m2")
        set_element_value("INSIDE_UNITS", @inside_units + "/m2")
        
        set_range_units
      else
        # enable match range
        @hash['MATCH_RANGE'] = true
        set_element_value('MATCH_RANGE', @hash['MATCH_RANGE'])
        enable_element('MATCH_RANGE')
       
        set_element_value("OUTSIDE_UNITS", @outside_units)
        set_element_value("INSIDE_UNITS", @inside_units)
        
        on_click_match_range
      end
    end

    def on_change_outside_variable
      range = get_variable_range(@hash['OUTSIDE_VARIABLE'])
      if (range)
        @outside_units = range[0].to_s
        @outside_min = "%0.2f" % range[1].to_s
        @outside_max = "%0.2f" % range[2].to_s
      else
        @outside_units = ""
        @outside_min = ""
        @outside_max = ""
      end

      set_element_value("OUTSIDE_MINIMUM", @outside_min)
      set_element_value("OUTSIDE_MAXIMUM", @outside_max)  
      
      if (@hash['VARIABLE_TYPE'] != "SURFACE")
        @hash['INSIDE_VARIABLE'] = @hash['OUTSIDE_VARIABLE']
        on_change_inside_variable
      else
        on_click_match_range
        on_click_normalize
      end
      
    end

    def on_change_inside_variable
      range = get_variable_range(@hash['INSIDE_VARIABLE'])
      if (range)
        @inside_units = range[0].to_s
        @inside_min = "%0.2f" % range[1].to_s
        @inside_max = "%0.2f" % range[2].to_s
      else
        @inside_units = ""
        @inside_min = ""
        @inside_max = ""
      end

      set_element_value("INSIDE_MINIMUM", @inside_min)
      set_element_value("INSIDE_MAXIMUM", @inside_max)
      
      on_click_match_range
      on_click_normalize
    end

    def on_change_appearance
      if (@hash['APPEARANCE'] == "COLOR")
        set_element_source("SCALE", "colorscale.bmp")
      else
        set_element_source("SCALE", "grayscale.bmp")
      end
    end

    def on_click_match_range
      set_range_min_max
      set_range_units
      if (@hash['MATCH_RANGE'])
        disable_element("RANGE_MINIMUM")
        disable_element("RANGE_MAXIMUM")
        disable_element("RANGE_UNITS")
      else
        enable_element("RANGE_MINIMUM")
        enable_element("RANGE_MAXIMUM")
        disable_element("RANGE_UNITS")
      end
    end

    def set_range_min_max
      if (@outside_min.empty? and @inside_min.empty?)
        @hash['RANGE_MINIMUM'] = ""
      elsif (@outside_min.empty?)
        @hash['RANGE_MINIMUM'] = @inside_min
      elsif (@inside_min.empty?)
        @hash['RANGE_MINIMUM'] = @outside_min
      else
        @hash['RANGE_MINIMUM'] = ([@outside_min.to_f, @inside_min.to_f].min).to_s
      end

      set_element_value("RANGE_MINIMUM", @hash['RANGE_MINIMUM'])

      if (@outside_max.empty? and @inside_max.empty?)
        @hash['RANGE_MAXIMUM'] = ""
      elsif (@outside_max.empty?)
        @hash['RANGE_MAXIMUM'] = @inside_max
      elsif (@inside_max.empty?)
        @hash['RANGE_MAXIMUM'] = @outside_max
      else
        @hash['RANGE_MAXIMUM'] = ([@outside_max.to_f, @inside_max.to_f].max).to_s
      end
      
      set_element_value("RANGE_MAXIMUM", @hash['RANGE_MAXIMUM'])
    end
    
    def set_range_units
      if (@outside_units.empty? and @inside_units.empty?)
        @hash['RANGE_UNITS'] = ""
      elsif (@outside_units.empty?)
        @hash['RANGE_UNITS'] = @inside_units
      elsif (@inside_units.empty?)
        @hash['RANGE_UNITS'] = @outside_units
      else
        if (@outside_units.upcase == @inside_units.upcase)
          @hash['RANGE_UNITS'] = @outside_units
        else
          @hash['RANGE_UNITS'] = "NA"
        end
      end    
      
      set_element_value("RANGE_UNITS", @hash['RANGE_UNITS'])
    end

    def get_surface_variables
      surface_variables = []
      
      sql_file = Plugin.model_manager.model_interface.openstudio_model.sqlFile
      if (not sql_file.empty?)
        surface_names = Plugin.model_manager.model_interface.all_surfaces.collect { |surface| surface.name.upcase }.to_a

        # todo: support more than hourly run periods
        sql_file.get.availableEnvPeriods.each do |envPeriod|
          sql_file.get.availableVariableNames(envPeriod, "Hourly").each do |varName|
            sql_file.get.availableKeyValues(envPeriod, "Hourly", varName).each do |keyName|
              if surface_names.include?(keyName)
                surface_variables << varName
                break
              end
            end
          end
        end
      end
        
      return(surface_variables.uniq)
    end

    def get_thermal_zone_variables
      thermal_zone_variables = []

      sql_file = Plugin.model_manager.model_interface.openstudio_model.sqlFile
      if (not sql_file.empty?)
        thermal_zone_names = Plugin.model_manager.model_interface.thermal_zones.collect { |thermal_zone| thermal_zone.name.upcase }.to_a

        # todo: support more than hourly run periods
        sql_file.get.availableEnvPeriods.each do |envPeriod|
          sql_file.get.availableVariableNames(envPeriod, "Hourly").each do |varName|
            sql_file.get.availableKeyValues(envPeriod, "Hourly", varName).each do |keyName|
              if thermal_zone_names.include?(keyName)
                thermal_zone_variables << varName
                break
              end
            end
          end
        end
      end

      return(thermal_zone_variables.uniq)
    end

    def get_variable_range(variable_name)
    
      result = nil
      
      if not variable_name
        return nil
      end
      
      # Returns the range of values across all variables with the same name.
      sql_file = Plugin.model_manager.model_interface.openstudio_model.sqlFile
      if (not sql_file.empty?)
        units = nil
        minimum = nil
        maximum = nil
        
        # scan across all timeseries
        sql_file.get.timeSeries(@run_period, "Hourly", variable_name).each do |timeseries|
         
          values = timeseries.values
          this_minimum = OpenStudio::minimum(values)
          this_maximum = OpenStudio::maximum(values)
          
          units = timeseries.units if not units
          minimum = this_minimum if ((not minimum) or (this_minimum < minimum))
          maximum = this_maximum if ((not maximum) or (this_maximum > maximum))
        end
        
        if units and minimum and maximum
          result = [units, minimum, maximum]
        end
      end
      
      return(result)
    end
    
    def on_load_results
      if output_file_path = @hash['OUTPUT_FILE_PATH']
        Plugin.model_manager.model_interface.results_interface.attach_output_file(output_file_path) 
        @hash['RUN_PERIOD'] = ""
        @run_period = @hash['RUN_PERIOD']
        Plugin.model_manager.model_interface.results_interface.run_period = @hash['RUN_PERIOD']
        on_change_output_file    
        update
        report
      end
    end
    
    def on_close
      output_file_path = Plugin.model_manager.model_interface.results_interface.output_file_path
      Plugin.model_manager.model_interface.openstudio_model.sqlFile.get.close
      Plugin.model_manager.model_interface.openstudio_model.resetSqlFile
      Plugin.model_manager.model_interface.results_interface = ResultsInterface.new(Plugin.model_manager.model_interface)
      Plugin.model_manager.model_interface.results_interface.output_file_path = output_file_path
      @hash['OUTPUT_FILE_PATH'] = output_file_path
      on_load
    end
    
    def on_resultsviewer
      output_file_path = Plugin.model_manager.model_interface.results_interface.output_file_path
      if (not output_file_path.empty?)    
        if Plugin.platform == Platform_Windows
          system("start \"\" \"#{$OpenStudio_BinaryDir}ResultsViewer\" \"#{output_file_path}\"")
        elsif Plugin.platform == Platform_Mac
          system("open -a \"#{$OpenStudio_BinaryDir}ResultsViewer.app\" --args \"#{output_file_path}\"")
        end
      end
    end
    
   
  end

  
end
