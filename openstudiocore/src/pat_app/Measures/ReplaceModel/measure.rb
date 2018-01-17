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

require 'json'

class ReplaceModel < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Replace OpenStudio Model"
  end

  # return a vector of arguments
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    alternativeModelPath = OpenStudio::Ruleset::OSArgument::makePathArgument("alternativeModelPath",true,"osm")
    alternativeModelPath.setDisplayName("Alternative Model Path")
    result << alternativeModelPath
    
    measures_json = OpenStudio::Ruleset::OSArgument::makeStringArgument("measures_json", true)
    measures_json.setDisplayName("Alternative Measures")
    measures_json.setDefaultValue("[]")
    result << measures_json
    
    return result
  end

  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end

    runner.registerInitialCondition("Initial model.")

    alternativeModelPath = runner.getPathArgumentValue("alternativeModelPath",user_arguments)
    measures_json = runner.getStringArgumentValue("measures_json",user_arguments)

    translator = OpenStudio::OSVersion::VersionTranslator.new
    oModel = translator.loadModel(alternativeModelPath)
    if oModel.empty?
      runner.registerError("Could not load alternative model from '" + alternativeModelPath.to_s + "'.")
      return false
    end

    newModel = oModel.get
    
    # parse measures_json and add capital costs to new model
    measures = JSON.parse(measures_json, symbolize_names: true)

    building = newModel.getBuilding
    measures.each do |measure|
      displayName = measure[:displayName]
      taxonomyTag = measure[:taxonomyTag]
      capitalCost = measure[:capitalCost]
      description = measure[:description]
      runner.registerInfo("Adding LifeCycleCost for alternative measure '#{displayName}'.")
      cost = OpenStudio::Model::LifeCycleCost.createLifeCycleCost(displayName, building, capitalCost, 'CostPerEach', 'Construction', 0, 0)
    end
    
    # pull original weather file object over
    weatherFile = newModel.getOptionalWeatherFile
    if not weatherFile.empty?
      weatherFile.get.remove
      runner.registerInfo("Removed alternate model's weather file object.")
    end
    originalWeatherFile = model.getOptionalWeatherFile
    if not originalWeatherFile.empty?
      originalWeatherFile.get.clone(newModel)
    end

    # pull original design days over
    newModel.getDesignDays.each { |designDay|
      designDay.remove
    }
    model.getDesignDays.each { |designDay|
      designDay.clone(newModel)
    }

    # swap underlying data in model with underlying data in newModel
    # model = newModel DOES NOT work
    
    # DLM: there is something wrong with the implementation of swap
    # model.swap(newModel)
    
    handlesToRemove = OpenStudio::UUIDVector.new
    model.objects.each do |object|
      handlesToRemove << object.handle
    end
    model.removeObjects(handlesToRemove)
    
    # must do a full serialization so we are adding IdfObjects
    model.addObjects(newModel.toIdfFile.objects)
    
    runner.registerFinalCondition("Model replaced with alternative #{alternativeModelPath}. Weather file and design days retained from original.")

    return true
  end

end

#this allows the measure to be used by the application
ReplaceModel.new.registerWithApplication
