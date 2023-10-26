/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilHeatingGas.hpp"
#include "../CoilHeatingGas_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../Schedule.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../PlantLoop.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingGas) {
  Model model;
  ScheduleConstant scheduleConstant(model);

  CoilHeatingGas coilHeatingGas(model, scheduleConstant);

  coilHeatingGas.setGasBurnerEfficiency(0.6);
  EXPECT_EQ(coilHeatingGas.gasBurnerEfficiency(), 0.6);

  coilHeatingGas.setNominalCapacity(1535.0);
  EXPECT_EQ(coilHeatingGas.nominalCapacity().get(), 1535.0);

  coilHeatingGas.setParasiticElectricLoad(48.0);
  EXPECT_EQ(coilHeatingGas.parasiticElectricLoad(), 48.0);

  coilHeatingGas.setParasiticGasLoad(51.0);
  EXPECT_EQ(coilHeatingGas.parasiticGasLoad(), 51.0);

  ScheduleConstant schedule2(model);
  coilHeatingGas.setAvailableSchedule(schedule2);
  EXPECT_EQ(coilHeatingGas.availableSchedule(), schedule2);

  EXPECT_EQ(coilHeatingGas.fuelType(), "NaturalGas");
  bool isOk = coilHeatingGas.setFuelType("Propane");
  EXPECT_TRUE(isOk);
  EXPECT_EQ(coilHeatingGas.fuelType(), "Propane");
  isOk = coilHeatingGas.setFuelType("Propane");
  EXPECT_TRUE(isOk);
  EXPECT_EQ(coilHeatingGas.fuelType(), "Propane");
  isOk = coilHeatingGas.setFuelType("Coal");
  EXPECT_TRUE(isOk);
  EXPECT_EQ(coilHeatingGas.fuelType(), "Coal");
  coilHeatingGas.resetFuelType();
  EXPECT_EQ(coilHeatingGas.fuelType(), "NaturalGas");

  std::vector<std::string> validFuelTypes(coilHeatingGas.validFuelTypeValues());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "Propane"), validFuelTypes.end());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "NaturalGas"), validFuelTypes.end());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "Diesel"), validFuelTypes.end());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "Gasoline"), validFuelTypes.end());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "Coal"), validFuelTypes.end());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "FuelOilNo1"), validFuelTypes.end());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "FuelOilNo2"), validFuelTypes.end());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "OtherFuel1"), validFuelTypes.end());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "OtherFuel2"), validFuelTypes.end());
  EXPECT_EQ(validFuelTypes.size(), 9);
}

TEST_F(ModelFixture, CoilHeatingGas_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();

  CoilHeatingGas testObject(m, s);

  AirLoopHVAC airLoop(m);
  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m, controllerOutdoorAir);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  outdoorAirSystem.addToNode(supplyOutletNode);

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  CoilHeatingGas testObject2(m, s);
  CoilHeatingGas testObject3(m, s);

  if (boost::optional<Node> OANode = outdoorAirSystem.outboardOANode()) {
    EXPECT_TRUE(testObject2.addToNode(*OANode));
    EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.oaComponents().size());
  }

  if (boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode()) {
    EXPECT_TRUE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.reliefComponents().size());
  }

  auto testObjectClone = testObject.clone(m).cast<CoilHeatingGas>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, airLoop.supplyComponents().size());
}
