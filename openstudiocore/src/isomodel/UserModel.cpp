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

#include "UserModel.hpp"

using namespace std;
namespace openstudio {
namespace isomodel {

 
  SimModel UserModel::toSimModel()
  {
    _valid = true;
    if (!_weather)
    {
      // weather file isn't loaded yet, let's try to
      _weather = loadWeather();
    }

    if(!_valid){
      throw std::runtime_error("UserModel is not valid, cannot create SimModel");
    }

    SimModel sim;

    std::shared_ptr<Population> pop(new Population);
    pop->setDaysStart(_buildingOccupancyFrom);
    pop->setDaysEnd(_buildingOccupancyTo);
    pop->setHoursEnd(_equivFullLoadOccupancyTo);
    pop->setHoursStart(_equivFullLoadOccupancyFrom);
    pop->setDensityOccupied(_peopleDensityOccupied);
    
    pop->setDensityUnoccupied(_peopleDensityUnoccupied);
    pop->setHeatGainPerPerson(_heatGainPerPerson);
    sim.setPop(pop);

    std::shared_ptr<Location> loc(new Location);
    loc->setTerrain(_terrainClass);
    loc->setWeatherData(_weather);
    sim.setLocation(loc);

    std::shared_ptr<Building> building(new Building);
    building->setBuildingEnergyManagement(_bemType);
    building->setConstantIllumination(_constantIlluminationControl);
    building->setElectricApplianceHeatGainOccupied(_elecPowerAppliancesOccupied);
    building->setElectricApplianceHeatGainUnoccupied(_elecPowerAppliancesUnoccupied);
    building->setGasApplianceHeatGainOccupied(_gasPowerAppliancesOccupied);
    building->setGasApplianceHeatGainUnoccupied(_gasPowerAppliancesUnoccupied);
    building->setLightingOccupancySensor(_lightingOccupancySensorSystem);
    sim.setBuilding(building);

    std::shared_ptr<Cooling> cooling(new Cooling);
    cooling->setCOP(_coolingSystemCOP);
    cooling->setHvacLossFactor(_hvacCoolingLossFactor);
    cooling->setPartialLoadValue(_coolingSystemIPLVToCOPRatio);
    cooling->setPumpControlReduction(_coolingPumpControl);
    cooling->setTemperatureSetPointOccupied(_coolingOccupiedSetpoint);
    cooling->setTemperatureSetPointUnoccupied(_coolingUnoccupiedSetpoint);
    sim.setCooling(cooling);
    
    std::shared_ptr<Heating> heating(new Heating);
    heating->setEfficiency(_heatingSystemEfficiency);
    heating->setEnergyType(_heatingEnergyCarrier);
    heating->setHotcoldWasteFactor(_hvacWasteFactor);//??
    heating->setHotWaterDemand(_dhwDemand);
    heating->setHotWaterDistributionEfficiency(_dhwDistributionEfficiency);
    heating->setHotWaterEnergyType(_dhwEnergyCarrier);
    heating->setHotWaterSystemEfficiency(_dhwEfficiency);
    heating->setHvacLossFactor(_hvacHeatingLossFactor);
    heating->setPumpControlReduction(_heatingPumpControl);
    heating->setTemperatureSetPointOccupied(_heatingOccupiedSetpoint);
    heating->setTemperatureSetPointUnoccupied(_heatingUnoccupiedSetpoint);
    sim.setHeating(heating);

    std::shared_ptr<Lighting> lighting(new Lighting);
    lighting->setDimmingFraction(_daylightSensorSystem);
    lighting->setExteriorEnergy(_exteriorLightingPower);
    lighting->setPowerDensityOccupied(_lightingPowerIntensityOccupied);
    lighting->setPowerDensityUnoccupied(_lightingPowerIntensityUnoccupied);
    sim.setLights(lighting);

    std::shared_ptr<Structure> structure(new Structure);
    structure->setFloorArea(_floorArea);
    structure->setBuildingHeight(_buildingHeight);
    structure->setInfiltrationRate(_buildingAirLeakage);
    structure->setInteriorHeatCapacity(_interiorHeatCapacity);
    //directions in the order [S, SE, E, NE, N, NW, W, SW, roof/skylight]
    Vector wallArea(9);
    wallArea[0] = _wallAreaS;
    wallArea[1] = _wallAreaSE;
    wallArea[2] = _wallAreaE;
    wallArea[3] = _wallAreaNE;
    wallArea[4] = _wallAreaN;
    wallArea[5] = _wallAreaNW;
    wallArea[6] = _wallAreaW;
    wallArea[7] = _wallAreaSW;
    wallArea[8] = _roofArea;
    structure->setWallArea(wallArea);//vector
    structure->setWallHeatCapacity(_exteriorHeatCapacity);//??

    Vector wallSolar(9);
    wallSolar[0] = _wallSolarAbsorptionS;
    wallSolar[1] = _wallSolarAbsorptionSE;
    wallSolar[2] = _wallSolarAbsorptionE;
    wallSolar[3] = _wallSolarAbsorptionNE;
    wallSolar[4] = _wallSolarAbsorptionN;
    wallSolar[5] = _wallSolarAbsorptionNW;
    wallSolar[6] = _wallSolarAbsorptionW;
    wallSolar[7] = _wallSolarAbsorptionSW;
    wallSolar[8] = _roofSolarAbsorption;
    structure->setWallSolarAbsorbtion(wallSolar);//vector

    Vector wallTherm(9);
    wallTherm[0] = _wallThermalEmissivityS;
    wallTherm[1] = _wallThermalEmissivitySE;
    wallTherm[2] = _wallThermalEmissivityE;
    wallTherm[3] = _wallThermalEmissivityNE;
    wallTherm[4] = _wallThermalEmissivityN;
    wallTherm[5] = _wallThermalEmissivityNW;
    wallTherm[6] = _wallThermalEmissivityW;
    wallTherm[7] = _wallThermalEmissivitySW;
    wallTherm[8] = _roofThermalEmissivity;
    structure->setWallThermalEmissivity(wallTherm);//vector

    Vector wallU(9);
    wallU[0] = _wallUvalueS;
    wallU[1] = _wallUvalueSE;
    wallU[2] = _wallUvalueE;
    wallU[3] = _wallUvalueNE;
    wallU[4] = _wallUvalueN;
    wallU[5] = _wallUvalueNW;
    wallU[6] = _wallUvalueW;
    wallU[7] = _wallUvalueSW;
    wallU[8] = _roofUValue;
    structure->setWallUniform(wallU);//vector


    Vector windowArea(9);
    windowArea[0] = _windowAreaS ;
    windowArea[1] = _windowAreaSE;
    windowArea[2] = _windowAreaE ;
    windowArea[3] = _windowAreaNE;
    windowArea[4] = _windowAreaN ;
    windowArea[5] = _windowAreaNW;
    windowArea[6] = _windowAreaW ;
    windowArea[7] = _windowAreaSW;
    windowArea[8] = _skylightArea ;
    structure->setWindowArea(windowArea);//vector

    Vector winSHGC(9);
    winSHGC[0] = _windowSHGCS ;
    winSHGC[1] = _windowSHGCSE;
    winSHGC[2] = _windowSHGCE ;
    winSHGC[3] = _windowSHGCNE;
    winSHGC[4] = _windowSHGCN ;
    winSHGC[5] = _windowSHGCNW;
    winSHGC[6] = _windowSHGCW ;
    winSHGC[7] = _windowSHGCSW;
    winSHGC[8] = _skylightSHGC;
    structure->setWindowNormalIncidenceSolarEnergyTransmittance(winSHGC);//vector
    
    Vector winSCF(9);
    winSCF[0] = _windowSCFS ;
    winSCF[1] = _windowSCFSE;
    winSCF[2] = _windowSCFE ;
    winSCF[3] = _windowSCFNE;
    winSCF[4] = _windowSCFN ;
    winSCF[5] = _windowSCFNW;
    winSCF[6] = _windowSCFW ;
    winSCF[7] = _windowSCFSW;
    winSCF[8] = _windowSCFN;
    structure->setWindowShadingCorrectionFactor(winSCF);//vector
    structure->setWindowShadingDevice(_windowSDFN);

    Vector winU(9);
    winU[0] = _windowUvalueS ;
    winU[1] = _windowUvalueSE;
    winU[2] = _windowUvalueE ;
    winU[3] = _windowUvalueNE;
    winU[4] = _windowUvalueN ;
    winU[5] = _windowUvalueNW;
    winU[6] = _windowUvalueW ;
    winU[7] = _windowUvalueSW;
    winU[8] = _skylightUvalue;
    structure->setWindowUniform(winU);//vector
    sim.setStructure(structure);

    std::shared_ptr<Ventilation> ventilation(new Ventilation);
    ventilation->setExhaustAirRecirculated(_exhaustAirRecirculation);
    ventilation->setFanControlFactor(_fanFlowControlFactor);
    ventilation->setFanPower(_specificFanPower);
    ventilation->setHeatRecoveryEfficiency(_heatRecovery);
    ventilation->setSupplyDifference(_supplyExhaustRate);
    ventilation->setSupplyRate(_freshAirFlowRate);
    ventilation->setType(_ventilationType);
    ventilation->setWasteFactor(_hvacWasteFactor);//??
    sim.setVentilation(ventilation);
    return sim;
  }
  //http://stackoverflow.com/questions/10051679/c-tokenize-string
  std::vector<std::string> inline stringSplit(const std::string &source, char delimiter = ' ', bool keepEmpty = false)
  {
    std::vector<std::string> results;

    size_t prev = 0;
    size_t next = 0;
    if(source.size()==0)
      return results;

    while ((next = source.find_first_of(delimiter, prev)) != std::string::npos)
    {
        if (keepEmpty || (next - prev != 0))
        {
            results.push_back(source.substr(prev, next - prev));
        }
        prev = next + 1;
    }

    if (prev < source.size())
    {
        results.push_back(source.substr(prev));
    }

    return results;
  }

  // trim from front
  static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
  }

  // trim from back
  static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
  }

  // trim from front and back ends
  static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
  }

  // trim from front and back ends
  static inline std::string &lcase(std::string &s) {
    for(unsigned int i = 0;i<s.size();i++){
      if(s.at(i) < 91){
        s.at(i) = s.at(i)+32;
      }
    }
    return ltrim(rtrim(s));
  }

  void UserModel::parseStructure(const std::string &attributeName, const char* attributeValue){
    //Window&Wall Values    
    if(attributeName == "windowuvaluen"){
      _windowUvalueN = (atof(attributeValue));
    } else if(attributeName == "windowshgcn"){
      _windowSHGCN = (atof(attributeValue));
    } else if(attributeName == "windowscfn"){
      _windowSCFN = (atof(attributeValue));
    } else if(attributeName == "windowsdfn"){
      _windowSDFN = (atof(attributeValue));
    } else if(attributeName == "walluvaluen"){
      _wallUvalueN = (atof(attributeValue));
    } else if(attributeName == "wallsolarabsorptionn"){
      _wallSolarAbsorptionN = (atof(attributeValue));
    } else if(attributeName == "wallthermalemissivityn"){
      _wallThermalEmissivityN = (atof(attributeValue));
    }
  
    else if(attributeName == "windowuvaluene"){
      _windowUvalueNE = (atof(attributeValue));
    } else if(attributeName == "windowshgcne"){
      _windowSHGCNE = (atof(attributeValue));
    } else if(attributeName == "windowscfne"){
      _windowSCFNE = (atof(attributeValue));
    } else if(attributeName == "windowsdfne"){
      _windowSDFNE = (atof(attributeValue));
    } else if(attributeName == "walluvaluene"){
      _wallUvalueNE = (atof(attributeValue));
    } else if(attributeName == "wallsolarabsorptionne"){
      _wallSolarAbsorptionNE = (atof(attributeValue));
    } else if(attributeName == "wallthermalemissivityne"){
      _wallThermalEmissivityNE = (atof(attributeValue));
    } 
  
    else if(attributeName == "windowuvaluee"){
      _windowUvalueE = (atof(attributeValue));
    } else if(attributeName == "windowshgce"){
      _windowSHGCE = (atof(attributeValue));
    } else if(attributeName == "windowscfe"){
      _windowSCFE = (atof(attributeValue));
    } else if(attributeName == "windowsdfe"){
      _windowSDFE = (atof(attributeValue));
    } else if(attributeName == "walluvaluee"){
      _wallUvalueE = (atof(attributeValue));
    } else if(attributeName == "wallsolarabsorptione"){
      _wallSolarAbsorptionE = (atof(attributeValue));
    } else if(attributeName == "wallthermalemissivitye"){
      _wallThermalEmissivityE = (atof(attributeValue));
    } 
  
    else if(attributeName == "windowuvaluese"){
      _windowUvalueSE =  (atof(attributeValue));
    } else if(attributeName == "windowshgcse"){
      _windowSHGCSE =  (atof(attributeValue));
    } else if(attributeName == "windowscfse"){
      _windowSCFSE =  (atof(attributeValue));
    } else if(attributeName == "windowsdfse"){
      _windowSDFSE =  (atof(attributeValue));
    } else if(attributeName == "walluvaluese"){
      _wallUvalueSE =  (atof(attributeValue));
    } else if(attributeName == "wallsolarabsorptionse"){
      _wallSolarAbsorptionSE =  (atof(attributeValue));
    } else if(attributeName == "wallthermalemissivityse"){
      _wallThermalEmissivitySE =  (atof(attributeValue));
    } 
  
    else if(attributeName == "windowuvalues"){
      _windowUvalueS =  (atof(attributeValue));
    } else if(attributeName == "windowshgcs"){
      _windowSHGCS =  (atof(attributeValue));
    } else if(attributeName == "windowscfs"){
      _windowSCFS =  (atof(attributeValue));
    } else if(attributeName == "windowsdfs"){
      _windowSDFS =  (atof(attributeValue));
    } else if(attributeName == "walluvalues"){
      _wallUvalueS =  (atof(attributeValue));
    } else if(attributeName == "wallsolarabsorptions"){
      _wallSolarAbsorptionS =  (atof(attributeValue));
    } else if(attributeName == "wallthermalemissivitys"){
      _wallThermalEmissivityS =  (atof(attributeValue));
    } 

    else if(attributeName == "windowuvaluesw"){
      _windowUvalueSW =  (atof(attributeValue));
    } else if(attributeName == "windowshgcsw"){
      _windowSHGCSW =  (atof(attributeValue));
    } else if(attributeName == "windowscfsw"){
      _windowSCFSW =  (atof(attributeValue));
    } else if(attributeName == "windowsdfsw"){
      _windowSDFSW =  (atof(attributeValue));
    } else if(attributeName == "walluvaluesw"){
      _wallUvalueSW =  (atof(attributeValue));
    } else if(attributeName == "wallsolarabsorptionsw"){
      _wallSolarAbsorptionSW =  (atof(attributeValue));
    } else if(attributeName == "wallthermalemissivitysw"){
      _wallThermalEmissivitySW =  (atof(attributeValue));
    } 
  
    else if(attributeName == "windowuvaluew"){
      _windowUvalueW =  (atof(attributeValue));
    } else if(attributeName == "windowshgcw"){
      _windowSHGCW =  (atof(attributeValue));
    } else if(attributeName == "windowscfw"){
      _windowSCFW =  (atof(attributeValue));
    } else if(attributeName == "windowsdfw"){
      _windowSDFW =  (atof(attributeValue));
    } else if(attributeName == "walluvaluew"){
      _wallUvalueW =  (atof(attributeValue));
    } else if(attributeName == "wallsolarabsorptionw"){
      _wallSolarAbsorptionW =  (atof(attributeValue));
    } else if(attributeName == "wallthermalemissivityw"){
      _wallThermalEmissivityW =  (atof(attributeValue));
    } 

    else if(attributeName == "windowuvaluenw"){
      _windowUvalueNW =  (atof(attributeValue));
    } else if(attributeName == "windowshgcnw"){
      _windowSHGCNW =  (atof(attributeValue));
    } else if(attributeName == "windowscfnw"){
      _windowSCFNW =  (atof(attributeValue));
    } else if(attributeName == "windowsdfnw"){
      _windowSDFNW =  (atof(attributeValue));
    } else if(attributeName == "walluvaluenw"){
      _wallUvalueNW =  (atof(attributeValue));
    } else if(attributeName == "wallsolarabsorptionnw"){
      _wallSolarAbsorptionNW =  (atof(attributeValue));
    } else if(attributeName == "wallthermalemissivitynw"){
      _wallThermalEmissivityNW =  (atof(attributeValue));
    } 
  }

  void UserModel::parseLine(const string &line){
    std::vector<std::string> linesplit = stringSplit(line, '=', true);
    if(linesplit.size()<2)
      return;
    for(unsigned int i = 0;i<linesplit.size();i++) {
      linesplit[i] = trim(linesplit[i]);
    }
    if(linesplit[0].at(0) == '#')
      return;
    string attributeName = lcase(linesplit[0]);
    
    const char* attributeValue = linesplit[1].c_str();
    if(attributeName == "terrainclass"){
      setTerrainClass(atof(attributeValue));
    } else if(attributeName == "buildingheight"){
      setBuildingHeight(atof(attributeValue));
    } else if(attributeName == "floorarea"){
      setFloorArea(atof(attributeValue));
    } else if(attributeName == "buildingoccupancyfrom"){
      setBuildingOccupancyFrom(atof(attributeValue));
    } else if(attributeName == "buildingoccupancyto"){
      setBuildingOccupancyTo(atof(attributeValue));
    } else if(attributeName == "equivfullloadoccupancyfrom"){
      setEquivFullLoadOccupancyFrom(atof(attributeValue));
    } else if(attributeName == "equivfullloadoccupancyto"){
      setEquivFullLoadOccupancyTo(atof(attributeValue));
    } else if(attributeName == "peopledensityoccupied"){
      setPeopleDensityOccupied(atof(attributeValue));
    } else if(attributeName == "peopledensityunoccupied"){
      setPeopleDensityUnoccupied(atof(attributeValue));
    } else if(attributeName == "lightingpowerintensityoccupied"){
      setLightingPowerIntensityOccupied(atof(attributeValue));
    } else if(attributeName == "lightingpowerintensityunoccupied"){
      setLightingPowerIntensityUnoccupied(atof(attributeValue));
    } else if(attributeName == "elecpowerappliancesoccupied"){
      setElecPowerAppliancesOccupied(atof(attributeValue));
    } else if(attributeName == "elecpowerappliancesunoccupied"){
      setElecPowerAppliancesUnoccupied(atof(attributeValue));
    } else if(attributeName == "gaspowerappliancesoccupied"){
      setGasPowerAppliancesOccupied(atof(attributeValue));
    } else if(attributeName == "gaspowerappliancesunoccupied"){
      setGasPowerAppliancesUnoccupied(atof(attributeValue));
    } else if(attributeName == "exteriorlightingpower"){
      setExteriorLightingPower(atof(attributeValue));
    } else if(attributeName == "hvacwastefactor"){
      setHvacWasteFactor(atof(attributeValue));
    } else if(attributeName == "hvacheatinglossfactor"){
      setHvacHeatingLossFactor(atof(attributeValue));
    } else if(attributeName == "hvaccoolinglossfactor"){
      setHvacCoolingLossFactor(atof(attributeValue));
    } else if(attributeName == "daylightsensorsystem"){
      setDaylightSensorSystem(atof(attributeValue));
    } else if(attributeName == "lightingoccupancysensorsystem"){
      setLightingOccupancySensorSystem(atof(attributeValue));
    } else if(attributeName == "constantilluminationcontrol"){//constantilluminaitoncontrol
      setConstantIlluminationControl(atof(attributeValue));
    } else if(attributeName == "coolingsystemcop"){
      setCoolingSystemCOP(atof(attributeValue));
    } else if(attributeName == "coolingsystemiplvtocopratio"){
      setCoolingSystemIPLVToCOPRatio (atof(attributeValue));
    } else if(attributeName == "heatingenergycarrier"){
      setHeatingEnergyCarrier(atof(attributeValue));
    } else if(attributeName == "heatingsystemefficiency"){
      setHeatingSystemEfficiency(atof(attributeValue));
    } else if(attributeName == "ventilationtype"){
      setVentilationType(atof(attributeValue));
    } else if(attributeName == "freshairflowrate"){
      setFreshAirFlowRate(atof(attributeValue));
    } else if(attributeName == "supplyexhaustrate"){
      setSupplyExhaustRate(atof(attributeValue));
    } else if(attributeName == "heatrecovery"){
      setHeatRecovery(atof(attributeValue));
    } else if(attributeName == "exhaustairrecirculation"){
      setExhaustAirRecirculation(atof(attributeValue));
    } else if(attributeName == "infiltration"){
      setBuildingAirLeakage(atof(attributeValue));
    } else if(attributeName == "dhwdemand"){
      setDhwDemand(atof(attributeValue));
    } else if(attributeName == "dhwsystemefficiency"){
      setDhwEfficiency(atof(attributeValue));
    } else if(attributeName == "dhwdistributionefficiency"){
      setDhwDistributionEfficiency(atof(attributeValue));
    } else if(attributeName == "dhwenergycarrier"){
      setDhwEnergyCarrier(atof(attributeValue));
    } else if(attributeName == "bemtype"){
      setBemType(atof(attributeValue));
    } else if(attributeName == "interiorheatcapacity"){
      setInteriorHeatCapacity(atof(attributeValue));
    } else if(attributeName == "exteriorheatcapacity"){
      setExteriorHeatCapacity(atof(attributeValue));
    } else if(attributeName == "heatingpumpcontrol"){
      setHeatingPumpControl(atof(attributeValue));
    } else if(attributeName == "coolingpumpcontrol"){
      setCoolingPumpControl(atof(attributeValue));
    } else if(attributeName == "heatgainperperson"){
      setHeatGainPerPerson(atof(attributeValue));
                                    //specificFanPower
    } else if(attributeName == "specificfanpower"){
      setSpecificFanPower(atof(attributeValue));
    } else if(attributeName == "fanflowcontrolfactor"){
      setFanFlowControlFactor(atof(attributeValue));
    } else if(attributeName == "roofuvalue"){
      setRoofUValue(atof(attributeValue));
    } else if(attributeName == "roofsolarabsorption"){
      setRoofSolarAbsorption(atof(attributeValue));
    } else if(attributeName == "roofthermalemissivity"){
      setRoofThermalEmissivity(atof(attributeValue));
    } else if(attributeName == "skylightuvalue"){
      setSkylightUvalue(atof(attributeValue));
    } else if(attributeName == "skylightshgc"){
      setSkylightSHGC(atof(attributeValue));
    } else if(attributeName == "wallareas"){
      setWallAreaS(atof(attributeValue));
    } else if(attributeName == "wallarease"){
      setWallAreaSE(atof(attributeValue));
    } else if(attributeName == "wallareae"){
      setWallAreaE(atof(attributeValue));
    } else if(attributeName == "wallareane"){
      setWallAreaNE(atof(attributeValue));
    } else if(attributeName == "wallarean"){
      setWallAreaN(atof(attributeValue));
    } else if(attributeName == "wallareanw"){
      setWallAreaNW(atof(attributeValue));
    } else if(attributeName == "wallareaw"){
      setWallAreaW(atof(attributeValue));
    } else if(attributeName == "wallareasw"){
      setWallAreaSW(atof(attributeValue));
    } else if(attributeName == "roofarea"){
      setRoofArea(atof(attributeValue));
    } else if(attributeName == "windowareas"){
      setWindowAreaS(atof(attributeValue));
    } else if(attributeName == "windowarease"){
      setWindowAreaSE(atof(attributeValue));
    } else if(attributeName == "windowareae"){
      setWindowAreaE(atof(attributeValue));
    } else if(attributeName == "windowareane"){
      setWindowAreaNE(atof(attributeValue));
    } else if(attributeName == "windowarean"){
      setWindowAreaN(atof(attributeValue));
    } else if(attributeName == "windowareanw"){
      setWindowAreaNW(atof(attributeValue));
    } else if(attributeName == "windowareaw"){
      setWindowAreaW(atof(attributeValue));
    } else if(attributeName == "windowareasw"){
      setWindowAreaSW(atof(attributeValue));
    } else if(attributeName == "skylightarea"){
      setSkylightArea(atof(attributeValue));
    } else if(attributeName == "coolingoccupiedsetpoint"){
      setCoolingOccupiedSetpoint(atof(attributeValue));
    } else if(attributeName == "coolingunoccupiedsetpoint"){
      setCoolingUnoccupiedSetpoint(atof(attributeValue));
    } else if(attributeName == "heatingoccupiedsetpoint"){
      setHeatingOccupiedSetpoint(atof(attributeValue));
    } else if(attributeName == "heatingunoccupiedsetpoint"){//weatherFilePath
      setHeatingUnoccupiedSetpoint(atof(attributeValue));
    } else if(attributeName == "weatherfilepath"){//weatherFilePath
      setWeatherFilePath(openstudio::toPath(linesplit[1]));
    } else if(boost::starts_with(attributeName.c_str(),"window") || 
              boost::starts_with(attributeName.c_str(),"wall") ) {
      parseStructure(attributeName,attributeValue);//avoid max nested ifs.  Might be better to change to a map eventually
    }    
    else {
      LOG(Error, "Unknown Attribute: "<< attributeName << " = " << attributeValue);
    }    
  }
  void UserModel::loadBuilding(const openstudio::path &buildingFile){
    string line;
    ifstream inputFile(openstudio::toString(buildingFile).c_str());
    if (inputFile.is_open()) {
      while (inputFile.good()) {
        getline (inputFile,line);
        if(line.size() > 0 && line[0] == '#')
          continue;
        parseLine(line);
      }
      inputFile.close();
    }
    else {
      LOG(Error, "Unable to open file" << openstudio::toString(buildingFile));
    }
  }
  int UserModel::weatherState(const std::string &header){
    if(header == "solar")
      return 1;
    else if(header == "hdbt")
      return 2;
    else if(header == "hEgh")
      return 3;
    else if(header == "mEgh")
      return 4;
    else if(header == "mdbt")
      return 5;
    else if(header == "mwind")
      return 6;
    else 
      return -1;
  }

  std::shared_ptr<WeatherData> UserModel::loadWeather(){
    openstudio::path weatherFilename;
    //see if weather file path is absolute path
    //if so, use it, else assemble relative path
    if(boost::filesystem::exists( _weatherFilePath ))
    {
      weatherFilename = _weatherFilePath;      
    }
    else
    {
      weatherFilename = _dataFile.parent_path() / _weatherFilePath;
      if ( !boost::filesystem::exists( weatherFilename ) )
      {
        LOG(Error, "Weather File Not Found: " << openstudio::toString(_weatherFilePath));
        _valid = false;
        return std::shared_ptr<WeatherData>();
      }
    }
    EpwData edata(weatherFilename);

    Matrix _msolar(12,8,0);
    Matrix _mhdbt(12,24,0);
    Matrix _mhEgh(12,24,0);
    Vector _mEgh(12);
    Vector _mdbt(12);
    Vector _mwind(12);

    edata.toISOData(_msolar, _mhdbt, _mhEgh, _mEgh, _mdbt, _mwind);

    std::shared_ptr<WeatherData> wdata(new WeatherData);
    wdata->setMdbt(_mdbt);
    wdata->setMEgh(_mEgh);
    wdata->setMhdbt(_mhdbt);
    wdata->setMhEgh(_mhEgh);
    wdata->setMsolar(_msolar);
    wdata->setMwind(_mwind);
    
    return wdata;
  }

  void UserModel::load(const openstudio::path &buildingFile){
    _dataFile = buildingFile;
    _valid = true;
    if ( !boost::filesystem::exists( buildingFile ) )
    {
      LOG(Error, "ISO Model File Not Found: " << openstudio::toString(buildingFile));
      _valid = false;
      return;
    }
    loadBuilding(buildingFile);
    _weather = loadWeather();
  }


  void UserModel::save(const openstudio::path &t_buildingFile) const 
  {

    std::ofstream ofile(openstudio::toString(t_buildingFile).c_str());

    ofile << "terrainClass = " << _terrainClass << std::endl;
    ofile << "floorArea = " << _floorArea << std::endl;
    ofile << "buildingHeight = " << _buildingHeight << std::endl;
    ofile << "buildingOccupancyFrom = " << _buildingOccupancyFrom << std::endl;
    ofile << "buildingOccupancyTo = " << _buildingOccupancyTo << std::endl;
    ofile << "equivFullLoadOccupancyFrom = " << _equivFullLoadOccupancyFrom << std::endl;
    ofile << "equivFullLoadOccupancyTo = " << _equivFullLoadOccupancyTo << std::endl;
    ofile << "peopleDensityOccupied = " << _peopleDensityOccupied << std::endl;
    ofile << "peopleDensityUnoccupied = " << _peopleDensityUnoccupied << std::endl;
    ofile << "heatingOccupiedSetpoint = " << _heatingOccupiedSetpoint << std::endl;
    ofile << "heatingUnoccupiedSetpoint = " << _heatingUnoccupiedSetpoint << std::endl;
    ofile << "coolingOccupiedSetpoint = " << _coolingOccupiedSetpoint << std::endl;
    ofile << "coolingUnoccupiedSetpoint = " << _coolingUnoccupiedSetpoint << std::endl;
    ofile << "elecPowerAppliancesOccupied = " << _elecPowerAppliancesOccupied << std::endl;
    ofile << "elecPowerAppliancesUnoccupied = " << _elecPowerAppliancesUnoccupied << std::endl;
    ofile << "gasPowerAppliancesOccupied = " << _gasPowerAppliancesOccupied << std::endl;
    ofile << "gasPowerAppliancesUnoccupied = " << _gasPowerAppliancesUnoccupied << std::endl;
    ofile << "lightingPowerIntensityOccupied = " << _lightingPowerIntensityOccupied << std::endl;
    ofile << "lightingPowerIntensityUnoccupied = " << _lightingPowerIntensityUnoccupied << std::endl;
    ofile << "exteriorLightingPower = " << _exteriorLightingPower << std::endl;
    ofile << "daylightSensorSystem = " << _daylightSensorSystem << std::endl;
    ofile << "lightingOccupancySensorSystem = " << _lightingOccupancySensorSystem << std::endl;
    ofile << "constantIlluminationControl = " << _constantIlluminationControl << std::endl;
    ofile << "coolingSystemCOP = " << _coolingSystemCOP << std::endl;
    ofile << "coolingSystemIPLVToCOPRatio = " << _coolingSystemIPLVToCOPRatio << std::endl;
    ofile << "heatingEnergyCarrier = " << _heatingEnergyCarrier << std::endl;
    ofile << "heatingSystemEfficiency = " << _heatingSystemEfficiency << std::endl;
    ofile << "ventilationType = " << _ventilationType << std::endl;
    ofile << "freshAirFlowRate = " << _freshAirFlowRate << std::endl;
    ofile << "supplyExhaustRate = " << _supplyExhaustRate << std::endl;
    ofile << "heatRecovery = " << _heatRecovery << std::endl;
    ofile << "exhaustAirRecirculation = " << _exhaustAirRecirculation << std::endl;
    ofile << "buildingAirLeakage = " << _buildingAirLeakage << std::endl;
    ofile << "dhwDemand = " << _dhwDemand << std::endl;
    ofile << "dhwSystemEfficiency = " << _dhwEfficiency << std::endl;
    ofile << "dhwDistributionSystem = " << _dhwDistributionSystem << std::endl;
    ofile << "dhwEnergyCarrier = " << _dhwEnergyCarrier << std::endl;
    ofile << "bemType = " << _bemType << std::endl;
    ofile << "interiorHeatCapacity = " << _interiorHeatCapacity << std::endl;
    ofile << "specificFanPower = " << _specificFanPower << std::endl;
    ofile << "fanFlowControlFactor = " << _fanFlowControlFactor << std::endl;
    ofile << "roofSHGC = " << _roofSHGC << std::endl;

    ofile << "wallAreaS = " << _wallAreaS << std::endl;
    ofile << "wallAreaSE = " << _wallAreaSE << std::endl;
    ofile << "wallAreaE = " << _wallAreaE << std::endl;
    ofile << "wallAreaNE = " << _wallAreaNE << std::endl;
    ofile << "wallAreaN = " << _wallAreaN << std::endl;
    ofile << "wallAreaNW = " << _wallAreaNW << std::endl;
    ofile << "wallAreaW = " << _wallAreaW << std::endl;
    ofile << "wallAreaSW = " << _wallAreaSW << std::endl;
    ofile << "roofArea = " << _roofArea << std::endl;
    
    ofile << "wallUvalueS = " << _wallUvalueS << std::endl;
    ofile << "wallUvalueSE = " << _wallUvalueSE << std::endl;
    ofile << "wallUvalueE = " << _wallUvalueE << std::endl;
    ofile << "wallUvalueNE = " << _wallUvalueNE << std::endl;
    ofile << "wallUvalueN = " << _wallUvalueN << std::endl;
    ofile << "wallUvalueNW = " << _wallUvalueNW << std::endl;
    ofile << "wallUvalueW = " << _wallUvalueW << std::endl;
    ofile << "wallUvalueSW = " << _wallUvalueSW << std::endl;
    ofile << "roofUValue = " << _roofUValue << std::endl;
    
    ofile << "wallSolarAbsorptionS = " << _wallSolarAbsorptionS << std::endl;
    ofile << "wallSolarAbsorptionSE = " << _wallSolarAbsorptionSE << std::endl;
    ofile << "wallSolarAbsorptionE = " << _wallSolarAbsorptionE << std::endl;
    ofile << "wallSolarAbsorptionNE = " << _wallSolarAbsorptionNE << std::endl;
    ofile << "wallSolarAbsorptionN = " << _wallSolarAbsorptionN << std::endl;
    ofile << "wallSolarAbsorptionNW = " << _wallSolarAbsorptionNW << std::endl;
    ofile << "wallSolarAbsorptionW = " << _wallSolarAbsorptionW << std::endl;
    ofile << "wallSolarAbsorptionSW = " << _wallSolarAbsorptionSW << std::endl;
    ofile << "roofSolarAbsorption = " << _roofSolarAbsorption << std::endl;

    ofile << "wallThermalEmissivityS = " << _wallThermalEmissivityS << std::endl;
    ofile << "wallThermalEmissivitySE = " << _wallThermalEmissivitySE << std::endl;
    ofile << "wallThermalEmissivityE = " << _wallThermalEmissivityE << std::endl;
    ofile << "wallThermalEmissivityNE = " << _wallThermalEmissivityNE << std::endl;
    ofile << "wallThermalEmissivityN = " << _wallThermalEmissivityN << std::endl;
    ofile << "wallThermalEmissivityNW = " << _wallThermalEmissivityNW << std::endl;
    ofile << "wallThermalEmissivityW = " << _wallThermalEmissivityW << std::endl;
    ofile << "wallThermalEmissivitySW = " << _wallThermalEmissivitySW << std::endl;
    ofile << "roofThermalEmissivity = " << _roofThermalEmissivity << std::endl;

    ofile << "windowAreaS = " << _windowAreaS << std::endl;
    ofile << "windowAreaSE = " << _windowAreaSE << std::endl;
    ofile << "windowAreaE = " << _windowAreaE << std::endl;
    ofile << "windowAreaNE = " << _windowAreaNE << std::endl;
    ofile << "windowAreaN = " << _windowAreaN << std::endl;
    ofile << "windowAreaNW = " << _windowAreaNW << std::endl;
    ofile << "windowAreaW = " << _windowAreaW << std::endl;
    ofile << "windowAreaSW = " << _windowAreaSW << std::endl;
    ofile << "skylightArea = " << _skylightArea << std::endl;

    ofile << "windowUvalueS = " << _windowUvalueS << std::endl;
    ofile << "windowUvalueSE = " << _windowUvalueSE << std::endl;
    ofile << "windowUvalueE = " << _windowUvalueE << std::endl;
    ofile << "windowUvalueNE = " << _windowUvalueNE << std::endl;
    ofile << "windowUvalueN = " << _windowUvalueN << std::endl;
    ofile << "windowUvalueNW = " << _windowUvalueNW << std::endl;
    ofile << "windowUvalueW = " << _windowUvalueW << std::endl;
    ofile << "windowUvalueSW = " << _windowUvalueSW << std::endl;
    ofile << "skylightUvalue = " << _skylightUvalue << std::endl;

    ofile << "windowSHGCS = " << _windowSHGCS << std::endl;
    ofile << "windowSHGCSE = " << _windowSHGCSE << std::endl;
    ofile << "windowSHGCE = " << _windowSHGCE << std::endl;
    ofile << "windowSHGCNE = " << _windowSHGCNE << std::endl;
    ofile << "windowSHGCN = " << _windowSHGCN << std::endl;
    ofile << "windowSHGCNW = " << _windowSHGCNW << std::endl;
    ofile << "windowSHGCW = " << _windowSHGCW << std::endl;
    ofile << "windowSHGCSW = " << _windowSHGCSW << std::endl;
    ofile << "skylightSHGC = " << _skylightSHGC << std::endl;

    ofile << "windowSCFS = " << _windowSCFS << std::endl;
    ofile << "windowSCFSE = " << _windowSCFSE << std::endl;
    ofile << "windowSCFE = " << _windowSCFE << std::endl;
    ofile << "windowSCFNE = " << _windowSCFNE << std::endl;
    ofile << "windowSCFN = " << _windowSCFN << std::endl;
    ofile << "windowSCFNW = " << _windowSCFNW << std::endl;
    ofile << "windowSCFW = " << _windowSCFW << std::endl;
    ofile << "windowSCFSW = " << _windowSCFSW << std::endl;

    ofile << "windowSDFS = " << _windowSDFS << std::endl;
    ofile << "windowSDFSE = " << _windowSDFSE << std::endl;
    ofile << "windowSDFE = " << _windowSDFE << std::endl;
    ofile << "windowSDFNE = " << _windowSDFNE << std::endl;
    ofile << "windowSDFN = " << _windowSDFN << std::endl;
    ofile << "windowSDFNW = " << _windowSDFNW << std::endl;
    ofile << "windowSDFW = " << _windowSDFW << std::endl;
    ofile << "windowSDFSW = " << _windowSDFSW << std::endl;

    ofile << "exteriorHeatCapacity = " << _exteriorHeatCapacity << std::endl;
    ofile << "infiltration = " << _infiltration << std::endl;
    ofile << "hvacWasteFactor = " << _hvacWasteFactor << std::endl;
    ofile << "hvacHeatingLossFactor = " << _hvacHeatingLossFactor << std::endl;
    ofile << "hvacCoolingLossFactor = " << _hvacCoolingLossFactor << std::endl;
    ofile << "dhwDistributionEfficiency = " << _dhwDistributionEfficiency << std::endl;
    ofile << "heatingPumpControl = " << _heatingPumpControl << std::endl;
    ofile << "coolingPumpControl = " << _coolingPumpControl << std::endl;
    ofile << "heatGainPerPerson = " << _heatGainPerPerson << std::endl;




  }




} // isomodel
} // openstudio

