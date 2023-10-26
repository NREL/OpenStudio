/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../GeneratorFuelSupply.hpp"
#include "../GeneratorFuelSupply_Impl.hpp"

#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeRegistry.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, FuelCellFuelSupply) {
  Model model;

  // check default GeneratorFuelSupply
  GeneratorFuelSupply fuelsupply(model);
  EXPECT_EQ("Scheduled", fuelsupply.fuelTemperatureModelingMode());
  boost::optional<Schedule> schedule = fuelsupply.fuelTemperatureSchedule();
  EXPECT_EQ(20, schedule.get().cast<ScheduleConstant>().value());

  CurveCubic curveCubic = fuelsupply.compressorPowerMultiplierFunctionofFuelRateCurve();
  EXPECT_EQ(0, curveCubic.coefficient1Constant());
  EXPECT_EQ(0, curveCubic.coefficient2x());
  EXPECT_EQ(0, curveCubic.coefficient3xPOW2());
  EXPECT_EQ(0, curveCubic.coefficient4xPOW3());
  EXPECT_EQ(-1.0e10, curveCubic.minimumValueofx());
  EXPECT_EQ(1.0e10, curveCubic.maximumValueofx());

  EXPECT_EQ(1, fuelsupply.compressorHeatLossFactor());

  EXPECT_EQ("GaseousConstituents", fuelsupply.fuelType());
  EXPECT_TRUE(fuelsupply.setFuelType("LiquidGeneric"));
  EXPECT_EQ("LiquidGeneric", fuelsupply.fuelType());
  fuelsupply.resetFuelType();
  EXPECT_EQ("GaseousConstituents", fuelsupply.fuelType());

  // LHV
  EXPECT_FALSE(fuelsupply.liquidGenericFuelLowerHeatingValue());
  EXPECT_TRUE(fuelsupply.setLiquidGenericFuelLowerHeatingValue(43100.0));
  ASSERT_TRUE(fuelsupply.liquidGenericFuelLowerHeatingValue());
  EXPECT_DOUBLE_EQ(43100.0, fuelsupply.liquidGenericFuelLowerHeatingValue().get());
  fuelsupply.resetLiquidGenericFuelLowerHeatingValue();
  EXPECT_FALSE(fuelsupply.liquidGenericFuelLowerHeatingValue());

  // HHV
  EXPECT_FALSE(fuelsupply.liquidGenericFuelHigherHeatingValue());
  EXPECT_TRUE(fuelsupply.setLiquidGenericFuelHigherHeatingValue(46200.0));
  ASSERT_TRUE(fuelsupply.liquidGenericFuelHigherHeatingValue());
  EXPECT_DOUBLE_EQ(46200.0, fuelsupply.liquidGenericFuelHigherHeatingValue().get());
  fuelsupply.resetLiquidGenericFuelHigherHeatingValue();
  EXPECT_FALSE(fuelsupply.liquidGenericFuelHigherHeatingValue());

  // Molecular weight
  EXPECT_FALSE(fuelsupply.liquidGenericFuelMolecularWeight());
  EXPECT_TRUE(fuelsupply.setLiquidGenericFuelMolecularWeight(1.0));
  ASSERT_TRUE(fuelsupply.liquidGenericFuelMolecularWeight());
  EXPECT_DOUBLE_EQ(1.0, fuelsupply.liquidGenericFuelMolecularWeight().get());
  fuelsupply.resetLiquidGenericFuelMolecularWeight();
  EXPECT_FALSE(fuelsupply.liquidGenericFuelMolecularWeight());

  // CO2 Emission Factor
  EXPECT_FALSE(fuelsupply.liquidGenericFuelCO2EmissionFactor());
  EXPECT_TRUE(fuelsupply.setLiquidGenericFuelCO2EmissionFactor(0.0));
  ASSERT_TRUE(fuelsupply.liquidGenericFuelCO2EmissionFactor());
  EXPECT_DOUBLE_EQ(0.0, fuelsupply.liquidGenericFuelCO2EmissionFactor().get());
  fuelsupply.resetLiquidGenericFuelCO2EmissionFactor();
  EXPECT_FALSE(fuelsupply.liquidGenericFuelCO2EmissionFactor());
  fuelsupply.resetLiquidGenericFuelCO2EmissionFactor();

  // CTOR creates 8 to match natural gas
  EXPECT_EQ(8, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  // And it should have a sum of molar fraction equal to 1
  EXPECT_DOUBLE_EQ(1.0, fuelsupply.sumofConstituentsMolarFractions());

  fuelsupply.removeAllConstituents();

  EXPECT_EQ(0, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  //should throw in Ctor of FuelSupplyConstituent since name is wrong
  EXPECT_THROW(fuelsupply.addConstituent("MadeUp", 0.0092), openstudio::Exception);
  EXPECT_EQ(0, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());

  EXPECT_EQ(0, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("CarbonDioxide", 0.0003));
  EXPECT_EQ(1, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Nitrogen", 0.7728));
  EXPECT_EQ(2, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Oxygen", 0.2073));
  EXPECT_EQ(3, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Water", 0.0104));
  EXPECT_EQ(4, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Argon", 0.0092));
  EXPECT_EQ(5, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Hydrogen", 0.00));
  EXPECT_EQ(6, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Methane", 0.00));
  EXPECT_EQ(7, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Ethane", 0.00));
  EXPECT_EQ(8, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Propane", 0.00));
  EXPECT_EQ(9, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Butane", 0.00));
  EXPECT_EQ(10, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Pentane", 0.00));
  EXPECT_EQ(11, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Hexane", 0.00));
  EXPECT_EQ(12, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());

  //should fail since only 12 allowed
  ASSERT_FALSE(fuelsupply.addConstituent("Methanol", 0.00));
  fuelsupply.removeConstituent(11);
  EXPECT_EQ(11, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Methanol", 0.00));
  EXPECT_EQ(12, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  fuelsupply.removeConstituent(11);
  ASSERT_TRUE(fuelsupply.addConstituent("Ethanol", 0.00));
  EXPECT_EQ(12, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  fuelsupply.removeAllConstituents();
  EXPECT_EQ(0, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  fuelsupply.removeConstituent(0);
  EXPECT_EQ(0, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());

  CurveCubic curveCubic2(model);
  fuelsupply.setCompressorPowerMultiplierFunctionofFuelRateCurve(curveCubic2);
  EXPECT_EQ(curveCubic2, fuelsupply.compressorPowerMultiplierFunctionofFuelRateCurve());
}
