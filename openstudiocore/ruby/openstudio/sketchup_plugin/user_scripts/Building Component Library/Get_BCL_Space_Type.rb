######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class GetBCLSpaceType < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Get BCL Space Type"
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

    return result
  end

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    # Check Auth Key in LocalBCL instance
    library = OpenStudio::LocalBCL::instance()
    if (library::prodAuthKey().empty?)
      if not runner.validateUserArguments(arguments(model),user_arguments)
        return false
      end
      
      authKey = runner.getStringArgumentValue("authKey",user_arguments)

      good_key = false
      if authKey
        good_key = library::setProdAuthKey(authKey)
      end
      
      if not good_key 
        runner.registerError("BCL authentication required for first time use, follow instructions at openstudio.nrel.gov/using-building-component-library-bcl-key-openstudio to get your auth key.")
        return false
      end
    end
    
    # open ruby console to show status
    begin
      Sketchup.send_action("showRubyPanel:")
    rescue Exception => e
    end
    
    remoteBCL = OpenStudio::RemoteBCL.new
    
    remoteBCL.downloadOnDemandGenerator("bb8aa6a0-6a25-012f-9521-00ff10704b07");
    generator = remoteBCL.waitForOnDemandGenerator();
    if generator.empty?
      runner.registerError("Could not retrieve on demand generator description")
      return false    
    end
    generator = generator.get
    
    ###########################################################################
    # Get the vintage
    
    vintageArgument = generator.getActiveArgument("NREL_reference_building_vintage")
    if vintageArgument.empty?
      runner.registerError("Cannot find argument NREL_reference_building_vintage")
      return false    
    end
    vintageArgument = vintageArgument.get
    
    choices = OpenStudio::StringVector.new
    vintageArgument.valueRestrictions.each do |valueRestriction|
      choices << valueRestriction.name + '        '
    end

    arguments = OpenStudio::Ruleset::OSArgumentVector.new
    vintage = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("NREL_reference_building_vintage", choices)
    vintage.setDisplayName(vintageArgument.displayName)
    vintage.setDefaultValue(choices[0])
    arguments << vintage
    
    # prompt user
    input = runner.getUserInput(arguments)
    nrel_reference_building_vintage = input["NREL_reference_building_vintage"]
    
    if not nrel_reference_building_vintage
      return false
    end
    
    generator.setArgumentValue("NREL_reference_building_vintage", nrel_reference_building_vintage.valueAsString.rstrip)
    
    ###########################################################################
    # Get the climate zone
    
    climateArgument = generator.getActiveArgument("Climate_zone")
    if climateArgument.empty?
      runner.registerError("Cannot find argument Climate_zone")
      return false    
    end
    climateArgument = climateArgument.get
    
    choices = OpenStudio::StringVector.new
    climateArgument.valueRestrictions.each do |valueRestriction|
      choices << valueRestriction.name + '        '
    end

    arguments = OpenStudio::Ruleset::OSArgumentVector.new
    climate = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("Climate_zone", choices)
    climate.setDisplayName(climateArgument.displayName)
    climate.setDefaultValue(choices[0])
    arguments << climate
    
    # prompt user
    input = runner.getUserInput(arguments)
    climate_zone = input["Climate_zone"]
    
    if not climate_zone
      return false
    end
    
    generator.setArgumentValue("Climate_zone", climate_zone.valueAsString.rstrip)
    
    ###########################################################################
    # Get the primary space type
    
    primarySpaceTypeArgument = generator.getActiveArgument("NREL_reference_building_primary_space_type")
    if primarySpaceTypeArgument.empty?
      runner.registerError("Cannot find argument NREL_reference_building_primary_space_type")
      return false    
    end
    primarySpaceTypeArgument = primarySpaceTypeArgument.get
    
    choices = OpenStudio::StringVector.new
    primarySpaceTypeArgument.valueRestrictions.each do |valueRestriction|
      choices << valueRestriction.name + '        '
    end

    arguments = OpenStudio::Ruleset::OSArgumentVector.new
    primarySpaceType = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("NREL_reference_building_primary_space_type", choices)
    primarySpaceType.setDisplayName(primarySpaceTypeArgument.displayName)
    primarySpaceType.setDefaultValue(choices[0])
    arguments << primarySpaceType
    
    # prompt user
    input = runner.getUserInput(arguments)
    nrel_reference_building_primary_space_type = input["NREL_reference_building_primary_space_type"]
    
    if not nrel_reference_building_primary_space_type
      return false
    end
    
    generator.setArgumentValue("NREL_reference_building_primary_space_type", nrel_reference_building_primary_space_type.valueAsString.rstrip)
    
    ###########################################################################
    # Loop over all the secondary space types
    
    secondarySpaceTypeArgument = generator.getActiveArgument("NREL_reference_building_secondary_space_type")
    if secondarySpaceTypeArgument.empty?
      runner.registerError("Cannot find argument NREL_reference_building_secondary_space_type")
      return false    
    end
    secondarySpaceTypeArgument = secondarySpaceTypeArgument.get

    versionTranslator = OpenStudio::OSVersion::VersionTranslator.new
    success = true
    secondarySpaceTypeArgument.valueRestrictions.each do |valueRestriction|
    
      puts "Searching for space type for secondary space type '#{valueRestriction.name}'"
      
      # set the value
      test = generator.setArgumentValue("NREL_reference_building_secondary_space_type", valueRestriction.name)
      if not test
        puts "Could not set argument value"
        
        success = false
        next
      end
      
      test = generator.checkArgumentValues
      if not test
        puts "Generator does not have valid values for all required inputs"
        
        success = false
        next
      end
      
      # check local library
      vintage = nrel_reference_building_vintage.valueAsString.rstrip.gsub('_',' ')
      climate = climate_zone.valueAsString.rstrip.gsub('_',' ')
      primary_space_type = nrel_reference_building_primary_space_type.valueAsString.rstrip.gsub('_',' ')
      secondary_space_type = valueRestriction.name.gsub('_',' ')
      query = "#{vintage} #{climate} #{primary_space_type} #{secondary_space_type}";
      matches = OpenStudio::LocalBCL::instance().searchComponents(query, "");
      if matches.length > 0
        puts "Space type found locally, skipping"
        component = matches[0]
      else
        puts "Space type not found locally, downloading"
      
        component = remoteBCL.getOnDemandComponent(generator)
        if component.empty?
          puts "Could not download space type for secondary space type '#{valueRestriction.name}'"
          
          success = false
          next
        end
        component = component.get
      end
      
      oscFiles = component.files("osc")
      if oscFiles.empty?
        "No osc files found"
        
        success = false
        next
      end

      oscPath = OpenStudio::Path.new(oscFiles[0])
      modelComponent = versionTranslator.loadComponent(oscPath)
      
      if modelComponent.empty?
        "Could not load component"
        
        success = false
        next
      end
      modelComponent = modelComponent.get
      
      model.insertComponent(modelComponent)

    end
    
    return success
    
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
GetBCLSpaceType.new.registerWithApplication
