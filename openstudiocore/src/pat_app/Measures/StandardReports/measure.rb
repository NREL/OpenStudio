require 'erb'

#start the measure
class StandardReports < OpenStudio::Ruleset::ReportingUserScript

  #define the name that a user will see, this method may be deprecated as
  #the display name in PAT comes from the name field in measure.xml
  def name
    return "Standard Reports"
  end

  #define the arguments that the user will input
  def arguments()
    args = OpenStudio::Ruleset::OSArgumentVector.new

    return args
  end #end the arguments method

  #define what happens when the measure is run
  def run(runner, user_arguments)
    super(runner, user_arguments)

    #use the built-in error checking
    if not runner.validateUserArguments(arguments(), user_arguments)
      return false
    end
    
    os_version = OpenStudio::VersionString.new(OpenStudio::openStudioVersion())
    min_version_feature1 = OpenStudio::VersionString.new("1.2.3")

    # get the last model and sql file

    model = runner.lastOpenStudioModel
    if model.empty?
      runner.registerError("Cannot find last model.")
      return false
    end
    model = model.get

    sqlFile = runner.lastEnergyPlusSqlFile
    if sqlFile.empty?
      runner.registerError("Cannot find last sql file.")
      return false
    end
    sqlFile = sqlFile.get
    model.setSqlFile(sqlFile)

    # put data into variables, these are available in the local scope binding
    #building_name = model.getBuilding.name.get

    web_asset_path = OpenStudio::getSharedResourcesPath() / OpenStudio::Path.new("web_assets")

    energy = "var consumption = {\n"
    fuel_type = ""
    units = ""

    site_energy_use = 0.0
    OpenStudio::EndUseFuelType::getValues.each do |fuel_type|
      energy << "\t\""
      fuel_type = OpenStudio::EndUseFuelType.new(fuel_type).valueDescription
      energy << OpenStudio::EndUseFuelType.new(fuel_type).valueDescription # append this to remove whitespace between words ".delete(' ')"
      energy << " Consumption\":{\n\t\t\"units\":"
      if fuel_type == "Electricity"
        units = "\"kWh\""
        unit_str = "kWh"
      else
        units = "\"Million Btu\""
        unit_str = "MBtu"
      end
      fuel_type_aggregation = 0.0
      energy << units
      energy << ",\n\t\t\"data\":{\n\t\t\t\""
      OpenStudio::EndUseCategoryType::getValues.each do |category_type|
        fuel_and_category_aggregation = 0.0
        category_str = OpenStudio::EndUseCategoryType.new(category_type).valueDescription
        energy << category_str # append this to remove whitespace between words ".delete(' ')"
        energy << "\":["
        OpenStudio::MonthOfYear::getValues.each do |month|
          if month >= 1 and month <= 12
            if not sqlFile.energyConsumptionByMonth(OpenStudio::EndUseFuelType.new(fuel_type),
                                                    OpenStudio::EndUseCategoryType.new(category_type),
                                                    OpenStudio::MonthOfYear.new(month)).empty?
              valInJ = sqlFile.energyConsumptionByMonth(OpenStudio::EndUseFuelType.new(fuel_type),
                                                        OpenStudio::EndUseCategoryType.new(category_type),
                                                        OpenStudio::MonthOfYear.new(month)).get
              fuel_and_category_aggregation += valInJ
              valInUnits = OpenStudio::convert(valInJ,"J",unit_str).get()
              temp = sprintf "%.3f", valInUnits
              energy << temp.to_s
              energy << ","
              if os_version >= min_version_feature1
                month_str = OpenStudio::MonthOfYear.new(month).valueDescription
                prefix_str = OpenStudio::toUnderscoreCase("#{fuel_type}_#{category_str}_#{month_str}")
                runner.registerValue("#{prefix_str}_si",valInJ,"J")
                runner.registerValue("#{prefix_str}_ip",valInUnits,unit_str)
              end
            else
              energy << "0,"
            end
          end
        end
        energy = energy[0..-2]
        energy << "],\n\t\t\t\""
        if (os_version >= min_version_feature1) 
          prefix_str = OpenStudio::toUnderscoreCase("#{fuel_type}_#{category_str}")
          runner.registerValue("#{prefix_str}_si",fuel_and_category_aggregation,"J")
          runner.registerValue("#{prefix_str}_ip",OpenStudio::convert(fuel_and_category_aggregation,"J",unit_str).get,unit_str)
        end
        fuel_type_aggregation += fuel_and_category_aggregation
      end
      energy = energy[0..-7]
      energy << "\n\t\t}\n\t},\n"
      if (os_version >= min_version_feature1)
        runner.registerValue(OpenStudio::toUnderscoreCase("#{fuel_type}_si"),fuel_type_aggregation,"J")
        runner.registerValue(OpenStudio::toUnderscoreCase("#{fuel_type}_ip"),
                             OpenStudio::convert(fuel_type_aggregation,"J",unit_str).get,
                             unit_str)
      end
      site_energy_use += fuel_type_aggregation
    end
    energy = energy[0..-3]
    energy << "\n};\n"
    if (os_version >= min_version_feature1)
      runner.registerValue("site_energy_use_si",OpenStudio::convert(site_energy_use,"J","GJ").get,"GJ")
      runner.registerValue("site_energy_use_ip",OpenStudio::convert(site_energy_use,"J","MBtu").get,"MBtu")
    end

    # echo out our values
    #runner.registerInfo("This building is named #{building_name}.")

    # read in template
    html_in_path = "#{File.dirname(__FILE__)}/resources/report.html.in"
    if File.exist?(html_in_path)
        html_in_path = html_in_path
    else
        html_in_path = "#{File.dirname(__FILE__)}/report.html.in"
    end
    html_in = ""
    File.open(html_in_path, 'r') do |file|
      html_in = file.read
    end

    # configure template with variable values
    renderer = ERB.new(html_in)
    html_out = renderer.result(binding)

    # write html file
    html_out_path = "./report.html"
    File.open(html_out_path, 'w') do |file|
      file << html_out
      
      # make sure data is written to the disk one way or the other      
      begin
        file.fsync
      rescue
        file.flush
      end
    end

    #closing the sql file
    sqlFile.close()

    #reporting final condition
    runner.registerFinalCondition("Standard Report generated successfully.")

    return true

  end #end the run method

end #end the measure

#this allows the measure to be use by the application
StandardReports.new.registerWithApplication