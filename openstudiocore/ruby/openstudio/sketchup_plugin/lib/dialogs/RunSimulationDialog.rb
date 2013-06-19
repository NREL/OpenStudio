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
require("openstudio/sketchup_plugin/lib/dialogs/ProgressDialog")

module OpenStudio

  class RunSimulationDialog < PropertiesDialog

    def initialize(container, interface, hash)
      super
      w = Plugin.platform_select(580, 610)
      h = Plugin.platform_select(700, 720)
      @container = WindowContainer.new("Run Simulation", w, h, 100, 50)
      @container.set_file(Plugin.dir + "/lib/dialogs/html/RunSimulation.html")

      add_callbacks
    end


    def add_callbacks
      super
      @container.web_dialog.add_action_callback("on_ddy_browse") { on_ddy_browse }
      @container.web_dialog.add_action_callback("on_epw_browse") { on_epw_browse }
      @container.web_dialog.add_action_callback("on_rvi_browse") { on_rvi_browse }
      @container.web_dialog.add_action_callback("on_click_annual_simulation") { |d, p| on_click_annual_simulation(d, p) }
      @container.web_dialog.add_action_callback("on_change_start_month") { |d, p| on_change_start_month(d, p) }
      @container.web_dialog.add_action_callback("on_change_end_month") { |d, p| on_change_end_month(d, p) }
      @container.web_dialog.add_action_callback("on_run") { run_simulation }
      @container.web_dialog.add_action_callback("on_status") { show_status }
    end
    
    # call equivalent to Dialog's on_key, allow returns in text area
    def on_key(d,p)
      if (p == "27")  # Escape key
        on_cancel
      end   
    end

    def on_load
      super

      # Manually trigger onChange for start and end months to set the day popup options
      @container.execute_function("setDateOptions()")

      # Manually set the date values
      @container.execute_function("setElementValue('START_DATE', '" + @hash['START_DATE'].to_s + "')")
      @container.execute_function("setElementValue('END_DATE', '" + @hash['END_DATE'].to_s + "')")

      # enable/disable elements as needed
      @container.execute_function("onClickRunDesignDays()")
      @container.execute_function("onClickRunWeatherFile()")
      @container.execute_function("onClickRunReadVars()")
      @container.execute_function("onClickAnnualSimulation()")
      
      show_weather_file_info
      show_ddy_info
    end


    def on_epw_browse
    
      model_interface = Plugin.model_manager.model_interface
      openstudio_model = model_interface.openstudio_model

      dir = ""
      file_name = "*.epw"
      previous_epw_name = ""

      last_dir = Plugin.read_pref("Last EPW Dir")
      if last_dir and not last_dir.empty?
        dir = last_dir
      else
        dir = model_interface.openstudio_dir  
      end

      if (epw_path = UI.open_panel("Locate Weather File", dir, file_name))
        if (File.exists?(epw_path))
          begin
      
            weather_file = openstudio_model.getOptionalWeatherFile
            if (not weather_file.empty?)
              weather_file_path = weather_file.get.path.get.to_s
              previous_epw_name = File.basename(weather_file_path)
            end
      
            epw_path = OpenStudio::Path.new(epw_path)
            new_path = model_interface.model_temp_dir / OpenStudio::Path.new("resources/files") / OpenStudio::Path.new(epw_path.filename())
            FileUtils.cp(epw_path.to_s, new_path.to_s)

            epw_file = OpenStudio::EpwFile.new(OpenStudio::Path.new(new_path))
            
            weather_name = "#{epw_file.city}, #{epw_file.stateProvinceRegion}, #{epw_file.country}"
            weather_lat = epw_file.latitude
            weather_lon = epw_file.longitude
            weather_time = epw_file.timeZone
            weather_elev = epw_file.elevation
            
            Plugin.command_manager.check_site("weather file", weather_name, weather_lat, weather_lon, weather_time, weather_elev)
            
            OpenStudio::Model::WeatherFile::setWeatherFile(openstudio_model, epw_file)
            weather_file = openstudio_model.getWeatherFile
            weather_file.makeUrlRelative(model_interface.model_temp_dir / OpenStudio::Path.new("resources"))
            
            weather_file_path = weather_file.path.get.to_s.gsub("\\", "/")
            @hash['EPW_PATH'] = weather_file_path

            # Save the dir so we can start here next time
            Plugin.write_pref("Last EPW Dir", File.dirname(epw_path.to_s))  
            
            # remove the old weather file
            if not previous_epw_name.empty? and OpenStudio::Path.new(epw_path.filename).to_s != previous_epw_name
              previous_epw = model_interface.model_temp_dir / OpenStudio::Path.new("resources/files") / OpenStudio::Path.new(previous_epw_name)
              if File.exists?(previous_epw.to_s)
                FileUtils.rm(previous_epw.to_s)
              end
            end
            
          rescue
            UI.messagebox("#{epw_path} does not seem to be a valid epw")
          end
        end
      end
      
      show_weather_file_info
      update
    end
    
    
    def on_rvi_browse
      dir = Plugin.model_manager.model_interface.openstudio_dir
      file_name = "*.rvi"      

      if (rvi_path = UI.open_panel("Locate RVI File", dir, file_name))
        if (File.exists?(rvi_path))
          @hash["RVI_FILE"] = rvi_path
        end
      end
      
      update
    end
    
    def on_ddy_browse
    
      openstudio_model = Plugin.model_manager.model_interface.openstudio_model

      dir = ""
      file_name = "*.ddy"
      
      weather_file = openstudio_model.getOptionalWeatherFile
      if (not weather_file.empty?)
        weather_file_path = weather_file.get.path.get.to_s
        file_name = File.basename(weather_file_path).gsub(/\.epw$/, ".ddy")
      end
      
      last_dir = Plugin.read_pref("Last DDY Dir")
      last_epw_dir = Plugin.read_pref("Last EPW Dir")
      if last_dir and not last_dir.empty?
        dir = last_dir
      elsif last_epw_dir and not last_epw_dir.empty?
        dir = last_epw_dir
      else
        dir = Plugin.model_manager.model_interface.openstudio_dir   
      end

      if (ddy_path = UI.open_panel("Locate DDY File", dir, file_name))
        if (File.exists?(ddy_path))
        
          ddy_file = nil
          if $OPENSTUDIO_SKETCHUPPLUGIN_PROGRESS_DIALOGS
            progress_dialog = ProgressDialog.new("Loading Design Days")
            ddy_file = OpenStudio::IdfFile::load(OpenStudio::Path.new(ddy_path), progress_dialog)
            progress_dialog.destroy
          else
            ddy_file = OpenStudio::IdfFile::load(OpenStudio::Path.new(ddy_path))
          end
          
          if not ddy_file.empty?
          
            # create a new workspace and add idf design days to it
            #ddy_workspace = OpenStudio::Workspace.new("None".to_StrictnessLevel, "EnergyPlus".to_IddFileType)
            ddy_workspace = OpenStudio::Workspace.new("Draft".to_StrictnessLevel, "EnergyPlus".to_IddFileType)
            ddy_file.get.objects.each do |idfObject|
              iddObjectType = idfObject.iddObject.type
              if iddObjectType == "SizingPeriod_DesignDay".to_IddObjectType or
                 iddObjectType == "SizingPeriod_WeatherFileDays".to_IddObjectType or
                 iddObjectType == "SizingPeriod_WeatherFileConditionType".to_IddObjectType
                 
                ddy_workspace.addObject(idfObject)
              end
            end
            
            # convert to a model
            ddy_model, errors, warnings = Plugin.model_manager.model_from_workspace(ddy_workspace)
            
            # if empty just return
            if ddy_model.numObjects == 0
              UI.messagebox("No design days could be imported from #{ddy_path}, check that this file contains objects of the correct EnergyPlus version.")
              return
            end
            
            # Save the dir so we can start here next time
            Plugin.write_pref("Last DDY Dir", File.dirname(ddy_path)) 
            
            # delete all current design days and add new ones
            openstudio_model.getSizingPeriods.each {|sp| sp.remove}
            openstudio_model.insertObjects(ddy_model.objects(true))

            # check current site vs ddy file location
            ddy_location = nil
            ddy_locations = ddy_file.get.getObjectsByType("Site_Location".to_IddObjectType)
            if not ddy_locations.empty?
              ddy_name = ddy_locations[0].getString(0, true).get
              ddy_lat = ddy_locations[0].getDouble(1, true).get
              ddy_lon = ddy_locations[0].getDouble(2, true).get
              ddy_time = ddy_locations[0].getDouble(3, true).get
              ddy_elev = ddy_locations[0].getDouble(4, true).get
              
              Plugin.command_manager.check_site("design days file", ddy_name, ddy_lat, ddy_lon, ddy_time, ddy_elev)
            end
            
          end
        end
      end
      
      show_weather_file_info
      show_ddy_info
      update
    end

    def show_weather_file_info()
      openstudio_model = Plugin.model_manager.model_interface.openstudio_model
      
      weather_file = openstudio_model.getOptionalWeatherFile
      if not weather_file.empty?
        weather_file = weather_file.get
        
        weather_file_path = weather_file.path.get.to_s.gsub("\\", "/")
        set_element_value("EPW_PATH", weather_file_path)
        
        location = weather_file.city + ', ' + weather_file.stateProvinceRegion + ', ' + weather_file.country
        set_element_value("WEATHER_LOCATION", location)

        set_element_value("WEATHER_LATITUDE", weather_file.latitude.to_s)
        set_element_value("WEATHER_LONGITUDE", weather_file.longitude.to_s)
        set_element_value("WEATHER_TIME_ZONE", weather_file.timeZone.to_s)
        set_element_value("WEATHER_ELEVATION", weather_file.elevation.to_s + " m")
      end
      
      site = openstudio_model.getOptionalSite
      if not site.empty?
        site = site.get
        
        set_element_value("SITE_LOCATION", site.name)
        set_element_value("SITE_LATITUDE", site.latitude.to_s)
        set_element_value("SITE_LONGITUDE", site.longitude.to_s)
        set_element_value("SITE_TIME_ZONE", site.timeZone.to_s)
        set_element_value("SITE_ELEVATION", site.elevation.to_s + " m")
      end
    end
    
    def show_ddy_info()
      openstudio_model = Plugin.model_manager.model_interface.openstudio_model
      num_design_days = openstudio_model.getSizingPeriods.size
      set_element_value("NUM_DESIGN_DAYS", num_design_days.to_s)
    end    


    def on_click_annual_simulation(d, p)
      on_change_element(d, p)

      @hash['START_MONTH'] = "1"
      @hash['START_DATE'] = "1"
      @hash['END_MONTH'] = "12"
      @hash['END_DATE'] = "31"

      @container.execute_function("setElementValue('START_MONTH', '1')")
      @container.execute_function("setStartMonthOptions()")
      @container.execute_function("setElementValue('START_DATE', '1')")
      
      @container.execute_function("setElementValue('END_MONTH', '12')")
      @container.execute_function("setEndMonthOptions()")
      @container.execute_function("setElementValue('END_DATE', '31')")
    end


    def on_change_start_month(d, p)
      on_change_element(d, p)
      @container.execute_function("setStartMonthOptions()")
      @container.execute_function("setElementValue('START_DATE', '" + @hash['START_DATE'].to_s + "')")
    end


    def on_change_end_month(d, p)
      on_change_element(d, p)
      @container.execute_function("setEndMonthOptions()")
      @container.execute_function("setElementValue('END_DATE', '" + @hash['END_DATE'].to_s + "')")
    end


    def run_simulation
      @interface.run_simulation
    end
    
    def show_status
      Plugin.simulation_manager.status_widget.show
      Plugin.simulation_manager.status_widget.activateWindow
    end

  end
  
end
