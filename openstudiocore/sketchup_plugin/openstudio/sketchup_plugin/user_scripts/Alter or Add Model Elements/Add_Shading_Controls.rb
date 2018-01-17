########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
class AddShadingControls < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Add Shading Controls"
  end

  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    material_hash = Hash.new
    model.getShadingMaterials.each do |sm|
      material_hash[sm.name.get] = sm
    end

    material_handles = OpenStudio::StringVector.new
    material_names = OpenStudio::StringVector.new

    material_handles << OpenStudio::toUUID("").to_s
    material_names << "*None*"

    material_handles << OpenStudio::createUUID().to_s
    material_names << "*New Blind*"

    material_handles << OpenStudio::createUUID().to_s
    material_names << "*New DaylightRedirectionDevice*"

    material_handles << OpenStudio::createUUID().to_s
    material_names << "*New Shade*"

    material_hash.sort.map do |material_name, material|
      material_handles << material.handle.to_s
      material_names << material_name
    end

    material = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("material", material_handles, material_names, true)
    material.setDisplayName("Shading material.")
    material.setDescription("Choose a shading material to deploy when sub surface is shaded.")
    material.setDefaultValue("*None*")
    result << material

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
    construction.setDisplayName("Switchable construction.")
    construction.setDescription("Choose a construction to represent the shaded state of a switchable glazing.")
    construction.setDefaultValue("*None*")
    result << construction


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

    puts user_arguments

    if not material.empty? and not material.get.to_ShadingMaterial.empty?
      material = material.get.to_ShadingMaterial.get
    else
      begin
        material = nil

        # look at display name of material argument
        user_arguments.each_value do |argument|
          if argument.name == 'material'
            puts 'found it!'
            valueDisplayName = argument.valueDisplayName
            if valueDisplayName == '*New Blind*'
              material = OpenStudio::Model::Blind.new(model)
            elsif valueDisplayName == '*New DaylightRedirectionDevice*'
              material = OpenStudio::Model::DaylightRedirectionDevice.new(model)
            elsif valueDisplayName == '*New Shade*'
              material = OpenStudio::Model::Shade.new(model)
            end
          end
        end
      rescue Exception => e
        #puts e.message
        #puts e.backtrace.inspect
      end
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
