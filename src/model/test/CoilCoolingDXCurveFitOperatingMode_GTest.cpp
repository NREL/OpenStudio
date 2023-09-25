/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include <string>
#include "ModelFixture.hpp"

#include "../CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../CoilCoolingDXCurveFitOperatingMode_Impl.hpp"

#include "../CoilCoolingDXCurveFitPerformance.hpp"
#include "../CoilCoolingDXCurveFitPerformance_Impl.hpp"
#include "../CoilCoolingDXCurveFitSpeed.hpp"
#include "../CoilCoolingDXCurveFitSpeed_Impl.hpp"

#include <algorithm>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXCurveFitOperatingMode_CoilCoolingDXCurveFitOperatingMode) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a coil cooling dx curve fit operating mode object to use
      CoilCoolingDXCurveFitOperatingMode operatingMode(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model model;

  // create a coil cooling dx curve fit operating mode object to use
  CoilCoolingDXCurveFitOperatingMode operatingMode(model);

  ASSERT_FALSE(operatingMode.ratedGrossTotalCoolingCapacity());
  EXPECT_TRUE(operatingMode.isRatedGrossTotalCoolingCapacityAutosized());
  ASSERT_FALSE(operatingMode.ratedEvaporatorAirFlowRate());
  EXPECT_TRUE(operatingMode.isRatedEvaporatorAirFlowRateAutosized());
  ASSERT_FALSE(operatingMode.ratedCondenserAirFlowRate());
  EXPECT_TRUE(operatingMode.isRatedCondenserAirFlowRateAutosized());
  EXPECT_EQ(0.0, operatingMode.maximumCyclingRate());
  EXPECT_EQ(0.0, operatingMode.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity());
  EXPECT_EQ(0.0, operatingMode.latentCapacityTimeConstant());
  EXPECT_EQ(0.0, operatingMode.nominalTimeforCondensateRemovaltoBegin());
  EXPECT_FALSE(operatingMode.applyLatentDegradationtoSpeedsGreaterthan1());
  EXPECT_EQ("AirCooled", operatingMode.condenserType());
  EXPECT_EQ(0, operatingMode.coilCoolingDXCurveFitPerformances().size());
  EXPECT_EQ(0, operatingMode.speeds().size());
}

TEST_F(ModelFixture, CoilCoolingDXCurveFitOperatingMode_GettersSetters) {
  // create a model to use
  Model model;

  // create a coil cooling dx curve fit operating mode object to use
  CoilCoolingDXCurveFitOperatingMode operatingMode(model);

  ASSERT_FALSE(operatingMode.ratedGrossTotalCoolingCapacity());
  ASSERT_TRUE(operatingMode.isRatedGrossTotalCoolingCapacityAutosized());
  ASSERT_FALSE(operatingMode.ratedEvaporatorAirFlowRate());
  ASSERT_TRUE(operatingMode.isRatedEvaporatorAirFlowRateAutosized());
  ASSERT_FALSE(operatingMode.ratedCondenserAirFlowRate());
  ASSERT_TRUE(operatingMode.isRatedCondenserAirFlowRateAutosized());
  EXPECT_EQ(operatingMode.maximumCyclingRate(), 0.0);
  EXPECT_EQ(operatingMode.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(), 0.0);
  EXPECT_EQ(operatingMode.latentCapacityTimeConstant(), 0.0);
  EXPECT_EQ(operatingMode.nominalTimeforCondensateRemovaltoBegin(), 0.0);
  EXPECT_FALSE(operatingMode.applyLatentDegradationtoSpeedsGreaterthan1());
  EXPECT_EQ(operatingMode.condenserType(), "AirCooled");
  ASSERT_FALSE(operatingMode.nominalEvaporativeCondenserPumpPower());
  ASSERT_TRUE(operatingMode.isNominalEvaporativeCondenserPumpPowerAutosized());
  EXPECT_EQ(operatingMode.nominalSpeedNumber(), 0);
  EXPECT_TRUE(operatingMode.isNominalSpeedNumberDefaulted());
  EXPECT_EQ(operatingMode.coilCoolingDXCurveFitPerformances().size(), 0u);
  EXPECT_EQ(operatingMode.speeds().size(), 0u);
  EXPECT_EQ(operatingMode.numberOfSpeeds(), 0);

  operatingMode.setRatedGrossTotalCoolingCapacity(0.1);
  operatingMode.setRatedEvaporatorAirFlowRate(0.2);
  operatingMode.setRatedCondenserAirFlowRate(0.3);
  operatingMode.setMaximumCyclingRate(1.0);
  operatingMode.setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(2.0);
  operatingMode.setLatentCapacityTimeConstant(3.0);
  operatingMode.setNominalTimeforCondensateRemovaltoBegin(4.0);
  operatingMode.setApplyLatentDegradationtoSpeedsGreaterthan1(true);
  operatingMode.setCondenserType("EvaporativelyCooled");
  operatingMode.setNominalEvaporativeCondenserPumpPower(5.0);
  operatingMode.setNominalSpeedNumber(1);

  ASSERT_TRUE(operatingMode.ratedGrossTotalCoolingCapacity());
  EXPECT_EQ(operatingMode.ratedGrossTotalCoolingCapacity().get(), 0.1);
  ASSERT_FALSE(operatingMode.isRatedGrossTotalCoolingCapacityAutosized());
  ASSERT_TRUE(operatingMode.ratedEvaporatorAirFlowRate());
  EXPECT_EQ(operatingMode.ratedEvaporatorAirFlowRate().get(), 0.2);
  ASSERT_FALSE(operatingMode.isRatedEvaporatorAirFlowRateAutosized());
  ASSERT_TRUE(operatingMode.ratedCondenserAirFlowRate());
  EXPECT_EQ(operatingMode.ratedCondenserAirFlowRate().get(), 0.3);
  ASSERT_FALSE(operatingMode.isRatedCondenserAirFlowRateAutosized());
  EXPECT_EQ(operatingMode.maximumCyclingRate(), 1.0);
  EXPECT_EQ(operatingMode.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(), 2.0);
  EXPECT_EQ(operatingMode.latentCapacityTimeConstant(), 3.0);
  EXPECT_EQ(operatingMode.nominalTimeforCondensateRemovaltoBegin(), 4.0);
  EXPECT_TRUE(operatingMode.applyLatentDegradationtoSpeedsGreaterthan1());
  EXPECT_EQ(operatingMode.condenserType(), "EvaporativelyCooled");
  ASSERT_TRUE(operatingMode.nominalEvaporativeCondenserPumpPower());
  EXPECT_EQ(operatingMode.nominalEvaporativeCondenserPumpPower().get(), 5.0);
  ASSERT_FALSE(operatingMode.isNominalEvaporativeCondenserPumpPowerAutosized());
  EXPECT_EQ(operatingMode.nominalSpeedNumber(), 0);
  EXPECT_TRUE(operatingMode.isNominalSpeedNumberDefaulted());

  operatingMode.autosizeRatedGrossTotalCoolingCapacity();
  operatingMode.autosizeRatedEvaporatorAirFlowRate();
  operatingMode.autosizeRatedCondenserAirFlowRate();
  operatingMode.autosizeNominalEvaporativeCondenserPumpPower();
  operatingMode.resetNominalSpeedNumber();

  ASSERT_FALSE(operatingMode.ratedGrossTotalCoolingCapacity());
  ASSERT_TRUE(operatingMode.isRatedGrossTotalCoolingCapacityAutosized());
  ASSERT_FALSE(operatingMode.ratedEvaporatorAirFlowRate());
  ASSERT_TRUE(operatingMode.isRatedEvaporatorAirFlowRateAutosized());
  ASSERT_FALSE(operatingMode.ratedCondenserAirFlowRate());
  ASSERT_TRUE(operatingMode.isRatedCondenserAirFlowRateAutosized());
  EXPECT_EQ(operatingMode.nominalSpeedNumber(), 0);
  EXPECT_TRUE(operatingMode.isNominalSpeedNumberDefaulted());
}

TEST_F(ModelFixture, CoilCoolingDXCurveFitOperatingMode_Speeds) {

  // create a model to use
  Model model;

  CoilCoolingDXCurveFitOperatingMode operatingMode(model);

  std::vector<CoilCoolingDXCurveFitSpeed> speeds;
  for (unsigned i = 1; i <= 10; ++i) {
    CoilCoolingDXCurveFitSpeed speed(model);
    speed.setName("Speed " + std::to_string(i));
    speeds.push_back(speed);
    EXPECT_TRUE(operatingMode.addSpeed(speed));
    EXPECT_EQ(i, operatingMode.numberOfSpeeds());
    EXPECT_EQ(speeds, operatingMode.speeds());
  }

  // Can't add more than 10 Speeds;
  CoilCoolingDXCurveFitSpeed anotherSpeed(model);
  EXPECT_FALSE(operatingMode.addSpeed(anotherSpeed));
  EXPECT_EQ(10, operatingMode.numberOfSpeeds());
  EXPECT_EQ(speeds, operatingMode.speeds());

  // Can't remove a speed that's not in there...
  EXPECT_FALSE(operatingMode.removeSpeed(anotherSpeed));
  EXPECT_EQ(10, operatingMode.numberOfSpeeds());
  EXPECT_EQ(speeds, operatingMode.speeds());

  {
    int speedIndex = 6;
    std::vector<CoilCoolingDXCurveFitSpeed> thisSpeeds = operatingMode.speeds();
    // Explicit copy, so we can keep using it after it's been removed
    const auto speedAtIndex = thisSpeeds[speedIndex - 1];
    EXPECT_TRUE(std::find(thisSpeeds.begin(), thisSpeeds.end(), speedAtIndex) != thisSpeeds.end());
    auto optIndex = operatingMode.speedIndex(speedAtIndex);
    ASSERT_TRUE(optIndex);
    EXPECT_EQ(speedIndex, optIndex.get());
    EXPECT_TRUE(operatingMode.removeSpeed(speedIndex));
    EXPECT_EQ(9, operatingMode.numberOfSpeeds());
    thisSpeeds = operatingMode.speeds();
    EXPECT_FALSE(std::find(thisSpeeds.begin(), thisSpeeds.end(), speedAtIndex) != thisSpeeds.end());
    // Do the same on our vector, so we're up to date...
    speeds.erase(speeds.begin() + speedIndex - 1);
    EXPECT_EQ(speeds, operatingMode.speeds());
  }

  {
    int speedIndex = 3;
    std::vector<CoilCoolingDXCurveFitSpeed> thisSpeeds = operatingMode.speeds();
    // Explicit copy, so we can keep using it after it's been removed
    const auto speedAtIndex = thisSpeeds[speedIndex - 1];
    EXPECT_TRUE(std::find(thisSpeeds.begin(), thisSpeeds.end(), speedAtIndex) != thisSpeeds.end());
    auto optIndex = operatingMode.speedIndex(speedAtIndex);
    ASSERT_TRUE(optIndex);
    EXPECT_EQ(speedIndex, optIndex.get());
    EXPECT_TRUE(operatingMode.removeSpeed(speedAtIndex));
    EXPECT_EQ(8, operatingMode.numberOfSpeeds());
    thisSpeeds = operatingMode.speeds();
    EXPECT_FALSE(std::find(thisSpeeds.begin(), thisSpeeds.end(), speedAtIndex) != thisSpeeds.end());
    // Do the same on our vector, so we're up to date...
    speeds.erase(std::find(speeds.begin(), speeds.end(), speedAtIndex));
    EXPECT_EQ(speeds, operatingMode.speeds());
  }

  {
    const auto& speedAtEnd = speeds.back();
    auto optIndex = operatingMode.speedIndex(speedAtEnd);
    ASSERT_TRUE(optIndex);
    EXPECT_EQ(operatingMode.numberOfSpeeds(), optIndex.get());

    EXPECT_TRUE(operatingMode.setSpeedIndex(speedAtEnd, 4));
    std::vector<CoilCoolingDXCurveFitSpeed> thisSpeeds = operatingMode.speeds();
    optIndex = operatingMode.speedIndex(speedAtEnd);
    ASSERT_TRUE(optIndex);
    EXPECT_EQ(4, optIndex.get());
    EXPECT_EQ(8, operatingMode.numberOfSpeeds());
    for (unsigned i = 1; i <= operatingMode.numberOfSpeeds(); ++i) {
      if (i < optIndex.get()) {
        EXPECT_EQ(speeds[i - 1], operatingMode.speeds()[i - 1]);
      } else if (i > optIndex.get()) {
        EXPECT_EQ(speeds[i - 2], operatingMode.speeds()[i - 1]);
      }
    }
  }

  operatingMode.removeAllSpeeds();
  EXPECT_EQ(0, operatingMode.numberOfSpeeds());

  EXPECT_TRUE(operatingMode.setSpeeds(speeds));
  EXPECT_EQ(8, operatingMode.numberOfSpeeds());
  EXPECT_EQ(speeds, operatingMode.speeds());

  for (unsigned i = 11; i <= 13; ++i) {
    CoilCoolingDXCurveFitSpeed speed(model);
    operatingMode.setName("Speed " + std::to_string(i));
    speeds.push_back(speed);
  }
  EXPECT_EQ(11u, speeds.size());
  operatingMode.removeAllSpeeds();
  EXPECT_TRUE(operatingMode.addSpeed(anotherSpeed));

  // This should clear, then assign the first 10, but then return false since the 11th failed
  EXPECT_FALSE(operatingMode.setSpeeds(speeds));
  EXPECT_EQ(10, operatingMode.numberOfSpeeds());
  {
    std::vector<CoilCoolingDXCurveFitSpeed> thisSpeeds = operatingMode.speeds();
    for (unsigned i = 0; i < 10; ++i) {
      EXPECT_EQ(speeds[i], thisSpeeds[i]);
    }
  }

  operatingMode.removeAllSpeeds();
  EXPECT_EQ(0u, operatingMode.numExtensibleGroups());
  EXPECT_EQ(0u, operatingMode.numberOfSpeeds());
  EXPECT_EQ(0u, operatingMode.speeds().size());

  // Test that added a speed from another model will fail but not add a blank extensible group
  Model model2;
  CoilCoolingDXCurveFitSpeed speedFromAnotherModel(model2);
  EXPECT_FALSE(operatingMode.addSpeed(speedFromAnotherModel));
  EXPECT_EQ(0u, operatingMode.numExtensibleGroups());
  EXPECT_EQ(0u, operatingMode.numberOfSpeeds());
  EXPECT_EQ(0u, operatingMode.speeds().size());
}

TEST_F(ModelFixture, CoilCoolingDXCurveFitOperatingMode_coilCoolingDXCurveFitPerformances) {
  Model model;

  CoilCoolingDXCurveFitOperatingMode operatingMode(model);
  CoilCoolingDXCurveFitPerformance performance1(model, operatingMode);

  EXPECT_EQ(1u, operatingMode.directUseCount());
  auto coilCoolingDXCurveFitPerformances = operatingMode.coilCoolingDXCurveFitPerformances();
  ASSERT_EQ(1u, coilCoolingDXCurveFitPerformances.size());
  EXPECT_EQ(performance1, coilCoolingDXCurveFitPerformances[0]);

  CoilCoolingDXCurveFitPerformance performance2(model, operatingMode);
  EXPECT_EQ(2u, operatingMode.directUseCount());
  coilCoolingDXCurveFitPerformances = operatingMode.coilCoolingDXCurveFitPerformances();
  ASSERT_EQ(2u, coilCoolingDXCurveFitPerformances.size());
  EXPECT_TRUE(std::find(coilCoolingDXCurveFitPerformances.begin(), coilCoolingDXCurveFitPerformances.end(), performance1)
              != coilCoolingDXCurveFitPerformances.end());
  EXPECT_TRUE(std::find(coilCoolingDXCurveFitPerformances.begin(), coilCoolingDXCurveFitPerformances.end(), performance2)
              != coilCoolingDXCurveFitPerformances.end());

  CoilCoolingDXCurveFitOperatingMode operatingMode2(model);
  EXPECT_TRUE(performance1.setBaseOperatingMode(operatingMode2));

  EXPECT_EQ(1u, operatingMode2.directUseCount());
  coilCoolingDXCurveFitPerformances = operatingMode2.coilCoolingDXCurveFitPerformances();
  ASSERT_EQ(1u, coilCoolingDXCurveFitPerformances.size());
  EXPECT_EQ(performance1, coilCoolingDXCurveFitPerformances[0]);

  EXPECT_EQ(1u, operatingMode.directUseCount());
  coilCoolingDXCurveFitPerformances = operatingMode.coilCoolingDXCurveFitPerformances();
  ASSERT_EQ(1u, coilCoolingDXCurveFitPerformances.size());
  EXPECT_EQ(performance2, coilCoolingDXCurveFitPerformances[0]);
}

TEST_F(ModelFixture, CoilCoolingDXCurveFitOperatingMode_clone) {

  // create a model to use
  Model model;

  CoilCoolingDXCurveFitOperatingMode operatingMode(model);
  CoilCoolingDXCurveFitPerformance performance(model, operatingMode);

  std::vector<CoilCoolingDXCurveFitSpeed> speeds;
  for (unsigned i = 1; i <= 10; ++i) {
    CoilCoolingDXCurveFitSpeed speed(model);
    speed.setName("Speed " + std::to_string(i));
    speeds.push_back(speed);
    EXPECT_TRUE(operatingMode.addSpeed(speed));
  }

  EXPECT_EQ(10u, operatingMode.numberOfSpeeds());
  EXPECT_EQ(speeds, operatingMode.speeds());

  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());
  EXPECT_EQ(10u, model.getConcreteModelObjects<CoilCoolingDXCurveFitSpeed>().size());

  auto operatingModeClone = operatingMode.clone(model).cast<CoilCoolingDXCurveFitOperatingMode>();

  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(2u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());
  EXPECT_EQ(10u, model.getConcreteModelObjects<CoilCoolingDXCurveFitSpeed>().size());

  EXPECT_EQ(10u, operatingMode.numberOfSpeeds());
  EXPECT_EQ(speeds, operatingMode.speeds());

  EXPECT_EQ(10u, operatingModeClone.numberOfSpeeds());
  EXPECT_EQ(speeds, operatingModeClone.speeds());

  // This shouldn't work, it's going to put the CoilCoolingDXCurveFitPerformance in a broken state
  auto rmed = operatingMode.remove();
  EXPECT_EQ(0u, rmed.size());
  EXPECT_NO_THROW(performance.baseOperatingMode());
  CoilCoolingDXCurveFitOperatingMode anotherOperatingMode(model);
  EXPECT_TRUE(performance.setBaseOperatingMode(anotherOperatingMode));
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(3u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());
  EXPECT_EQ(10u, model.getConcreteModelObjects<CoilCoolingDXCurveFitSpeed>().size());

  rmed = operatingMode.remove();
  EXPECT_EQ(1u, rmed.size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(2u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());
  EXPECT_EQ(10u, model.getConcreteModelObjects<CoilCoolingDXCurveFitSpeed>().size());
  EXPECT_EQ(10u, operatingModeClone.numberOfSpeeds());
  EXPECT_EQ(speeds, operatingModeClone.speeds());

  rmed = operatingModeClone.remove();
  EXPECT_EQ(11u, rmed.size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());
  EXPECT_EQ(0u, model.getConcreteModelObjects<CoilCoolingDXCurveFitSpeed>().size());
}
