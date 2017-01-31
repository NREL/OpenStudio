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
class GetBCLWeatherFile < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Get BCL Weather File"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    # Check Auth Key in LocalBCL instance
    library = OpenStudio::LocalBCL::instance()
    if (library::prodAuthKey().empty?)
      authKey = OpenStudio::Ruleset::OSArgument::makeStringArgument("authKey")
      authKey.setDisplayName("BCL AuthKey")
      result << authKey
    end
    
    city = OpenStudio::Ruleset::OSArgument::makeStringArgument("city",false)
    city.setDisplayName("City Name")
    city.setDefaultValue("Denver")
    result << city
    
    return result
  end

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)
    
    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
  
    # Check Auth Key in LocalBCL instance
    library = OpenStudio::LocalBCL::instance()
    if (library::prodAuthKey().empty?)
      authKey = runner.getStringArgumentValue("authKey",user_arguments)
      
      good_key = false
      if authKey
        good_key = library::setProdAuthKey(authKey)
      end
      
      if not good_key 
        runner.registerError("BCL authentication required for first time use, follow instructions at http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/ to get your auth key.")
        return false
      end
    end
    
    city = runner.getStringArgumentValue("city",user_arguments)
    
    if city.empty?
      runner.registerError("Please enter city to search for.")
      return false
    end
    
    remote = OpenStudio::RemoteBCL.new
    
    # Search for weather files
    responses = remote.searchComponentLibrary(city, "Weather File")
    
    if responses.empty?
      runner.registerError("No weather file BCL results for city '" + city + "'.")
      return false
    end
    
    # Create options for user prompt
    name_to_uid = Hash.new
    choices = OpenStudio::StringVector.new
    responses.each do |response|
      name_to_uid[response.name] = response.uid
      choices << response.name
    end
    
    arguments = OpenStudio::Ruleset::OSArgumentVector.new
    name = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("name", choices)
    name.setDisplayName("Weather File")
    name.setDefaultValue(choices[0])
    arguments << name
        
    # prompt user
    input = runner.getUserInput(arguments)
    
    name = input["name"]
    
    if not name
      # user canceled
      return false
    end
    
    uid = name_to_uid[name.valueAsString]
    
    if uid.empty?
      runner.registerError("Error, could not find uid for #{name.valueAsString}.  Please try a different weather file.")
      return false    
    end
    
    remote.downloadComponent(uid)
    component = remote.waitForComponentDownload()
    
    if component.empty?
      runner.registerError("Cannot find local component")
      return false    
    end
    component = component.get
    
    # get epw file
    files = component.files("epw")
    if files.empty?
      runner.registerError("No epw file found")
      return false    
    end
    puts "files = #{files}"
    epw_path = component.files("epw")[0]
    puts "epw_path = #{epw_path}"
    # parse epw file
    epw_file = OpenStudio::EpwFile.new(OpenStudio::Path.new(epw_path))
    
    # set weather file
    OpenStudio::Model::WeatherFile::setWeatherFile(model, epw_file)
    
    weather_name = "#{epw_file.city}, #{epw_file.stateProvinceRegion}, #{epw_file.country}"
    weather_lat = epw_file.latitude
    weather_lon = epw_file.longitude
    weather_time = epw_file.timeZone
    weather_elev = epw_file.elevation
    
    # set location, warning not portable
    OpenStudio::Plugin.command_manager.check_site("weather file", weather_name, weather_lat, weather_lon, weather_time, weather_elev)
      
    runner.registerFinalCondition("Successfully set weather file to #{epw_path}")
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
GetBCLWeatherFile.new.registerWithApplication
