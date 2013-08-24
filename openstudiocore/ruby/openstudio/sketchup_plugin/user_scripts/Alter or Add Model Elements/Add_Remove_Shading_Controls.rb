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

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class AddRemoveShadingControls < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Add or Remove Shading Controls"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    choices = OpenStudio::StringVector.new
    choices << "Add Shading Controls"
    choices << "Remove Shading Controls"
    operation = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("operation", choices, false)
    operation.setDisplayName("Operation")
    operation.setDefaultValue("Add Shading Controls")
    result << operation
    
    return result
  end
  

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model,runner,user_arguments) # initializes runner for new script

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
    
    operation = runner.getStringArgumentValue("operation",user_arguments)

    any_in_selection = false
    model.getSubSurfaces.each do |s|

      next if not runner.inSelection(s)
      
      any_in_selection = true

      shadingControl = s.shadingControl
      
      if operation == "Add Shading Controls"
        if shadingControl.empty?
          shadingControl = s.addShadingControl
          if shadingControl.empty?
            runner.registerInfo("Could not add shading controls to " + s.briefDescription + ".")    
          else
            runner.registerInfo("Added shading controls to " + s.briefDescription + ".")        
          end
        end
      elsif operation == "Remove Shading Controls"
        if not shadingControl.empty?
          s.resetShadingControl
          runner.registerInfo("Removed shading controls from " + s.briefDescription + ".")    
        end
      end

    end
    
    if not any_in_selection
      runner.registerAsNotApplicable("No sub surfaces in the current selection. Please select sub surfaces, surfaces, or spaces to modify.")
    end
    
    return true
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
AddRemoveShadingControls.new.registerWithApplication
