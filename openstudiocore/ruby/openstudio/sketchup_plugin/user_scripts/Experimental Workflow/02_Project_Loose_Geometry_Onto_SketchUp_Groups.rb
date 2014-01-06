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
class ProjectLooseGeometryOntoSketchUpGroups < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Project Loose Geometry Onto SketchUp Groups"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    selection_only = OpenStudio::Ruleset::OSArgument::makeBoolArgument("selection_only",false)
    selection_only.setDisplayName("Project Only Selected Loose Geometry? ")
    selection_only.setDefaultValue(false)
    result << selection_only

    return result
  end
    
  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments) # initializes runner for new script

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end

    selection_only = runner.getBoolArgumentValue("selection_only",user_arguments)

    # get sketchup model
    suModel = Sketchup.active_model
    entities = suModel.active_entities
    selection = suModel.selection
    groups = []
    components = []

    if not selection_only
      selection = entities
    end

    # save selection
    saved_selection = []

    selection.each do |entity|
      if entity.class.to_s == "Sketchup::Face" or entity.class.to_s == "Sketchup::Edge"
        saved_selection << entity
      end
    end

    # get groups in model
    entities.each do |entity|
      if entity.class.to_s == "Sketchup::Group"
        entity.visible = false
        groups << entity
      end
      if entity.class.to_s == "Sketchup::ComponentInstance"
        puts "geometry won't be altered for #{entity.name}, it is a component instance."
        entity.visible = false
        components << entity
      end
    end #end of entities.each do

    # loop through groups intersecting with selection
    groups.each do |group|
      group.visible = true
        group.entities.intersect_with(true, group.transformation, group.entities.parent, group.transformation, false, selection.to_a)
      group.visible = false
    end #end of groups.each do

    #unhide everything
    groups.each do |group|
      group.visible = true
    end
    components.each do |component|
      component.visible = true
    end

    # create or confirm layer called "OpenStudio - Project Loose Geometry" exists
    layers = suModel.layers
    new_layer = layers.add("SketchUp - Project Loose Geometry")

    # turn off layer visibility
    new_layer.visible  = false

    # make group out of selection and put onto OS Loose Geometry Layer
    loose_geometry = Sketchup.active_model.entities.add_group(saved_selection)
    loose_geometry.layer = new_layer

  end  #end of run

end #end of user script

# this call registers your script with the OpenStudio SketchUp plug-in
ProjectLooseGeometryOntoSketchUpGroups.new.registerWithApplication
