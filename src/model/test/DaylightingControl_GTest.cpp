/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Transformation.hpp"
#include "../../utilities/geometry/Vector3d.hpp"
#include "../../utilities/data/Matrix.hpp"

#include <cmath>

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

  EXPECT_EQ(static_cast<unsigned>(1), model.getConcreteModelObjects<DaylightingControl>().size());
  ModelObject object = control.clone(model);
  EXPECT_EQ(static_cast<unsigned>(2), model.getConcreteModelObjects<DaylightingControl>().size());

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

TEST_F(ModelFixture, DaylightingControl_Geometry) {

  Model m;

  constexpr double width = 3.0;
  // Position of the sensor
  constexpr double xPos = 2.0;
  constexpr double yPos = 1.0;
  constexpr double zPos = 1.5;  // Eye level

  // Counterclockwise
  std::vector<Point3d> floorPoints{{0.0, 0.0, 0.0}, {0.0, width, 0.0}, {width, width, 0.0}, {width, 0.0, 0.0}};
  auto space_ = Space::fromFloorPrint(floorPoints, 3, m);  // Space is a cube of 3m side
  ASSERT_TRUE(space_);

  Point3d origin(0, 0, 0);
  Point3d position(xPos, yPos, zPos);

  // Point3d target(xTarget, yTarget, zTarget);

  //auto identityMatrix = Matrix(boost::numeric::ublas::identity_matrix<double>(4));

  DaylightingControl d(m);
  EXPECT_EQ(origin, d.position());
  EXPECT_EQ(0.0, d.positionXCoordinate());
  EXPECT_EQ(0.0, d.positionYCoordinate());
  EXPECT_EQ(0.0, d.positionZCoordinate());
  EXPECT_EQ(origin, d.position());

  EXPECT_EQ(0.0, d.psiRotationAroundXAxis());
  EXPECT_EQ(0.0, d.thetaRotationAroundYAxis());
  EXPECT_EQ(0.0, d.phiRotationAroundZAxis());
  Transformation t = d.transformation();
  //EXPECT_EQ(identityMatrix, t.matrix());

  EXPECT_TRUE(d.setPosition(position));
  EXPECT_EQ(position, d.position());
  EXPECT_EQ(xPos, d.positionXCoordinate());
  EXPECT_EQ(yPos, d.positionYCoordinate());
  EXPECT_EQ(zPos, d.positionZCoordinate());
  EXPECT_EQ(position, d.position());

  // Horizontal plane
  //   y
  //   ▲
  //   │
  //   │          t3                        t2
  // 3 ├───────────x───────────┬───────────x
  //   │           │\          │          /│
  //   │           │ \         │         / │
  //   │           │  \        │        /  │
  //   │           │   \       │       /   │
  //   │           │    \      │      /    │
  // 2 ├───────────┼──── \─────┼─────/─────┤
  //   │           │      \    │    /      │
  //   │           │       \   │   /       │
  //   │           │        \  │  /        │
  //   │           │         \ │ /         │
  //   │           │           │/          │
  // 1 ├───────────┼───────────O───────────x t1
  //   │           │           │           │
  //   │           │           │           │
  //   │           │           │           │
  //   │           │           │           │
  //   │           │           │           │
  //   └───────────┴───────────┴───────────┴────► x
  // O             1           2           3

  {
    // translation positive on the x axis, expect a -90° rotation from y axis, around z
    Point3d target1(xPos + 1.0, yPos, zPos);
    EXPECT_TRUE(d.aimAt(target1));
    EXPECT_EQ(0.0, d.psiRotationAroundXAxis());
    EXPECT_EQ(0.0, d.thetaRotationAroundYAxis());
    EXPECT_EQ(-90.0, d.phiRotationAroundZAxis());

    // Horizontal plane. Going from (x, y) = (2,1) to (3,3). angle from y axis, around z
    Point3d target2(xPos + 1.0, yPos + 2.0, zPos);
    EXPECT_TRUE(d.aimAt(target2));
    EXPECT_EQ(0.0, d.psiRotationAroundXAxis());
    EXPECT_EQ(0.0, d.thetaRotationAroundYAxis());
    // Phi is counterclockwise (right-hand rule), so result is negative here
    EXPECT_DOUBLE_EQ(std::atan(-1.0 / 2.0), degToRad(d.phiRotationAroundZAxis()));
    EXPECT_DOUBLE_EQ(-26.56505117707799, d.phiRotationAroundZAxis());  // -atan (1/2), converted to deg. Python: math.degrees(math.atan2(-1,2))

    // Horizontal plane. Going from (x, y) = (2,1) to (1,3). angle from y axis, around z
    Point3d target3(xPos - 1.0, yPos + 2.0, zPos);
    EXPECT_TRUE(d.aimAt(target3));
    EXPECT_EQ(0.0, d.psiRotationAroundXAxis());
    EXPECT_EQ(0.0, d.thetaRotationAroundYAxis());
    // Phi is counterclockwise (right-hand rule), so result is negative here
    EXPECT_DOUBLE_EQ(std::atan(1.0 / 2.0), degToRad(d.phiRotationAroundZAxis()));
    EXPECT_DOUBLE_EQ(26.56505117707799, d.phiRotationAroundZAxis());  // -atan (1/2), converted to deg. Python: math.degrees(math.atan2(-1,2))
  }

  // Vertical plane, going up. Expect a rotation from y axis of +90°, around x axis
  {
    Point3d target(xPos, yPos, zPos + 1.0);
    EXPECT_TRUE(d.aimAt(target));

    EXPECT_EQ(90.0, d.psiRotationAroundXAxis());
    EXPECT_EQ(0.0, d.thetaRotationAroundYAxis());
    EXPECT_EQ(0.0, d.phiRotationAroundZAxis());
  }

  {
    Point3d target(xPos + 1.0, yPos + 2.0, zPos + 3.0);
    EXPECT_TRUE(d.aimAt(target));

    EXPECT_DOUBLE_EQ(54.068333240158303, d.psiRotationAroundXAxis());
    EXPECT_DOUBLE_EQ(8.0272034759063704, d.thetaRotationAroundYAxis());
    EXPECT_DOUBLE_EQ(-15.658662176743, d.phiRotationAroundZAxis());
  }
}
