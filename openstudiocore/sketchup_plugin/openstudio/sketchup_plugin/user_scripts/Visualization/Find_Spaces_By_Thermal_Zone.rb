########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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
class FindByThermalZone < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Find Spaces By Thermal Zone"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    thermal_zone_name = OpenStudio::Ruleset::makeChoiceArgumentOfWorkspaceObjects("thermal_zone_name", "OS_ThermalZone".to_IddObjectType, model, true)
    thermal_zone_name.setDisplayName("Thermal Zone")
    result << thermal_zone_name
    
    return result
  end
    
  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)
  
    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
    
    thermal_zone_handle = runner.getStringArgumentValue("thermal_zone_name",user_arguments)
    # if the user doesn't pick a thermal zone this part of script wont run. Model visibility is left in current state
    
    # get all spaces
    spaces = model.getSpaces

    runner.createProgressBar("Searching Spaces")
    num_total = spaces.size
    num_complete = 0

    # loop over each space
    spaces.each do |space|
      if drawing_interface = space.drawing_interface
        if entity = drawing_interface.entity

          if space.thermalZone.is_initialized 
            thermal_zone = space.thermalZone.get
          else
            next
          end
          
          if thermal_zone_handle == thermal_zone.handle.to_s
            entity.visible = true
          else
            entity.visible = false
          end
          
        end
      end
    end

    runner.destroyProgressBar

    # set hidden to visible so easier for users to select and unide
    Sketchup.active_model.rendering_options["DrawHidden"] = true
    
    return true
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
FindByThermalZone.new.registerWithApplication
