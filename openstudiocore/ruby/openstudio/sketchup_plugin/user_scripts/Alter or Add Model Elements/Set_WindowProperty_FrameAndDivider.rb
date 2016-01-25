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
class SetWindowPropertyFrameAndDivider < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Set Window Property Frame and Divider"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    choices = OpenStudio::StringVector.new
    
    model.getWindowPropertyFrameAndDividers.each do |c|
      choices << c.name.get
    end
    choices << "<None>"

    name = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("name", choices, false)
    name.setDisplayName("Window Property Frame and Divider")
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
    frameAndDivider = nil
    if name != "<None>"
      remove = false
    
      model.getWindowPropertyFrameAndDividers.each do |c|
        if name == c.name.get
          frameAndDivider = c
          break
        end
      end
      
      if not frameAndDivider
        runner.registerError("Could not find WindowPropertyFrameAndDivider '" + name + "'.")     
        return(false)
      end      
      
    end
    
    model.getSubSurfaces.each do |s|

      next if not runner.inSelection(s)
      
      if remove
        s.resetWindowPropertyFrameAndDivider
      else
        if !s.setWindowPropertyFrameAndDivider(frameAndDivider)
          # could be an opaque door
          runner.registerWarning("Could not set WindowPropertyFrameAndDivider '" + name + "' for SubSurface '" + s.name.to_s + "'.")    
        end
      end
      
    end
    
  end
    
end

# this call registers your script with the OpenStudio SketchUp plug-in
SetWindowPropertyFrameAndDivider.new.registerWithApplication
