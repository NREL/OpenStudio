######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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
class RemovePhotovoltaics < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Remove Photovoltaics"
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
    super(model,runner,user_arguments) # initializes runner for new script

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
   
    num_pv_removed = 0
    num_elcd_removed = 0
    num_inverter_removed = 0
    
    model.getPlanarSurfaces.each do |s|

      # Only remove for selected surfaces
      next if not runner.inSelection(s)
    
      s.generatorPhotovoltaics.each do |pv|
      
        if pv.electricLoadCenterDistribution.empty?
          pv.remove
          num_pv_removed += 1
        else
          elcd = pv.electricLoadCenterDistribution.get
          pv.remove
          num_pv_removed += 1
          # If the ElectricLoadCenterDistribution has no generators left, need to delete it
          if elcd.generators.empty?
            # If it's got an inverter, need to delete it first
            if !elcd.inverter.empty?
              inverter = elcd.inverter.get
              inverter.remove
              num_inverter_removed += 1
            end
            # Delete the Elcd
            elcd.remove
            num_elcd_removed += 1
          end
        end
      end
    
    end
    
    if num_pv_removed == 0
      runner.registerAsNotApplicable("No GeneratorPhotovoltaic objects removed.")
    else
      runner.registerInfo("#{num_pv_removed} GeneratorPhotovoltaic, #{num_inverter_removed} inverters and #{num_elcd_removed} ElectricLoadCenterDistribution objects removed.")
    end
    
    return(true)
  end
end

# this call registers your script with the OpenStudio SketchUp plug-in
RemovePhotovoltaics.new.registerWithApplication
