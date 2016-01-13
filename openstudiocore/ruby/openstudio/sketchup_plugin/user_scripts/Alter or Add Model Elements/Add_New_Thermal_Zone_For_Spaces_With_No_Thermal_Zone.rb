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
class AssignUniqueZonesToUntaggedSpaces < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Add New Thermal Zone For Spaces With No Thermal Zone"
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
    super(model, runner, user_arguments) # initializes runner for new script
  
    # get all spaces
    spaces = model.getSpaces
    
    runner.createProgressBar("Creating Zones for Untagged Spaces")
    num_total = spaces.size
    num_complete = 0

    # loop through spaces
    spaces.each do |space| # this is going through all, not just selection
      if space.thermalZone.empty?
        newthermalzone = OpenStudio::Model::ThermalZone.new(model)
        space.setThermalZone(newthermalzone)
        runner.registerInfo("Created " + newthermalzone.briefDescription + " and assigned " + space.briefDescription + " to it.")
      end

      num_complete += 1
      runner.updateProgress((100*num_complete)/num_total)
    end

    runner.destroyProgressBar
    
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
AssignUniqueZonesToUntaggedSpaces.new.registerWithApplication
