/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <isomodel/EpwData.hpp>
#include <isomodel/SimModel.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <utilities/core/Logger.hpp>

namespace openstudio {

namespace isomodel {
  
  class SimModel;
  class WeatherData;

  class ISOMODEL_API UserModel {
  private:
    std::string resolveFilename(std::string baseFile, std::string relativeFile);
    void parseStructure(std::string attributeName, const char* attributeValue);

    REGISTER_LOGGER("openstudio.isomodel.UserModel");
    boost::shared_ptr<WeatherData> _weather; 
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
    double _exhaustAirRecirclation;
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

    std::string _weatherFilePath;
    std::string dataFile;


    void parseLine(std::string line);
    void loadBuilding(std::string buildingFile);
    int weatherState(std::string header);

  public:
    /**
     * Loads the specified weather data from disk.
     * Exposed to allow for separate loading from Ruby Scripts
     * Call setWeatherFilePath(path) then loadWeather() to update
     * the UserModel with a new set of weather data
     */
    boost::shared_ptr<WeatherData> loadWeather();
    /**
     * Loads an ISO model from the specified .ISO file
     */
    void load(std::string buildingFile);

    /**
     * Generates a SimModel from the specified parameters of the 
     * UserModel
     */  
    SimModel toSimModel() const;

    /**
     * Indicates whether or not the user model loaded in correctly
     * If either the ISO file or the Weather File cannot be found
     * valid will be false
     * userModel.load(<filename>)
     * if(userModel.valid()){
     *     userModel.toSimModel().simulate();
     * }
     */
    bool valid(){return _valid;}
    std::string weatherFilePath(){return _weatherFilePath;}
    double terrainClass(){return _terrainClass;} const 
    double floorArea(){return _floorArea;} const 
    double buildingHeight(){return _buildingHeight;} const 
    double buildingOccupancyFrom(){return _buildingOccupancyFrom;} const 
    double buildingOccupancyTo(){return _buildingOccupancyTo;} const 
    double equivFullLoadOccupancyFrom(){return _equivFullLoadOccupancyFrom;} const 
    double equivFullLoadOccupancyTo(){return _equivFullLoadOccupancyTo;} const 
    double peopleDensityOccupied(){return _peopleDensityOccupied;} const 
    double peopleDensityUnoccupied(){return _peopleDensityUnoccupied;} const 
    double heatingOccupiedSetpoint(){return _heatingOccupiedSetpoint;} const 
    double heatingUnoccupiedSetpoint(){return _heatingUnoccupiedSetpoint;} const 
    double coolingOccupiedSetpoint(){return _coolingOccupiedSetpoint;} const 
    double coolingUnoccupiedSetpoint(){return _coolingUnoccupiedSetpoint;} const 
    double elecPowerAppliancesOccupied(){return _elecPowerAppliancesOccupied;} const 
    double elecPowerAppliancesUnoccupied(){return _elecPowerAppliancesUnoccupied;} const 
    double gasPowerAppliancesOccupied(){return _gasPowerAppliancesOccupied;} const 
    double gasPowerAppliancesUnoccupied(){return _gasPowerAppliancesUnoccupied;} const 
    double lightingPowerIntensityOccupied(){return _lightingPowerIntensityOccupied;} const 
    double lightingPowerIntensityUnoccupied(){return _lightingPowerIntensityUnoccupied;} const 
    double exteriorLightingPower(){return _exteriorLightingPower;} const 
    double daylightSensorSystem(){return _daylightSensorSystem;} const 
    double lightingOccupancySensorSystem(){return _lightingOccupancySensorSystem;} const 
    double constantIlluminationControl(){return _constantIlluminationControl;} const 
    double coolingSystemCOP(){return _coolingSystemCOP;} const 
    double coolingSystemIPLVToCOPRatio(){return _coolingSystemIPLVToCOPRatio;} const 
    double heatingEnergyCarrier(){return _heatingEnergyCarrier;} const 
    double heatingSystemEfficiency(){return _heatingSystemEfficiency;} const 
    double ventilationType(){return _ventilationType;} const 
    double freshAirFlowRate(){return _freshAirFlowRate;} const 
    double supplyExhaustRate(){return _supplyExhaustRate;} const 
    double heatRecovery(){return _heatRecovery;} const 
    double exhaustAirRecirclation(){return _exhaustAirRecirclation;} const 
    double buildingAirLeakage(){return _buildingAirLeakage;} const 
    double dhwDemand(){return _dhwDemand;} const 
    double dhwEfficiency(){return _dhwEfficiency;} const 
    double dhwDistributionSystem(){return _dhwDistributionSystem;} const 
    double dhwEnergyCarrier(){return _dhwEnergyCarrier;} const 
    double bemType(){return _bemType;} const 
    double interiorHeatCapacity(){return _interiorHeatCapacity;} const 
    double specificFanPower(){return _specificFanPower;} const 
    double fanFlowControlFactor(){return _fanFlowControlFactor;} const 

    double roofUValue(){return _roofUValue;} const
    double roofSHGC(){return _roofSHGC;} const
    double wallUvalueS(){return _wallUvalueS;} const	
    double wallUvalueSE(){return _wallUvalueSE;} const	
    double wallUvalueE(){return _wallUvalueE;} const	
    double wallUvalueNE(){return _wallUvalueNE;} const	
    double wallUvalueN(){return _wallUvalueN;} const	
    double wallUvalueNW(){return _wallUvalueNW;} const	
    double wallUvalueW(){return _wallUvalueW;} const	
    double wallUvalueSW(){return _wallUvalueSW;} const	
    double wallSolarAbsorptionS(){return _wallSolarAbsorptionS;} const
    double wallSolarAbsorptionSE(){return _wallSolarAbsorptionSE;} const
    double wallSolarAbsorptionE(){return _wallSolarAbsorptionE;} const
    double wallSolarAbsorptionNE(){return _wallSolarAbsorptionNE;} const
    double wallSolarAbsorptionN(){return _wallSolarAbsorptionN;} const
    double wallSolarAbsorptionNW(){return _wallSolarAbsorptionNW;} const
    double wallSolarAbsorptionW(){return _wallSolarAbsorptionW;} const
    double wallSolarAbsorptionSW(){return _wallSolarAbsorptionSW;} const
    double wallThermalEmissivityS(){return _wallThermalEmissivityS;} const
    double wallThermalEmissivitySE(){return _wallThermalEmissivitySE;} const
    double wallThermalEmissivityE(){return _wallThermalEmissivityE;} const
    double wallThermalEmissivityNE(){return _wallThermalEmissivityNE;} const
    double wallThermalEmissivityN(){return _wallThermalEmissivityN;} const
    double wallThermalEmissivityNW(){return _wallThermalEmissivityNW;} const
    double wallThermalEmissivityW(){return _wallThermalEmissivityW;} const
    double wallThermalEmissivitySW(){return _wallThermalEmissivitySW;} const
    double windowUvalueS(){return _windowUvalueS;} const
    double windowUvalueSE(){return _windowUvalueSE;} const
    double windowUvalueE(){return _windowUvalueE;} const
    double windowUvalueNE(){return _windowUvalueNE;} const
    double windowUvalueN(){return _windowUvalueN;} const
    double windowUvalueNW(){return _windowUvalueNW;} const
    double windowUvalueW(){return _windowUvalueW;} const
    double windowUvalueSW(){return _windowUvalueSW;} const
    double windowSHGCS(){return _windowSHGCS;} const
    double windowSHGCSE(){return _windowSHGCSE;} const
    double windowSHGCE(){return _windowSHGCE;} const
    double windowSHGCNE(){return _windowSHGCNE;} const
    double windowSHGCN(){return _windowSHGCN;} const
    double windowSHGCNW(){return _windowSHGCNW;} const
    double windowSHGCW(){return _windowSHGCW;} const
    double windowSHGCSW(){return _windowSHGCSW;} const
    double windowSCFS(){return _windowSCFS;} const
    double windowSCFSE(){return _windowSCFSE;} const
    double windowSCFE(){return _windowSCFE;} const
    double windowSCFNE(){return _windowSCFNE;} const
    double windowSCFN(){return _windowSCFN;} const
    double windowSCFNW(){return _windowSCFNW;} const
    double windowSCFW(){return _windowSCFW;} const
    double windowSCFSW(){return _windowSCFSW;} const
    double windowSDFS(){return _windowSDFS;} const
    double windowSDFSE(){return _windowSDFSE;} const
    double windowSDFE(){return _windowSDFE;} const
    double windowSDFNE(){return _windowSDFNE;} const
    double windowSDFN(){return _windowSDFN;} const
    double windowSDFNW(){return _windowSDFNW;} const
    double windowSDFW(){return _windowSDFW;} const
    double windowSDFSW(){return _windowSDFSW;} const


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


    double wallAreaS(){return _wallAreaS;}
    double wallAreaSE(){return _wallAreaSE;}
    double wallAreaE(){return _wallAreaE;}
    double wallAreaNE(){return _wallAreaNE;}
    double wallAreaN(){return _wallAreaN;}
    double wallAreaNW(){return _wallAreaNW;}
    double wallAreaW(){return _wallAreaW;}
    double wallAreaSW(){return _wallAreaSW;}
    double roofArea(){return _roofArea;}
    double windowAreaS(){return _windowAreaS;}
    double windowAreaSE(){return _windowAreaSE;}
    double windowAreaE(){return _windowAreaE;}
    double windowAreaNE(){return _windowAreaNE;}
    double windowAreaN(){return _windowAreaN;}
    double windowAreaNW(){return _windowAreaNW;}
    double windowAreaW(){return _windowAreaW;}
    double windowAreaSW(){return _windowAreaSW;}
    double skylightArea(){return _skylightArea;}
    double roofSolarAbsorption(){return _roofSolarAbsorption;}
    double roofThermalEmissivity(){return _roofThermalEmissivity;}
    double skylightUvalue(){return _skylightUvalue;}
    double skylightSHGC(){return _skylightSHGC;}
    
    double exteriorHeatCapacity(){return _exteriorHeatCapacity;}
    double infiltration(){return _infiltration;}
    double hvacWasteFactor(){return _hvacWasteFactor;}
    double hvacHeatingLossFactor(){return _hvacHeatingLossFactor;}
    double hvacCoolingLossFactor(){return _hvacCoolingLossFactor;}
    double dhwDistributionEfficiency(){return _dhwDistributionEfficiency;}
    double heatingPumpControl(){return _heatingPumpControl;}
    double coolingPumpControl(){return _coolingPumpControl;}
    double heatGainPerPerson(){return _heatGainPerPerson;}
    
    void setWeatherFilePath(std::string val){_weatherFilePath = val;}
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
    void setExhaustAirRecirclation(double val){ _exhaustAirRecirclation = val;}
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
  };

} // isomodel
} // openstudio

#endif // ISOMODEL_USERMODEL_HPP
