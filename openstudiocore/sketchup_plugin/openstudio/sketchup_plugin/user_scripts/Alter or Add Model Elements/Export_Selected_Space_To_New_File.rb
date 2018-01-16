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
class ExportSpaces < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Export Selected Spaces to a new External Model"
  end

  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    save_path = OpenStudio::Ruleset::OSArgument::makePathArgument("save_path", false, "osm", false)
    save_path.setDisplayName("Save Export Spaces As ")
    save_path.setDefaultValue("ExportedSpaces.osm")
    result << save_path

    begin
      SKETCHUP_CONSOLE.show
    rescue => e
    end

    return result
  end

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end

    osmPath_2 = runner.getStringArgumentValue("save_path",user_arguments)

    # stop script if no spaces are selected.
    anyInSelection = false
    model.getSpaces.each do |space|
      if runner.inSelection(space)
        anyInSelection = true
        break
      end
    end

    if not anyInSelection
      runner.registerAsNotApplicable("No spaces selected.")
      return true
    end

    # create a new empty model
    model_2 = OpenStudio::Model::Model.new

    # loop through and clone spaces
    count = 0
    model.getSpaces.each do |space|
      if runner.inSelection(space)
        runner.registerInfo("Adding " + space.briefDescription + " to " + osmPath_2.to_s + ".")
        space.clone(model_2)
        count += 1
      end
    end

    # save as osm
    model_2.save(OpenStudio::Path.new(osmPath_2),true)
    runner.registerFinalCondition("File named '"+ osmPath_2 + "' created with " + count.to_s + " spaces.")
    puts 'File named "'+ osmPath_2 + '" has been generated'

  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
ExportSpaces.new.registerWithApplication


