/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../GeneratorFuelCellStackCooler.hpp"
#include "../GeneratorFuelCellStackCooler_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, FuelCellStackCooler) {
  Model model;

  // check default StackCooler
  GeneratorFuelCellStackCooler stackCooler(model);
  EXPECT_EQ(20, stackCooler.nominalStackTemperature());
  EXPECT_EQ(20, stackCooler.actualStackTemperature());
  EXPECT_EQ(0, stackCooler.coefficientr0());
  EXPECT_EQ(0, stackCooler.coefficientr1());
  EXPECT_EQ(0, stackCooler.coefficientr2());
  EXPECT_EQ(0, stackCooler.coefficientr3());
  EXPECT_EQ(1, stackCooler.stackCoolantFlowRate());
  EXPECT_EQ(1, stackCooler.stackCoolerUFactorTimesAreaValue());
  EXPECT_EQ(0, stackCooler.fscogenAdjustmentFactor());
  EXPECT_EQ(1, stackCooler.stackCogenerationExchangerArea());
  EXPECT_EQ(1, stackCooler.stackCogenerationExchangerNominalFlowRate());
  EXPECT_EQ(0, stackCooler.stackCogenerationExchangerNominalHeatTransferCoefficient());
  EXPECT_EQ(0, stackCooler.stackCogenerationExchangerNominalHeatTransferCoefficientExponent());
  EXPECT_EQ(0, stackCooler.stackCoolerPumpPower());
  EXPECT_EQ(0, stackCooler.stackCoolerPumpHeatLossFraction());
  EXPECT_EQ(0, stackCooler.stackAirCoolerFanCoefficientf0());
  EXPECT_EQ(0, stackCooler.stackAirCoolerFanCoefficientf1());
  EXPECT_EQ(0, stackCooler.stackAirCoolerFanCoefficientf2());
  //change defaults
  stackCooler.setNominalStackTemperature(200);
  EXPECT_EQ(200, stackCooler.nominalStackTemperature());
  stackCooler.setActualStackTemperature(200);
  EXPECT_EQ(200, stackCooler.actualStackTemperature());
  stackCooler.setCoefficientr0(10);
  EXPECT_EQ(10, stackCooler.coefficientr0());
  stackCooler.setCoefficientr1(10);
  EXPECT_EQ(10, stackCooler.coefficientr1());
  stackCooler.setCoefficientr2(10);
  EXPECT_EQ(10, stackCooler.coefficientr2());
  stackCooler.setCoefficientr3(10);
  EXPECT_EQ(10, stackCooler.coefficientr3());
  stackCooler.setStackCoolantFlowRate(11);
  EXPECT_EQ(11, stackCooler.stackCoolantFlowRate());
  stackCooler.setStackCoolerUFactorTimesAreaValue(11);
  EXPECT_EQ(11, stackCooler.stackCoolerUFactorTimesAreaValue());
  stackCooler.setFscogenAdjustmentFactor(10);
  EXPECT_EQ(10, stackCooler.fscogenAdjustmentFactor());
  stackCooler.setStackCogenerationExchangerArea(11);
  EXPECT_EQ(11, stackCooler.stackCogenerationExchangerArea());
  stackCooler.setStackCogenerationExchangerNominalFlowRate(11);
  EXPECT_EQ(11, stackCooler.stackCogenerationExchangerNominalFlowRate());
  stackCooler.setStackCogenerationExchangerNominalHeatTransferCoefficient(10);
  EXPECT_EQ(10, stackCooler.stackCogenerationExchangerNominalHeatTransferCoefficient());
  stackCooler.setStackCogenerationExchangerNominalHeatTransferCoefficientExponent(10);
  EXPECT_EQ(10, stackCooler.stackCogenerationExchangerNominalHeatTransferCoefficientExponent());
  stackCooler.setStackCoolerPumpPower(10);
  EXPECT_EQ(10, stackCooler.stackCoolerPumpPower());
  //must be between 0,1
  ASSERT_FALSE(stackCooler.setStackCoolerPumpHeatLossFraction(2));
  ASSERT_TRUE(stackCooler.setStackCoolerPumpHeatLossFraction(1));
  EXPECT_EQ(1, stackCooler.stackCoolerPumpHeatLossFraction());
  stackCooler.setStackAirCoolerFanCoefficientf0(10);
  EXPECT_EQ(10, stackCooler.stackAirCoolerFanCoefficientf0());
  stackCooler.setStackAirCoolerFanCoefficientf1(10);
  EXPECT_EQ(10, stackCooler.stackAirCoolerFanCoefficientf1());
  stackCooler.setStackAirCoolerFanCoefficientf2(10);
  EXPECT_EQ(10, stackCooler.stackAirCoolerFanCoefficientf2());
}

TEST_F(ModelFixture, FuelCellStackCooler_addToNode) {
  Model m;
  GeneratorFuelCellStackCooler testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());

  GeneratorFuelCellStackCooler testObject2(m);

  EXPECT_TRUE(testObject2.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.demandComponents().size());

  PlantLoop plantLoop2(m);
  demandOutletNode = plantLoop2.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop2.demandComponents().size());

  //GeneratorFuelCellStackCooler testObjectClone = testObject.clone(m).cast<GeneratorFuelCellStackCooler>();
  //supplyOutletNode = plantLoop.supplyOutletNode();

  //EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  //EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());
}
