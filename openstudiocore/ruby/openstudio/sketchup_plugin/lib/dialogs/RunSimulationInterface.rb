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
require("openstudio/sketchup_plugin/lib/dialogs/RunSimulationDialog")

module OpenStudio

  class RunSimulationInterface < DialogInterface

    def initialize
      super
      @dialog = RunSimulationDialog.new(nil, self, @hash)
    end

    def populate_hash
      #puts "RunSimulationInterface.populate_hash"
    
      openstudio_model = Plugin.model_manager.model_interface.openstudio_model
      
      # DLM: this warning can be removed
      @hash['MULTIPLE_RUNPERIOD_WARNING'] = ""
     
      simulation = openstudio_model.getSimulationControl      

      @hash['ZONE_SIZING'] = simulation.doZoneSizingCalculation

      @hash['SYSTEM_SIZING'] = simulation.doSystemSizingCalculation

      @hash['PLANT_SIZING'] = simulation.doPlantSizingCalculation

      @hash['RUN_DESIGN_DAYS'] = simulation.runSimulationforSizingPeriods

      @hash['RUN_WEATHER_FILE'] = simulation.runSimulationforWeatherFileRunPeriods

      object = openstudio_model.getOptionalRunPeriod
      if (object.empty?)
      
        @hash['MULTIPLE_RUNPERIOD_WARNING'] = ""
        @hash['ANNUAL_SIMULATION'] = true
        @hash['START_MONTH'] = "1"
        @hash['START_DATE'] = "1"
        @hash['END_MONTH'] = "12"
        @hash['END_DATE'] = "31"
          
      else
        run_period = object.get
        
        @hash['MULTIPLE_RUNPERIOD_WARNING'] = ""
        @hash['START_MONTH'] = run_period.getBeginMonth.to_s
        @hash['START_DATE'] = run_period.getBeginDayOfMonth.to_s
        @hash['END_MONTH'] = run_period.getEndMonth.to_s
        @hash['END_DATE'] = run_period.getEndDayOfMonth.to_s     

        if (@hash['START_MONTH'] == "1" and @hash['START_DATE'] == "1" and @hash['END_MONTH'] == "12" and @hash['END_DATE'] == "31")
          @hash['ANNUAL_SIMULATION'] = true
        else
          @hash['ANNUAL_SIMULATION'] = false
        end
        
      end
      
      year_description = openstudio_model.getOptionalYearDescription
      if year_description.empty?
        @hash['START_DAY'] = "USEWEATHERFILE"
      else
        @hash['START_DAY'] = year_description.get.dayofWeekforStartDay.upcase
      end
      
      weather_file = openstudio_model.getOptionalWeatherFile
      if not weather_file.empty?
        weather_file_path = weather_file.get.path.get.to_s.gsub("\\", "/")
        @hash['EPW_PATH'] = weather_file_path
      end 
      
      @hash['RUN_READVARS'] = Plugin.model_manager.model_interface.get_attribute("RUN_READVARS")
      @hash['RVI_FILE'] = Plugin.model_manager.model_interface.get_attribute("RVI_FILE")
    end


    def show
      if (Plugin.simulation_manager.busy?)
        @dialog.show_status
        Plugin.dialog_manager.remove(self)
      else
        super
      end
    end


    def report
    
      openstudio_model = Plugin.model_manager.model_interface.openstudio_model
      
      simulation = openstudio_model.getSimulationControl 

      simulation.setDoZoneSizingCalculation(@hash['ZONE_SIZING'])
      
      simulation.setDoSystemSizingCalculation(@hash['SYSTEM_SIZING'])

      simulation.setDoPlantSizingCalculation(@hash['PLANT_SIZING'])

      simulation.setRunSimulationforSizingPeriods(@hash['RUN_DESIGN_DAYS'])

      simulation.setRunSimulationforWeatherFileRunPeriods(@hash['RUN_WEATHER_FILE'])
      
      run_period = openstudio_model.getRunPeriod
      run_period.setBeginMonth(@hash['START_MONTH'].to_i)
      run_period.setBeginDayOfMonth(@hash['START_DATE'].to_i)
      run_period.setEndMonth(@hash['END_MONTH'].to_i)
      run_period.setEndDayOfMonth(@hash['END_DATE'].to_i)
      
      year_description = openstudio_model.getYearDescription
      year_description.setDayofWeekforStartDay(@hash['START_DAY'])

      Plugin.model_manager.model_interface.set_attribute("RUN_READVARS", @hash['RUN_READVARS'])
      Plugin.model_manager.model_interface.set_attribute("RVI_FILE", @hash['RVI_FILE'])
      
      return(true)
    end


    def run_simulation
      if (report)
        if (Plugin.simulation_manager.run_simulation)
          close
        end
      end
    end

  end

end
