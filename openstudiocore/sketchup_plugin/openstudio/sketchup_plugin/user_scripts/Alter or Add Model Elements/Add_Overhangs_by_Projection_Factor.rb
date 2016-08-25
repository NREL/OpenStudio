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
class OverhangScript < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Add Overhangs by Projection Factor"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    projection_factor = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("projection_factor",false)
    projection_factor.setDisplayName("Projection Factor (Fraction)")
    projection_factor.setDefaultValue("0.5".to_f)
    result << projection_factor
    
    offset = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("offset",false)
    offset.setDisplayName("Offset (Fraction)")
    offset.setDefaultValue("0.0".to_f)
    result << offset
    
    remove_existing_shading = OpenStudio::Ruleset::OSArgument::makeBoolArgument("remove_existing_shading",false)
    remove_existing_shading.setDisplayName("Remove Existing Space Shading Groups")
    remove_existing_shading.setDefaultValue(false)
    result << remove_existing_shading
    
    return result
  end
  

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model,runner,user_arguments) # initializes runner for new script

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
    
    projection_factor = runner.getDoubleArgumentValue("projection_factor",user_arguments)
    offset = runner.getDoubleArgumentValue("offset",user_arguments)
    remove_existing_shading = runner.getBoolArgumentValue("remove_existing_shading",user_arguments)
        
    any_in_selection = false
    cleaned_space_names = []
    model.getSubSurfaces.each do |s|

      next if not runner.inSelection(s)
      
      any_in_selection = true

      next if not (s.subSurfaceType == "FixedWindow" or s.subSurfaceType == "OperableWindow")
      
      # see if we need to clean this space's shading groups
      if remove_existing_shading
        surface = s.surface
        if not surface.empty?
          space = surface.get.space
          if not space.empty?
            space_name = space.get.name.get
            if not cleaned_space_names.include?(space_name)
              space.get.shadingSurfaceGroups.each do |shadingSurfaceGroup|
                runner.registerInfo("Removing " + shadingSurfaceGroup.briefDescription + ".")
                shadingSurfaceGroup.remove
              end
              
              # make sure not to clean the same space twice
              if cleaned_space_names.index(space_name) == nil
                cleaned_space_names << space_name
              end
            end
          end
        end
      end

      new_overhang = s.addOverhangByProjectionFactor(projection_factor, offset)
      if new_overhang.empty?
        ok = runner.registerWarning("Unable to add overhang to " + s.briefDescription + 
                 " with projection factor " + projection_factor.to_s + " and offset " + offset.to_s + ".")
        return false if not ok
      else
        new_overhang.get.setName("#{s.name} - Overhang")
        runner.registerInfo("Added overhang " + new_overhang.get.briefDescription + " to " + 
            s.briefDescription + " with projection factor " + projection_factor.to_s + 
            " and offset " + offset.to_s + ".")
      end
    end
    
    if not any_in_selection
      runner.registerAsNotApplicable("No sub surfaces in the current selection. Please select sub surfaces, surfaces, or spaces to add overhangs.")
    end
    
    return true
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
OverhangScript.new.registerWithApplication
