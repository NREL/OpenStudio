########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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
