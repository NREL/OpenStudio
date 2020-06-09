/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    ::testing::ExitedWithCode(0),
    ""
  );

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

  // TODO:
}

TEST_F(ModelFixture, CoilCoolingDXCurveFitOperatingMode_Speeds) {

  // create a model to use
  Model model;

  CoilCoolingDXCurveFitOperatingMode operatingMode(model);

  std::vector<CoilCoolingDXCurveFitSpeed> speeds;
  for (unsigned i = 1; i <= 10; ++i) {
    CoilCoolingDXCurveFitSpeed speed(model);
    operatingMode.setName("Speed " + std::to_string(i));
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
    const auto& speedAtIndex = thisSpeeds[speedIndex - 1];
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
    const auto& speedAtIndex = thisSpeeds[speedIndex - 1];
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
        EXPECT_EQ(speeds[i-1], operatingMode.speeds()[i-1]);
      } else if (i > optIndex.get()) {
        EXPECT_EQ(speeds[i-2], operatingMode.speeds()[i-1]);
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
}
