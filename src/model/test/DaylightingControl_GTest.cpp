/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../ThermalZone.hpp"
#include "../DaylightingControl.hpp"
#include "../DaylightingControl_Impl.hpp"
#include "../GlareSensor.hpp"
#include "../GlareSensor_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, DaylightingControl_Clone) {
  Model model;

  DaylightingControl control(model);
  Space space(model);

  EXPECT_FALSE(control.space());
  EXPECT_EQ(0u, space.daylightingControls().size());

  control.setSpace(space);

  ASSERT_TRUE(control.space());
  EXPECT_EQ(space.handle(), control.space()->handle());
  EXPECT_EQ(1u, space.daylightingControls().size());

  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<DaylightingControl>().size());
  ModelObject object = control.clone(model);
  EXPECT_EQ(static_cast<unsigned>(2), model.getModelObjects<DaylightingControl>().size());

  ASSERT_TRUE(object.optionalCast<DaylightingControl>());
  EXPECT_EQ(space.handle(), control.space()->handle());
  EXPECT_EQ(space.handle(), object.cast<DaylightingControl>().space()->handle());
  EXPECT_EQ(2u, space.daylightingControls().size());
}

TEST_F(ModelFixture, DaylightingControl_Basic) {

  Model m;
  DaylightingControl daylightingControl(m);

  // Space Name: Optional Object
  EXPECT_FALSE(daylightingControl.space());
  // Counterclockwise
  std::vector<Point3d> floorPoints{{0, 0, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0}};
  auto space_ = Space::fromFloorPrint(floorPoints, 3, m);
  ASSERT_TRUE(space_);
  EXPECT_TRUE(daylightingControl.setSpace(space_.get()));
  ASSERT_TRUE(daylightingControl.space());
  EXPECT_EQ(space_.get(), daylightingControl.space().get());

  // Position X-Coordinate: Optional Double
  EXPECT_TRUE(daylightingControl.setPositionXCoordinate(1.0));
  EXPECT_EQ(1.0, daylightingControl.positionXCoordinate());

  // Position Y-Coordinate: Optional Double
  EXPECT_TRUE(daylightingControl.setPositionYCoordinate(2.0));
  EXPECT_EQ(2.0, daylightingControl.positionYCoordinate());

  // Position Z-Coordinate: Optional Double
  EXPECT_TRUE(daylightingControl.setPositionZCoordinate(3.0));
  EXPECT_EQ(3.0, daylightingControl.positionZCoordinate());

  // Psi Rotation Around X-Axis: Optional Double
  EXPECT_EQ(0.0, daylightingControl.psiRotationAroundXAxis());
  EXPECT_TRUE(daylightingControl.isPsiRotationAroundXAxisDefaulted());
  EXPECT_TRUE(daylightingControl.setPsiRotationAroundXAxis(4.0));
  EXPECT_EQ(4.0, daylightingControl.psiRotationAroundXAxis());
  EXPECT_FALSE(daylightingControl.isPsiRotationAroundXAxisDefaulted());
  daylightingControl.resetPsiRotationAroundXAxis();
  EXPECT_EQ(0.0, daylightingControl.psiRotationAroundXAxis());
  EXPECT_TRUE(daylightingControl.isPsiRotationAroundXAxisDefaulted());

  // Theta Rotation Around Y-Axis: Optional Double
  EXPECT_EQ(0.0, daylightingControl.thetaRotationAroundYAxis());
  EXPECT_TRUE(daylightingControl.isThetaRotationAroundYAxisDefaulted());
  EXPECT_TRUE(daylightingControl.setThetaRotationAroundYAxis(5.0));
  EXPECT_EQ(5.0, daylightingControl.thetaRotationAroundYAxis());
  EXPECT_FALSE(daylightingControl.isThetaRotationAroundYAxisDefaulted());
  daylightingControl.resetThetaRotationAroundYAxis();
  EXPECT_EQ(0.0, daylightingControl.thetaRotationAroundYAxis());
  EXPECT_TRUE(daylightingControl.isThetaRotationAroundYAxisDefaulted());

  // Phi Rotation Around Z-Axis: Optional Double
  EXPECT_EQ(0.0, daylightingControl.phiRotationAroundZAxis());
  EXPECT_TRUE(daylightingControl.isPhiRotationAroundZAxisDefaulted());
  EXPECT_TRUE(daylightingControl.setPhiRotationAroundZAxis(6.0));
  EXPECT_EQ(6.0, daylightingControl.phiRotationAroundZAxis());
  EXPECT_FALSE(daylightingControl.isPhiRotationAroundZAxisDefaulted());
  daylightingControl.resetPhiRotationAroundZAxis();
  EXPECT_EQ(0.0, daylightingControl.phiRotationAroundZAxis());
  EXPECT_TRUE(daylightingControl.isPhiRotationAroundZAxisDefaulted());

  // Illuminance Setpoint: Optional Double
  EXPECT_TRUE(daylightingControl.isIlluminanceSetpointDefaulted());
  EXPECT_EQ(500.0, daylightingControl.illuminanceSetpoint());
  EXPECT_TRUE(daylightingControl.setIlluminanceSetpoint(300.0));
  EXPECT_EQ(300.0, daylightingControl.illuminanceSetpoint());
  EXPECT_FALSE(daylightingControl.isIlluminanceSetpointDefaulted());
  // Bad Value
  EXPECT_FALSE(daylightingControl.setIlluminanceSetpoint(-10.0));
  EXPECT_EQ(300.0, daylightingControl.illuminanceSetpoint());
  daylightingControl.resetIlluminanceSetpoint();
  EXPECT_TRUE(daylightingControl.isIlluminanceSetpointDefaulted());
  EXPECT_EQ(500.0, daylightingControl.illuminanceSetpoint());

  // Lighting Control Type: Optional String
  EXPECT_TRUE(daylightingControl.isLightingControlTypeDefaulted());
  EXPECT_EQ("Continuous", daylightingControl.lightingControlType());
  EXPECT_TRUE(daylightingControl.setLightingControlType("None"));
  EXPECT_EQ("None", daylightingControl.lightingControlType());
  EXPECT_FALSE(daylightingControl.isLightingControlTypeDefaulted());
  // Bad Value
  EXPECT_FALSE(daylightingControl.setLightingControlType("BADENUM"));
  EXPECT_EQ("None", daylightingControl.lightingControlType());
  daylightingControl.resetLightingControlType();
  EXPECT_TRUE(daylightingControl.isLightingControlTypeDefaulted());
  EXPECT_EQ("Continuous", daylightingControl.lightingControlType());

  // Minimum Input Power Fraction for Continuous Dimming Control: Optional Double
  EXPECT_TRUE(daylightingControl.isMinimumInputPowerFractionforContinuousDimmingControlDefaulted());
  EXPECT_EQ(0.3, daylightingControl.minimumInputPowerFractionforContinuousDimmingControl());
  EXPECT_TRUE(daylightingControl.setMinimumInputPowerFractionforContinuousDimmingControl(0.58));
  EXPECT_EQ(0.58, daylightingControl.minimumInputPowerFractionforContinuousDimmingControl());
  EXPECT_FALSE(daylightingControl.isMinimumInputPowerFractionforContinuousDimmingControlDefaulted());
  // Bad Value
  EXPECT_FALSE(daylightingControl.setMinimumInputPowerFractionforContinuousDimmingControl(-10.0));
  EXPECT_EQ(0.58, daylightingControl.minimumInputPowerFractionforContinuousDimmingControl());
  daylightingControl.resetMinimumInputPowerFractionforContinuousDimmingControl();
  EXPECT_TRUE(daylightingControl.isMinimumInputPowerFractionforContinuousDimmingControlDefaulted());
  EXPECT_EQ(0.3, daylightingControl.minimumInputPowerFractionforContinuousDimmingControl());

  // Minimum Light Output Fraction for Continuous Dimming Control: Optional Double
  EXPECT_TRUE(daylightingControl.isMinimumLightOutputFractionforContinuousDimmingControlDefaulted());
  EXPECT_EQ(0.2, daylightingControl.minimumLightOutputFractionforContinuousDimmingControl());
  EXPECT_TRUE(daylightingControl.setMinimumLightOutputFractionforContinuousDimmingControl(0.35));
  EXPECT_EQ(0.35, daylightingControl.minimumLightOutputFractionforContinuousDimmingControl());
  EXPECT_FALSE(daylightingControl.isMinimumLightOutputFractionforContinuousDimmingControlDefaulted());
  // Bad Value
  EXPECT_FALSE(daylightingControl.setMinimumLightOutputFractionforContinuousDimmingControl(-10.0));
  EXPECT_EQ(0.35, daylightingControl.minimumLightOutputFractionforContinuousDimmingControl());
  daylightingControl.resetMinimumLightOutputFractionforContinuousDimmingControl();
  EXPECT_TRUE(daylightingControl.isMinimumLightOutputFractionforContinuousDimmingControlDefaulted());
  EXPECT_EQ(0.2, daylightingControl.minimumLightOutputFractionforContinuousDimmingControl());

  // Number of Stepped Control Steps: Optional Integer
  EXPECT_TRUE(daylightingControl.isNumberofSteppedControlStepsDefaulted());
  EXPECT_EQ(1, daylightingControl.numberofSteppedControlSteps());
  EXPECT_TRUE(daylightingControl.setNumberofSteppedControlSteps(3));
  EXPECT_EQ(3, daylightingControl.numberofSteppedControlSteps());
  EXPECT_FALSE(daylightingControl.isNumberofSteppedControlStepsDefaulted());
  daylightingControl.resetNumberofSteppedControlSteps();
  EXPECT_TRUE(daylightingControl.isNumberofSteppedControlStepsDefaulted());
  EXPECT_EQ(1, daylightingControl.numberofSteppedControlSteps());

  // Probability Lighting will be Reset When Needed in Manual Stepped Control: Optional Double
  EXPECT_TRUE(daylightingControl.isProbabilityLightingwillbeResetWhenNeededinManualSteppedControlDefaulted());
  EXPECT_EQ(1.0, daylightingControl.probabilityLightingwillbeResetWhenNeededinManualSteppedControl());
  EXPECT_TRUE(daylightingControl.setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(0.55));
  EXPECT_EQ(0.55, daylightingControl.probabilityLightingwillbeResetWhenNeededinManualSteppedControl());
  EXPECT_FALSE(daylightingControl.isProbabilityLightingwillbeResetWhenNeededinManualSteppedControlDefaulted());
  // Bad Value
  EXPECT_FALSE(daylightingControl.setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(-10.0));
  EXPECT_EQ(0.55, daylightingControl.probabilityLightingwillbeResetWhenNeededinManualSteppedControl());
  daylightingControl.resetProbabilityLightingwillbeResetWhenNeededinManualSteppedControl();
  EXPECT_TRUE(daylightingControl.isProbabilityLightingwillbeResetWhenNeededinManualSteppedControlDefaulted());
  EXPECT_EQ(1.0, daylightingControl.probabilityLightingwillbeResetWhenNeededinManualSteppedControl());

  // Number of Daylighting Views: Optional Integer
  EXPECT_TRUE(daylightingControl.isNumberofDaylightingViewsDefaulted());
  EXPECT_EQ(1, daylightingControl.numberofDaylightingViews());
  EXPECT_TRUE(daylightingControl.setNumberofDaylightingViews(3));
  EXPECT_EQ(3, daylightingControl.numberofDaylightingViews());
  // Bad Value
  EXPECT_FALSE(daylightingControl.setNumberofDaylightingViews(-10));
  EXPECT_EQ(3, daylightingControl.numberofDaylightingViews());
  daylightingControl.resetNumberofDaylightingViews();
  EXPECT_TRUE(daylightingControl.isNumberofDaylightingViewsDefaulted());
  EXPECT_EQ(1, daylightingControl.numberofDaylightingViews());

  // Maximum Allowable Discomfort Glare Index: Optional Double
  EXPECT_FALSE(daylightingControl.maximumAllowableDiscomfortGlareIndex());
  EXPECT_TRUE(daylightingControl.setMaximumAllowableDiscomfortGlareIndex(1.1));
  ASSERT_TRUE(daylightingControl.maximumAllowableDiscomfortGlareIndex());
  EXPECT_EQ(1.1, daylightingControl.maximumAllowableDiscomfortGlareIndex().get());
  // Bad Value
  EXPECT_FALSE(daylightingControl.setMaximumAllowableDiscomfortGlareIndex(-9.0));
  ASSERT_TRUE(daylightingControl.maximumAllowableDiscomfortGlareIndex());
  EXPECT_EQ(1.1, daylightingControl.maximumAllowableDiscomfortGlareIndex().get());

  EXPECT_FALSE(daylightingControl.isPrimaryDaylightingControl());
  EXPECT_FALSE(daylightingControl.isSecondaryDaylightingControl());
  ThermalZone z(m);
  space_->setThermalZone(z);
  EXPECT_TRUE(z.setPrimaryDaylightingControl(daylightingControl));
  EXPECT_TRUE(daylightingControl.isPrimaryDaylightingControl());
}

}
