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
class RemoveOrphanPhotovoltaics < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Remove Orphan Photovoltaics"
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
    
    num_removed = 0
    model.getGeneratorPhotovoltaics.each do |pv|

      elcd = pv.electricLoadCenterDistribution
      if elcd.empty?
        pv.remove
        num_removed += 1
        next
      end
      
      s = pv.surface
      if s.empty?
        pv.remove
        num_removed += 1
      end

    end
    
    if num_removed == 0
      runner.registerAsNotApplicable("No GeneratorPhotovoltaic objects removed.")
    else
      runner.registerInfo("#{num_removed} GeneratorPhotovoltaic objects removed.")
    end
    
    return(true)
  end
end

# this call registers your script with the OpenStudio SketchUp plug-in
RemoveOrphanPhotovoltaics.new.registerWithApplication
