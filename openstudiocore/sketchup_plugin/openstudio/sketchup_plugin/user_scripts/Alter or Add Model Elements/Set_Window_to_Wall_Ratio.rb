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
class WWRScript < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Set Window to Wall Ratio"
  end

  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    wwr = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("wwr",false)
    wwr.setDisplayName("Window to Wall Ratio (Fraction)")
    wwr.setDefaultValue(0.4)
    result << wwr

    offset = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("offset",false)
    offset.setDisplayName("Offset (meters)")
    offset.setDefaultValue(0.76)
    result << offset

    choices = OpenStudio::StringVector.new
    choices << "Above Floor"
    choices << "Below Ceiling"
    application_type = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("application_type", choices, false)
    application_type.setDisplayName("Application Type")
    application_type.setDefaultValue("Above Floor")
    result << application_type

    return result
  end

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model,runner,user_arguments) # initializes runner for new script

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end

    wwr = runner.getDoubleArgumentValue("wwr",user_arguments)
    offset = runner.getDoubleArgumentValue("offset",user_arguments)
    application_type = runner.getStringArgumentValue("application_type",user_arguments)

    if wwr <= 0 or wwr >= 1
      runner.registerError("Window to Wall Ratio must be greater than 0 and less than 1.")
      return false
    end

    if offset <= 0
      runner.registerError("Offset value must be greater than 0.")
      return false
    end

    heightOffsetFromFloor = nil
    if application_type == "Above Floor"
      heightOffsetFromFloor = true
    else
      heightOffsetFromFloor = false
    end

    any_in_selection = false
    model.getSurfaces.each do |s|

      next if not runner.inSelection(s)

      any_in_selection = true

      next if not s.outsideBoundaryCondition == "Outdoors"

      next if not s.surfaceType == "Wall"

      new_window = s.setWindowToWallRatio(wwr, offset, heightOffsetFromFloor)

      if new_window.empty?
        ok = runner.registerWarning("Unable to set window-to-wall ratio of " + s.name.get + " to " + wwr.to_s + ".")
      else
        # not fully accurate - Dan to refactor wiggliness out of C++
        actual = new_window.get.grossArea / s.grossArea
        runner.registerInfo("Set window-to-wall ratio of " + s.name.get + " to " + actual.to_s + ".")
        if not (OpenStudio::DoublesRelativeError(wwr,actual) < 1.0E-3)
          ok = runner.registerInfo("Tried to set window-to-wall ratio of " + s.name.get + " to " +
                                   wwr.to_s + ", but set to " + actual.to_s + " instead.")
        end
      end
    end

    if not any_in_selection
      runner.registerAsNotApplicable("No surfaces in the current selection.  Please select surfaces or spaces to set Window to Wall Ratio.")
    end

    return true
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
WWRScript.new.registerWithApplication


