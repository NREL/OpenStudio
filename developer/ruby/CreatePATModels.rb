######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

######################################################################
# == Synopsis 
#
#   Create osm files from the 16 new reference buildings. Add weather
#   and standards information.
#
# == Usage
#
#   ruby CreatePATModels.rb ARGV[0] ARGV[1]
#     
#     ARGV[0] - Path to reference buildings (folder containing 
#               FullSvcRest, Hospital, etc.) 
#     ARGV[1] - Path to PAT buildings folder.
#
######################################################################

require 'openstudio'
require 'set'

OpenStudio::Logger::instance.standardOutLogger.disable
logSink = OpenStudio::FileLogSink.new(OpenStudio::Path.new("./CreatePATModels.log"))
logSink.setLogLevel(-3)

buildings = []
buildings.push(["FullSvcRest","WoodFramingAndOther","MetalFramingWall","1"])
buildings.push(["Hospital","WoodFramingAndOther","Mass","5"])
buildings.push(["LgHotel","WoodFramingAndOther","Mass","6"])
buildings.push(["LgOff","WoodFramingAndOther","Mass","12"])
buildings.push(["MdOff","WoodFramingAndOther","MetalFramingWall","3"])
buildings.push(["MRapt","WoodFramingAndOther","MetalFramingWall","4"])
buildings.push(["OutP","WoodFramingAndOther","MetalFramingWall","3"])
buildings.push(["QkSvcRest","WoodFramingAndOther","WoodFramingandOtherWall","1"])
buildings.push(["Retail","WoodFramingAndOther","Mass","1"])
buildings.push(["Sch_pri","WoodFramingAndOther","MetalFramingWall","1"])
buildings.push(["Sch_sec","WoodFramingAndOther","MetalFramingWall","2"])
buildings.push(["SMarket","WoodFramingAndOther","Mass","1"])
buildings.push(["SmHotel","WoodFramingAndOther","MetalFramingWall","4"])
buildings.push(["SmOff","WoodFramingAndOther","Mass","1"])
buildings.push(["StMall","WoodFramingAndOther","MetalFramingWall","1"])
buildings.push(["Ware","MetalBuilding","MetalBuildingWall","1"])

cities = []
cities.push(["Albuquerque","4B","","NM"])
cities.push(["Atlanta","3A","","GA"])
cities.push(["Baltimore","4A","","MD"])
cities.push(["Boulder","5B","","CO"])
cities.push(["Chicago","5A","","IL"])
cities.push(["Duluth","7","","MN"])
cities.push(["Fairbanks","8","","AK"])
cities.push(["Helena","6B","","MT"])
cities.push(["Houston","2A","","TX"])
cities.push(["LasVegas","3B","","NV"])
cities.push(["LosAngeles","3B","6","CA"])
cities.push(["Miami","1A","","FL"])
cities.push(["Minneapolis","6A","","MN"])
cities.push(["Phoenix","2B","","AZ"])
cities.push(["SanFrancisco","3C","3","CA"])
cities.push(["Seattle","4C","","WA"])

refBldgsPath = OpenStudio::Path.new(ARGV[0])
patBldgsPath = OpenStudio::Path.new(ARGV[1])
puts "Reference buildings will be pulled from '" + refBldgsPath.to_s + "'."
puts "PAT buildings will be placed in '" + patBldgsPath.to_s + "'."

translator = OpenStudio::EnergyPlus::ReverseTranslator.new
untranslatedObjectTypes = Hash.new

buildingCount = 0
buildings.each{ |building|
  buildingCount += 1

  stubIdfPath = refBldgsPath / 
                OpenStudio::Path.new(building[0]) / 
                OpenStudio::Path.new("new_90.1-2004")

  cityCount = 0  
  cities.each{ |city|
    cityCount += 1
  
    puts "Processing " + building[0] + " in " + city[0] + "."
  
    osmPath = patBldgsPath / OpenStudio::Path.new(city[0])
    OpenStudio::create_directory(osmPath)
    
    
    # load and translate idf
    idfPath = stubIdfPath / OpenStudio::Path.new(city[0]) / OpenStudio::Path.new("in.idf")
    oModel = translator.loadModel(idfPath)
    raise "Unable to translate " + building[0] + " in " + city[0] + " to osm." if oModel.empty?
    model = oModel.get
    
    # compile information on untranslated objects
    if cityCount == 1
      translator.untranslatedIdfObjects.each { |object|
        currentList = untranslatedObjectTypes[object.iddObject.type.valueDescription]
        if not currentList
          untranslatedObjectTypes[object.iddObject.type.valueDescription] = 
              Array.new(buildings.size,false)
        end
        untranslatedObjectTypes[object.iddObject.type.valueDescription][buildingCount-1] = true
      }      
    end
    
    
    # create required unique object
    facility = model.getFacility
    
    
    # set climate zones, weather file, and time dependent valuation
    climateZones = model.getClimateZones
    climateZones.setClimateZone(OpenStudio::Model::ClimateZones.ashraeInstitutionName,
                                   city[1])
    climateZones.setActiveClimateZone(OpenStudio::Model::ClimateZones.ashraeInstitutionName,
                                      OpenStudio::Model::ClimateZones.ashraeDefaultYear)

    weatherFilePath = OpenStudio::Path.new(city[1] + "_USA_" + city[3] + "_" + city[0].upcase + "_TMY2.epw")
    if not city[2].empty?
      # ca-specific weather
      climateZones.appendClimateZone(OpenStudio::Model::ClimateZones.cecInstitutionName,
                                     city[2])
      climateZones.setActiveClimateZone(OpenStudio::Model::ClimateZones.cecInstitutionName,
                                        OpenStudio::Model::ClimateZones.cecDefaultYear)      
      weatherFilePath = OpenStudio::Path.new("CZ0" + city[2] + "RV2.epw")
      # tdv (ca-only)
      tdvFilePath = OpenStudio::Path.new("TDV_2008_kBtu_CZ0" + city[2] + ".csv")
      oTdvFile = OpenStudio::TimeDependentValuationFile::load(patBldgsPath / 
          OpenStudio::Path.new("../tdv/") / tdvFilePath)
      raise "Unable to load TDV file '" + tdvFilePath.to_s + "'." if oTdvFile.empty?
      tdvFile = oTdvFile.get
      tdv = OpenStudio::Model::TimeDependentValuation::setTimeDependentValuation(model,tdvFile)
      tdv.makeUrlRelative
    end
    weatherFile = OpenStudio::EpwFile.new(patBldgsPath / OpenStudio::Path.new("../weather/") / weatherFilePath)
    OpenStudio::Model::WeatherFile::setWeatherFile(model,weatherFile)
    weatherFile = model.getWeatherFile
    weatherFile.makeUrlRelative
    
    
    # set roof/ceiling type and cost
    wrappedModel = OpenStudio::StandardsInterface::OSStandardsInterface.new(model)
    roofResult = wrappedModel.getObjects("Roof")
    raise "Unable to retrieve roof objects from wrapped model." if not roofResult.good?
    roofObjects = roofResult.get
    roofConstructions = OpenStudio::Model::ModelObjectSet.new
    roofObjects.each { |roof| 
      roofConstructionType = building[1] + "Roof"
      ok = wrappedModel.setAttribute("Roof",roof,"RoofConstructionType",roofConstructionType)
      raise "Unable to set roof construction type because '" + ok.what + "'." if not ok.good?
      roofConstructions.insert(roof.to_Surface.get.construction.get)
    }
    ceilingResult = wrappedModel.getObjects("Ceiling")
    raise "Unable to retrieve ceiling objects from wrapped model." if not ceilingResult.good?
    ceilingObjects = ceilingResult.get
    ceilingObjects.each { |ceiling|
      # if exterior, set construction type and add to roofConstructions
      thermalTypeResult = wrappedModel.enumAttribute("Ceiling",ceiling,"PartitionThermalType")
      raise "Unable to extract ceiling partition thermal type because '" + stringResult.what + "'." if not thermalTypeResult.good?
      thermalType = thermalTypeResult.get
      if (thermalType == "Exterior")
        ok = wrappedModel.setAttribute("Ceiling",ceiling,"CeilingConstructionType",building[1] + "Ceiling")
        raise "Unable to set ceiling construction type because '" + ok.what + "'." if not ok.good?
        roofConstructions.insert(ceiling.to_Surface.get.construction.get)
      end
    }
    roofConstructions.each { |construction|
      lineItemCost = OpenStudio::Model::ComponentCost_LineItem.new(model)
      ok = lineItemCost.setName(construction.name.get + " Cost")
      raise "Unable to set line item cost name." if not ok
      lineItemCost.setLineItemType("Construction")
      lineItemCost.setItemName(construction.name.get)
      lineItemCost.setMaterialCostUnits("CostPerArea")
      lineItemCost.setMaterialCost(0.0)
      lineItemCost.setInstallationCostUnits("CostPerArea")
      lineItemCost.setInstallationCost(0.0)
      lineItemCost.setFixedOMCostUnits("CostPerArea")
      lineItemCost.setFixedOM(0.0)
      lineItemCost.setVariableOMCostUnits("CostPerArea")
      lineItemCost.setVariableOM(0.0)      
      lineItemCost.setSalvageCostUnits("CostPerArea")
      lineItemCost.setSalvageCost(0.0)
      lineItemCost.setExpectedLife(0)
    }   
    
    # set wall type and cost
    wallResult = wrappedModel.getObjects("Wall")
    raise "Unable to retrieve wall objects from wrapped model." if not wallResult.good?
    wallObjects = wallResult.get
    # filter out any internal walls
    exteriorWallObjects = OpenStudio::Model::ModelObjectVector.new
    wallObjects.each { |wall|
      thermalTypeResult = wrappedModel.enumAttribute("Wall",wall,"PartitionThermalType")
      raise "Unable to determine thermal type of " + wall.briefDescription + "." if not thermalTypeResult.good?
      thermalType = thermalTypeResult.get
      if (thermalType == "Exterior")
        exteriorWallObjects.push(wall)
      end
    }
    puts "Building has " + wallObjects.size.to_s + " walls, and " + exteriorWallObjects.size.to_s + " exterior walls."
    wallConstructions = OpenStudio::Model::ModelObjectSet.new
    exteriorWallObjects.each { |wall|
      wallConstructionType = building[2]  
      if (wallConstructionType == "Mass") 
        # "Mass" -> "MassLightWall" if construction.heatCapacity() < 7.0 Btu/ft^2*R
        # "Mass" -> "MassHeavyWall" if construction.heatCapacity() >= 15.0 Btu/ft^2*R
        # "Mass" -> "MassMediumWall" otherwise        
        heatCapacityUnitsInBtu = OpenStudio::BTUUnit.new(OpenStudio::BTUExpnt.new(1,-2,0,-1))
        lightMassCutoffInBtu = OpenStudio::Quantity.new(7.0,heatCapacityUnitsInBtu)
        lightMassCutoffInSI = OpenStudio::convert(lightMassCutoffInBtu,"SI".to_UnitSystem).get
        heavyMassCutoffInBtu = OpenStudio::Quantity.new(15.0,heatCapacityUnitsInBtu)
        heavyMassCutoffInSI = OpenStudio::convert(heavyMassCutoffInBtu,"SI".to_UnitSystem).get
          
        heatCapacityResult = wrappedModel.doubleAttribute("Wall",wall,"HeatCapacity")
        raise "Unable to determine heat capacity for " + wall.briefDescription + "." if not heatCapacityResult.good?
        heatCapacity = heatCapacityResult.get
        if (heatCapacity < lightMassCutoffInSI.value) 
          wallConstructionType = "MassLightWall"
          puts "Setting construction type of " + wall.briefDescription + 
               " to MassLightWall beacuse its heat capacity is " + heatCapacity.to_s + " < " + 
               lightMassCutoffInSI.to_s + "."
        elsif (heatCapacity >= heavyMassCutoffInSI.value) 
          wallConstructionType = "MassHeavyWall"
          puts "Setting construction type of " + wall.briefDescription + 
               " to MassHeavyWall because its heat capacity is " + heatCapacity.to_s + " >= " +
               heavyMassCutoffInSI.to_s + "."
        else
          wallConstructionType = "MassMediumWall"
          puts "Setting construction type of " + wall.briefDescription + 
         " to MassMediumWall because its heat capacity is " + lightMassCutoffInSI.to_s + 
         ">=" + heatCapacity.to_s + " < " + heavyMassCutoffInSI.to_s + "."
        end        
      end
      
      ok = wrappedModel.setAttribute("Wall",wall,"WallConstructionType",wallConstructionType)
      raise "Unable to set wall construction type because '" + ok.what + "'." if not ok.good?
      
      wallConstructions.insert(wall.to_Surface.get.construction.get)
    }
    wallConstructions.each { |construction|
      lineItemCost = OpenStudio::Model::ComponentCost_LineItem.new(model)
      ok = lineItemCost.setName(construction.name.get + " Cost")
      raise "Unable to set line item cost name." if not ok
      lineItemCost.setLineItemType("Construction")
      lineItemCost.setItemName(construction.name.get)
      lineItemCost.setMaterialCostUnits("CostPerArea")
      lineItemCost.setMaterialCost(0.0)
      lineItemCost.setInstallationCostUnits("CostPerArea")
      lineItemCost.setInstallationCost(0.0)
      lineItemCost.setFixedOMCostUnits("CostPerArea")
      lineItemCost.setFixedOM(0.0)
      lineItemCost.setVariableOMCostUnits("CostPerArea")
      lineItemCost.setVariableOM(0.0)      
      lineItemCost.setSalvageCostUnits("CostPerArea")
      lineItemCost.setSalvageCost(0.0)
      lineItemCost.setExpectedLife(0)
    }    
    
    
    # set number of above ground stories
    buildingResult = wrappedModel.getObjects("Building")
    raise "Unable to retrieve building objects from wrapped model." if not buildingResult.good?
    buildingObjects = buildingResult.get
    raise "Unexpected number of building objects." if not (buildingObjects.size == 1)
    buildingObject = buildingObjects[0]
    ok = wrappedModel.setAttribute("Building",buildingObject,"AboveGradeStoryCount",building[3].to_i)
    raise "Unable to set above grade story count because '" + ok.what + "'." if not ok.good?
    
    
    # set economic parameters
    lccParams = model.getLifeCycleCost_Parameters
    # lccParams.setDiscountingConvention("EndOfYear") # default = "EndOfYear"
    lccParams.setInflationApproach(OpenStudio::OptionalString.new("ConstantDollar")) # default = "ConstantDollar"
    lccParams.setRealDiscountRate(OpenStudio::OptionalDouble.new(0.023))
    # lccParams.setNominalDiscountRate(
    # lccParams.setInflation(
    # lccParams.setBaseDateMonth("January") # default = "January"
    # lccParams.setBaseDateYear(
    # lccParams.setServiceDateMonth("January") # default = "January"
    # lccParams.setServiceDateYear(
    lccParams.setLengthOfStudyPeriodInYears(OpenStudio::OptionalInt.new(30))
    # lccParams.setTaxRate(
    # lccParams.setDepreciationMethod("None") # default = "None"
    # lccParams.setOmInflation(
    # lccParams.setGasInflation(
    # lccParams.setElecInflation(
    # lccParams.setCoolInflation(
    # lccParams.setHeatInflation(
    
    
    osmPath = osmPath / OpenStudio::Path.new(building[0] + ".osm")
    ok = model.save(osmPath,true)    
    raise "Unable to save osm file '" + osmPath + "'." if not ok
  }

}

untranslatedObjectsCsv = OpenStudio::Table.new
row = OpenStudio::TableRow.new
row << OpenStudio::TableElement.new("")
buildings.each { |bldg|
  row << OpenStudio::TableElement.new(bldg[0])
}
untranslatedObjectsCsv << row
row = OpenStudio::TableRow.new
untranslatedObjectTypes.each { |objType|
  row << OpenStudio::TableElement.new(objType[0])
  objType[1].each { |bldgTF|
    if bldgTF
      row << OpenStudio::TableElement.new("x")
    else
      row << OpenStudio::TableElement.new("")
    end
  }
  untranslatedObjectsCsv << row
  row = OpenStudio::TableRow.new
}
untranslatedObjectsCsv.save(patBldgsPath / OpenStudio::Path.new("../../../../../doc/ReferenceBuildingUntranslatedObjectTypes.csv"))
