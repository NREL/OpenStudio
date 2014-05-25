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

    os_version = OpenStudio::VersionString.new(OpenStudio::openStudioVersion())
    min_version_feature1 = OpenStudio::VersionString.new("1.2.2")
    
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
    if os_version >= min_version_feature1
      runner.registerValue("ashrae_max_nmbe",maxNMBE.get,"%")
    end

    maxCVRMSE = OpenStudio::Model::UtilityBill::maxCVRMSE(calibrationGuidelines[0]);
    energy << "var ashraeMaxCVRMSE = "
    if not maxCVRMSE.empty?
      energy << maxCVRMSE.get.to_s
    else
      energy << "N/A"
    end
    energy << ";\n"
    if os_version >= min_version_feature1
      runner.registerValue("ashrae_max_cvrmse",maxCVRMSE.get,"%")
    end    

    maxNMBE = OpenStudio::Model::UtilityBill::maxNMBE(calibrationGuidelines[1]);
    energy << "var fempMaxNMBE = "
    if not maxNMBE.empty?
      energy << maxNMBE.get.to_s
    else
      energy << "N/A"
    end
    energy << ";\n"
    if os_version >= min_version_feature1
      runner.registerValue("femp_max_nmbe",maxNMBE.get,"%")
    end    

    maxCVRMSE = OpenStudio::Model::UtilityBill::maxCVRMSE(calibrationGuidelines[1]);
    energy << "var fempMaxCVRMSE = "
    if not maxCVRMSE.empty?
      energy << maxCVRMSE.get.to_s
    else
      energy << "N/A"
    end
    energy << ";\n"
    if os_version >= min_version_feature1
      runner.registerValue("femp_max_cvrmse",maxCVRMSE.get,"%")
    end        

    energyElec = "\t\"Electricity Consumption\":{\n\t\t\"units\":\"kWh\",\n"
    energyDemand =  "\t\"Electricity Demand\":{\n\t\t\"units\":\"kW\",\n"
    energyGas = "\t\"Natural Gas Consumption\":{\n\t\t\"units\":\"therms\",\n"
    tempStartDate = ""
    tempEndDate = ""
    elecStartDate = "\t\t\t\t\"Start\":["
    elecEndDate = "\t\t\t\t\"End\":["
    gasStartDate = "\t\t\t\t\"Start\":["
    gasEndDate = "\t\t\t\t\"End\":["
    elecActualConsumption = "\t\t\t\t\"Actual\":["
    elecModelConsumption = "\t\t\t\t\"Model\":["
    actualPeakDemand = "\t\t\t\t\"Actual\":["
    modelPeakDemand = "\t\t\t\t\"Model\":[" 
    gasActualConsumption = "\t\t\t\t\"Actual\":["
    gasModelConsumption = "\t\t\t\t\"Model\":["
    elecNMBE = "\t\t\t\t\"NMBE\":["
    demandNMBE = "\t\t\t\t\"NMBE\":["
    gasNMBE = "\t\t\t\t\"NMBE\":["
    peakDemandUnitConversionFactor = 1.0
    consumptionUnitConversionFactor = 1.0
    hasElec = false
    hasDemand = false
    hasGas = false

    missingData = false
    # must have a runPeriod
    runPeriod = model.runPeriod
    if runPeriod.empty?
      missingData = true
      runner.registerWarning("Model has no run period and cannot generate all data.")
    end
    
    # must have a calendarYear to generate model data
    yearDescription = model.yearDescription
    if yearDescription.empty?
      missingData = true
      runner.registerWarning("Model has no year description and cannot generate all data.")
    end
    calendarYear = yearDescription.get.calendarYear
    if calendarYear.empty?
      missingData = true
      runner.registerWarning("Model has no calendar year and cannot generate all data.")
    end
    
    bill_index = 1
    model.getUtilityBills.each do |utilityBill|
    
      if os_version >= min_version_feature1
        runner.registerValue("utility_bill_#{bill_index}_name",utilityBill.name.get)
        runner.registerValue("utility_bill_#{bill_index}_fuel_type",utilityBill.fuelType.valueDescription)
      end

      cvrsme = 0.0
      if not utilityBill.CVRMSE.empty?
        cvrsme = utilityBill.CVRMSE.get
        if os_version >= min_version_feature1
          runner.registerValue("utility_bill_#{bill_index}_consumption_cvrmse",cvrsme,"%")
        end
        cvrsme =  sprintf "%.2f", cvrsme
      end

      nmbe = 0.0
      if not utilityBill.NMBE.empty?
        nmbe = utilityBill.NMBE.get
        if os_version >= min_version_feature1
          runner.registerValue("utility_bill_#{bill_index}_consumption_nmbe",nmbe,"%")
        end        
        nmbe = sprintf "%.2f", nmbe
      end

      string = ""
      string << "\t\t\"cvrsme\":\"" << cvrsme.to_s << "\",\n\t\t\"nmbe\":\"" << nmbe.to_s << "\",\n\t\t\t\"data\":{\n"

      hasDemandValues = false;
      if not utilityBill.peakDemandUnitConversionFactor.empty?
        hasDemandValues = true;
        energyElec << string
        energyDemand << string
        peakDemandUnitConversionFactor = utilityBill.peakDemandUnitConversionFactor.get
      else
        hasDemandValues = false;
        energyGas << string
      end

      consumptionUnitConversionFactor = utilityBill.consumptionUnitConversionFactor

      period_index = 1
      actual_consumption = 0.0
      modeled_consumption = 0.0
      actual_demand = 0.0
      modeled_demand = 0.0
      
      utilityBill.billingPeriods.each do |billingPeriod|
        tempStartDate = "\"" << billingPeriod.startDate.monthOfYear.value.to_s
        tempStartDate << "/"
        tempStartDate << billingPeriod.startDate.dayOfMonth.to_s << "\""
        tempEndDate =  "\"" <<billingPeriod.endDate.monthOfYear.value.to_s
        tempEndDate << "/"
        tempEndDate << billingPeriod.endDate.dayOfMonth.to_s << "\""
        if os_version >= min_version_feature1
          runner.registerValue("utility_bill_#{bill_index}_period_#{period_index}_start_date",billingPeriod.startDate.to_s)
          runner.registerValue("utility_bill_#{bill_index}_period_#{period_index}_end_date",billingPeriod.endDate.to_s)
        end

        if hasDemandValues
          elecStartDate << tempStartDate << ","
          elecEndDate << tempEndDate << ","

          consumption = billingPeriod.consumption
          if not consumption.empty?
            hasElec = true
            elecActualConsumption << consumption.get.to_s
            actual_consumption += consumption.get
            if os_version >= min_version_feature1
              runner.registerValue("utility_bill_#{bill_index}_period_#{period_index}_consumption_actual",
                                   consumption.get,
                                   utilityBill.consumptionUnit)
            end            
          else
            elecActualConsumption << "0"
          end
          elecActualConsumption << ","

          consumption = billingPeriod.modelConsumption
          if not consumption.empty?
            hasElec = true
            temp = consumption.get / consumptionUnitConversionFactor
            elecModelConsumption << temp.round.to_s
            modeled_consumption += temp
            if os_version >= min_version_feature1
              runner.registerValue("utility_bill_#{bill_index}_period_#{period_index}_consumption_modeled",
                                   temp,
                                   utilityBill.consumptionUnit)
            end              
          else
            elecModelConsumption << "0"
          end
          elecModelConsumption << ","
         
          peakDemand = billingPeriod.peakDemand
          if not peakDemand.empty?
            hasDemand = true
            actualPeakDemand << peakDemand.get.to_s
            if peakDemand.get > actual_demand
              actual_demand = peakDemand.get
            end
            if os_version >= min_version_feature1
              runner.registerValue("utility_bill_#{bill_index}_period_#{period_index}_peak_demand_actual",
                                   peakDemand.get,
                                   utilityBill.peakDemandUnit.get)
            end
          else
            actualPeakDemand << "0"
          end
          actualPeakDemand << ","

          peakDemand = billingPeriod.modelPeakDemand
          if not peakDemand.empty?
            hasDemand = true
            temp = peakDemand.get / 1000
            temp_str = sprintf "%.1f", temp
            modelPeakDemand << temp_str.to_s
            if temp > modeled_demand
              modeled_demand = temp
            end
            if os_version >= min_version_feature1
              runner.registerValue("utility_bill_#{bill_index}_period_#{period_index}_peak_demand_modeled",
                                   temp,
                                   utilityBill.peakDemandUnit.get)
            end            
          else
            modelPeakDemand << "0"
          end
          modelPeakDemand << ","

          if not billingPeriod.peakDemand.empty? and not billingPeriod.modelPeakDemand.empty? and not billingPeriod.peakDemand.get == 0
            percent = 100 * ((billingPeriod.modelPeakDemand.get / 1000) - billingPeriod.peakDemand.get) / billingPeriod.peakDemand.get
            percent_str = sprintf "%.2f", percent
            demandNMBE << percent_str.to_s
            if os_version >= min_version_feature1
              runner.registerValue("utility_bill_#{bill_index}_period_#{period_index}_peak_demand_nmbe",
                                   percent,
                                   "%")
            end            
          else
            demandNMBE << "0"
          end
          demandNMBE << ","
          
          if not billingPeriod.consumption.empty? and not billingPeriod.modelConsumption.empty? and not billingPeriod.consumption.get == 0
            percent = 100 * ((billingPeriod.modelConsumption.get / consumptionUnitConversionFactor) - billingPeriod.consumption.get) / billingPeriod.consumption.get
            percent_str = sprintf "%.2f", percent
            elecNMBE << percent_str.to_s
            if os_version >= min_version_feature1
              runner.registerValue("utility_bill_#{bill_index}_period_#{period_index}_consumption_nmbe",
                                   percent,
                                   "%")
            end                
          else
            elecNMBE << "0"
          end
          elecNMBE << ","

        else
          gasStartDate << tempStartDate << ","
          gasEndDate << tempEndDate << ","

          consumption = billingPeriod.consumption
          if not consumption.empty?
            hasGas = true
            gasActualConsumption << consumption.get.to_s
            actual_consumption += consumption.get
            if os_version >= min_version_feature1
              runner.registerValue("utility_bill_#{bill_index}_period_#{period_index}_consumption_actual",
                                   consumption.get,
                                   utilityBill.consumptionUnit)
            end
          else
            gasActualConsumption << "0"
          end
          gasActualConsumption << ","

          consumption = billingPeriod.modelConsumption
          if not consumption.empty?
            hasGas = true
            temp = consumption.get / consumptionUnitConversionFactor
            gasModelConsumption << temp.round.to_s
            modeled_consumption += temp
            if os_version >= min_version_feature1
              runner.registerValue("utility_bill_#{bill_index}_period_#{period_index}_consumption_modeled",
                                   temp,
                                   utilityBill.consumptionUnit)
            end             
          else
            gasModelConsumption << "0"
          end
          gasModelConsumption << ","

          if not billingPeriod.consumption.empty? and not billingPeriod.modelConsumption.empty? and not billingPeriod.consumption.get == 0
            percent = 100 * ((billingPeriod.modelConsumption.get / consumptionUnitConversionFactor) - billingPeriod.consumption.get) / billingPeriod.consumption.get
            percent_str = sprintf "%.2f", percent
            gasNMBE << percent_str.to_s
            if os_version >= min_version_feature1
              runner.registerValue("utility_bill_#{bill_index}_period_#{period_index}_consumption_nmbe",
                                   percent,
                                   "%")
            end
          else
            gasNMBE << "0"
          end
          gasNMBE << ","

        end
        period_index += 1
      end
      
      if os_version >= min_version_feature1
        if actual_consumption > 0.0
          runner.registerValue("utility_bill_#{bill_index}_consumption_actual",
                               actual_consumption,
                               utilityBill.consumptionUnit)
          runner.registerValue("utility_bill_#{bill_index}_consumption_modeled",
                               modeled_consumption,
                               utilityBill.consumptionUnit)
        end
        if actual_demand > 0.0
          runner.registerValue("utility_bill_#{bill_index}_peak_demand_actual",
                               actual_demand,
                               utilityBill.peakDemandUnit.get)
          runner.registerValue("utility_bill_#{bill_index}_peak_demand_modeled",
                               modeled_demand,
                               utilityBill.peakDemandUnit.get)
          runner.registerValue("utility_bill_#{bill_index}_peak_demand_nmbe",
                               100.0 * (modeled_demand - actual_demand) / actual_demand,
                               "%")
        end
      end
      
      bill_index += 1
    end

    elecStartDate = elecStartDate[0..-2]
    elecStartDate << "],\n"
    elecEndDate = elecEndDate[0..-2]
    elecEndDate << "],\n"
    elecActualConsumption = elecActualConsumption[0..-2]
    elecActualConsumption << "],\n"
    elecModelConsumption = elecModelConsumption[0..-2]
    elecModelConsumption << "],\n"
    actualPeakDemand = actualPeakDemand[0..-2]
    actualPeakDemand << "],\n"
    modelPeakDemand = modelPeakDemand[0..-2]
    modelPeakDemand << "],\n"
    elecNMBE = elecNMBE[0..-2]
    elecNMBE << "]\n"
    demandNMBE = demandNMBE[0..-2]
    demandNMBE << "]\n"
    energyElec << elecStartDate << elecEndDate << elecActualConsumption << elecModelConsumption << elecNMBE
    energyElec << "\t\t}\n" << "\t},\n"
    
    energyDemand << elecStartDate << elecEndDate << actualPeakDemand << modelPeakDemand << demandNMBE
    energyDemand << "\t\t}\n" << "\t},\n"

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
    energyGas << "\t\t}\n" << "\t},\n"

    
        energy  << "var consumption = {\n"

    
    if hasElec
        energy << energyElec
    end

    if hasDemand
        energy << energyDemand
    end
    
    if hasGas
        energy << energyGas
    end


        energy  << "};" 

    
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
    if missingData == true
        runner.registerFinalCondition("Calibration Report was not generated successfully.")
    else
        runner.registerFinalCondition("Calibration Report generated successfully.")
    end

    return true

  end #end the run method

end #end the measure

#this allows the measure to be use by the application
CalibrationReports.new.registerWithApplication