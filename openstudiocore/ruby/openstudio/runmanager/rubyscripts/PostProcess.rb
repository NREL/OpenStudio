require 'openstudio'
require 'optparse'

options = Hash.new
optparse = OptionParser.new do|opts|

  opts.on('--modelPath MODELPATH', String, "Path to EnergyPlus Idf or OpenStudio Model" ) do |modelPath|
    options[:modelPath] = modelPath
  end
  
  opts.on('--sqlPath SQLPATH', String, "SQLite output file" ) do |sqlPath|
    options[:sqlPath] = sqlPath
  end
  
end

optparse.parse!

if not options[:modelPath]
  options[:modelPath] = "in.osm"
end

if not options[:sqlPath]
  options[:sqlPath] = "eplusout.sql"
end

model_path = OpenStudio::Path.new(options[:modelPath])
idf_file = OpenStudio::IdfFile::load(model_path)

temp = OpenStudio::AttributeVector.new
temp2 = OpenStudio::AttributeVector.new
report_vector = OpenStudio::AttributeVector.new

if idf_file.empty?

  report_vector << OpenStudio::Attribute.new("error", "No in.osm file")
  
else

  model = nil
  if idf_file.get.iddFileType == "EnergyPlus".to_IddFileType
    workspace = OpenStudio::Workspace.new(idf_file.get)
    translator = OpenStudio::EnergyPlus::ReverseTranslator.new()
    model = translator.translateWorkspace(workspace)
  else
    model = OpenStudio::Model::Model.new(idf_file.get)
  end
  
  # get name as an attribute
  building = model.getBuilding
  attribute = building.getAttribute("name")
  report_vector << attribute.get if not attribute.empty?
  
  # can add other measures as well
  report_vector << OpenStudio::Attribute.new("numberOfZones", model.getThermalZones.size.to_i)
  
  # can create hierarchical reports
  temp.clear
  model.getThermalZones.each do |zone|
    attribute = zone.getAttribute("name")
    temp << attribute.get if not attribute.empty?
  end
  report_vector << OpenStudio::Attribute.new("zones", temp)
   
  # see if we can find sql file
  sql_path = OpenStudio::Path.new(options[:sqlPath])
  if OpenStudio::exists(sql_path)

    # translate the results and load them into the model
    sqlFile = OpenStudio::SqlFile.new(sql_path)
    model.setSqlFile(sqlFile)

    # get results as attributes
    facility = model.getFacility

    attribute = facility.getAttribute("totalSiteEnergy")
    report_vector << attribute.get if not attribute.empty?
    
    attribute = facility.getAttribute("netSiteEnergy")
    report_vector << attribute.get if not attribute.empty?
    
    attribute = facility.getAttribute("totalSourceEnergy")
    report_vector << attribute.get if not attribute.empty?
    
    attribute = facility.getAttribute("netSourceEnergy")
    report_vector << attribute.get if not attribute.empty?
    
    attribute = facility.getAttribute("annualTotalUtilityCost")
    report_vector << attribute.get if not attribute.empty?
    
    attribute = facility.getAttribute("annualElectricTotalCost")
    report_vector << attribute.get if not attribute.empty?
    
    attribute = facility.getAttribute("annualGasTotalCost")
    report_vector << attribute.get if not attribute.empty?
    
    attribute = facility.getAttribute("annualDistrictCoolingTotalCost")
    report_vector << attribute.get if not attribute.empty?
    
    attribute = facility.getAttribute("annualDistrictHeatingTotalCost")
    report_vector << attribute.get if not attribute.empty?
    
    attribute = facility.getAttribute("annualWaterTotalCost")
    report_vector << attribute.get if not attribute.empty?
    
    # can get custom results 
    hours_simulated = model.sqlFile().get().execAndReturnFirstDouble("SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' AND ReportForString='Entire Facility' AND TableName='General' AND RowName='Hours Simulated' AND ColumnName='Value' AND Units='hrs'")
    if not hours_simulated.empty?
      report_vector << OpenStudio::Attribute.new("hoursSimulated", hours_simulated.get.to_f)
    end
    
    # end use table
    fuel_types = [['Electricity', 'GJ'], ['Natural Gas', 'GJ'], ['Other Fuel', 'GJ'], ['District Cooling', 'GJ'], ['District Heating', 'GJ'], ['Water', 'm3']]
    use_types = ['Heating', 'Cooling', 'Interior Lighting', 'Exterior Lighting', 'Interior Equipment', 'Exterior Equipment', 'Fans', 'Pumps', 'Heat Rejection', 
                 'Humidification', 'Heat Recovery', 'Water Systems', 'Refrigeration', 'Generators']
    temp.clear
    use_types.each do |use_type|
      temp2.clear
      fuel_types.each do |fuel_type|
        fuel_name = fuel_type[0]
        fuel_units = fuel_type[1]
        value = model.sqlFile().get().execAndReturnFirstDouble("SELECT Value FROM tabulardatawithstrings WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' AND ReportForString='Entire Facility' AND TableName='End Uses' AND RowName='#{use_type}' AND ColumnName='#{fuel_name}' AND Units='#{fuel_units}'")
        if not value.empty? or value.get == 0.0
          temp2 << OpenStudio::Attribute.new(fuel_name, value.get.to_f, fuel_units)
        end
      end
      temp << OpenStudio::Attribute.new(use_type, temp2)
    end
    report_vector << OpenStudio::Attribute.new("EndUses", temp)
      
  end
  
end

report = OpenStudio::Attribute.new("Report", report_vector)

report.saveToXml(OpenStudio::Path.new("report.xml"))
