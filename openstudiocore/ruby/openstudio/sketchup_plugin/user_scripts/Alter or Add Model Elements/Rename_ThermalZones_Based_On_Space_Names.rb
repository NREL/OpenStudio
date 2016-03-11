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

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class RenameThermalZonesBasedOnSpaceNames < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Rename Thermal Zones Based On Space Names"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    return result
  end
    
  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)
  
    # get all thermal zones
    thermal_zones = model.getThermalZones
    
    runner.createProgressBar("Creating Zones for Untagged Spaces")
    num_total = thermal_zones.size
    num_complete = 0

    # loop through thermal zones
    thermal_zones.each do |thermal_zone| # this is going through all, not just selection
    
      # reset the array of spaces to be empty
      spaces_in_thermal_zone = []
      # reset length of array of spaces
      number_of_spaces = 0
      
      # get list of spaces in thermal zone
      spaces = thermal_zone.spaces
      spaces.each do |space|

        # make an array instead of the puts statement
        spaces_in_thermal_zone.push space.name.to_s
 
      end
      
      # store length of array
      number_of_spaces = spaces_in_thermal_zone.size
      
      # sort the array
      spaces_in_thermal_zone = spaces_in_thermal_zone.sort
      
      # setup a suffix if the thermal zone contains more than one space
      if number_of_spaces > 1
        multi = " - Plus"
      else
        multi = ""
      end
      
      # rename thermal zone based on first space with prefix added e.g. ThermalZone 203
      if number_of_spaces > 0
        new_name = "Thermal Zone: " + spaces_in_thermal_zone[0] + multi
        thermal_zone.setName(new_name)
      else
        puts "#{thermal_zone.name.to_s} did not have any spaces, and will not be renamed." 
      end
      
      # if someone tries to run this after it has already been run, and results in trying to use an already used theral zone name we could
      # add error checking, but I don't think we need it for now. Eventually I'd like to expand this with some logic to automatically
      # zone the model based on adjacent spaces with similar exterior exposure and the same space type on the same building story

      num_complete += 1
      runner.updateProgress((100*num_complete)/num_total)
    end

    runner.destroyProgressBar
    
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
RenameThermalZonesBasedOnSpaceNames.new.registerWithApplication
