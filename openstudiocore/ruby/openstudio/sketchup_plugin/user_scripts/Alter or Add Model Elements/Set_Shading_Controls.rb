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
class SetShadingControls < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Set Shading Controls"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    choices = OpenStudio::StringVector.new
    
    model.getShadingControls.each do |c|
      choices << c.name.get
    end
    choices << "<No Shading Control>"

    name = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("name", choices, false)
    name.setDisplayName("Set Shading Controls")
    name.setDefaultValue(choices[0])

    result << name
    
    return result
  end
  

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model,runner,user_arguments) # initializes runner for new script

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
    
    name = runner.getStringArgumentValue("name",user_arguments)
    
    remove = true
    shadingControl = nil
    if name != "<No Shading Control>"
      remove = false
    
      model.getShadingControls.each do |c|
        if name == c.name.get
          shadingControl = c
          break
        end
      end
      
      if not shadingControl
        runner.registerError("Could not find ShadingControl '" + name + "'.")     
        return(false)
      end      
      
    end
    
    model.getSubSurfaces.each do |s|

      next if not runner.inSelection(s)
      
      if remove
        s.resetShadingControl
      else
        s.setShadingControl(shadingControl)
      end
      
    end
    
  end
    
end

# this call registers your script with the OpenStudio SketchUp plug-in
SetShadingControls.new.registerWithApplication
