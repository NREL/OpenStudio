########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class RemovePhotovoltaics < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Remove Photovoltaics"
  end

  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    choices = OpenStudio::StringVector.new
    choices << "Yes"
    choices << "No"

    remove_elcd = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("remove_elcd", choices, choices, true)
    remove_elcd.setDisplayName("Remove Empty Electric Load Center Distribution And Orphaned Inverters?")
    remove_elcd.setDefaultValue("Yes")
    result << remove_elcd

    return result
  end


  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model,runner,user_arguments) # initializes runner for new script

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end

    remove_elcd = runner.getStringArgumentValue("remove_elcd",user_arguments)

    num_pv_removed = 0
    num_elcd_removed = 0
    num_inverter_removed = 0

    model.getPlanarSurfaces.each do |s|

      # Only remove for selected surfaces
      next if not runner.inSelection(s)

      s.generatorPhotovoltaics.each do |pv|
        pv.remove
        num_pv_removed += 1
      end

    end

    if remove_elcd == "Yes"
      # remove ElectricLoadCenterDistributions without any generators
      model.getElectricLoadCenterDistributions.each do |elcd|
        if elcd.generators.empty?
          inverter = elcd.inverter
          if !inverter.empty?
            inverter.get.remove
            num_inverter_removed += 1
          end
          elcd.remove
          num_elcd_removed += 1
        end
      end

      # remove inverters that are not connected to an ElectricLoadCenterDistribution
      model.getInverters.each do |inverter|
        if inverter.electricLoadCenterDistribution.empty?
          inverter.remove
          num_inverter_removed += 1
        end
      end
    end

    if (num_pv_removed + num_elcd_removed + num_inverter_removed) == 0
      runner.registerAsNotApplicable("No objects removed.")
    else
      runner.registerInfo("#{num_pv_removed} GeneratorPhotovoltaic, #{num_elcd_removed} ElectricLoadCenterDistribution, #{num_inverter_removed} Inverter objects removed.")
    end

    return(true)
  end
end

# this call registers your script with the OpenStudio SketchUp plug-in
RemovePhotovoltaics.new.registerWithApplication
