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
class CreateSketchUpGroupsFromDiagram < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Create SketchUp Groups From Diagram"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    height = OpenStudio::Ruleset::OSArgument::makeStringArgument("height",false)
    height.setDisplayName("Floor Height (SketchUp Units) ")
    height.setDefaultValue("10'")
    result << height

    num_floors = OpenStudio::Ruleset::OSArgument::makeIntegerArgument("num_floors",false)
    num_floors.setDisplayName("Number of Floors ")
    num_floors.setDefaultValue(1)
    result << num_floors

    return result
  end
    
  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments) # initializes runner for new script

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end

    height = runner.getStringArgumentValue("height",user_arguments)
    num_floors = runner.getIntegerArgumentValue("num_floors",user_arguments)

    # error check input to make sure positive values
    valid_input = true

    #convert string to SketchUp length
    height = height.to_l

    if height <= 0
      UI.messagebox("Height must be greater than 0 meters.")
      valid_input = false
    end

    num_floors = num_floors.to_i
    if num_floors < 1
      UI.messagebox("Number of floors must be greater than or equal to 1.")
      valid_input = false
    end

    # get sketchup model
    suModel = Sketchup.active_model
    selection = suModel.selection

    # save selection
    saved_selection = []
    selection.each {|e| saved_selection << e}

    valid_diagram = true

    # canel if there is no selection (remove OpenStudio objects first)
    if selection.empty?
      UI.messagebox("No loose geometry is selected, please select objects to extrude into OpenStudio spaces")
      valid_diagram = false
    end

    # cancel if all faces in selection are not horizontal, flip reversed faces
    expected_normal = Geom::Vector3d.new 0,0,-1
    inv_expected_normal = Geom::Vector3d.new 0,0,1
    status = 0
    selection.each do |index|
      if index.typename == "Face"
        if index.normal != expected_normal
          if index.normal == inv_expected_normal
            puts "reversing face normal of #{index}"
            flip = index.reverse!
          else
            status = 1
          end
        end
      end
    end #end of selection.each do

    if status == 1
      UI.messagebox("Not all selected surfaces are horizontal, please limit selection to horizontal surfaces")
      valid_diagram = false
    end

    if valid_diagram and valid_input # skip over if not valid selection for space diagram or valid input

      # sort SketchUp selection and pass this to OpenStudio
      sel_sort = Sketchup.active_model.selection.to_a
      sel_sort.delete_if {|e| not e.is_a?(Sketchup::Face) }
      sel_sort.sort! {|a,b|
        ([a.vertices.min{|v1,v2| v1.position.y <=> v2.position.y }.position.y,
          a.vertices.min{|v1,v2| v1.position.x <=> v2.position.x }.position.x] <=>
            [b.vertices.min{|v1,v2| v1.position.y <=> v2.position.y }.position.y,
             b.vertices.min{|v1,v2| v1.position.x <=> v2.position.x }.position.x] )
      }

      faces = []
      sel_sort.each { |entity| faces << entity if entity.class == Sketchup::Face }

      # create or confirm layers
      layers = suModel.layers
      new_layer_diagram = layers.add("SketchUp - Space Diagrams")
      new_layer_spaces = layers.add("OpenStudio BackgroundModel Space")

      # add loop to create multiple floors. Will need to create new stories and adjust z values
      for floor in (1..num_floors)

        # rest room counter
        rm = 0

        # loop through faces in the selection
        faces.each do |face|

          # get vertices
          pts = face.outer_loop.vertices

          # make group
          group = Sketchup.active_model.entities.add_group

          #put on space layer
          group.layer = new_layer_spaces

          # create face in group
          entities = group.entities
          face = entities.add_face pts

          # transform group to adjust for number of floors
          base_height = height*(floor-1)
          new_transform = Geom::Transformation.new([0,0,base_height])
          group.transformation = new_transform

          # extrude surface by floor height
          status = face.pushpull height*-1, false

          rm = rm + 1
          # set space name
          if faces.length < 100
            padded_room_number = "Space " + floor.to_s + "%02d" % rm
          else
            padded_room_number = "Space " + floor.to_s + "%03d" % rm
          end

          # name group
          group.name = padded_room_number

        end # end of selection loop

      end # end of floor loop

      # turn off layer visibility
      new_layer_diagram.visible  = false

      # make group out of selection and put onto OS Loose Geometry Layer
      thermal_diagram = Sketchup.active_model.entities.add_group(saved_selection)
      thermal_diagram.layer = new_layer_diagram

    end #end of if valid_diagram

  end  #end of run

end #end of user script

# this call registers your script with the OpenStudio SketchUp plug-in
CreateSketchUpGroupsFromDiagram.new.registerWithApplication