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
class RemoveUnusedThermalZones < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Remove Unused ThermalZones"
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

    # this was added to give the user a chance to cancel the operation if they inadvertently hit it
    continue_operation = runner.yesNoPrompt("This will remove thermal zones without spaces or equipment. Click Yes to proceed, click No to cancel.")
    if not continue_operation
      puts "Operation canceled, your model was not altered."
      runner.registerAsNotApplicable("Operation canceled, your model was not altered.")
      return true
    end
    
    thermal_zones = model.getThermalZones
    
    thermal_zone_handles_to_remove = OpenStudio::UUIDVector.new
    thermal_zones.each do |thermal_zone|
      if thermal_zone.spaces.empty? && thermal_zone.equipment.empty? && thermal_zone.isRemovable
        thermal_zone_handles_to_remove << thermal_zone.handle
      end
    end
    
    if not thermal_zone_handles_to_remove.empty?
      model.removeObjects(thermal_zone_handles_to_remove)
      runner.registerFinalCondition("Removing #{thermal_zone_handles_to_remove.size} thermal zones.")
    else
      runner.registerFinalCondition("No unused thermal zones to remove.")
    end
    
    return true
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
RemoveUnusedThermalZones.new.registerWithApplication
