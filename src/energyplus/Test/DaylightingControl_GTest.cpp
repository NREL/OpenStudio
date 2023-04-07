/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/DaylightingControl.hpp"
#include "../../model/DaylightingControl_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Transformation.hpp"
#include "../../utilities/geometry/Vector3d.hpp"
#include "../../utilities/data/Matrix.hpp"

#include <utilities/idd/Daylighting_Controls_FieldEnums.hxx>
#include <utilities/idd/Daylighting_ReferencePoint_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_DaylightingControl_NoZone) {
  Model model;
  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  DaylightingControl daylightingControl(model);
  daylightingControl.setSpace(space);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::Daylighting_Controls).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_DaylightingControl_OneControl) {
  Model model;
  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  DaylightingControl daylightingControl(model);
  daylightingControl.setSpace(space);

  EXPECT_TRUE(thermalZone.setPrimaryDaylightingControl(daylightingControl));

  ForwardTranslator forwardTranslator;
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(1, workspace.getObjectsByType(IddObjectType::Daylighting_Controls).size());
  }

  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(1, workspace.getObjectsByType(IddObjectType::Daylighting_Controls).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_DaylightingControl_TwoControl) {
  Model model;
  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  DaylightingControl daylightingControl1(model);
  daylightingControl1.setSpace(space);
  DaylightingControl daylightingControl2(model);
  daylightingControl2.setSpace(space);

  EXPECT_TRUE(thermalZone.setPrimaryDaylightingControl(daylightingControl1));
  EXPECT_TRUE(thermalZone.setSecondaryDaylightingControl(daylightingControl2));

  ForwardTranslator forwardTranslator;
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(1, workspace.getObjectsByType(IddObjectType::Daylighting_Controls).size());
  }

  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(1, workspace.getObjectsByType(IddObjectType::Daylighting_Controls).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_DaylightingControl_3216) {
  Model model;

  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);

  DaylightingControl daylightingControl(model);
  daylightingControl.setThetaRotationAroundYAxis(90.0);
  daylightingControl.setPhiRotationAroundZAxis(-180.0);

  EXPECT_TRUE(thermalZone.setPrimaryDaylightingControl(daylightingControl));

  ForwardTranslator ft;
  {
    ft.setExcludeSpaceTranslation(true);

    Workspace w = ft.translateModel(model);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Daylighting_Controls);
    ASSERT_EQ(1, idfObjs.size());
    WorkspaceObject idf_d(idfObjs[0]);

    EXPECT_EQ(-daylightingControl.phiRotationAroundZAxis(),
              idf_d.getDouble(Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis).get());
  }

  {
    ft.setExcludeSpaceTranslation(false);

    Workspace w = ft.translateModel(model);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Daylighting_Controls);
    ASSERT_EQ(1, idfObjs.size());
    WorkspaceObject idf_d(idfObjs[0]);

    EXPECT_EQ(-daylightingControl.phiRotationAroundZAxis(),
              idf_d.getDouble(Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis).get());
  }

  daylightingControl.setPhiRotationAroundZAxis(45.0);
  {
    ft.setExcludeSpaceTranslation(true);

    Workspace w = ft.translateModel(model);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Daylighting_Controls);
    ASSERT_EQ(1, idfObjs.size());
    WorkspaceObject idf_d(idfObjs[0]);

    // -45, so +360 to end up with a positive value => 315
    EXPECT_EQ(315, idf_d.getDouble(Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis).get());
  }

  {
    ft.setExcludeSpaceTranslation(false);

    Workspace w = ft.translateModel(model);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Daylighting_Controls);
    ASSERT_EQ(1, idfObjs.size());
    WorkspaceObject idf_d(idfObjs[0]);

    // -45, so +360 to end up with a positive value => 315
    EXPECT_EQ(315, idf_d.getDouble(Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_DaylightingControl_AvailabilitySchedule) {
  // Test for #4364 - Availability Schedule for DaylightingControl
  Model model;

  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  DaylightingControl daylightingControl(model);
  daylightingControl.setSpace(space);
  EXPECT_TRUE(thermalZone.setPrimaryDaylightingControl(daylightingControl));

  ForwardTranslator forwardTranslator;
  {

    forwardTranslator.setExcludeSpaceTranslation(true);

    Workspace workspace = forwardTranslator.translateModel(model);

    WorkspaceObjectVector idfObjs = workspace.getObjectsByType(IddObjectType::Daylighting_Controls);
    ASSERT_EQ(1, idfObjs.size());
    WorkspaceObject idf_d(idfObjs[0]);

    EXPECT_EQ("", idf_d.getString(Daylighting_ControlsFields::AvailabilityScheduleName).get());
  }

  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);

    WorkspaceObjectVector idfObjs = workspace.getObjectsByType(IddObjectType::Daylighting_Controls);
    ASSERT_EQ(1, idfObjs.size());
    WorkspaceObject idf_d(idfObjs[0]);

    EXPECT_EQ("", idf_d.getString(Daylighting_ControlsFields::AvailabilityScheduleName).get());
  }

  auto schedule = model.alwaysOffDiscreteSchedule();
  thermalZone.setDaylightingControlsAvailabilitySchedule(schedule);

  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    WorkspaceObjectVector idfObjs = workspace.getObjectsByType(IddObjectType::Daylighting_Controls);
    ASSERT_EQ(1, idfObjs.size());
    WorkspaceObject idf_d(idfObjs[0]);

    EXPECT_EQ(schedule.nameString(), idf_d.getString(Daylighting_ControlsFields::AvailabilityScheduleName).get());
  }

  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);

    WorkspaceObjectVector idfObjs = workspace.getObjectsByType(IddObjectType::Daylighting_Controls);
    ASSERT_EQ(1, idfObjs.size());
    WorkspaceObject idf_d(idfObjs[0]);

    EXPECT_EQ(schedule.nameString(), idf_d.getString(Daylighting_ControlsFields::AvailabilityScheduleName).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_DaylightingControl_ThreeSpacesOneZone) {

  Model m;

  constexpr double width = 10.0;
  // Position of the sensor **within space 2**
  constexpr double xPosSpace2 = 2.0;
  constexpr double yPosSpace2 = 1.0;
  constexpr double zPosSpace2 = 1.5;  // Eye level

  // Position of the sensor **within space 2**
  constexpr double xPosSpace3 = 8.0;
  constexpr double yPosSpace3 = 7.0;
  constexpr double zPosSpace3 = 1.0;

  //            y (=North)
  //   ▲
  //   │                  building height = 3m
  // 10├────────┼────────┼────────┤
  //   │        │        │  (S) x │
  //   │        │        │        │
  //   │ Space 1│ Space 2│ Space 3│
  //   │        │  x (P) │        │
  //   └────────┴────────┴────────┴──► x
  //  0        10       20        30

  // Counterclockwise points
  double x1 = 0.0;
  std::vector<Point3d> floorPointsSpace1{{x1 + 0.0, 0.0, 0.0}, {x1 + 0.0, width, 0.0}, {x1 + width, width, 0.0}, {x1 + width, 0.0, 0.0}};
  double x2 = width;
  std::vector<Point3d> floorPointsSpace2{{x2 + 0.0, 0.0, 0.0}, {x2 + 0.0, width, 0.0}, {x2 + width, width, 0.0}, {x2 + width, 0.0, 0.0}};

  auto space1 = Space::fromFloorPrint(floorPointsSpace1, 3, m).get();
  auto space2 = Space::fromFloorPrint(floorPointsSpace2, 3, m).get();
  // For space3, we construct from the same vector as Space1, except we'll set the xOrigin
  auto space3 = Space::fromFloorPrint(floorPointsSpace1, 3, m).get();
  space3.setXOrigin(width * 2);

  Point3d origin(0, 0, 0);
  Point3d positionOfSensorInSpace2(width + xPosSpace2, yPosSpace2, zPosSpace2);  // The origin the Space2 is the (0, 0) point of the zone
  Point3d positionOfSensorInSpace3(xPosSpace3, yPosSpace3, zPosSpace3);

  ThermalZone z(m);
  EXPECT_TRUE(space1.setThermalZone(z));
  EXPECT_TRUE(space2.setThermalZone(z));
  EXPECT_TRUE(space3.setThermalZone(z));

  // NOTE: for this to work, we need to have matched surfaces! So we can replace the interior walls by ConstructionAirBoundary
  space1.matchSurfaces(space2);
  space2.matchSurfaces(space3);

  DaylightingControl dSpace2 = [&m, &space2, &z, &positionOfSensorInSpace2]() {
    DaylightingControl dSpace2(m);
    dSpace2.setName("Sensor Space 2");
    EXPECT_TRUE(dSpace2.setSpace(space2));
    EXPECT_TRUE(dSpace2.setPosition(positionOfSensorInSpace2));
    EXPECT_TRUE(z.setPrimaryDaylightingControl(dSpace2));

    EXPECT_TRUE(dSpace2.setPsiRotationAroundXAxis(4.0));
    EXPECT_TRUE(dSpace2.setThetaRotationAroundYAxis(5.0));
    EXPECT_TRUE(dSpace2.setPhiRotationAroundZAxis(-6.0));
    EXPECT_TRUE(dSpace2.setIlluminanceSetpoint(300.0));
    EXPECT_TRUE(dSpace2.setLightingControlType("Continuous"));
    EXPECT_TRUE(dSpace2.setMinimumInputPowerFractionforContinuousDimmingControl(0.58));
    EXPECT_TRUE(dSpace2.setMinimumLightOutputFractionforContinuousDimmingControl(0.35));
    EXPECT_TRUE(dSpace2.setNumberofSteppedControlSteps(3));
    EXPECT_TRUE(dSpace2.setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(0.55));
    EXPECT_TRUE(dSpace2.setNumberofDaylightingViews(3));
    EXPECT_TRUE(dSpace2.setMaximumAllowableDiscomfortGlareIndex(1.1));

    EXPECT_TRUE(z.setFractionofZoneControlledbyPrimaryDaylightingControl(0.50));

    return dSpace2;
  }();

  DaylightingControl dSpace3 = [&m, &space3, &z, &positionOfSensorInSpace3]() {
    DaylightingControl dSpace3(m);
    EXPECT_TRUE(dSpace3.setSpace(space3));
    dSpace3.setName("Sensor Space 3");
    EXPECT_TRUE(dSpace3.setPosition(positionOfSensorInSpace3));
    EXPECT_TRUE(z.setSecondaryDaylightingControl(dSpace3));

    EXPECT_TRUE(dSpace3.setPsiRotationAroundXAxis(40.0));
    EXPECT_TRUE(dSpace3.setThetaRotationAroundYAxis(50.0));
    EXPECT_TRUE(dSpace3.setPhiRotationAroundZAxis(60.0));
    EXPECT_TRUE(dSpace3.setIlluminanceSetpoint(450.0));
    EXPECT_TRUE(dSpace3.setLightingControlType("Stepped"));
    EXPECT_TRUE(dSpace3.setMinimumInputPowerFractionforContinuousDimmingControl(0.38));
    EXPECT_TRUE(dSpace3.setMinimumLightOutputFractionforContinuousDimmingControl(0.25));
    EXPECT_TRUE(dSpace3.setNumberofSteppedControlSteps(2));
    EXPECT_TRUE(dSpace3.setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(0.75));
    EXPECT_TRUE(dSpace3.setNumberofDaylightingViews(2));
    EXPECT_TRUE(dSpace3.setMaximumAllowableDiscomfortGlareIndex(1.5));

    EXPECT_TRUE(z.setFractionofZoneControlledbySecondaryDaylightingControl(0.25));

    return dSpace3;
  }();

  auto alwaysOn = m.alwaysOnDiscreteSchedule();
  EXPECT_TRUE(z.setDaylightingControlsAvailabilitySchedule(alwaysOn));

  ForwardTranslator ft;

  // When excluding space translation (historical behavior)
  {
    ft.setExcludeSpaceTranslation(true);

    Workspace w = ft.translateModel(m);
    auto ds = w.getObjectsByType(IddObjectType::Daylighting_Controls);
    ASSERT_EQ(1, ds.size());
    auto d = ds[0];
    // High level info is populated from the **Primary** control + fields on ThermalZone
    EXPECT_EQ(z.nameString(), d.getString(Daylighting_ControlsFields::ZoneorSpaceName).get());
    EXPECT_TRUE(d.isEmpty(Daylighting_ControlsFields::DaylightingMethod));
    EXPECT_EQ(alwaysOn.nameString(), d.getString(Daylighting_ControlsFields::AvailabilityScheduleName).get());
    EXPECT_EQ(dSpace2.lightingControlType(), d.getString(Daylighting_ControlsFields::LightingControlType).get());
    EXPECT_EQ(dSpace2.minimumInputPowerFractionforContinuousDimmingControl(),
              d.getDouble(Daylighting_ControlsFields::MinimumInputPowerFractionforContinuousorContinuousOffDimmingControl).get());
    EXPECT_EQ(dSpace2.minimumLightOutputFractionforContinuousDimmingControl(),
              d.getDouble(Daylighting_ControlsFields::MinimumLightOutputFractionforContinuousorContinuousOffDimmingControl).get());
    EXPECT_EQ(dSpace2.numberofSteppedControlSteps(), d.getInt(Daylighting_ControlsFields::NumberofSteppedControlSteps).get());
    EXPECT_EQ(dSpace2.probabilityLightingwillbeResetWhenNeededinManualSteppedControl(),
              d.getDouble(Daylighting_ControlsFields::ProbabilityLightingwillbeResetWhenNeededinManualSteppedControl).get());

    EXPECT_EQ(dSpace2.nameString(), d.getString(Daylighting_ControlsFields::GlareCalculationDaylightingReferencePointName).get());

    EXPECT_EQ(-dSpace2.phiRotationAroundZAxis(),
              d.getDouble(Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis).get());
    EXPECT_EQ(dSpace2.maximumAllowableDiscomfortGlareIndex().get(),
              d.getDouble(Daylighting_ControlsFields::MaximumAllowableDiscomfortGlareIndex).get());
    EXPECT_TRUE(d.isEmpty(Daylighting_ControlsFields::DaylightingMethod));
    EXPECT_TRUE(d.isEmpty(Daylighting_ControlsFields::DElightGriddingResolution));

    ASSERT_EQ(2, d.numExtensibleGroups());
    {
      auto eg1 = d.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
      EXPECT_EQ(dSpace2.nameString(), eg1.getString(Daylighting_ControlsExtensibleFields::DaylightingReferencePointName).get());
      EXPECT_EQ(z.fractionofZoneControlledbyPrimaryDaylightingControl(),
                eg1.getDouble(Daylighting_ControlsExtensibleFields::FractionofLightsControlledbyReferencePoint).get());
      EXPECT_EQ(dSpace2.illuminanceSetpoint(), eg1.getDouble(Daylighting_ControlsExtensibleFields::IlluminanceSetpointatReferencePoint).get());
    }

    {
      auto eg2 = d.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
      EXPECT_EQ(dSpace3.nameString(), eg2.getString(Daylighting_ControlsExtensibleFields::DaylightingReferencePointName).get());
      EXPECT_EQ(z.fractionofZoneControlledbySecondaryDaylightingControl(),
                eg2.getDouble(Daylighting_ControlsExtensibleFields::FractionofLightsControlledbyReferencePoint).get());
      EXPECT_EQ(dSpace3.illuminanceSetpoint(), eg2.getDouble(Daylighting_ControlsExtensibleFields::IlluminanceSetpointatReferencePoint).get());
    }

    EXPECT_EQ(2, w.getObjectsByType(IddObjectType::Daylighting_ReferencePoint).size());

    {
      auto wRefSpace2_ = w.getObjectByTypeAndName(IddObjectType::Daylighting_ReferencePoint, dSpace2.nameString());
      ASSERT_TRUE(wRefSpace2_);
      EXPECT_EQ(z.nameString(), wRefSpace2_->getString(Daylighting_ReferencePointFields::ZoneorSpaceName).get());
      EXPECT_EQ(width + xPosSpace2, wRefSpace2_->getDouble(Daylighting_ReferencePointFields::XCoordinateofReferencePoint).get());
      EXPECT_EQ(yPosSpace2, wRefSpace2_->getDouble(Daylighting_ReferencePointFields::YCoordinateofReferencePoint).get());
      EXPECT_EQ(zPosSpace2, wRefSpace2_->getDouble(Daylighting_ReferencePointFields::ZCoordinateofReferencePoint).get());
    }
    {
      auto wRefSpace3_ = w.getObjectByTypeAndName(IddObjectType::Daylighting_ReferencePoint, dSpace3.nameString());
      ASSERT_TRUE(wRefSpace3_);
      EXPECT_EQ(z.nameString(), wRefSpace3_->getString(Daylighting_ReferencePointFields::ZoneorSpaceName).get());
      EXPECT_EQ(2 * width + xPosSpace3, wRefSpace3_->getDouble(Daylighting_ReferencePointFields::XCoordinateofReferencePoint).get());
      EXPECT_EQ(yPosSpace3, wRefSpace3_->getDouble(Daylighting_ReferencePointFields::YCoordinateofReferencePoint).get());
      EXPECT_EQ(zPosSpace3, wRefSpace3_->getDouble(Daylighting_ReferencePointFields::ZCoordinateofReferencePoint).get());
    }
  }

  // When including Space translation (new E+ 9.6.0)
  // TODO: given the current API of DaylightingControls in OS, we cannot translate the DaylightingControls to individual Spaces, so we do exactly the
  // same thing as before: we place them for the Zone itself
  {
    ft.setExcludeSpaceTranslation(false);

    Workspace w = ft.translateModel(m);
    auto ds = w.getObjectsByType(IddObjectType::Daylighting_Controls);
    ASSERT_EQ(1, ds.size());
    auto d = ds[0];
    // High level info is populated from the **Primary** control + fields on ThermalZone
    EXPECT_EQ(z.nameString(), d.getString(Daylighting_ControlsFields::ZoneorSpaceName).get());  // TODO: this is the **Zone** name for now
    EXPECT_TRUE(d.isEmpty(Daylighting_ControlsFields::DaylightingMethod));
    EXPECT_EQ(alwaysOn.nameString(), d.getString(Daylighting_ControlsFields::AvailabilityScheduleName).get());
    EXPECT_EQ(dSpace2.lightingControlType(), d.getString(Daylighting_ControlsFields::LightingControlType).get());
    EXPECT_EQ(dSpace2.minimumInputPowerFractionforContinuousDimmingControl(),
              d.getDouble(Daylighting_ControlsFields::MinimumInputPowerFractionforContinuousorContinuousOffDimmingControl).get());
    EXPECT_EQ(dSpace2.minimumLightOutputFractionforContinuousDimmingControl(),
              d.getDouble(Daylighting_ControlsFields::MinimumLightOutputFractionforContinuousorContinuousOffDimmingControl).get());
    EXPECT_EQ(dSpace2.numberofSteppedControlSteps(), d.getInt(Daylighting_ControlsFields::NumberofSteppedControlSteps).get());
    EXPECT_EQ(dSpace2.probabilityLightingwillbeResetWhenNeededinManualSteppedControl(),
              d.getDouble(Daylighting_ControlsFields::ProbabilityLightingwillbeResetWhenNeededinManualSteppedControl).get());

    EXPECT_EQ(dSpace2.nameString(), d.getString(Daylighting_ControlsFields::GlareCalculationDaylightingReferencePointName).get());

    EXPECT_EQ(-dSpace2.phiRotationAroundZAxis(),
              d.getDouble(Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis).get());
    EXPECT_EQ(dSpace2.maximumAllowableDiscomfortGlareIndex().get(),
              d.getDouble(Daylighting_ControlsFields::MaximumAllowableDiscomfortGlareIndex).get());
    EXPECT_TRUE(d.isEmpty(Daylighting_ControlsFields::DaylightingMethod));
    EXPECT_TRUE(d.isEmpty(Daylighting_ControlsFields::DElightGriddingResolution));

    ASSERT_EQ(2, d.numExtensibleGroups());
    {
      auto eg1 = d.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
      EXPECT_EQ(dSpace2.nameString(), eg1.getString(Daylighting_ControlsExtensibleFields::DaylightingReferencePointName).get());
      EXPECT_EQ(z.fractionofZoneControlledbyPrimaryDaylightingControl(),
                eg1.getDouble(Daylighting_ControlsExtensibleFields::FractionofLightsControlledbyReferencePoint).get());
      EXPECT_EQ(dSpace2.illuminanceSetpoint(), eg1.getDouble(Daylighting_ControlsExtensibleFields::IlluminanceSetpointatReferencePoint).get());
    }

    {
      auto eg2 = d.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
      EXPECT_EQ(dSpace3.nameString(), eg2.getString(Daylighting_ControlsExtensibleFields::DaylightingReferencePointName).get());
      EXPECT_EQ(z.fractionofZoneControlledbySecondaryDaylightingControl(),
                eg2.getDouble(Daylighting_ControlsExtensibleFields::FractionofLightsControlledbyReferencePoint).get());
      EXPECT_EQ(dSpace3.illuminanceSetpoint(), eg2.getDouble(Daylighting_ControlsExtensibleFields::IlluminanceSetpointatReferencePoint).get());
    }

    EXPECT_EQ(2, w.getObjectsByType(IddObjectType::Daylighting_ReferencePoint).size());

    {
      auto wRefSpace2_ = w.getObjectByTypeAndName(IddObjectType::Daylighting_ReferencePoint, dSpace2.nameString());
      ASSERT_TRUE(wRefSpace2_);
      // TODO: this is the **Zone** name for now
      EXPECT_EQ(z.nameString(), wRefSpace2_->getString(Daylighting_ReferencePointFields::ZoneorSpaceName).get());
      EXPECT_EQ(width + xPosSpace2, wRefSpace2_->getDouble(Daylighting_ReferencePointFields::XCoordinateofReferencePoint).get());
      EXPECT_EQ(yPosSpace2, wRefSpace2_->getDouble(Daylighting_ReferencePointFields::YCoordinateofReferencePoint).get());
      EXPECT_EQ(zPosSpace2, wRefSpace2_->getDouble(Daylighting_ReferencePointFields::ZCoordinateofReferencePoint).get());
    }
    {
      auto wRefSpace3_ = w.getObjectByTypeAndName(IddObjectType::Daylighting_ReferencePoint, dSpace3.nameString());
      ASSERT_TRUE(wRefSpace3_);
      // TODO: this is the **Zone** name for now
      EXPECT_EQ(z.nameString(), wRefSpace3_->getString(Daylighting_ReferencePointFields::ZoneorSpaceName).get());
      EXPECT_EQ(2 * width + xPosSpace3, wRefSpace3_->getDouble(Daylighting_ReferencePointFields::XCoordinateofReferencePoint).get());
      EXPECT_EQ(yPosSpace3, wRefSpace3_->getDouble(Daylighting_ReferencePointFields::YCoordinateofReferencePoint).get());
      EXPECT_EQ(zPosSpace3, wRefSpace3_->getDouble(Daylighting_ReferencePointFields::ZCoordinateofReferencePoint).get());
    }
  }
}
