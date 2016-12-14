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
class RenameThermalZonesBasedOnSpaceNames < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Rename Thermal Zones Based On Space Names"
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
  
    # get all thermal zones
    thermal_zones = model.getThermalZones
    
    runner.createProgressBar("Creating Zones for Untagged Spaces")
    num_total = thermal_zones.size
    num_complete = 0

    # loop through thermal zones
    thermal_zones.each do |thermal_zone| # this is going through all, not just selection
    
      # reset the array of spaces to be empty
      spaces_in_thermal_zone = []
      # reset length of array of spaces
      number_of_spaces = 0
      
      # get list of spaces in thermal zone
      spaces = thermal_zone.spaces
      spaces.each do |space|

        # make an array instead of the puts statement
        spaces_in_thermal_zone.push space.name.to_s
 
      end
      
      # store length of array
      number_of_spaces = spaces_in_thermal_zone.size
      
      # sort the array
      spaces_in_thermal_zone = spaces_in_thermal_zone.sort
      
      # setup a suffix if the thermal zone contains more than one space
      if number_of_spaces > 1
        multi = " - Plus"
      else
        multi = ""
      end
      
      # rename thermal zone based on first space with prefix added e.g. ThermalZone 203
      if number_of_spaces > 0
        new_name = "Thermal Zone: " + spaces_in_thermal_zone[0] + multi
        thermal_zone.setName(new_name)
      else
        puts "#{thermal_zone.name.to_s} did not have any spaces, and will not be renamed." 
      end
      
      # if someone tries to run this after it has already been run, and results in trying to use an already used theral zone name we could
      # add error checking, but I don't think we need it for now. Eventually I'd like to expand this with some logic to automatically
      # zone the model based on adjacent spaces with similar exterior exposure and the same space type on the same building story

      num_complete += 1
      runner.updateProgress((100*num_complete)/num_total)
    end

    runner.destroyProgressBar
    
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
RenameThermalZonesBasedOnSpaceNames.new.registerWithApplication
