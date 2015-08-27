######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
#  All rights reserved.
#
#  This library is free software you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

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
