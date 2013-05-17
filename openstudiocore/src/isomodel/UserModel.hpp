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

#include <utilities/core/Logger.hpp>

namespace openstudio {

namespace isomodel {

  
  class SimModel;

  class ISOMODEL_API UserModel {
  private:
    REGISTER_LOGGER("openstudio.isomodel.UserModel");
    double _terrainClass;
    double _stories;
    double _floorArea;
    double _floorToFloorHeight;
    double _buildingOccupancyFrom;
    double _buildingOccupancyTo;
    double _equivFullLoadOccupancyFrom;
    double _qeuivFullLoadOccupancyTo;
    double _peopleDensityOccupied;
    double _peopleDensityUnoccupied;
    double _heatingOccupiedSetpoint;
    double _heatingUnoccupiedSetpoint;
    double _coolingOccupiedSetpoint;
    double _coolingUnoccupiedSetpoint;
    double _elecPowerAppliancesOccupied;
    double _elecPowerAppliancesUnoccupied;
    double _gasPowerAppliancedOccupied;
    double _gasPowerAppliancedUnoccupied;
    double _lightingpowerIntensityOccupied;
    double _lightingpowerIntensityUnoccupied;
    double _exteriorLightingPower;
    double _daylightSensorSystem;
    double _lightingOccupancySensorSystem;
    double _constantIlluminationControl;
    double _coolingSystemCOP;
    double _coolingSystemIPLV;
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
    double _windowUValue;
    double _windowSolarAbsorbtion;
    double _windowThermalEmissivity;

    double _windowSHGC;
    double _windowSCF;
    double _windowSD;
    double _wallUValue;
    double _wallSolarAbsorbtion;
    double _wallThermalEmissivity;
    double _roofUValue;
    double _roofSHGC;
    double _wallAreaS;
    double _wallAreaSE;
    double _wallAreaE;
    double _wallAreaNE;
    double _wallAreaN;
    double _wallAreaNW;
    double _wallAreaW;
    double _wallAreaSW;
    double _roofArea;
    double _windowAreaS;
    double _windowAreaSE;
    double _windowAreaE;
    double _windowAreaNE;
    double _windowAreaN;
    double _windowAreaNW;
    double _windowAreaW;
    double _windowAreaSW;
    double _skylightArea;

    double _exteriorHeatCapacity;
    double _infiltration;
    double _hvacWasteFactor;
    double _hvacHeatingLossFactor;
    double _hvacCoolingLossFactor;
    double _dhwDistributionEfficiency;
    double _heatingPumpControl;
    double _coolingPumpControl;
    double _heatGainPerPerson;
  public:
    UserModel();
    virtual ~UserModel();    
    SimModel toSimModel() const;
    double terrainClass(){return _terrainClass;}
    double stories(){return _stories;}
    double floorArea(){return _floorArea;}
    double floorToFloorHeight(){return _floorToFloorHeight;}
    double buildingOccupancyFrom(){return _buildingOccupancyFrom;}
    double buildingOccupancyTo(){return _buildingOccupancyTo;}
    double equivFullLoadOccupancyFrom(){return _equivFullLoadOccupancyFrom;}
    double qeuivFullLoadOccupancyTo(){return _qeuivFullLoadOccupancyTo;}
    double peopleDensityOccupied(){return _peopleDensityOccupied;}
    double peopleDensityUnoccupied(){return _peopleDensityUnoccupied;}
    double heatingOccupiedSetpoint(){return _heatingOccupiedSetpoint;}
    double heatingUnoccupiedSetpoint(){return _heatingUnoccupiedSetpoint;}
    double coolingOccupiedSetpoint(){return _coolingOccupiedSetpoint;}
    double coolingUnoccupiedSetpoint(){return _coolingUnoccupiedSetpoint;}
    double elecPowerAppliancesOccupied(){return _elecPowerAppliancesOccupied;}
    double elecPowerAppliancesUnoccupied(){return _elecPowerAppliancesUnoccupied;}
    double gasPowerAppliancedOccupied(){return _gasPowerAppliancedOccupied;}
    double gasPowerAppliancedUnoccupied(){return _gasPowerAppliancedUnoccupied;}
    double lightingpowerIntensityOccupied(){return _lightingpowerIntensityOccupied;}
    double lightingpowerIntensityUnoccupied(){return _lightingpowerIntensityUnoccupied;}
    double exteriorLightingPower(){return _exteriorLightingPower;}
    double daylightSensorSystem(){return _daylightSensorSystem;}
    double lightingOccupancySensorSystem(){return _lightingOccupancySensorSystem;}
    double constantIlluminationControl(){return _constantIlluminationControl;}
    double coolingSystemCOP(){return _coolingSystemCOP;}
    double coolingSystemIPLV(){return _coolingSystemIPLV;}
    double heatingEnergyCarrier(){return _heatingEnergyCarrier;}
    double heatingSystemEfficiency(){return _heatingSystemEfficiency;}
    double ventilationType(){return _ventilationType;}
    double freshAirFlowRate(){return _freshAirFlowRate;}
    double supplyExhaustRate(){return _supplyExhaustRate;}
    double heatRecovery(){return _heatRecovery;}
    double exhaustAirRecirculation(){return _exhaustAirRecirculation;}
    double buildingAirLeakage(){return _buildingAirLeakage;}
    double dhwDemand(){return _dhwDemand;}
    double dhwEfficiency(){return _dhwEfficiency;}
    double dhwDistributionSystem(){return _dhwDistributionSystem;}
    double dhwEnergyCarrier(){return _dhwEnergyCarrier;}
    double bemType(){return _bemType;}
    double interiorHeatCapacity(){return _interiorHeatCapacity;}
    double specificFanPower(){return _specificFanPower;}
    double fanFlowControlFactor(){return _fanFlowControlFactor;}
    double windowUValue(){return _windowUValue;}
    double windowSolarAbsorbtion(){return _windowSolarAbsorbtion;}
    double windowThermalEmissivity(){return _windowThermalEmissivity;}

    double windowSHGC(){return _windowSHGC;}
    double windowSCF(){return _windowSCF;}
    double windowSD(){return _windowSD;}
    double wallUValue(){return _wallUValue;}
    double wallSolarAbsorbtion(){return _wallSolarAbsorbtion;}
    double wallThermalEmissivity(){return _wallThermalEmissivity;}
    double roofUValue(){return _roofUValue;}
    double roofSHGC(){return _roofSHGC;}
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
    
    double exteriorHeatCapacity(){return _exteriorHeatCapacity;}
    double infiltration(){return _infiltration;}
    double hvacWasteFactor(){return _hvacWasteFactor;}
    double hvacHeatingLossFactor(){return _hvacHeatingLossFactor;}
    double hvacCoolingLossFactor(){return _hvacCoolingLossFactor;}
    double dhwDistributionEfficiency(){return _dhwDistributionEfficiency;}
    double heatingPumpControl(){return _heatingPumpControl;}
    double coolingPumpControl(){return _coolingPumpControl;}
    double heatGainPerPerson(){return _heatGainPerPerson;}
    
    void setTerrainClass(double val){ _terrainClass = val;}
    void setStories(double val){ _stories = val;}
    void setFloorArea(double val){ _floorArea = val;}
    void setFloorToFloorHeight(double val){ _floorToFloorHeight = val;}
    void setBuildingOccupancyFrom(double val){ _buildingOccupancyFrom = val;}
    void setBuildingOccupancyTo(double val){ _buildingOccupancyTo = val;}
    void setEquivFullLoadOccupancyFrom(double val){ _equivFullLoadOccupancyFrom = val;}
    void setQeuivFullLoadOccupancyTo(double val){ _qeuivFullLoadOccupancyTo = val;}
    void setPeopleDensityOccupied(double val){ _peopleDensityOccupied = val;}
    void setPeopleDensityUnoccupied(double val){ _peopleDensityUnoccupied = val;}
    void setHeatingOccupiedSetpoint(double val){ _heatingOccupiedSetpoint = val;}
    void setHeatingUnoccupiedSetpoint(double val){ _heatingUnoccupiedSetpoint = val;}
    void setCoolingOccupiedSetpoint(double val){ _coolingOccupiedSetpoint = val;}
    void setCoolingUnoccupiedSetpoint(double val){ _coolingUnoccupiedSetpoint = val;}
    void setElecPowerAppliancesOccupied(double val){ _elecPowerAppliancesOccupied = val;}
    void setElecPowerAppliancesUnoccupied(double val){ _elecPowerAppliancesUnoccupied = val;}
    void setGasPowerAppliancedOccupied(double val){ _gasPowerAppliancedOccupied = val;}
    void setGasPowerAppliancedUnoccupied(double val){ _gasPowerAppliancedUnoccupied = val;}
    void setLightingpowerIntensityOccupied(double val){ _lightingpowerIntensityOccupied = val;}
    void setLightingpowerIntensityUnoccupied(double val){ _lightingpowerIntensityUnoccupied = val;}
    void setExteriorLightingPower(double val){ _exteriorLightingPower = val;}
    void setDaylightSensorSystem(double val){ _daylightSensorSystem = val;}
    void setLightingOccupancySensorSystem(double val){ _lightingOccupancySensorSystem = val;}
    void setConstantIlluminationControl(double val){ _constantIlluminationControl = val;}
    void setCoolingSystemCOP(double val){ _coolingSystemCOP = val;}
    void setCoolingSystemIPLV(double val){ _coolingSystemIPLV = val;}
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
    void setWindowUValue(double val){ _windowUValue = val;}
    void setWindowSolarAbsorbtion(double val){ _windowSolarAbsorbtion = val;}
    void setWindowThermalEmissivity(double val){ _windowThermalEmissivity = val;}

    void setWindowSHGC(double val){ _windowSHGC = val;}
    void setWindowSCF(double val){ _windowSCF = val;}
    void setWindowSD(double val){ _windowSD = val;}
    void setWallUValue(double val){ _wallUValue = val;}
    void setWallSolarAbsorbtion(double val){ _wallSolarAbsorbtion = val;}
    void setWallThermalEmissivity(double val){ _wallThermalEmissivity = val;}
    void setRoofUValue(double val){ _roofUValue = val;}
    void setRoofSHGC(double val){ _roofSHGC = val;}
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
