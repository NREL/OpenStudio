/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"

#include "../GeneratorWindTurbine.hpp"
#include "../GeneratorWindTurbine_Impl.hpp"
#include "../ElectricLoadCenterDistribution.hpp"
#include "../ElectricLoadCenterDistribution_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, GeneratorWindTurbine) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;

      GeneratorWindTurbine generator(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model model;

  GeneratorWindTurbine generator(model);

  EXPECT_TRUE(generator.availabilitySchedule());
  EXPECT_EQ("HorizontalAxisWindTurbine", generator.rotorType());
  EXPECT_EQ("VariableSpeedVariablePitch", generator.powerControl());
  EXPECT_EQ(41.0, generator.ratedRotorSpeed());
  EXPECT_EQ(19.2, generator.rotorDiameter());
  EXPECT_EQ(30.5, generator.overallHeight());
  EXPECT_EQ(3, generator.numberofBlades());
  EXPECT_EQ(55000.0, generator.ratedPower());
  EXPECT_EQ(11.0, generator.ratedWindSpeed());
  EXPECT_EQ(3.5, generator.cutInWindSpeed());
  EXPECT_EQ(25.0, generator.cutOutWindSpeed());
  EXPECT_EQ(0.835, generator.fractionSystemEfficiency());
  EXPECT_EQ(5.0, generator.maximumTipSpeedRatio());
  EXPECT_EQ(0.25, generator.maximumPowerCoefficient());
  EXPECT_FALSE(generator.annualLocalAverageWindSpeed());
  EXPECT_EQ(50.0, generator.heightforLocalAverageWindSpeed());
  EXPECT_EQ(2.08, generator.bladeChordArea());
  EXPECT_EQ(0.9, generator.bladeDragCoefficient());
  EXPECT_EQ(0.05, generator.bladeLiftCoefficient());
  EXPECT_EQ(0.5176, generator.powerCoefficientC1());
  EXPECT_EQ(116.0, generator.powerCoefficientC2());
  EXPECT_EQ(0.4, generator.powerCoefficientC3());
  EXPECT_EQ(0.0, generator.powerCoefficientC4());
  EXPECT_EQ(5.0, generator.powerCoefficientC5());
  EXPECT_EQ(21.0, generator.powerCoefficientC6());
}

TEST_F(ModelFixture, GeneratorWindTurbine_SetGetFields) {
  Model model;

  GeneratorWindTurbine generator(model);

  ScheduleConstant schedule(model);
  generator.setAvailabilitySchedule(schedule);
  generator.setRotorType("VerticalAxisWindTurbine");
  generator.setPowerControl("FixedSpeedFixedPitch");
  generator.setRatedRotorSpeed(42.0);
  generator.setRotorDiameter(20.0);
  generator.setOverallHeight(31.0);
  generator.setNumberofBlades(4);
  generator.setRatedPower(0.1);
  generator.setRatedWindSpeed(15.0);
  generator.setCutInWindSpeed(4.0);
  generator.setCutOutWindSpeed(30.0);
  generator.setFractionSystemEfficiency(0.9);
  generator.setMaximumTipSpeedRatio(6.0);
  generator.setMaximumPowerCoefficient(0.3);
  generator.setAnnualLocalAverageWindSpeed(10.0);
  generator.setHeightforLocalAverageWindSpeed(55.0);
  generator.setBladeChordArea(3.0);
  generator.setBladeDragCoefficient(1.0);
  generator.setBladeLiftCoefficient(0.2);
  generator.setPowerCoefficientC1(0.6);
  generator.setPowerCoefficientC2(120.0);
  generator.setPowerCoefficientC3(0.1);
  generator.setPowerCoefficientC4(0.3);
  generator.setPowerCoefficientC5(8.0);
  generator.setPowerCoefficientC6(25.0);

  EXPECT_TRUE(generator.availabilitySchedule());
  EXPECT_EQ("VerticalAxisWindTurbine", generator.rotorType());
  EXPECT_EQ("FixedSpeedFixedPitch", generator.powerControl());
  EXPECT_EQ(42.0, generator.ratedRotorSpeed());
  EXPECT_EQ(20.0, generator.rotorDiameter());
  EXPECT_EQ(31.0, generator.overallHeight());
  EXPECT_EQ(4, generator.numberofBlades());
  EXPECT_EQ(0.1, generator.ratedPower());
  EXPECT_EQ(15.0, generator.ratedWindSpeed());
  EXPECT_EQ(4.0, generator.cutInWindSpeed());
  EXPECT_EQ(30.0, generator.cutOutWindSpeed());
  EXPECT_EQ(0.9, generator.fractionSystemEfficiency());
  EXPECT_EQ(6.0, generator.maximumTipSpeedRatio());
  EXPECT_EQ(0.3, generator.maximumPowerCoefficient());
  ASSERT_TRUE(generator.annualLocalAverageWindSpeed());
  EXPECT_EQ(10.0, generator.annualLocalAverageWindSpeed().get());
  EXPECT_EQ(55.0, generator.heightforLocalAverageWindSpeed());
  EXPECT_EQ(3.0, generator.bladeChordArea());
  EXPECT_EQ(1.0, generator.bladeDragCoefficient());
  EXPECT_EQ(0.2, generator.bladeLiftCoefficient());
  EXPECT_EQ(0.6, generator.powerCoefficientC1());
  EXPECT_EQ(120.0, generator.powerCoefficientC2());
  EXPECT_EQ(0.1, generator.powerCoefficientC3());
  EXPECT_EQ(0.3, generator.powerCoefficientC4());
  EXPECT_EQ(8.0, generator.powerCoefficientC5());
  EXPECT_EQ(25.0, generator.powerCoefficientC6());

  generator.resetAnnualLocalAverageWindSpeed();

  EXPECT_FALSE(generator.annualLocalAverageWindSpeed());
}

TEST_F(ModelFixture, GeneratorWindTurbine_Clone) {
  Model model;
  GeneratorWindTurbine generator(model);
  generator.setAnnualLocalAverageWindSpeed(12.0);

  // clone it into the same model
  auto generatorClone = generator.clone(model).cast<GeneratorWindTurbine>();
  ASSERT_TRUE(generatorClone.annualLocalAverageWindSpeed());

  // clone it into a different model
  Model model2;
  auto generatorClone2 = generator.clone(model2).cast<GeneratorWindTurbine>();
  ASSERT_TRUE(generatorClone2.annualLocalAverageWindSpeed());
}

TEST_F(ModelFixture, GeneratorWindTurbine_Remove) {
  Model model;
  //start with 0
  std::vector<GeneratorWindTurbine> gens = model.getConcreteModelObjects<GeneratorWindTurbine>();
  EXPECT_EQ(0u, gens.size());
  //add 1
  GeneratorWindTurbine generator(model);
  gens = model.getConcreteModelObjects<GeneratorWindTurbine>();
  EXPECT_EQ(1u, gens.size());
  //remove
  EXPECT_FALSE(generator.remove().empty());
  gens = model.getConcreteModelObjects<GeneratorWindTurbine>();
  EXPECT_EQ(0u, gens.size());
}

TEST_F(ModelFixture, GeneratorWindTurbine_ElectricLoadCenterDistribution) {
  Model model;
  ElectricLoadCenterDistribution elcd(model);
  EXPECT_EQ(0, elcd.generators().size());
  GeneratorWindTurbine generator(model);
  elcd.addGenerator(generator);
  EXPECT_EQ(1u, elcd.generators().size());
  boost::optional<ElectricLoadCenterDistribution> optelcd = generator.electricLoadCenterDistribution();
  EXPECT_TRUE(optelcd);
  ElectricLoadCenterDistribution elcd2 = optelcd.get();
  EXPECT_EQ(elcd, elcd2);
  EXPECT_TRUE(elcd.removeGenerator(generator));
  EXPECT_FALSE(generator.electricLoadCenterDistribution());
  ASSERT_EQ(0, elcd.generators().size());
  ASSERT_EQ(0, elcd2.generators().size());
  elcd.addGenerator(generator);
  elcd.remove();
  EXPECT_FALSE(generator.electricLoadCenterDistribution());
}

TEST_F(ModelFixture, GeneratorWindTurbine_ElectricLoadCenterDistribution2) {
  Model model;

  GeneratorWindTurbine generator(model);

  //should be 0 default ELCD attached to wind turbine
  std::vector<ElectricLoadCenterDistribution> elcd = model.getConcreteModelObjects<ElectricLoadCenterDistribution>();
  EXPECT_EQ(0u, elcd.size());
  EXPECT_FALSE(generator.electricLoadCenterDistribution());
  //Add a ELCD
  ElectricLoadCenterDistribution elcd1(model);
  EXPECT_TRUE(elcd1.addGenerator(generator));
  EXPECT_EQ(elcd1.handle(), generator.electricLoadCenterDistribution().get().handle());
  //Add another ELCD
  ElectricLoadCenterDistribution elcd2(model);
  EXPECT_EQ(2, model.getConcreteModelObjects<ElectricLoadCenterDistribution>().size());
  //Add the wind turbine to it which should remove the existing one attached to wind turbine
  EXPECT_TRUE(elcd2.addGenerator(generator));
  EXPECT_EQ(0, elcd1.generators().size());
  EXPECT_EQ(elcd2.handle(), generator.electricLoadCenterDistribution().get().handle());
}
