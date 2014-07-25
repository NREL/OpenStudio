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
class HideManifoldSolidSpaces < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Hide Manifold Solid Spaces"
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
    super(model, runner, user_arguments)
    
    #this script is not portable, uses SketchUp specific methods
  
    # get all spaces
    spaces = model.getSpaces
    
    runner.createProgressBar("Hiding Manifold Spaces")
    num_total = spaces.size
    num_complete = 0
    
    # loop over each space
    spaces.each do |space|
      if drawing_interface = space.drawing_interface
        if entity = drawing_interface.entity
    
          # add a temporary group at the top level
          temp_group_outer = Sketchup.active_model.entities.add_group
          
          # add a temporary group inside the outer temporary group
          temp_group = temp_group_outer.entities.add_group
          
          # loop through all child entities in the space
          entity.entities.each do |child_entity|
            if child_entity.typename == "Face"
              temp_group.entities.add_face(child_entity.outer_loop.vertices)
            elsif child_entity.typename == "Edge"
              temp_group.entities.add_line(child_entity.vertices)
            end
          end

          # explode group to add back in with collinear points (needed for manifold test)
          temp_group.explode

          # run manifold solid test on outer group
          status = temp_group_outer.manifold?

          if status 
            # if test passes then hide space
            entity.visible = false
          else
            # show space
            entity.visible = true
          end

          # delete temporary outer group
          temp_group_outer.erase!
          
        end
      end
      
      num_complete += 1
      runner.updateProgress((100*num_complete)/num_total)
    end
    
    runner.destroyProgressBar
      
    # set hidden to visible so easier for users to select and unide
    Sketchup.active_model.rendering_options["DrawHidden"] = true
    
    return true
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
HideManifoldSolidSpaces.new.registerWithApplication
