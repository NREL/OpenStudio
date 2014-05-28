/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef ISOMODEL_USERMODEL_HPP
#define ISOMODEL_USERMODEL_HPP

#include "ISOModelAPI.hpp"
#include "EpwData.hpp"
#include "SimModel.hpp"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include "../utilities/core/Logger.hpp"

namespace openstudio {

namespace isomodel {
  
  class SimModel;
  class WeatherData;

  class ISOMODEL_API UserModel {
  public:
    /**
     * Loads the specified weather data from disk.
     * Exposed to allow for separate loading from Ruby Scripts
     * Call setWeatherFilePath(path) then loadWeather() to update
     * the UserModel with a new set of weather data
     */
    std::shared_ptr<WeatherData> loadWeather();

    /**
     * Loads an ISO model from the specified .ISO file
     */
    void load(const openstudio::path &t_buildingFile);

    /** 
     * Saves an ISOModel to the specified path
     */
    void save(const openstudio::path &t_buildingFile) const;

    /**
     * Generates a SimModel from the specified parameters of the 
     * UserModel
     */  
    SimModel toSimModel();

    /**
     * Indicates whether or not the user model loaded in correctly
     * If either the ISO file or the Weather File cannot be found
     * valid will be false
     * userModel.load(<filename>)
     * if(userModel.valid()){
     *     userModel.toSimModel().simulate();
     * }
     */
    bool valid() const {return _valid;}
    openstudio::path weatherFilePath() const {return _weatherFilePath;}
    double terrainClass() const {return _terrainClass;} 
    double floorArea() const {return _floorArea;} 
    double buildingHeight() const {return _buildingHeight;} 
    double buildingOccupancyFrom() const {return _buildingOccupancyFrom;} 
    double buildingOccupancyTo() const {return _buildingOccupancyTo;} 
    double equivFullLoadOccupancyFrom() const {return _equivFullLoadOccupancyFrom;} 
    double equivFullLoadOccupancyTo() const {return _equivFullLoadOccupancyTo;} 
    double peopleDensityOccupied() const {return _peopleDensityOccupied;} 
    double peopleDensityUnoccupied() const {return _peopleDensityUnoccupied;} 
    double heatingOccupiedSetpoint() const {return _heatingOccupiedSetpoint;} 
    double heatingUnoccupiedSetpoint() const {return _heatingUnoccupiedSetpoint;} 
    double coolingOccupiedSetpoint() const {return _coolingOccupiedSetpoint;} 
    double coolingUnoccupiedSetpoint() const {return _coolingUnoccupiedSetpoint;} 
    double elecPowerAppliancesOccupied() const {return _elecPowerAppliancesOccupied;}
    double elecPowerAppliancesUnoccupied() const {return _elecPowerAppliancesUnoccupied;}
    double gasPowerAppliancesOccupied() const {return _gasPowerAppliancesOccupied;} 
    double gasPowerAppliancesUnoccupied() const {return _gasPowerAppliancesUnoccupied;}
    double lightingPowerIntensityOccupied() const {return _lightingPowerIntensityOccupied;}
    double lightingPowerIntensityUnoccupied() const {return _lightingPowerIntensityUnoccupied;}
    double exteriorLightingPower() const {return _exteriorLightingPower;}
    double daylightSensorSystem() const {return _daylightSensorSystem;}
    double lightingOccupancySensorSystem() const {return _lightingOccupancySensorSystem;} 
    double constantIlluminationControl() const {return _constantIlluminationControl;} 
    double coolingSystemCOP() const {return _coolingSystemCOP;} 
    double coolingSystemIPLVToCOPRatio() const {return _coolingSystemIPLVToCOPRatio;} 
    double heatingEnergyCarrier() const {return _heatingEnergyCarrier;} 
    double heatingSystemEfficiency() const {return _heatingSystemEfficiency;}
    double ventilationType() const {return _ventilationType;} 
    double freshAirFlowRate() const {return _freshAirFlowRate;} 
    double supplyExhaustRate() const {return _supplyExhaustRate;} 
    double heatRecovery() const {return _heatRecovery;} 
    double exhaustAirRecirculation() const {return _exhaustAirRecirculation;} 
    double buildingAirLeakage() const {return _buildingAirLeakage;} 
    double dhwDemand() const {return _dhwDemand;} 
    double dhwEfficiency() const {return _dhwEfficiency;} 
    double dhwDistributionSystem() const {return _dhwDistributionSystem;} 
    double dhwEnergyCarrier() const {return _dhwEnergyCarrier;} 
    double bemType() const {return _bemType;} 
    double interiorHeatCapacity() const {return _interiorHeatCapacity;} 
    double specificFanPower() const {return _specificFanPower;}
    double fanFlowControlFactor() const {return _fanFlowControlFactor;} 

    double roofUValue() const {return _roofUValue;}
    double roofSHGC() const {return _roofSHGC;}
    double wallUvalueS() const {return _wallUvalueS;} 
    double wallUvalueSE() const {return _wallUvalueSE;} 
    double wallUvalueE() const {return _wallUvalueE;} 
    double wallUvalueNE() const {return _wallUvalueNE;} 
    double wallUvalueN() const {return _wallUvalueN;} 
    double wallUvalueNW() const {return _wallUvalueNW;}
    double wallUvalueW() const {return _wallUvalueW;}
    double wallUvalueSW() const {return _wallUvalueSW;}
    double wallSolarAbsorptionS() const {return _wallSolarAbsorptionS;} 
    double wallSolarAbsorptionSE() const {return _wallSolarAbsorptionSE;}
    double wallSolarAbsorptionE() const {return _wallSolarAbsorptionE;} 
    double wallSolarAbsorptionNE() const {return _wallSolarAbsorptionNE;}
    double wallSolarAbsorptionN() const {return _wallSolarAbsorptionN;}
    double wallSolarAbsorptionNW() const {return _wallSolarAbsorptionNW;}
    double wallSolarAbsorptionW() const {return _wallSolarAbsorptionW;}
    double wallSolarAbsorptionSW() const {return _wallSolarAbsorptionSW;}
    double wallThermalEmissivityS() const {return _wallThermalEmissivityS;} 
    double wallThermalEmissivitySE() const {return _wallThermalEmissivitySE;} 
    double wallThermalEmissivityE() const {return _wallThermalEmissivityE;} 
    double wallThermalEmissivityNE() const {return _wallThermalEmissivityNE;}
    double wallThermalEmissivityN() const {return _wallThermalEmissivityN;}
    double wallThermalEmissivityNW() const {return _wallThermalEmissivityNW;}
    double wallThermalEmissivityW() const {return _wallThermalEmissivityW;}
    double wallThermalEmissivitySW() const {return _wallThermalEmissivitySW;} 
    double windowUvalueS() const {return _windowUvalueS;} 
    double windowUvalueSE() const {return _windowUvalueSE;}
    double windowUvalueE() const {return _windowUvalueE;}
    double windowUvalueNE() const {return _windowUvalueNE;}
    double windowUvalueN() const {return _windowUvalueN;} 
    double windowUvalueNW() const {return _windowUvalueNW;}
    double windowUvalueW() const {return _windowUvalueW;} 
    double windowUvalueSW() const {return _windowUvalueSW;}
    double windowSHGCS() const {return _windowSHGCS;} 
    double windowSHGCSE() const {return _windowSHGCSE;}
    double windowSHGCE() const {return _windowSHGCE;} 
    double windowSHGCNE() const {return _windowSHGCNE;} 
    double windowSHGCN() const {return _windowSHGCN;}
    double windowSHGCNW() const {return _windowSHGCNW;} 
    double windowSHGCW() const {return _windowSHGCW;}
    double windowSHGCSW() const {return _windowSHGCSW;}
    double windowSCFS() const {return _windowSCFS;}
    double windowSCFSE() const {return _windowSCFSE;}
    double windowSCFE() const {return _windowSCFE;} 
    double windowSCFNE() const {return _windowSCFNE;} 
    double windowSCFN() const {return _windowSCFN;}
    double windowSCFNW() const {return _windowSCFNW;}
    double windowSCFW() const {return _windowSCFW;}
    double windowSCFSW() const {return _windowSCFSW;}
    double windowSDFS() const {return _windowSDFS;}
    double windowSDFSE() const {return _windowSDFSE;}
    double windowSDFE() const {return _windowSDFE;} 
    double windowSDFNE() const {return _windowSDFNE;} 
    double windowSDFN() const {return _windowSDFN;} 
    double windowSDFNW() const {return _windowSDFNW;} 
    double windowSDFW() const {return _windowSDFW;}
    double windowSDFSW() const {return _windowSDFSW;} 


    void setValid(bool val){_valid = val;}
    void setWallUvalueW(double val){_wallUvalueW=val;}
    void setWallUvalueNW(double val){_wallUvalueNW=val;}
    void setWallUvalueN(double val){_wallUvalueN=val;}
    void setWallUvalueNE(double val){_wallUvalueNE=val;}
    void setRoofSHGC(double val){_roofSHGC=val;}
    void setWallUvalueE(double val){_wallUvalueE=val;}
    void setWallUvalueSE(double val){_wallUvalueSE=val;}
    void setWallUvalueS(double val){_wallUvalueS=val;}
    void setWallUvalueSW(double val){_wallUvalueSW=val;}
    void setWallSolarAbsorptionS(double val){_wallSolarAbsorptionS=val;}
    void setWallSolarAbsorptionSE(double val){_wallSolarAbsorptionSE=val;}
    void setWallSolarAbsorptionE(double val){_wallSolarAbsorptionE=val;}
    void setWallSolarAbsorptionNE(double val){_wallSolarAbsorptionNE=val;}
    void setWallSolarAbsorptionN(double val){_wallSolarAbsorptionN=val;}
    void setWallSolarAbsorptionNW(double val){_wallSolarAbsorptionNW=val;}
    void setWallSolarAbsorptionW(double val){_wallSolarAbsorptionW=val;}
    void setWallSolarAbsorptionSW(double val){_wallSolarAbsorptionSW=val;}
    void setWallThermalEmissivityS(double val){_wallThermalEmissivityS=val;}
    void setWallThermalEmissivitySE(double val){_wallThermalEmissivitySE=val;}
    void setWallThermalEmissivityE(double val){_wallThermalEmissivityE=val;}
    void setWallThermalEmissivityNE(double val){_wallThermalEmissivityNE=val;}
    void setWallThermalEmissivityN(double val){_wallThermalEmissivityN=val;}
    void setWallThermalEmissivityNW(double val){_wallThermalEmissivityNW=val;}
    void setWallThermalEmissivityW(double val){_wallThermalEmissivityW=val;}
    void setWallThermalEmissivitySW(double val){_wallThermalEmissivitySW=val;}
    void setWindowSCFS(double val){_windowSCFS=val;}
    void setWindowSCFSE(double val){_windowSCFSE=val;}
    void setWindowSCFE(double val){_windowSCFE=val;}
    void setWindowSCFNE(double val){_windowSCFNE=val;}
    void setWindowSCFN(double val){_windowSCFN=val;}
    void setWindowSCFNW(double val){_windowSCFNW=val;}
    void setWindowSCFW(double val){_windowSCFW=val;}
    void setWindowSCFSW(double val){_windowSCFSW=val;}
    void setWindowSDFS(double val){_windowSDFS=val;}
    void setWindowSDFSE(double val){_windowSDFSE=val;}
    void setWindowSDFE(double val){_windowSDFE=val;}
    void setWindowSDFNE(double val){_windowSDFNE=val;}
    void setWindowSDFN(double val){_windowSDFN=val;}
    void setWindowSDFNW(double val){_windowSDFNW=val;}
    void setWindowSDFW(double val){_windowSDFW=val;}
    void setWindowSDFSW(double val){_windowSDFSW=val;}
    void setWindowSHGCS(double val){_windowSHGCS=val;}
    void setWindowSHGCSE(double val){_windowSHGCSE=val;}
    void setWindowSHGCE(double val){_windowSHGCE=val;}
    void setWindowSHGCNE(double val){_windowSHGCNE=val;}
    void setWindowSHGCN(double val){_windowSHGCN=val;}
    void setWindowSHGCNW(double val){_windowSHGCNW=val;}
    void setWindowSHGCW(double val){_windowSHGCW=val;}
    void setWindowSHGCSW(double val){_windowSHGCSW=val;}
    void setWindowUvalueS(double val){_windowUvalueS=val;}
    void setWindowUvalueSE(double val){_windowUvalueSE=val;}
    void setWindowUvalueE(double val){_windowUvalueE=val;}
    void setWindowUvalueNE(double val){_windowUvalueNE=val;}
    void setWindowUvalueN(double val){_windowUvalueN=val;}
    void setWindowUvalueNW(double val){_windowUvalueNW=val;}
    void setWindowUvalueW(double val){_windowUvalueW=val;}
    void setWindowUvalueSW(double val){_windowUvalueSW=val;}


    double wallAreaS() const {return _wallAreaS;}
    double wallAreaSE() const {return _wallAreaSE;}
    double wallAreaE() const {return _wallAreaE;}
    double wallAreaNE() const {return _wallAreaNE;}
    double wallAreaN() const {return _wallAreaN;}
    double wallAreaNW() const {return _wallAreaNW;}
    double wallAreaW() const {return _wallAreaW;}
    double wallAreaSW() const {return _wallAreaSW;}
    double roofArea() const {return _roofArea;}
    double windowAreaS() const {return _windowAreaS;}
    double windowAreaSE() const {return _windowAreaSE;}
    double windowAreaE() const {return _windowAreaE;}
    double windowAreaNE() const {return _windowAreaNE;}
    double windowAreaN() const {return _windowAreaN;}
    double windowAreaNW() const {return _windowAreaNW;}
    double windowAreaW() const {return _windowAreaW;}
    double windowAreaSW() const {return _windowAreaSW;}
    double skylightArea() const {return _skylightArea;}
    double roofSolarAbsorption() const {return _roofSolarAbsorption;}
    double roofThermalEmissivity() const {return _roofThermalEmissivity;}
    double skylightUvalue() const {return _skylightUvalue;}
    double skylightSHGC() const {return _skylightSHGC;}
    
    double exteriorHeatCapacity() const {return _exteriorHeatCapacity;}
    double infiltration() const {return _infiltration;}
    double hvacWasteFactor() const {return _hvacWasteFactor;}
    double hvacHeatingLossFactor() const {return _hvacHeatingLossFactor;}
    double hvacCoolingLossFactor() const {return _hvacCoolingLossFactor;}
    double dhwDistributionEfficiency() const {return _dhwDistributionEfficiency;}
    double heatingPumpControl() const {return _heatingPumpControl;}
    double coolingPumpControl() const {return _coolingPumpControl;}
    double heatGainPerPerson() const {return _heatGainPerPerson;}
    
    void setWeatherFilePath(const openstudio::path &val){_weatherFilePath = val;}
    void setTerrainClass(double val){ _terrainClass = val;}
    void setFloorArea(double val){ _floorArea = val;}
    void setBuildingHeight(double val){ _buildingHeight = val;}
    void setBuildingOccupancyFrom(double val){ _buildingOccupancyFrom = val;}
    void setBuildingOccupancyTo(double val){ _buildingOccupancyTo = val;}
    void setEquivFullLoadOccupancyFrom(double val){ _equivFullLoadOccupancyFrom = val;}
    void setEquivFullLoadOccupancyTo(double val){ _equivFullLoadOccupancyTo = val;}
    void setPeopleDensityOccupied(double val){ _peopleDensityOccupied = val;}
    void setPeopleDensityUnoccupied(double val){ _peopleDensityUnoccupied = val;}
    void setHeatingOccupiedSetpoint(double val){ _heatingOccupiedSetpoint = val;}
    void setHeatingUnoccupiedSetpoint(double val){ _heatingUnoccupiedSetpoint = val;}
    void setCoolingOccupiedSetpoint(double val){ _coolingOccupiedSetpoint = val;}
    void setCoolingUnoccupiedSetpoint(double val){ _coolingUnoccupiedSetpoint = val;}
    void setElecPowerAppliancesOccupied(double val){ _elecPowerAppliancesOccupied = val;}
    void setElecPowerAppliancesUnoccupied(double val){ _elecPowerAppliancesUnoccupied = val;}
    void setGasPowerAppliancesOccupied(double val){ _gasPowerAppliancesOccupied = val;}
    void setGasPowerAppliancesUnoccupied(double val){ _gasPowerAppliancesUnoccupied = val;}
    void setLightingPowerIntensityOccupied(double val){ _lightingPowerIntensityOccupied = val;}
    void setLightingPowerIntensityUnoccupied(double val){ _lightingPowerIntensityUnoccupied = val;}
    void setExteriorLightingPower(double val){ _exteriorLightingPower = val;}
    void setDaylightSensorSystem(double val){ _daylightSensorSystem = val;}
    void setLightingOccupancySensorSystem(double val){ _lightingOccupancySensorSystem = val;}
    void setConstantIlluminationControl(double val){ _constantIlluminationControl = val;}
    void setCoolingSystemCOP(double val){ _coolingSystemCOP = val;}
    void setCoolingSystemIPLVToCOPRatio(double val){ _coolingSystemIPLVToCOPRatio = val;}
    void setHeatingEnergyCarrier(double val){ _heatingEnergyCarrier = val;}
    void setHeatingSystemEfficiency(double val){ _heatingSystemEfficiency = val;}
    void setVentilationType(double val){ _ventilationType = val;}
    void setFreshAirFlowRate(double val){ _freshAirFlowRate = val;}
    void setSupplyExhaustRate(double val){ _supplyExhaustRate = val;}
    void setHeatRecovery(double val){ _heatRecovery = val;}
    void setExhaustAirRecirculation(double val){ _exhaustAirRecirculation = val;}
    void setBuildingAirLeakage(double val){ _buildingAirLeakage = val;}
    void setDhwDemand(double val){ _dhwDemand = val;}
    void setDhwEfficiency(double val){ _dhwEfficiency = val;}
    void setDhwDistributionSystem(double val){ _dhwDistributionSystem = val;}
    void setDhwEnergyCarrier(double val){ _dhwEnergyCarrier = val;}
    void setBemType(double val){ _bemType = val;}
    void setInteriorHeatCapacity(double val){ _interiorHeatCapacity = val;}
    void setSpecificFanPower(double val){ _specificFanPower = val;}
    void setFanFlowControlFactor(double val){ _fanFlowControlFactor = val;}
    void setRoofUValue(double val){ _roofUValue = val;}
    void setWallAreaS(double val){ _wallAreaS = val;}
    void setWallAreaSE(double val){ _wallAreaSE = val;}
    void setWallAreaE(double val){ _wallAreaE = val;}
    void setWallAreaNE(double val){ _wallAreaNE = val;}
    void setWallAreaN(double val){ _wallAreaN = val;}
    void setWallAreaNW(double val){ _wallAreaNW = val;}
    void setWallAreaW(double val){ _wallAreaW = val;}
    void setWallAreaSW(double val){ _wallAreaSW = val;}
    void setRoofArea(double val){ _roofArea = val;}
    void setWindowAreaS(double val){ _windowAreaS = val;}
    void setWindowAreaSE(double val){ _windowAreaSE = val;}
    void setWindowAreaE(double val){ _windowAreaE = val;}
    void setWindowAreaNE(double val){ _windowAreaNE = val;}
    void setWindowAreaN(double val){ _windowAreaN = val;}
    void setWindowAreaNW(double val){ _windowAreaNW = val;}
    void setWindowAreaW(double val){ _windowAreaW = val;}
    void setWindowAreaSW(double val){ _windowAreaSW = val;}
    void setSkylightArea(double val){ _skylightArea = val;}
    void setRoofSolarAbsorption(double val){_roofSolarAbsorption = val;}
    void setRoofThermalEmissivity(double val){_roofThermalEmissivity = val;}
    void setSkylightUvalue(double val){_skylightUvalue = val;}
    void setSkylightSHGC(double val){_skylightSHGC = val;}

    void setExteriorHeatCapacity(double val){ _exteriorHeatCapacity = val;}
    void setInfiltration(double val){ _infiltration = val;}
    void setHvacWasteFactor(double val){ _hvacWasteFactor = val;}
    void setHvacHeatingLossFactor(double val){ _hvacHeatingLossFactor = val;}
    void setHvacCoolingLossFactor(double val){ _hvacCoolingLossFactor = val;}
    void setDhwDistributionEfficiency(double val){ _dhwDistributionEfficiency = val;}
    void setHeatingPumpControl(double val){ _heatingPumpControl = val;}
    void setCoolingPumpControl(double val){ _coolingPumpControl = val;}
    void setHeatGainPerPerson(double val){ _heatGainPerPerson = val;}

  private:
    void parseStructure(const std::string &attributeName, const char* attributeValue);

    REGISTER_LOGGER("openstudio.isomodel.UserModel");
    std::shared_ptr<WeatherData> _weather; 
    bool _valid;
    double _terrainClass;
    double _floorArea;
    double _buildingHeight;
    double _buildingOccupancyFrom;
    double _buildingOccupancyTo;
    double _equivFullLoadOccupancyFrom;
    double _equivFullLoadOccupancyTo;
    double _peopleDensityOccupied;
    double _peopleDensityUnoccupied;
    double _heatingOccupiedSetpoint;
    double _heatingUnoccupiedSetpoint;
    double _coolingOccupiedSetpoint;
    double _coolingUnoccupiedSetpoint;
    double _elecPowerAppliancesOccupied;
    double _elecPowerAppliancesUnoccupied;
    double _gasPowerAppliancesOccupied;
    double _gasPowerAppliancesUnoccupied;
    double _lightingPowerIntensityOccupied;
    double _lightingPowerIntensityUnoccupied;
    double _exteriorLightingPower;
    double _daylightSensorSystem;
    double _lightingOccupancySensorSystem;
    double _constantIlluminationControl;
    double _coolingSystemCOP;
    double _coolingSystemIPLVToCOPRatio;
    double _heatingEnergyCarrier;
    double _heatingSystemEfficiency;
    double _ventilationType;
    double _freshAirFlowRate;
    double _supplyExhaustRate;
    double _heatRecovery;
    double _exhaustAirRecirculation;
    double _buildingAirLeakage;
    double _dhwDemand;
    double _dhwEfficiency;
    double _dhwDistributionSystem;
    double _dhwEnergyCarrier;
    double _bemType;
    double _interiorHeatCapacity;
    double _specificFanPower;
    double _fanFlowControlFactor;
    double _roofSHGC;

    /* Area */
    double _wallAreaS;
    double _wallAreaSE;
    double _wallAreaE;
    double _wallAreaNE;
    double _wallAreaN;
    double _wallAreaNW;
    double _wallAreaW;
    double _wallAreaSW;
    double _roofArea;
    
    /* UValue */
    double _wallUvalueS;
    double _wallUvalueSE;
    double _wallUvalueE;
    double _wallUvalueNE;
    double _wallUvalueN;
    double _wallUvalueNW;
    double _wallUvalueW;
    double _wallUvalueSW;
    double _roofUValue;
    
    /* SolarAbsorption */
    double _wallSolarAbsorptionS;
    double _wallSolarAbsorptionSE;
    double _wallSolarAbsorptionE;
    double _wallSolarAbsorptionNE;
    double _wallSolarAbsorptionN;
    double _wallSolarAbsorptionNW;
    double _wallSolarAbsorptionW;
    double _wallSolarAbsorptionSW;
    double _roofSolarAbsorption;

    /* ThermalEmissivity */
    double _wallThermalEmissivityS;
    double _wallThermalEmissivitySE;
    double _wallThermalEmissivityE;
    double _wallThermalEmissivityNE;
    double _wallThermalEmissivityN;
    double _wallThermalEmissivityNW;
    double _wallThermalEmissivityW;
    double _wallThermalEmissivitySW;
    double _roofThermalEmissivity;

    double _windowAreaS;
    double _windowAreaSE;
    double _windowAreaE;
    double _windowAreaNE;
    double _windowAreaN;
    double _windowAreaNW;
    double _windowAreaW;
    double _windowAreaSW;
    double _skylightArea;

    double _windowUvalueS;
    double _windowUvalueSE;
    double _windowUvalueE;
    double _windowUvalueNE;
    double _windowUvalueN;
    double _windowUvalueNW;
    double _windowUvalueW;
    double _windowUvalueSW;
    double _skylightUvalue;

    double _windowSHGCS;
    double _windowSHGCSE;
    double _windowSHGCE;
    double _windowSHGCNE;
    double _windowSHGCN;
    double _windowSHGCNW;
    double _windowSHGCW;
    double _windowSHGCSW;
    double _skylightSHGC;

    double _windowSCFS;
    double _windowSCFSE;
    double _windowSCFE;
    double _windowSCFNE;
    double _windowSCFN;
    double _windowSCFNW;
    double _windowSCFW;
    double _windowSCFSW;

    double _windowSDFS;
    double _windowSDFSE;
    double _windowSDFE;
    double _windowSDFNE;
    double _windowSDFN;
    double _windowSDFNW;
    double _windowSDFW;
    double _windowSDFSW;

    double _exteriorHeatCapacity;
    double _infiltration;
    double _hvacWasteFactor;
    double _hvacHeatingLossFactor;
    double _hvacCoolingLossFactor;
    double _dhwDistributionEfficiency;
    double _heatingPumpControl;
    double _coolingPumpControl;
    double _heatGainPerPerson;

    openstudio::path _weatherFilePath;
    openstudio::path _dataFile;


    void parseLine(const std::string &line);
    void loadBuilding(const openstudio::path &t_buildingFile);
    int weatherState(const std::string &header);

  };

} // isomodel
} // openstudio

#endif // ISOMODEL_USERMODEL_HPP
