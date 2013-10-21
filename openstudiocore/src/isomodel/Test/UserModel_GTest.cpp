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

#include <gtest/gtest.h>
#include <isomodel/Test/ISOModelFixture.hpp>

#include <isomodel/UserModel.hpp>
#include <isomodel/SimModel.hpp>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::isomodel;
using namespace openstudio;

TEST_F(ISOModelFixture, UserModel)
{
  UserModel userModel;
  userModel.setTerrainClass(0.366569597990189);
  userModel.setFloorArea(0.13797878192703);
  userModel.setBuildingHeight(0.425419263581922);
  userModel.setBuildingOccupancyFrom(0.665995505182317);
  userModel.setBuildingOccupancyTo(0.400372234106352);
  userModel.setEquivFullLoadOccupancyFrom(0.254850243633116);
  userModel.setEquivFullLoadOccupancyTo(0.713362082549865);
  userModel.setPeopleDensityOccupied(0.0453028919599623);
  userModel.setPeopleDensityUnoccupied(0.374398515315959);
  userModel.setHeatingOccupiedSetpoint(0.308476836073534);
  userModel.setHeatingUnoccupiedSetpoint(0.96115521837837);
  userModel.setCoolingOccupiedSetpoint(0.0182141291000549);
  userModel.setCoolingUnoccupiedSetpoint(0.47279017381788);
  userModel.setElecPowerAppliancesOccupied(0.0159043563230605);
  userModel.setElecPowerAppliancesUnoccupied(0.877197046873451);
  userModel.setGasPowerAppliancesOccupied(0.413231779700794);
  userModel.setGasPowerAppliancesUnoccupied(0.735954395099727);
  userModel.setLightingPowerIntensityOccupied(0.827607402688993);
  userModel.setLightingPowerIntensityUnoccupied(0.210627783574828);
  userModel.setExteriorLightingPower(0.688613300586997);
  userModel.setDaylightSensorSystem(0.952066322499152);
  userModel.setLightingOccupancySensorSystem(0.191200546809349);
  userModel.setConstantIlluminationControl(0.295905191092175);
  userModel.setCoolingSystemCOP(0.977647331541828);
  userModel.setCoolingSystemIPLVToCOPRatio(0.86953551426846);
  userModel.setHeatingEnergyCarrier(0.263002176275548);
  userModel.setHeatingSystemEfficiency(0.710454137223511);
  userModel.setVentilationType(0.0841726806995226);
  userModel.setFreshAirFlowRate(0.903704085971796);
  userModel.setSupplyExhaustRate(0.724248760195895);
  userModel.setHeatRecovery(0.49985550202677);
  userModel.setExhaustAirRecirclation(0.846564029275989);
  userModel.setBuildingAirLeakage(0.863462404238138);
  userModel.setDhwDemand(0.881916031629701);
  userModel.setDhwEfficiency(0.105230439331114);
  userModel.setDhwDistributionSystem(0.791092991177229);
  userModel.setDhwEnergyCarrier(0.789220796023767);
  userModel.setBemType(0.293374792126407);
  userModel.setInteriorHeatCapacity(0.590020871911987);
  userModel.setSpecificFanPower(0.256509943938684);
  userModel.setFanFlowControlFactor(0.171213718831364);
  userModel.setRoofSHGC(0.577629926945883);
  userModel.setWallAreaS(0.351700449083525);
  userModel.setWallAreaSE(0.638796629077831);
  userModel.setWallAreaE(0.713877579934114);
  userModel.setWallAreaNE(0.0544635225207429);
  userModel.setWallAreaN(0.713312047950444);
  userModel.setWallAreaNW(0.316883353660591);
  userModel.setWallAreaW(0.963602582100428);
  userModel.setWallAreaSW(0.950016805325306);
  userModel.setRoofArea(0.401348851386038);
  userModel.setWallUvalueS(0.479173557940235);
  userModel.setWallUvalueSE(0.598665235979741);
  userModel.setWallUvalueE(0.592537203218594);
  userModel.setWallUvalueNE(0.317076189922438);
  userModel.setWallUvalueN(0.857610736439619);
  userModel.setWallUvalueNW(0.494959077705813);
  userModel.setWallUvalueW(0.710302412967452);
  userModel.setWallUvalueSW(0.755347362509827);
  userModel.setRoofUValue(0.508937055452772);
  userModel.setWallSolarAbsorptionS(0.91461449925898);
  userModel.setWallSolarAbsorptionSE(0.928931093579599);
  userModel.setWallSolarAbsorptionE(0.435542934183637);
  userModel.setWallSolarAbsorptionNE(0.793609339380358);
  userModel.setWallSolarAbsorptionN(0.902389688647158);
  userModel.setWallSolarAbsorptionNW(0.336318028981842);
  userModel.setWallSolarAbsorptionW(0.37153202026125);
  userModel.setWallSolarAbsorptionSW(0.418783890513947);
  userModel.setRoofSolarAbsorption(0.223964378497134);
  userModel.setWallThermalEmissivityS(0.583098358149272);
  userModel.setWallThermalEmissivitySE(0.141381800284656);
  userModel.setWallThermalEmissivityE(0.837222292557137);
  userModel.setWallThermalEmissivityNE(0.49538931179426);
  userModel.setWallThermalEmissivityN(0.871379477772421);
  userModel.setWallThermalEmissivityNW(0.170422643070764);
  userModel.setWallThermalEmissivityW(0.761063022176878);
  userModel.setWallThermalEmissivitySW(0.186495812844654);
  userModel.setRoofThermalEmissivity(0.907924653508436);
  userModel.setWindowAreaS(0.606074602940241);
  userModel.setWindowAreaSE(0.404342798081098);
  userModel.setWindowAreaE(0.0612029472801275);
  userModel.setWindowAreaNE(0.289843899154198);
  userModel.setWindowAreaN(0.540818859803666);
  userModel.setWindowAreaNW(0.41253025448177);
  userModel.setWindowAreaW(0.014956739105872);
  userModel.setWindowAreaSW(0.899839246505665);
  userModel.setSkylightArea(0.135269594888848);
  userModel.setWindowUvalueS(0.232560858068808);
  userModel.setWindowUvalueSE(0.431164085960324);
  userModel.setWindowUvalueE(0.00477022329159593);
  userModel.setWindowUvalueNE(0.71516207439754);
  userModel.setWindowUvalueN(0.280649559810701);
  userModel.setWindowUvalueNW(0.355908313708148);
  userModel.setWindowUvalueW(0.112872065367925);
  userModel.setWindowUvalueSW(0.398611796542468);
  userModel.setSkylightUvalue(0.712266965230007);
  userModel.setWindowSHGCS(0.255902968619523);
  userModel.setWindowSHGCSE(0.401818741289806);
  userModel.setWindowSHGCE(0.536223533889905);
  userModel.setWindowSHGCNE(0.251096592939623);
  userModel.setWindowSHGCN(0.931256342309665);
  userModel.setWindowSHGCNW(0.896808057579816);
  userModel.setWindowSHGCW(0.981291583238567);
  userModel.setWindowSHGCSW(0.148339469077549);
  userModel.setSkylightSHGC(0.531228639942613);
  userModel.setWindowSCFS(0.719753126248692);
  userModel.setWindowSCFSE(0.719295130996734);
  userModel.setWindowSCFE(0.62587251635714);
  userModel.setWindowSCFNE(0.789338364373816);
  userModel.setWindowSCFN(0.620542267432122);
  userModel.setWindowSCFNW(0.300503015955268);
  userModel.setWindowSCFW(0.128976467360588);
  userModel.setWindowSCFSW(0.947178709804832);
  userModel.setWindowSDFS(0.902216926946315);
  userModel.setWindowSDFSE(0.632486442302954);
  userModel.setWindowSDFE(0.719004834647601);
  userModel.setWindowSDFNE(0.504956302525102);
  userModel.setWindowSDFN(0.212427137938556);
  userModel.setWindowSDFNW(0.0746662195816253);
  userModel.setWindowSDFW(0.970579615803331);
  userModel.setWindowSDFSW(0.617489329894299);
  userModel.setExteriorHeatCapacity(0.523964673586454);
  userModel.setInfiltration(0.139585598177502);
  userModel.setHvacWasteFactor(0.287554068015519);
  userModel.setHvacHeatingLossFactor(0.801121347575538);
  userModel.setHvacCoolingLossFactor(0.919509843310335);
  userModel.setDhwDistributionEfficiency(0.33038965168355);
  userModel.setHeatingPumpControl(0.625403806654488);
  userModel.setCoolingPumpControl(0.0184589116025784);
  userModel.setHeatGainPerPerson(0.976673863929532);

  EXPECT_EQ(0.366569597990189, userModel.terrainClass());
  EXPECT_EQ(0.13797878192703, userModel.floorArea());
  EXPECT_EQ(0.425419263581922, userModel.buildingHeight());
  EXPECT_EQ(0.665995505182317, userModel.buildingOccupancyFrom());
  EXPECT_EQ(0.400372234106352, userModel.buildingOccupancyTo());
  EXPECT_EQ(0.254850243633116, userModel.equivFullLoadOccupancyFrom());
  EXPECT_EQ(0.713362082549865, userModel.equivFullLoadOccupancyTo());
  EXPECT_EQ(0.0453028919599623, userModel.peopleDensityOccupied());
  EXPECT_EQ(0.374398515315959, userModel.peopleDensityUnoccupied());
  EXPECT_EQ(0.308476836073534, userModel.heatingOccupiedSetpoint());
  EXPECT_EQ(0.96115521837837, userModel.heatingUnoccupiedSetpoint());
  EXPECT_EQ(0.0182141291000549, userModel.coolingOccupiedSetpoint());
  EXPECT_EQ(0.47279017381788, userModel.coolingUnoccupiedSetpoint());
  EXPECT_EQ(0.0159043563230605, userModel.elecPowerAppliancesOccupied());
  EXPECT_EQ(0.877197046873451, userModel.elecPowerAppliancesUnoccupied());
  EXPECT_EQ(0.413231779700794, userModel.gasPowerAppliancesOccupied());
  EXPECT_EQ(0.735954395099727, userModel.gasPowerAppliancesUnoccupied());
  EXPECT_EQ(0.827607402688993, userModel.lightingPowerIntensityOccupied());
  EXPECT_EQ(0.210627783574828, userModel.lightingPowerIntensityUnoccupied());
  EXPECT_EQ(0.688613300586997, userModel.exteriorLightingPower());
  EXPECT_EQ(0.952066322499152, userModel.daylightSensorSystem());
  EXPECT_EQ(0.191200546809349, userModel.lightingOccupancySensorSystem());
  EXPECT_EQ(0.295905191092175, userModel.constantIlluminationControl());
  EXPECT_EQ(0.977647331541828, userModel.coolingSystemCOP());
  EXPECT_EQ(0.86953551426846, userModel.coolingSystemIPLVToCOPRatio());
  EXPECT_EQ(0.263002176275548, userModel.heatingEnergyCarrier());
  EXPECT_EQ(0.710454137223511, userModel.heatingSystemEfficiency());
  EXPECT_EQ(0.0841726806995226, userModel.ventilationType());
  EXPECT_EQ(0.903704085971796, userModel.freshAirFlowRate());
  EXPECT_EQ(0.724248760195895, userModel.supplyExhaustRate());
  EXPECT_EQ(0.49985550202677, userModel.heatRecovery());
  EXPECT_EQ(0.846564029275989, userModel.exhaustAirRecirclation());
  EXPECT_EQ(0.863462404238138, userModel.buildingAirLeakage());
  EXPECT_EQ(0.881916031629701, userModel.dhwDemand());
  EXPECT_EQ(0.105230439331114, userModel.dhwEfficiency());
  EXPECT_EQ(0.791092991177229, userModel.dhwDistributionSystem());
  EXPECT_EQ(0.789220796023767, userModel.dhwEnergyCarrier());
  EXPECT_EQ(0.293374792126407, userModel.bemType());
  EXPECT_EQ(0.590020871911987, userModel.interiorHeatCapacity());
  EXPECT_EQ(0.256509943938684, userModel.specificFanPower());
  EXPECT_EQ(0.171213718831364, userModel.fanFlowControlFactor());
  EXPECT_EQ(0.577629926945883, userModel.roofSHGC());
  EXPECT_EQ(0.351700449083525, userModel.wallAreaS());
  EXPECT_EQ(0.638796629077831, userModel.wallAreaSE());
  EXPECT_EQ(0.713877579934114, userModel.wallAreaE());
  EXPECT_EQ(0.0544635225207429, userModel.wallAreaNE());
  EXPECT_EQ(0.713312047950444, userModel.wallAreaN());
  EXPECT_EQ(0.316883353660591, userModel.wallAreaNW());
  EXPECT_EQ(0.963602582100428, userModel.wallAreaW());
  EXPECT_EQ(0.950016805325306, userModel.wallAreaSW());
  EXPECT_EQ(0.401348851386038, userModel.roofArea());
  EXPECT_EQ(0.479173557940235, userModel.wallUvalueS());
  EXPECT_EQ(0.598665235979741, userModel.wallUvalueSE());
  EXPECT_EQ(0.592537203218594, userModel.wallUvalueE());
  EXPECT_EQ(0.317076189922438, userModel.wallUvalueNE());
  EXPECT_EQ(0.857610736439619, userModel.wallUvalueN());
  EXPECT_EQ(0.494959077705813, userModel.wallUvalueNW());
  EXPECT_EQ(0.710302412967452, userModel.wallUvalueW());
  EXPECT_EQ(0.755347362509827, userModel.wallUvalueSW());
  EXPECT_EQ(0.508937055452772, userModel.roofUValue());
  EXPECT_EQ(0.91461449925898, userModel.wallSolarAbsorptionS());
  EXPECT_EQ(0.928931093579599, userModel.wallSolarAbsorptionSE());
  EXPECT_EQ(0.435542934183637, userModel.wallSolarAbsorptionE());
  EXPECT_EQ(0.793609339380358, userModel.wallSolarAbsorptionNE());
  EXPECT_EQ(0.902389688647158, userModel.wallSolarAbsorptionN());
  EXPECT_EQ(0.336318028981842, userModel.wallSolarAbsorptionNW());
  EXPECT_EQ(0.37153202026125, userModel.wallSolarAbsorptionW());
  EXPECT_EQ(0.418783890513947, userModel.wallSolarAbsorptionSW());
  EXPECT_EQ(0.223964378497134, userModel.roofSolarAbsorption());
  EXPECT_EQ(0.583098358149272, userModel.wallThermalEmissivityS());
  EXPECT_EQ(0.141381800284656, userModel.wallThermalEmissivitySE());
  EXPECT_EQ(0.837222292557137, userModel.wallThermalEmissivityE());
  EXPECT_EQ(0.49538931179426, userModel.wallThermalEmissivityNE());
  EXPECT_EQ(0.871379477772421, userModel.wallThermalEmissivityN());
  EXPECT_EQ(0.170422643070764, userModel.wallThermalEmissivityNW());
  EXPECT_EQ(0.761063022176878, userModel.wallThermalEmissivityW());
  EXPECT_EQ(0.186495812844654, userModel.wallThermalEmissivitySW());
  EXPECT_EQ(0.907924653508436, userModel.roofThermalEmissivity());
  EXPECT_EQ(0.606074602940241, userModel.windowAreaS());
  EXPECT_EQ(0.404342798081098, userModel.windowAreaSE());
  EXPECT_EQ(0.0612029472801275, userModel.windowAreaE());
  EXPECT_EQ(0.289843899154198, userModel.windowAreaNE());
  EXPECT_EQ(0.540818859803666, userModel.windowAreaN());
  EXPECT_EQ(0.41253025448177, userModel.windowAreaNW());
  EXPECT_EQ(0.014956739105872, userModel.windowAreaW());
  EXPECT_EQ(0.899839246505665, userModel.windowAreaSW());
  EXPECT_EQ(0.135269594888848, userModel.skylightArea());
  EXPECT_EQ(0.232560858068808, userModel.windowUvalueS());
  EXPECT_EQ(0.431164085960324, userModel.windowUvalueSE());
  EXPECT_EQ(0.00477022329159593, userModel.windowUvalueE());
  EXPECT_EQ(0.71516207439754, userModel.windowUvalueNE());
  EXPECT_EQ(0.280649559810701, userModel.windowUvalueN());
  EXPECT_EQ(0.355908313708148, userModel.windowUvalueNW());
  EXPECT_EQ(0.112872065367925, userModel.windowUvalueW());
  EXPECT_EQ(0.398611796542468, userModel.windowUvalueSW());
  EXPECT_EQ(0.712266965230007, userModel.skylightUvalue());
  EXPECT_EQ(0.255902968619523, userModel.windowSHGCS());
  EXPECT_EQ(0.401818741289806, userModel.windowSHGCSE());
  EXPECT_EQ(0.536223533889905, userModel.windowSHGCE());
  EXPECT_EQ(0.251096592939623, userModel.windowSHGCNE());
  EXPECT_EQ(0.931256342309665, userModel.windowSHGCN());
  EXPECT_EQ(0.896808057579816, userModel.windowSHGCNW());
  EXPECT_EQ(0.981291583238567, userModel.windowSHGCW());
  EXPECT_EQ(0.148339469077549, userModel.windowSHGCSW());
  EXPECT_EQ(0.531228639942613, userModel.skylightSHGC());
  EXPECT_EQ(0.719753126248692, userModel.windowSCFS());
  EXPECT_EQ(0.719295130996734, userModel.windowSCFSE());
  EXPECT_EQ(0.62587251635714, userModel.windowSCFE());
  EXPECT_EQ(0.789338364373816, userModel.windowSCFNE());
  EXPECT_EQ(0.620542267432122, userModel.windowSCFN());
  EXPECT_EQ(0.300503015955268, userModel.windowSCFNW());
  EXPECT_EQ(0.128976467360588, userModel.windowSCFW());
  EXPECT_EQ(0.947178709804832, userModel.windowSCFSW());
  EXPECT_EQ(0.902216926946315, userModel.windowSDFS());
  EXPECT_EQ(0.632486442302954, userModel.windowSDFSE());
  EXPECT_EQ(0.719004834647601, userModel.windowSDFE());
  EXPECT_EQ(0.504956302525102, userModel.windowSDFNE());
  EXPECT_EQ(0.212427137938556, userModel.windowSDFN());
  EXPECT_EQ(0.0746662195816253, userModel.windowSDFNW());
  EXPECT_EQ(0.970579615803331, userModel.windowSDFW());
  EXPECT_EQ(0.617489329894299, userModel.windowSDFSW());
  EXPECT_EQ(0.523964673586454, userModel.exteriorHeatCapacity());
  EXPECT_EQ(0.139585598177502, userModel.infiltration());
  EXPECT_EQ(0.287554068015519, userModel.hvacWasteFactor());
  EXPECT_EQ(0.801121347575538, userModel.hvacHeatingLossFactor());
  EXPECT_EQ(0.919509843310335, userModel.hvacCoolingLossFactor());
  EXPECT_EQ(0.33038965168355, userModel.dhwDistributionEfficiency());
  EXPECT_EQ(0.625403806654488, userModel.heatingPumpControl());
  EXPECT_EQ(0.0184589116025784, userModel.coolingPumpControl());
  EXPECT_EQ(0.976673863929532, userModel.heatGainPerPerson());

  std::string wpath = "test";
  userModel.load(wpath);
  EXPECT_FALSE(userModel.valid());
  userModel.setWeatherFilePath(wpath);
  EXPECT_EQ(userModel.weatherFilePath(), openstudio::toPath(wpath));
  userModel.loadWeather();
  EXPECT_FALSE(userModel.valid());

  path p = resourcesPath() / openstudio::toPath("isomodel/exampleModel.ISO");
  userModel.load(openstudio::toString(p));

  EXPECT_EQ(0.9, userModel.terrainClass());
  EXPECT_EQ(10000.0, userModel.floorArea());
  EXPECT_EQ(8.0, userModel.buildingHeight());
  EXPECT_EQ(1.0, userModel.buildingOccupancyFrom());
  EXPECT_EQ(5.0, userModel.buildingOccupancyTo());
  EXPECT_EQ(7.0, userModel.equivFullLoadOccupancyFrom());
  EXPECT_EQ(18.0, userModel.equivFullLoadOccupancyTo());
  EXPECT_EQ(22.386274698794701, userModel.peopleDensityOccupied());
  EXPECT_EQ(171.87446672181500, userModel.peopleDensityUnoccupied());
  EXPECT_EQ(24.0, userModel.heatingOccupiedSetpoint());
  EXPECT_EQ(24.0, userModel.heatingUnoccupiedSetpoint());
  EXPECT_EQ(28.0, userModel.coolingOccupiedSetpoint());
  EXPECT_EQ(28.0, userModel.coolingUnoccupiedSetpoint());
  EXPECT_EQ(8.9340456458692508, userModel.elecPowerAppliancesOccupied());
  EXPECT_EQ(1.1636399740730901, userModel.elecPowerAppliancesUnoccupied());
  EXPECT_EQ(26.183103184023899, userModel.gasPowerAppliancesOccupied());
  EXPECT_EQ(3.4102921249677900, userModel.gasPowerAppliancesUnoccupied());
  EXPECT_EQ(8.9340456458692508, userModel.lightingPowerIntensityOccupied());
  EXPECT_EQ(1.1636399740730901, userModel.lightingPowerIntensityUnoccupied());
  EXPECT_EQ(0.0, userModel.exteriorLightingPower());
  EXPECT_EQ(1.0, userModel.daylightSensorSystem());
  EXPECT_EQ(1.0, userModel.lightingOccupancySensorSystem());
  EXPECT_EQ(1.0, userModel.constantIlluminationControl());
  EXPECT_EQ(3.0, userModel.coolingSystemCOP());
  EXPECT_EQ(1.0, userModel.coolingSystemIPLVToCOPRatio());
  EXPECT_EQ(2.0, userModel.heatingEnergyCarrier());
  EXPECT_EQ(0.80000000000000004, userModel.heatingSystemEfficiency());
  EXPECT_EQ(1.0, userModel.ventilationType());
  EXPECT_EQ(5080, userModel.freshAirFlowRate());
  EXPECT_EQ(5080, userModel.supplyExhaustRate());
  EXPECT_EQ(0.0, userModel.heatRecovery());
  EXPECT_EQ(0.0, userModel.exhaustAirRecirclation());
  EXPECT_EQ(2.4610800585487800, userModel.buildingAirLeakage());
  EXPECT_EQ(0.0, userModel.dhwDemand());
  EXPECT_EQ(0.9, userModel.dhwEfficiency());
  EXPECT_EQ(0.791092991177229, userModel.dhwDistributionSystem());//unmodified by load
  EXPECT_EQ(1.0, userModel.dhwEnergyCarrier());
  EXPECT_EQ(1.0, userModel.bemType());
  EXPECT_EQ(224141.05370757400, userModel.interiorHeatCapacity());
  EXPECT_EQ(2.5, userModel.specificFanPower());
  EXPECT_EQ(1.0, userModel.fanFlowControlFactor());
  EXPECT_EQ(0.577629926945883, userModel.roofSHGC());//unmodified by load
  EXPECT_EQ(480.0, userModel.wallAreaS());
  EXPECT_EQ(0.0, userModel.wallAreaSE());
  EXPECT_EQ(240, userModel.wallAreaE());
  EXPECT_EQ(0.0, userModel.wallAreaNE());
  EXPECT_EQ(480.0, userModel.wallAreaN());
  EXPECT_EQ(0.0, userModel.wallAreaNW());
  EXPECT_EQ(240.0, userModel.wallAreaW());
  EXPECT_EQ(0.0, userModel.wallAreaSW());
  EXPECT_EQ(10000.0, userModel.roofArea());
  EXPECT_EQ(0.30930199989518098, userModel.wallUvalueS());
  EXPECT_EQ(0.29999999999999999, userModel.wallUvalueSE());
  EXPECT_EQ(0.30930199989518098, userModel.wallUvalueE());
  EXPECT_EQ(0.29999999999999999, userModel.wallUvalueNE());
  EXPECT_EQ(0.30930199989518098, userModel.wallUvalueN());
  EXPECT_EQ(0.29999999999999999, userModel.wallUvalueNW());
  EXPECT_EQ(0.30930199989518098, userModel.wallUvalueW());
  EXPECT_EQ(0.29999999999999999, userModel.wallUvalueSW());
  EXPECT_EQ(0.22146791744515701, userModel.roofUValue());
  EXPECT_EQ(0.69999999999999996, userModel.wallSolarAbsorptionS());
  EXPECT_EQ(0.5, userModel.wallSolarAbsorptionSE());
  EXPECT_EQ(0.69999999999999996, userModel.wallSolarAbsorptionE());
  EXPECT_EQ(0.5, userModel.wallSolarAbsorptionNE());
  EXPECT_EQ(0.69999999999999996, userModel.wallSolarAbsorptionN());
  EXPECT_EQ(0.5, userModel.wallSolarAbsorptionNW());
  EXPECT_EQ(0.69999999999999996, userModel.wallSolarAbsorptionW());
  EXPECT_EQ(0.5, userModel.wallSolarAbsorptionSW());
  EXPECT_EQ(0.69999999999999996, userModel.roofSolarAbsorption());
  EXPECT_EQ(0.90000000000000002, userModel.wallThermalEmissivityS());
  EXPECT_EQ(0.5, userModel.wallThermalEmissivitySE());
  EXPECT_EQ(0.90000000000000002, userModel.wallThermalEmissivityE());
  EXPECT_EQ(0.5, userModel.wallThermalEmissivityNE());
  EXPECT_EQ(0.90000000000000002, userModel.wallThermalEmissivityN());
  EXPECT_EQ(0.5, userModel.wallThermalEmissivityNW());
  EXPECT_EQ(0.90000000000000002, userModel.wallThermalEmissivityW());
  EXPECT_EQ(0.5, userModel.wallThermalEmissivitySW());
  EXPECT_EQ(0.90000000000000002, userModel.roofThermalEmissivity());
  EXPECT_EQ(320.0, userModel.windowAreaS());
  EXPECT_EQ(0.0, userModel.windowAreaSE());
  EXPECT_EQ(160.0, userModel.windowAreaE());
  EXPECT_EQ(0.0, userModel.windowAreaNE());
  EXPECT_EQ(320.0, userModel.windowAreaN());
  EXPECT_EQ(0.0, userModel.windowAreaNW());
  EXPECT_EQ(160.0, userModel.windowAreaW());
  EXPECT_EQ(0.0, userModel.windowAreaSW());
  EXPECT_EQ(0.0, userModel.skylightArea());
  EXPECT_EQ(3.8904056162246499, userModel.windowUvalueS());
  EXPECT_EQ(3.0, userModel.windowUvalueSE());
  EXPECT_EQ(3.8904056162246601, userModel.windowUvalueE());
  EXPECT_EQ(3.0, userModel.windowUvalueNE());
  EXPECT_EQ(3.8904056162246499, userModel.windowUvalueN());
  EXPECT_EQ(3.0, userModel.windowUvalueNW());
  EXPECT_EQ(3.8904056162246601, userModel.windowUvalueW());
  EXPECT_EQ(3.0, userModel.windowUvalueSW());
  EXPECT_EQ(3.0, userModel.skylightUvalue());
  EXPECT_EQ(0.49665000000000098, userModel.windowSHGCS());
  EXPECT_EQ(0.5, userModel.windowSHGCSE());
  EXPECT_EQ(0.49665000000000098, userModel.windowSHGCE());
  EXPECT_EQ(0.5, userModel.windowSHGCNE());
  EXPECT_EQ(0.49665000000000098, userModel.windowSHGCN());
  EXPECT_EQ(0.5, userModel.windowSHGCNW());
  EXPECT_EQ(0.49665000000000098, userModel.windowSHGCW());
  EXPECT_EQ(0.5, userModel.windowSHGCSW());
  EXPECT_EQ(0.5, userModel.skylightSHGC());
  EXPECT_EQ(1.0, userModel.windowSCFS());
  EXPECT_EQ(1.0, userModel.windowSCFSE());
  EXPECT_EQ(1.0, userModel.windowSCFE());
  EXPECT_EQ(1.0, userModel.windowSCFNE());
  EXPECT_EQ(1.0, userModel.windowSCFN());
  EXPECT_EQ(1.0, userModel.windowSCFNW());
  EXPECT_EQ(1.0, userModel.windowSCFW());
  EXPECT_EQ(1.0, userModel.windowSCFSW());
  EXPECT_EQ(1.0, userModel.windowSDFS());
  EXPECT_EQ(1.0, userModel.windowSDFSE());
  EXPECT_EQ(1.0, userModel.windowSDFE());
  EXPECT_EQ(1.0, userModel.windowSDFNE());
  EXPECT_EQ(1.0, userModel.windowSDFN());
  EXPECT_EQ(1.0, userModel.windowSDFNW());
  EXPECT_EQ(1.0, userModel.windowSDFW());
  EXPECT_EQ(1.0, userModel.windowSDFSW());
  EXPECT_EQ(58083.259716774199, userModel.exteriorHeatCapacity());
  EXPECT_EQ(0.139585598177502, userModel.infiltration());//unmodified by load
  EXPECT_EQ(0.0, userModel.hvacWasteFactor());
  EXPECT_EQ(0.25, userModel.hvacHeatingLossFactor());
  EXPECT_EQ(0.0, userModel.hvacCoolingLossFactor());
  EXPECT_EQ(0.59999999999999998, userModel.dhwDistributionEfficiency());
  EXPECT_EQ(1.0, userModel.heatingPumpControl());
  EXPECT_EQ(1.0, userModel.coolingPumpControl());
  EXPECT_EQ(120.0, userModel.heatGainPerPerson());
  EXPECT_EQ(userModel.weatherFilePath(), openstudio::toPath("weather.epw"));
  WeatherData wd = *userModel.loadWeather();

  Matrix msolar = wd.msolar();
  Matrix mhdbt = wd.mhdbt();
  Matrix mhEgh = wd.mhEgh();
  Vector mEgh = wd.mEgh();
  Vector mdbt = wd.mdbt();
  Vector mwind = wd.mwind();

  double msolarExp[] = {
          144.545,144.529,144.488,144.423,144.371,144.451,144.506,144.538,
          174.11,174.09,174.042,173.965,173.908,174.001,174.065,174.102,
          197.025,197.005,196.954,196.872,196.804,196.905,196.976,197.016,
          213.837,213.813,213.758,213.671,213.615,213.718,213.789,213.829,
          260.515,260.487,260.42,260.317,260.253,260.375,260.459,260.506,
          265.371,265.343,265.278,265.175,265.106,265.228,265.314,265.361,
          259.414,259.384,259.318,259.216,259.17,259.285,259.364,259.407,
          237.662,237.636,237.576,237.482,237.422,237.533,237.61,237.653,
          225.103,225.08,225.021,224.929,224.85,224.966,225.046,225.092,
          193.282,193.262,193.211,193.128,193.051,193.156,193.229,193.271,
          132.18,132.166,132.131,132.074,132.022,132.094,132.144,132.173,
          132.474,132.459,132.423,132.364,132.307,132.382,132.435,132.465};
  double mhdbtExp[] = {
          -6.28387,-6.18065,-6.0871,-6.27419,-6.52258,-6.80645,-6.78065,-6.55806,-5.55161,-4.36129,-3.54516,-2.79032,-2.02903,-1.68387,-1.59677,-1.86129,-2.9129,-3.82581,-4.54516,-4.50968,-4.70968,-4.97742,-5.23871,-5.88387,
          -3.65714,-3.92857,-4.14643,-4.46429,-4.90714,-5.12143,-5.26429,-4.625,-3.29286,-2.09643,-1.02143,-0.242857,0.257143,0.396429,0.496429,0.260714,-0.464286,-1.425,-2.34643,-2.58214,-2.85,-2.925,-3.15,-3.38571,
          2.02258,1.81613,1.76774,1.24839,1.13226,0.919355,1.42258,2.75484,3.96774,5.05484,5.93871,6.81935,7.1871,7.32581,7.19355,6.76774,6.13548,5.03871,3.94194,3.3,2.94194,2.60968,2.37419,2.09355,
          7.79333,7.61333,7.56333,7.19333,6.95667,7.03667,8.09333,9.25667,10.1867,10.9467,11.7933,12.2767,12.7633,13.12,13.0133,12.84,12.41,11.5033,10.5767,9.88,9.48667,9.18,8.91667,8.42,
          10.7097,10.3161,10.1677,10.0516,9.89355,11.2258,13.5677,15.4742,17.2935,18.6968,19.8194,20.5806,21.0548,20.9452,20.5742,19.9871,19.5323,18.0968,16.0742,14.4871,13.3323,12.4548,11.7613,11.3516,
          17.5967,17.2533,17.2067,16.8633,16.72,17.5933,19.5733,21.2833,22.5033,23.59,24.3833,24.83,25.2267,25.4667,25.39,25.1333,24.7567,23.9567,22.3333,20.68,19.41,18.79,18.1333,17.9467,
          20.7129,20.3419,19.871,19.7323,19.5323,20.7065,22.4,23.7323,24.9613,26.0774,27.0387,27.8323,28.4419,28.5774,28.4968,27.9806,27.3194,26.5161,25.4,24.0484,23.2452,22.6484,22.0419,21.5806,
          18.5935,18.3194,18.1613,17.671,17.6645,18.029,20.0871,21.7097,23.2581,24.6032,25.2032,25.4323,25.9,25.7194,25.429,25.1742,24.5387,23.771,22.3645,21.4645,20.6,20.2742,19.5806,19.0194,
          14.91,14.4867,14.14,13.88,13.7167,13.6067,15.5367,17.65,19.3933,20.46,21.4033,22.2433,22.8367,23.15,22.8867,22.4867,21.5567,19.96,18.6,17.8,16.8667,16.4967,15.8767,15.27,
          8.49677,8.22903,8,7.8129,7.79677,7.54516,8.06129,9.87742,11.7548,13.1774,14.3258,14.9452,15.2484,15.429,15.3452,14.6387,13.2613,11.7645,11.0194,10.429,9.84194,9.33871,8.74194,8.45806,
          3.71333,3.45333,3.35667,3.35667,3.06,2.85,2.75333,3.47667,4.34333,5.48667,6.31333,6.98667,7.24333,7.39333,7.27333,6.68667,5.92333,5.27667,4.63333,4.22667,4.15333,4.08,3.80333,3.71667,
          -4.89355,-4.78387,-4.93871,-5.23871,-5.29677,-5.64516,-5.69355,-5.35161,-4.48065,-3.2129,-2.1,-1.49355,-0.919355,-0.703226,-0.683871,-1.47097,-2.47419,-3.3129,-3.67742,-4.02258,-4.15484,-4.36774,-4.54839,-5.00323};
  double mhEghExp[] = {
          0,0,0,0,0,0,0,20.3226,92.871,191.226,260.129,302.742,305.903,276.774,192.516,98.8065,22.6774,0,0,0,0,0,0,0,
          0,0,0,0,0,0,3.07143,48.25,149.5,271.393,364.5,390.393,395.179,342.286,273.929,179.643,67.6071,7.60714,0,0,0,0,0,0,
          0,0,0,0,0,0.580645,31.7742,132,241.419,368.484,455.516,479.71,503.903,451.097,361.032,252.903,128.065,33.129,0.548387,0,0,0,0,0,
          0,0,0,0,0,7.13333,148.4,241.533,371.533,463.8,516.1,548.533,519.5,528.933,438.933,327.933,193.567,85.5333,2.7,0,0,0,0,0,
          0,0,0,0,8.74194,68.5484,202.226,355.323,507.258,631.903,680.742,710.581,723.903,670.806,538.29,418.129,287.645,135.774,35.4194,0.580645,0,0,0,0,
          0,0,0,0,16.2667,95.1,232.833,384.467,533.2,625.5,709.1,702.633,698.333,660.9,586.867,475.2,319.9,185.6,61.7333,5.86667,0,0,0,0,
          0,0,0,0,8.3871,67.5484,194.323,338,477.097,600.613,664.355,732.29,761.323,681.71,611.194,464.161,333.032,181.129,56.8065,4.80645,0,0,0,0,
          0,0,0,0,0.322581,31.9355,138.452,277.806,413.323,548.968,611.097,647.613,624.258,575.903,489.194,407.71,253.097,116.806,24.7742,0.16129,0,0,0,0,
          0,0,0,0,0,10.9333,86.6333,219.067,367.367,479.533,533.833,555.533,558.033,500.5,398.067,289.9,150.1,40.9667,2.16667,0,0,0,0,0,
          0,0,0,0,0,0.322581,33.3871,136.065,265.032,374.871,424.355,427.903,425.516,378.194,265.871,156.968,48.9677,3.74194,0,0,0,0,0,0,
          0,0,0,0,0,0,4.73333,50.8667,146.333,217.267,286.433,319,301.6,249.033,160.8,70.1,12.4667,0,0,0,0,0,0,0,
          0,0,0,0,0,0,0,22.2258,98.7097,182.581,252.129,262,262.484,213.29,143.871,59.7419,6.96774,0,0,0,0,0,0,0};
  double mEghExp[] = {73.4987, 103.89, 143.34, 183.089, 248.995, 262.229, 257.366, 215.059, 174.693, 122.55, 75.7764, 62.6667};
  double mdbtExp[] = {-4.64651, -2.52024, 3.82392, 9.95083, 15.3103, 21.1092, 24.1348, 21.7737, 18.1339, 10.9808, 4.73167, -3.68616};
  double mwindExp[] = {4.88199, 5.06756, 5.48978, 4.83181, 3.74718, 4.88611, 4.23884, 3.84543, 3.41347, 4.78804, 5.29569, 4.29476};

  int v=0;
  for(size_t r = 0;r<msolar.size1();r++){
    for(size_t c = 0;c<msolar.size2();c++,v++){
      EXPECT_EQ(msolarExp[v], msolar(r,c));
    }
  }
  v=0;
  for(size_t r = 0;r<mhdbt.size1();r++){
    for(size_t c = 0;c<mhdbt.size2();c++,v++){
      EXPECT_EQ(mhdbtExp[v], mhdbt(r,c));
    }
  }
  v=0;
  for(size_t r = 0;r<mhEgh.size1();r++){
    for(size_t c = 0;c<mhEgh.size2();c++,v++){
      EXPECT_EQ(mhEghExp[v], mhEgh(r,c));
    }
  }
  v=0;
  for(int r = 0;r < 12; r++,v++)
  {
    EXPECT_EQ(mEghExp[v], mEgh[r]);
  }
  v=0;
  for(int r = 0;r < 12; r++,v++)
  {
    EXPECT_EQ(mdbtExp[v], mdbt[r]);
  }
  v=0;
  for(int r = 0;r < 12; r++,v++)
  {
    EXPECT_EQ(mwindExp[v], mwind[r]);
  }




}
