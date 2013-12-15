require 'erb'

#start the measure
class CalibrationReports < OpenStudio::Ruleset::ReportingUserScript

  #define the name that a user will see, this method may be deprecated as
  #the display name in PAT comes from the name field in measure.xml
  def name
    return "Calibration Reports"
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

    # put data into variables, these are available in the local scope binding
    #building_name = model.getBuilding.name.get

    energy = ""

    calibrationGuidelines = OpenStudio::Model::UtilityBill::calibrationGuidelines
    energy << "var calibrationGuidelines  = ["
    calibrationGuidelines.each do |calibrationGuideline|
      energy << "\"" << calibrationGuideline << "\","
    end
    energy = energy[0..-2]
    energy << "];\n"

    maxNMBE = OpenStudio::Model::UtilityBill::maxNMBE(calibrationGuidelines[0]);
    energy << "var ashraeMaxNMBE = "
    if not maxNMBE.empty?
      energy << maxNMBE.get.to_s
    else
      energy << "N/A"
    end
    energy << ";\n"

    maxCVRMSE = OpenStudio::Model::UtilityBill::maxCVRMSE(calibrationGuidelines[0]);
    energy << "var ashraeMaxCVRMSE = "
    if not maxCVRMSE.empty?
      energy << maxCVRMSE.get.to_s
    else
      energy << "N/A"
    end
    energy << ";\n"

    maxNMBE = OpenStudio::Model::UtilityBill::maxNMBE(calibrationGuidelines[1]);
    energy << "var fempMaxNMBE = "
    if not maxNMBE.empty?
      energy << maxNMBE.get.to_s
    else
      energy << "N/A"
    end
    energy << ";\n"

    maxCVRMSE = OpenStudio::Model::UtilityBill::maxCVRMSE(calibrationGuidelines[1]);
    energy << "var fempMaxCVRMSE = "
    if not maxCVRMSE.empty?
      energy << maxCVRMSE.get.to_s
    else
      energy << "N/A"
    end
    energy << ";\n"

    energyElec = "var consumption = {\n\t\"Electricity Consumption\":{\n\t\t\"units\":\"kWh\",\n\t\t\t\"data\":{\n"
    energyGas = "\t\"Natural Gas Consumption\":{\n\t\t\"units\":\"therms\",\n\t\t\t\"data\":{\n"
    tempStartDate = ""
    tempEndDate = ""
    elecStartDate = "\t\t\t\t\"Start\":["
    elecEndDate = "\t\t\t\t\"End\":["
    gasStartDate = "\t\t\t\t\"Start\":["
    gasEndDate = "\t\t\t\t\"End\":["
    actualPeakDemand = "\t\t\t\t\"Actual\":["
    modelPeakDemand = "\t\t\t\t\"Model\":["
    elecActualConsumption = "\t\t\t\t\"Actual\":["
    elecModelConsumption = "\t\t\t\t\"Model\":["
    gasActualConsumption = "\t\t\t\t\"Actual\":["
    gasModelConsumption = "\t\t\t\t\"Model\":["
    elecNMBE = "\t\t\t\t\"NMBE\":["
    gasNMBE = "\t\t\t\t\"NMBE\":["
    peakDemandUnitConversionFactor = 1.0
    consumptionUnitConversionFactor = 1.0

    model.getUtilityBills.each do |utilityBill|
      hasDemand = false;
      if not utilityBill.peakDemandUnitConversionFactor.empty?
        hasDemand = true;
        peakDemandUnitConversionFactor = utilityBill.peakDemandUnitConversionFactor.get
      else
        hasDemand = false;
      end

      consumptionUnitConversionFactor = utilityBill.consumptionUnitConversionFactor

      utilityBill.billingPeriods.each do |billingPeriod|
        tempStartDate = "\"" << billingPeriod.startDate.monthOfYear.value.to_s
        tempStartDate << "/"
        tempStartDate << billingPeriod.startDate.dayOfMonth.to_s << "\""
        tempEndDate =  "\"" <<billingPeriod.endDate.monthOfYear.value.to_s
        tempEndDate << "/"
        tempEndDate << billingPeriod.endDate.dayOfMonth.to_s << "\""

        if hasDemand
          elecStartDate << tempStartDate << ","
          elecEndDate << tempEndDate << ","

          consumption = billingPeriod.consumption
          if not consumption.empty?
            elecActualConsumption << consumption.get.to_s
          else
            elecActualConsumption << "0"
          end
          elecActualConsumption << ","

          consumption = billingPeriod.modelConsumption
          if not consumption.empty?
            temp = consumption.get / consumptionUnitConversionFactor
            elecModelConsumption << temp.round.to_s
          else
            elecModelConsumption << "0"
          end
          elecModelConsumption << ","

          peakDemand = billingPeriod.peakDemand
          if not peakDemand.empty?
            actualPeakDemand << peakDemand.get.to_s
          else
            actualPeakDemand << "0"
          end
          actualPeakDemand << ","

          peakDemand = billingPeriod.modelPeakDemand
          if not peakDemand.empty?
            modelPeakDemand << peakDemand.get.to_s
          else
            modelPeakDemand << "0"
          end
          modelPeakDemand << ","

          if not billingPeriod.consumption.empty? and not billingPeriod.modelConsumption.empty? # and not billingPeriod.consumption.get = 0
            percent = 100 * ((billingPeriod.modelConsumption.get / consumptionUnitConversionFactor) - billingPeriod.consumption.get) / billingPeriod.consumption.get
            percent = sprintf "%.2f", percent
            elecNMBE << percent.to_s
          else
            elecNMBE << "0"
          end
          elecNMBE << ","

        else
          gasStartDate << tempStartDate << ","
          gasEndDate << tempEndDate << ","

          consumption = billingPeriod.consumption
          if not consumption.empty?
            gasActualConsumption << consumption.get.to_s
          else
            gasActualConsumption << "0"
          end
          gasActualConsumption << ","

          consumption = billingPeriod.modelConsumption
          if not consumption.empty?
            temp = consumption.get / consumptionUnitConversionFactor
            gasModelConsumption << temp.round.to_s
          else
            gasModelConsumption << "0"
          end
          gasModelConsumption << ","

          if not billingPeriod.consumption.empty? and not billingPeriod.modelConsumption.empty? # and not billingPeriod.consumption.get = 0
            percent = 100 * ((billingPeriod.modelConsumption.get / consumptionUnitConversionFactor) - billingPeriod.consumption.get) / billingPeriod.consumption.get
            percent = sprintf "%.2f", percent
            gasNMBE << percent.to_s
          else
            gasNMBE << "0"
          end
          gasNMBE << ","

        end
      end
    end

    elecStartDate = elecStartDate[0..-2]
    elecStartDate << "],\n"
    elecEndDate = elecEndDate[0..-2]
    elecEndDate << "],\n"
    elecActualConsumption = elecActualConsumption[0..-2]
    elecActualConsumption << "],\n"
    elecModelConsumption = elecModelConsumption[0..-2]
    elecModelConsumption << "],\n"
    #actualPeakDemand = actualPeakDemand[0..-2]
    #actualPeakDemand << "],\n"
    #modelPeakDemand = modelPeakDemand[0..-2]
    #modelPeakDemand << "],\n"
    elecNMBE = elecNMBE[0..-2]
    elecNMBE << "]\n"
    energyElec << elecStartDate << elecEndDate << elecActualConsumption << elecModelConsumption << elecNMBE
    energyElec << "\t\t}\n" << "\t},\n"

    gasStartDate = gasStartDate[0..-2]
    gasStartDate << "],\n"
    gasEndDate = gasEndDate[0..-2]
    gasEndDate << "],\n"
    gasActualConsumption = gasActualConsumption[0..-2]
    gasActualConsumption << "],\n"
    gasModelConsumption = gasModelConsumption[0..-2]
    gasModelConsumption << "],\n"
    gasNMBE = gasNMBE[0..-2]
    gasNMBE << "]\n"
    energyGas << gasStartDate << gasEndDate << gasActualConsumption << gasModelConsumption << gasNMBE
    energyGas << "\t\t}\n" << "\t}\n" << "};"

    energy << energyElec << energyGas

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
CalibrationReports.new.registerWithApplication