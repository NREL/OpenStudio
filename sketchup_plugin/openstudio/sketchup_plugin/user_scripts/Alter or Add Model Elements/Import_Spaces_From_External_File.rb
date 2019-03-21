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
class ImportSpaces < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Merge Spaces From External File"
  end

  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    import_path = OpenStudio::Ruleset::OSArgument::makePathArgument("import_path", true, "osm")
    import_path.setDisplayName("Select OSM File to Import Spaces From")
    result << import_path

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

    osmPath_2 = runner.getStringArgumentValue("import_path",user_arguments)
    if osmPath_2.empty?
      runner.registerError("Non-empty path argument is required.")
      return false
    end

    # Open OSM file
    model_2 = OpenStudio::Model::Model::load(OpenStudio::Path.new(osmPath_2)).get

    # loop through spaces instead of cloning building.
    model_2.getSpaces.each do |space|
      already_cloned = false
      # if space by same name already exists, then delete it.
      model.getSpaces.each do |existing_space|
        if space.name.to_s == existing_space.name.to_s
          # store building story name
          stored_story = existing_space.buildingStory
          # store thermal zone name
          # space will be deleted and then added to thermal zone. Could alter HVAC systems, in particular terminals.
          stored_thermal_zone = existing_space.thermalZone
          # remove space
          runner.registerInfo("Replacing Space '" + existing_space.name.to_s + "'")
          puts 'Replacing Space "' + existing_space.name.to_s + '"'
          existing_space.remove

          # clone space
          cloned_space = space.clone(model)
          cloned_space = cloned_space.to_Space.get

          # set story for space
          if not stored_story.empty?
            cloned_space.setBuildingStory(stored_story.get)
          end

          # set thermal zone for space
          if not stored_thermal_zone.empty?
            cloned_space.setThermalZone(stored_thermal_zone.get)
          end

          # set flag to show that this space has been cloned
          already_cloned = true

        end

      end

      if not already_cloned
        # clone space
        space.clone(model)
      end

    end

    runner.registerFinalCondition("Spaces from file named '" + osmPath_2 + "' have been imported")
    puts 'Spaces from file named "'+ osmPath_2 + '" have been imported'

  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
ImportSpaces.new.registerWithApplication


