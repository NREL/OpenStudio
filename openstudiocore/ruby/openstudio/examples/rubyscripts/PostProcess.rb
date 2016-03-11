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

logger = OpenStudio::Logger::instance().standardErrLogger
logger.enable
logger.setLogLevel(-2)

model_path = OpenStudio::Path.new(options[:modelPath])
model = OpenStudio::Model::Model::load(model_path)

temp = OpenStudio::AttributeVector.new
temp2 = OpenStudio::AttributeVector.new
report_vector = OpenStudio::AttributeVector.new

if model.empty?

  report_vector << OpenStudio::Attribute.new("error", "No in.osm file")
  
else

  model = model.get
  
  # see if we can find sql file
  sql_path = OpenStudio::Path.new(options[:sqlPath])
  if OpenStudio::exists(sql_path)

    # translate the results and load them into the model
    sqlFile = OpenStudio::SqlFile.new(sql_path)
    model.setSqlFile(sqlFile)
    
  end

  # get path to this file
  report_vector << OpenStudio::Attribute.new("path", File.expand_path(__FILE__))
  
  # get wall insulation object
  wall_insulation_object = model.getObjectByTypeAndName("OS_Material".to_IddObjectType, 
                                                        "Insulation board")
  if wall_insulation_object.empty?
    # baseline -- try to find original insulation layer
    model.getSurfaces.each { |surface|
      if (surface.surfaceType.downcase == "wall") and 
         (surface.outsideBoundaryCondition.downcase == "outdoors")
         
        construction = surface.construction
        if not construction.empty?
          construction = construction.get.to_LayeredConstruction
          if not construction.empty? 
            construction = construction.get
            wall_insulation_object = construction.insulation
            if wall_insulation_object.empty?
              # look for "insulation in name
              construction.layers.each { |layer|
                layerName = layer.name.get
                if layerName.match(/[iI]nsulation/)
                  wall_insulation_object = OpenStudio::Model::OptionalMaterial.new(layer)
                  break
                end
              }              
            end
          end        
        end
        
        if not wall_insulation_object.empty?
          break
        end
         
      end
    }    
  end
  if not wall_insulation_object.empty?
    wall_insulation_object = wall_insulation_object.get.to_StandardOpaqueMaterial.get
    report_vector << OpenStudio::Attribute.new("wallInsulationThickness", 
                                               wall_insulation_object.thickness)
  end
  
  # get window object
  window_object = model.getObjectByTypeAndName("OS_WindowMaterial_SimpleGlazingSystem".to_IddObjectType, 
                                               "WindowMaterial")
  uFactor = nil
  if window_object.empty?
    # baseline -- try to find original fenestration
    model.getSubSurfaces.each { |subSurface|
      
      if subSurface.subSurfaceType.downcase.match(/window/) and (subSurface.outsideBoundaryCondition.downcase == "outdoors")
         
        puts "Found Window '" + subSurface.name.get + "'."         
         
        window_object = subSurface
        od = window_object.uFactor
        if not od.empty?
          puts "Window has U-Factor " + od.get.to_s
          uFactor = od.get
          break
        end
      
      end
    }
  else
    window_object = window_object.get.to_SimpleGlazing.get  
    uFactor = window_object.uFactor
  end
  if uFactor
    report_vector << OpenStudio::Attribute.new("windowUValue", uFactor)
  end
  
  # get hierarchical zone reports
  temp.clear
  model.getThermalZones.each do |zone|
    attribute = zone.getAttribute("name")
    temp << attribute.get if not attribute.empty?
  end
  report_vector << OpenStudio::Attribute.new("zones", temp)
   
  # see if we can find sql file  
  if not model.sqlFile.empty?

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
    
    cost_estimate = model.sqlFile().get().execAndReturnFirstDouble("SELECT Value FROM tabulardatawithstrings WHERE ReportName='Construction Cost Estimate Summary' AND ReportForString='Entire Facility' AND TableName='Construction Cost Estimate Summary' AND RowName='Cost Estimate Total (~~$~~)'  AND ColumnName='Current Bldg. Model'")
    if not cost_estimate.empty?
      report_vector << OpenStudio::Attribute.new("costEstimate", cost_estimate.get.to_f)
    end
    
    # end use table
    endUses = facility.endUses
    if not endUses.empty?
      report_vector << endUses.get.attribute
    end
      
  end
  
end

report = OpenStudio::Attribute.new("Report", report_vector)

report.saveToXml(OpenStudio::Path.new("report.xml"))
