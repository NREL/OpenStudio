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
class RemoveUnusedThermalZones < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Remove Unused ThermalZones"
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

    # this was added to give the user a chance to cancel the operation if they inadvertently hit it
    continue_operation = runner.yesNoPrompt("This will remove thermal zones without spaces or equipment. Click Yes to proceed, click No to cancel.")
    if not continue_operation
      puts "Operation canceled, your model was not altered."
      runner.registerAsNotApplicable("Operation canceled, your model was not altered.")
      return true
    end
    
    thermal_zones = model.getThermalZones
    
    thermal_zone_handles_to_remove = OpenStudio::UUIDVector.new
    thermal_zones.each do |thermal_zone|
      if thermal_zone.spaces.empty? && thermal_zone.equipment.empty? && thermal_zone.isRemovable
        thermal_zone_handles_to_remove << thermal_zone.handle
      end
    end
    
    if not thermal_zone_handles_to_remove.empty?
      model.removeObjects(thermal_zone_handles_to_remove)
      runner.registerFinalCondition("Removing #{thermal_zone_handles_to_remove.size} thermal zones.")
    else
      runner.registerFinalCondition("No unused thermal zones to remove.")
    end
    
    return true
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
RemoveUnusedThermalZones.new.registerWithApplication
