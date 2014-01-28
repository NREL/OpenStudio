######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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
class AddShadingControls < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Add Shading Controls"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    choices = OpenStudio::StringVector.new
    
    model.getConstructions.each do |c|
      c.layers.each do |layer|
        if not layer.to_ShadingMaterial.empty?
          choices << ("Construction " + c.name.get)
          break
        end
      end
    end
    model.getShadingMaterials.each do |sm|
      choices << ("Material " + sm.name.get)
    end
    operation = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("operation", choices, false)
    operation.setDisplayName("Add Shading Control For")
    if not choices.empty?
      operation.setDefaultValue(choices[0])
    end
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
    
    remove = true
    shadingControl = nil
    
    if match_data = /Construction (.*)/.match(operation)
      remove = false
      name = match_data[1]
      
      shadingConstruction = nil
      model.getConstructions.each do |c|
        if name == c.name.get
          shadingConstruction = c
          break
        end
      end
      
      if not shadingConstruction
        runner.registerError("Could not find Construction '" + name + "'.")     
        return(false)
      end
      
      shadingControl = OpenStudio::Model::ShadingControl.new(shadingConstruction)
      
    elsif match_data = /Material (.*)/.match(operation)
      remove = false
      name = match_data[1]
      
      shadingMaterial = nil
      model.getShadingMaterials.each do |sm|
        if name == sm.name.get
          shadingMaterial = sm
          break
        end
      end
      
      if not shadingMaterial
        runner.registerError("Could not find ShadingMaterial '" + name + "'.")     
        return(false)
      end
      
      shadingControl = OpenStudio::Model::ShadingControl.new(shadingMaterial)
      
    end
  end
end

# this call registers your script with the OpenStudio SketchUp plug-in
AddShadingControls.new.registerWithApplication
