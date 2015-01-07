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
class AddShadingControls < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Add Shading Controls"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    construction_hash = Hash.new
    model.getConstructions.each do |c|
      c.layers.each do |layer|
        if not layer.to_FenestrationMaterial.empty?
          construction_hash[c.name.get] = c
          break
        end        
      end
    end

    construction_handles = OpenStudio::StringVector.new
    construction_names = OpenStudio::StringVector.new
    
    construction_handles << OpenStudio::toUUID("").to_s
    construction_names << "*None*"
    
    construction_hash.sort.map do |construction_name, construction|
      construction_handles << construction.handle.to_s
      construction_names << construction_name
    end
    
    construction = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("construction", construction_handles, construction_names, true)
    construction.setDisplayName("Choose shaded construction.")
    construction.setDefaultValue("*None*")
    result << construction
    
    material_hash = Hash.new
    model.getShadingMaterials.each do |sm|
      material_hash[sm.name.get] = sm
    end
    
    material_handles = OpenStudio::StringVector.new
    material_names = OpenStudio::StringVector.new
    
    material_handles << OpenStudio::toUUID("").to_s
    material_names << "*None*"
    
    material_hash.sort.map do |material_name, material|
      material_handles << material.handle.to_s
      material_names << material_name
    end
    
    material = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("material", material_handles, material_names, true)
    material.setDisplayName("Choose shading material.")
    material.setDefaultValue("*None*")
    result << material
    
    return result
  end
  

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model,runner,user_arguments) # initializes runner for new script

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
    
    construction = runner.getOptionalWorkspaceObjectChoiceValue("construction",user_arguments,model) #model is passed in because of argument type
    material = runner.getOptionalWorkspaceObjectChoiceValue("material",user_arguments,model) #model is passed in because of argument type
    
    if not construction.empty? and not construction.get.to_Construction.empty?
      construction = construction.get.to_Construction.get
    else
      construction = nil
    end
    
    if not material.empty? and not material.get.to_ShadingMaterial.empty?
      material = material.get.to_ShadingMaterial.get
    else
      material = nil
    end
    
    if construction and material
      runner.registerWarning("Selected both shading material and a shaded construction, creating shading control for both.")     
      shadingControl = OpenStudio::Model::ShadingControl.new(construction)
      runner.registerInfo("Created shading control #{shadingControl.name.get}.")    
      shadingControl = OpenStudio::Model::ShadingControl.new(material)
      runner.registerInfo("Created shading control #{shadingControl.name.get}.")  
    elsif construction
      shadingControl = OpenStudio::Model::ShadingControl.new(construction)
      runner.registerInfo("Created shading control #{shadingControl.name.get}.")  
    elsif material
      shadingControl = OpenStudio::Model::ShadingControl.new(material)
      runner.registerInfo("Created shading control #{shadingControl.name.get}.")  
    else
      runner.registerError("Need to choose either a shading material or a shaded construction.")     
      return(false)
    end
    
    return(true)
  end
end

# this call registers your script with the OpenStudio SketchUp plug-in
AddShadingControls.new.registerWithApplication
