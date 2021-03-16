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

#include "ModelFixture.hpp"

#include "../ElectricLoadCenterStorageConverter.hpp"
#include "../ThermalZone.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"
#include "../CurveQuadratic.hpp"
#include "../ElectricLoadCenterDistribution.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ElectricLoadCenterStorageConverter_Instantiate) {

  bool result;
  Model model;
  ThermalZone thermalZone(model);

  ElectricLoadCenterStorageConverter elcConv(model);

  // Availability Schedule, defaults to model.alwaysOnDiscrete
  EXPECT_TRUE(elcConv.isAvailabilityScheduleDefaulted());
  EXPECT_EQ(elcConv.availabilitySchedule(), model.alwaysOnDiscreteSchedule());
  ScheduleCompact scheduleCompact(model);
  EXPECT_TRUE(elcConv.setAvailabilitySchedule(scheduleCompact));
  EXPECT_EQ(elcConv.availabilitySchedule(), scheduleCompact);
  elcConv.resetAvailabilitySchedule();
  EXPECT_TRUE(elcConv.isAvailabilityScheduleDefaulted());
  EXPECT_EQ(elcConv.availabilitySchedule(), model.alwaysOnDiscreteSchedule());

  // SimpleFixedEfficiency
  EXPECT_TRUE(elcConv.simpleFixedEfficiency());
  EXPECT_EQ(elcConv.simpleFixedEfficiency().get(), 0.95);
  EXPECT_EQ(elcConv.powerConversionEfficiencyMethod(), "SimpleFixed");
  EXPECT_TRUE(elcConv.setSimpleFixedEfficiency(0.80));
  EXPECT_EQ(elcConv.simpleFixedEfficiency().get(), 0.80);

  // designMaximumContinuousInputPower
  EXPECT_FALSE(elcConv.designMaximumContinuousInputPower());
  EXPECT_TRUE(elcConv.setDesignMaximumContinuousInputPower(10000.0));
  EXPECT_EQ(elcConv.powerConversionEfficiencyMethod(), "FunctionOfPower");
  EXPECT_FALSE(elcConv.simpleFixedEfficiency());
  EXPECT_EQ(elcConv.designMaximumContinuousInputPower().get(), 10000.0);

  // Reset to SimpleFixed
  EXPECT_TRUE(elcConv.setSimpleFixedEfficiency(0.80));
  EXPECT_EQ(elcConv.powerConversionEfficiencyMethod(), "SimpleFixed");
  EXPECT_FALSE(elcConv.designMaximumContinuousInputPower());
  EXPECT_FALSE(elcConv.efficiencyFunctionofPowerCurve());

  // efficiencyFunctionofPowerCurveName
  CurveQuadratic effFPower = CurveQuadratic(model);
  EXPECT_FALSE(elcConv.efficiencyFunctionofPowerCurve());
  EXPECT_TRUE(elcConv.setEfficiencyFunctionofPowerCurve(effFPower));
  EXPECT_EQ(elcConv.powerConversionEfficiencyMethod(), "FunctionOfPower");
  EXPECT_FALSE(elcConv.simpleFixedEfficiency());
  ASSERT_TRUE(elcConv.efficiencyFunctionofPowerCurve());
  if (boost::optional<Curve> setCurve = elcConv.efficiencyFunctionofPowerCurve()) {
    EXPECT_EQ(effFPower.handle(), setCurve->handle());
  }

  EXPECT_TRUE(elcConv.setDesignMaximumContinuousInputPower(10000.0));

  // Reset to SimpleFixed
  EXPECT_TRUE(elcConv.setSimpleFixedEfficiency(0.80));
  EXPECT_EQ(elcConv.powerConversionEfficiencyMethod(), "SimpleFixed");
  EXPECT_FALSE(elcConv.designMaximumContinuousInputPower());
  EXPECT_FALSE(elcConv.efficiencyFunctionofPowerCurve());

  // ancillaryPowerConsumedInStandby
  EXPECT_TRUE(elcConv.isAncillaryPowerConsumedInStandbyDefaulted());
  EXPECT_EQ(0.0, elcConv.ancillaryPowerConsumedInStandby());
  elcConv.setAncillaryPowerConsumedInStandby(20);
  EXPECT_EQ(20.0, elcConv.ancillaryPowerConsumedInStandby());
  EXPECT_FALSE(elcConv.isAncillaryPowerConsumedInStandbyDefaulted());
  elcConv.resetAncillaryPowerConsumedInStandby();
  EXPECT_TRUE(elcConv.isAncillaryPowerConsumedInStandbyDefaulted());

  // ZoneName
  EXPECT_FALSE(elcConv.thermalZone());
  EXPECT_TRUE(elcConv.setThermalZone(thermalZone));
  ASSERT_TRUE(elcConv.thermalZone());

  // radiativeFraction
  EXPECT_TRUE(elcConv.isRadiativeFractionDefaulted());
  result = elcConv.setRadiativeFraction(0.25);
  EXPECT_TRUE(result);
  EXPECT_FALSE(elcConv.isRadiativeFractionDefaulted());
  EXPECT_EQ(elcConv.radiativeFraction(), 0.25);
  elcConv.resetRadiativeFraction();
  EXPECT_TRUE(elcConv.isRadiativeFractionDefaulted());

  // Try to assign it to an ELCD
  ElectricLoadCenterDistribution elcd(model);
  EXPECT_TRUE(elcd.setStorageConverter(elcConv));
  if (boost::optional<ElectricLoadCenterStorageConverter> setConv = elcd.storageConverter()) {
    EXPECT_EQ(elcConv.handle(), setConv->handle());
  }
  elcd.resetStorageConverter();
  EXPECT_TRUE(elcd.setStorageConverter(elcConv));
}
