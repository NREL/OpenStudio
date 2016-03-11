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
class MakeSelectedSurfacesAdiabatic < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Make Selected Surfaces Adiabatic and Assign a Construction"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
  
    construction_name = OpenStudio::Ruleset::makeChoiceArgumentOfWorkspaceObjects("construction_name", "OS_Construction".to_IddObjectType, model, false)
    construction_name.setDisplayName("Pick a Construction For Selected Surfaces (Optional)")
    result << construction_name
      
    return result
  end
    
  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)      
    super(model, runner, user_arguments)
    
    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end    
  
    # change boundary condition
    model.getSurfaces.each do |surface|
      if runner.inSelection(surface)
        surface.setOutsideBoundaryCondition("Adiabatic")
      end
    end
        
    construction_name = runner.getStringArgumentValue("construction_name",user_arguments)
    return true if construction_name.empty?
    
    construction_uuid = OpenStudio::toUUID(construction_name)
  
    construction = nil
    c = model.getConstructionBase(construction_uuid)
    if c.empty?
      runner.registerError("Unable to locate construction " + construction_name + " in model.")
      return false
    end
    construction = c.get
    
    runner.registerInfo("Setting selected surfaces' construction to " + construction.briefDescription + ".")
    
    # if construction was picked, apply to surfaces
    model.getSurfaces.each do |surface|
      if runner.inSelection(surface)
        surface.setConstruction(construction)
      end
    end
    
    return true    
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
MakeSelectedSurfacesAdiabatic.new.registerWithApplication
