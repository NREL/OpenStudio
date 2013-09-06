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
  userModel.setCoolingSystemIPLV(0.86953551426846);
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
  EXPECT_EQ(0.86953551426846, userModel.coolingSystemIPLV());
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

  std::string wpath;
  wpath = "test";
  userModel.setWeatherFilePath(wpath);
  EXPECT_EQ(0, std::strcmp("test",userModel.weatherFilePath()));

  path p = resourcesPath() / openstudio::toPath("isomodel/exampleModel.ISO");
  userModel.load((const char*)p.string().c_str());
}
