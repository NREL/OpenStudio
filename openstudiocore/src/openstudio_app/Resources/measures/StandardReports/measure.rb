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

    # Conversion base : 1 GJ = 0.94708628903179 MMBtu
    convertToBtu = 0.94708628903179 / 1000000000

    # Conversion base : 1 GJ = 277.77777777778 kWh
    convertToKwh = 277.77777777778 / 1000000000

    conversionFactor = 1.0

    # put data into variables, these are available in the local scope binding
    #building_name = model.getBuilding.name.get

    web_asset_path = OpenStudio::getSharedResourcesPath() / OpenStudio::Path.new("web_assets")

    energy = "var consumption = {\n"
    fuel_type = ""
    units = ""

    OpenStudio::EndUseFuelType::getValues.each do |fuel_type|
      energy << "\t\""
      fuel_type = OpenStudio::EndUseFuelType.new(fuel_type).valueDescription
      energy << OpenStudio::EndUseFuelType.new(fuel_type).valueDescription # append this to remove whitespace between words ".delete(' ')"
      energy << " Consumption\":{\n\t\t\"units\":"
      if fuel_type == "Electricity"
        units = "\"kWh\""
        conversionFactor = convertToKwh
      else
        units = "\"Million Btu\""
        conversionFactor = convertToBtu
      end
      energy << units
      energy << ",\n\t\t\"data\":{\n\t\t\t\""
      OpenStudio::EndUseCategoryType::getValues.each do |category_type|
        energy << OpenStudio::EndUseCategoryType.new(category_type).valueDescription # append this to remove whitespace between words ".delete(' ')"
        energy << "\":["
        OpenStudio::MonthOfYear::getValues.each do |month|
          if month >= 1 and month <= 12
            if not sqlFile.energyConsumptionByMonth(OpenStudio::EndUseFuelType.new(fuel_type),OpenStudio::EndUseCategoryType.new(category_type),OpenStudio::MonthOfYear.new(month)).empty?
              temp = sqlFile.energyConsumptionByMonth(OpenStudio::EndUseFuelType.new(fuel_type),OpenStudio::EndUseCategoryType.new(category_type),OpenStudio::MonthOfYear.new(month)).get
              temp = temp * conversionFactor
              temp = sprintf "%.3f", temp
              energy << temp.to_s
              energy << ","
            else
              energy << "0,"
            end
          end
        end
        energy = energy[0..-2]
        energy << "],\n\t\t\t\""
      end
      energy = energy[0..-7]
      energy << "\n\t\t}\n\t},\n"
    end
    energy = energy[0..-3]
    energy << "\n};\n"

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
    runner.registerFinalCondition("Goodbye.")

    return true

  end #end the run method

end #end the measure

#this allows the measure to be use by the application
StandardReports.new.registerWithApplication