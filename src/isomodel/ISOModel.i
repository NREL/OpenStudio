#ifndef ISOMODEL_I
#define ISOMODEL_I

#ifdef SWIGPYTHON
%module openstudioisomodel
#endif

#define UTILITIES_API
#define ISOMODEL_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%{
  #include <isomodel/ForwardTranslator.hpp>
  #include <isomodel/UserModel.hpp>
  #include <isomodel/SimModel.hpp>

  using namespace openstudio::isomodel;
  using namespace openstudio;

  #include <model/Model.hpp>
  #include <utilities/data/EndUses.hpp>
%}

// #ifdef SWIGCSHARP
%rename(ISOModelForwardTranslator) openstudio::isomodel::ForwardTranslator;

// #endif

%ignore openstudio::isomodel::mult;

%rename("terrainClass=") openstudio::isomodel::UserModel::setTerrainClass(double value);
%rename("floorArea=") openstudio::isomodel::UserModel::setFloorArea(double value);
%rename("buildingHeight=") openstudio::isomodel::UserModel::setBuildingHeight(double value);
%rename("buildingOccupancyFrom=") openstudio::isomodel::UserModel::setBuildingOccupancyFrom(double value);
%rename("buildingOccupancyTo=") openstudio::isomodel::UserModel::setBuildingOccupancyTo(double value);
%rename("equivFullLoadOccupancyFrom=") openstudio::isomodel::UserModel::setEquivFullLoadOccupancyFrom(double value);
%rename("equivFullLoadOccupancyTo=") openstudio::isomodel::UserModel::setEquivFullLoadOccupancyTo(double value);
%rename("peopleDensityOccupied=") openstudio::isomodel::UserModel::setPeopleDensityOccupied(double value);
%rename("peopleDensityUnoccupied=") openstudio::isomodel::UserModel::setPeopleDensityUnoccupied(double value);
%rename("heatingOccupiedSetpoint=") openstudio::isomodel::UserModel::setHeatingOccupiedSetpoint(double value);
%rename("heatingUnoccupiedSetpoint=") openstudio::isomodel::UserModel::setHeatingUnoccupiedSetpoint(double value);
%rename("coolingOccupiedSetpoint=") openstudio::isomodel::UserModel::setCoolingOccupiedSetpoint(double value);
%rename("coolingUnoccupiedSetpoint=") openstudio::isomodel::UserModel::setCoolingUnoccupiedSetpoint(double value);
%rename("elecPowerAppliancesOccupied=") openstudio::isomodel::UserModel::setElecPowerAppliancesOccupied(double value);
%rename("elecPowerAppliancesUnoccupied=") openstudio::isomodel::UserModel::setElecPowerAppliancesUnoccupied(double value);
%rename("gasPowerAppliancesOccupied=") openstudio::isomodel::UserModel::setGasPowerAppliancesOccupied(double value);
%rename("gasPowerAppliancesUnoccupied=") openstudio::isomodel::UserModel::setGasPowerAppliancesUnoccupied(double value);
%rename("lightingPowerIntensityOccupied=") openstudio::isomodel::UserModel::setLightingPowerIntensityOccupied(double value);
%rename("lightingPowerIntensityUnoccupied=") openstudio::isomodel::UserModel::setLightingPowerIntensityUnoccupied(double value);
%rename("exteriorLightingPower=") openstudio::isomodel::UserModel::setExteriorLightingPower(double value);
%rename("daylightSensorSystem=") openstudio::isomodel::UserModel::setDaylightSensorSystem(double value);
%rename("lightingOccupancySensorSystem=") openstudio::isomodel::UserModel::setLightingOccupancySensorSystem(double value);
%rename("constantIlluminationControl=") openstudio::isomodel::UserModel::setConstantIlluminationControl(double value);
%rename("coolingSystemCOP=") openstudio::isomodel::UserModel::setCoolingSystemCOP(double value);
%rename("coolingSystemIPLV=") openstudio::isomodel::UserModel::setCoolingSystemIPLV(double value);
%rename("heatingEnergyCarrier=") openstudio::isomodel::UserModel::setHeatingEnergyCarrier(double value);
%rename("heatingSystemEfficiency=") openstudio::isomodel::UserModel::setHeatingSystemEfficiency(double value);
%rename("ventilationType=") openstudio::isomodel::UserModel::setVentilationType(double value);
%rename("freshAirFlowRate=") openstudio::isomodel::UserModel::setFreshAirFlowRate(double value);
%rename("supplyExhaustRate=") openstudio::isomodel::UserModel::setSupplyExhaustRate(double value);
%rename("heatRecovery=") openstudio::isomodel::UserModel::setHeatRecovery(double value);
%rename("exhaustAirRecirclation=") openstudio::isomodel::UserModel::setExhaustAirRecirclation(double value);
%rename("buildingAirLeakage=") openstudio::isomodel::UserModel::setBuildingAirLeakage(double value);
%rename("dhwDemand=") openstudio::isomodel::UserModel::setDhwDemand(double value);
%rename("dhwEfficiency=") openstudio::isomodel::UserModel::setDhwEfficiency(double value);
%rename("dhwDistributionSystem=") openstudio::isomodel::UserModel::setDhwDistributionSystem(double value);
%rename("dhwEnergyCarrier=") openstudio::isomodel::UserModel::setDhwEnergyCarrier(double value);
%rename("bemType=") openstudio::isomodel::UserModel::setBemType(double value);
%rename("interiorHeatCapacity=") openstudio::isomodel::UserModel::setInteriorHeatCapacity(double value);
%rename("specificFanPower=") openstudio::isomodel::UserModel::setSpecificFanPower(double value);
%rename("fanFlowControlFactor=") openstudio::isomodel::UserModel::setFanFlowControlFactor(double value);
%rename("roofSHGC=") openstudio::isomodel::UserModel::setRoofSHGC(double value);
%rename("wallAreaS=") openstudio::isomodel::UserModel::setWallAreaS(double value);
%rename("wallAreaSE=") openstudio::isomodel::UserModel::setWallAreaSE(double value);
%rename("wallAreaE=") openstudio::isomodel::UserModel::setWallAreaE(double value);
%rename("wallAreaNE=") openstudio::isomodel::UserModel::setWallAreaNE(double value);
%rename("wallAreaN=") openstudio::isomodel::UserModel::setWallAreaN(double value);
%rename("wallAreaNW=") openstudio::isomodel::UserModel::setWallAreaNW(double value);
%rename("wallAreaW=") openstudio::isomodel::UserModel::setWallAreaW(double value);
%rename("wallAreaSW=") openstudio::isomodel::UserModel::setWallAreaSW(double value);
%rename("roofArea=") openstudio::isomodel::UserModel::setRoofArea(double value);
%rename("wallUvalueS=") openstudio::isomodel::UserModel::setWallUvalueS(double value);
%rename("wallUvalueSE=") openstudio::isomodel::UserModel::setWallUvalueSE(double value);
%rename("wallUvalueE=") openstudio::isomodel::UserModel::setWallUvalueE(double value);
%rename("wallUvalueNE=") openstudio::isomodel::UserModel::setWallUvalueNE(double value);
%rename("wallUvalueN=") openstudio::isomodel::UserModel::setWallUvalueN(double value);
%rename("wallUvalueNW=") openstudio::isomodel::UserModel::setWallUvalueNW(double value);
%rename("wallUvalueW=") openstudio::isomodel::UserModel::setWallUvalueW(double value);
%rename("wallUvalueSW=") openstudio::isomodel::UserModel::setWallUvalueSW(double value);
%rename("roofUValue=") openstudio::isomodel::UserModel::setRoofUValue(double value);
%rename("wallSolarAbsorptionS=") openstudio::isomodel::UserModel::setWallSolarAbsorptionS(double value);
%rename("wallSolarAbsorptionSE=") openstudio::isomodel::UserModel::setWallSolarAbsorptionSE(double value);
%rename("wallSolarAbsorptionE=") openstudio::isomodel::UserModel::setWallSolarAbsorptionE(double value);
%rename("wallSolarAbsorptionNE=") openstudio::isomodel::UserModel::setWallSolarAbsorptionNE(double value);
%rename("wallSolarAbsorptionN=") openstudio::isomodel::UserModel::setWallSolarAbsorptionN(double value);
%rename("wallSolarAbsorptionNW=") openstudio::isomodel::UserModel::setWallSolarAbsorptionNW(double value);
%rename("wallSolarAbsorptionW=") openstudio::isomodel::UserModel::setWallSolarAbsorptionW(double value);
%rename("wallSolarAbsorptionSW=") openstudio::isomodel::UserModel::setWallSolarAbsorptionSW(double value);
%rename("roofSolarAbsorption=") openstudio::isomodel::UserModel::setRoofSolarAbsorption(double value);
%rename("wallThermalEmissivityS=") openstudio::isomodel::UserModel::setWallThermalEmissivityS(double value);
%rename("wallThermalEmissivitySE=") openstudio::isomodel::UserModel::setWallThermalEmissivitySE(double value);
%rename("wallThermalEmissivityE=") openstudio::isomodel::UserModel::setWallThermalEmissivityE(double value);
%rename("wallThermalEmissivityNE=") openstudio::isomodel::UserModel::setWallThermalEmissivityNE(double value);
%rename("wallThermalEmissivityN=") openstudio::isomodel::UserModel::setWallThermalEmissivityN(double value);
%rename("wallThermalEmissivityNW=") openstudio::isomodel::UserModel::setWallThermalEmissivityNW(double value);
%rename("wallThermalEmissivityW=") openstudio::isomodel::UserModel::setWallThermalEmissivityW(double value);
%rename("wallThermalEmissivitySW=") openstudio::isomodel::UserModel::setWallThermalEmissivitySW(double value);
%rename("roofThermalEmissivity=") openstudio::isomodel::UserModel::setRoofThermalEmissivity(double value);
%rename("windowAreaS=") openstudio::isomodel::UserModel::setWindowAreaS(double value);
%rename("windowAreaSE=") openstudio::isomodel::UserModel::setWindowAreaSE(double value);
%rename("windowAreaE=") openstudio::isomodel::UserModel::setWindowAreaE(double value);
%rename("windowAreaNE=") openstudio::isomodel::UserModel::setWindowAreaNE(double value);
%rename("windowAreaN=") openstudio::isomodel::UserModel::setWindowAreaN(double value);
%rename("windowAreaNW=") openstudio::isomodel::UserModel::setWindowAreaNW(double value);
%rename("windowAreaW=") openstudio::isomodel::UserModel::setWindowAreaW(double value);
%rename("windowAreaSW=") openstudio::isomodel::UserModel::setWindowAreaSW(double value);
%rename("skylightArea=") openstudio::isomodel::UserModel::setSkylightArea(double value);
%rename("windowUvalueS=") openstudio::isomodel::UserModel::setWindowUvalueS(double value);
%rename("windowUvalueSE=") openstudio::isomodel::UserModel::setWindowUvalueSE(double value);
%rename("windowUvalueE=") openstudio::isomodel::UserModel::setWindowUvalueE(double value);
%rename("windowUvalueNE=") openstudio::isomodel::UserModel::setWindowUvalueNE(double value);
%rename("windowUvalueN=") openstudio::isomodel::UserModel::setWindowUvalueN(double value);
%rename("windowUvalueNW=") openstudio::isomodel::UserModel::setWindowUvalueNW(double value);
%rename("windowUvalueW=") openstudio::isomodel::UserModel::setWindowUvalueW(double value);
%rename("windowUvalueSW=") openstudio::isomodel::UserModel::setWindowUvalueSW(double value);
%rename("skylightUvalue=") openstudio::isomodel::UserModel::setSkylightUvalue(double value);
%rename("windowSHGCS=") openstudio::isomodel::UserModel::setWindowSHGCS(double value);
%rename("windowSHGCSE=") openstudio::isomodel::UserModel::setWindowSHGCSE(double value);
%rename("windowSHGCE=") openstudio::isomodel::UserModel::setWindowSHGCE(double value);
%rename("windowSHGCNE=") openstudio::isomodel::UserModel::setWindowSHGCNE(double value);
%rename("windowSHGCN=") openstudio::isomodel::UserModel::setWindowSHGCN(double value);
%rename("windowSHGCNW=") openstudio::isomodel::UserModel::setWindowSHGCNW(double value);
%rename("windowSHGCW=") openstudio::isomodel::UserModel::setWindowSHGCW(double value);
%rename("windowSHGCSW=") openstudio::isomodel::UserModel::setWindowSHGCSW(double value);
%rename("skylightSHGC=") openstudio::isomodel::UserModel::setSkylightSHGC(double value);
%rename("windowSCFS=") openstudio::isomodel::UserModel::setWindowSCFS(double value);
%rename("windowSCFSE=") openstudio::isomodel::UserModel::setWindowSCFSE(double value);
%rename("windowSCFE=") openstudio::isomodel::UserModel::setWindowSCFE(double value);
%rename("windowSCFNE=") openstudio::isomodel::UserModel::setWindowSCFNE(double value);
%rename("windowSCFN=") openstudio::isomodel::UserModel::setWindowSCFN(double value);
%rename("windowSCFNW=") openstudio::isomodel::UserModel::setWindowSCFNW(double value);
%rename("windowSCFW=") openstudio::isomodel::UserModel::setWindowSCFW(double value);
%rename("windowSCFSW=") openstudio::isomodel::UserModel::setWindowSCFSW(double value);
%rename("windowSDFS=") openstudio::isomodel::UserModel::setWindowSDFS(double value);
%rename("windowSDFSE=") openstudio::isomodel::UserModel::setWindowSDFSE(double value);
%rename("windowSDFE=") openstudio::isomodel::UserModel::setWindowSDFE(double value);
%rename("windowSDFNE=") openstudio::isomodel::UserModel::setWindowSDFNE(double value);
%rename("windowSDFN=") openstudio::isomodel::UserModel::setWindowSDFN(double value);
%rename("windowSDFNW=") openstudio::isomodel::UserModel::setWindowSDFNW(double value);
%rename("windowSDFW=") openstudio::isomodel::UserModel::setWindowSDFW(double value);
%rename("windowSDFSW=") openstudio::isomodel::UserModel::setWindowSDFSW(double value);
%rename("exteriorHeatCapacity=") openstudio::isomodel::UserModel::setExteriorHeatCapacity(double value);
%rename("infiltration=") openstudio::isomodel::UserModel::setInfiltration(double value);
%rename("hvacWasteFactor=") openstudio::isomodel::UserModel::setHvacWasteFactor(double value);
%rename("hvacHeatingLossFactor=") openstudio::isomodel::UserModel::setHvacHeatingLossFactor(double value);
%rename("hvacCoolingLossFactor=") openstudio::isomodel::UserModel::setHvacCoolingLossFactor(double value);
%rename("dhwDistributionEfficiency=") openstudio::isomodel::UserModel::setDhwDistributionEfficiency(double value);
%rename("heatingPumpControl=") openstudio::isomodel::UserModel::setHeatingPumpControl(double value);
%rename("coolingPumpControl=") openstudio::isomodel::UserModel::setCoolingPumpControl(double value);
%rename("heatGainPerPerson=") openstudio::isomodel::UserModel::setHeatGainPerPerson(double value);

%rename("weatherFilePath=") openstudio::isomodel::UserModel::setWeatherFilePath(std::string value);

%include <isomodel/SimModel.hpp>
%include <isomodel/UserModel.hpp>
%include <isomodel/ForwardTranslator.hpp>
#endif //ISOMODEL_I
