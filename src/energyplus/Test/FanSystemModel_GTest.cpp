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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/FanSystemModel.hpp"
#include "../../model/FanSystemModel_Impl.hpp"

#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/ThermalZone.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/BranchList_FieldEnums.hxx>
#include <utilities/idd/Branch_FieldEnums.hxx>
#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_FanSystemModel) {

  ForwardTranslator ft;

  Model m;
  FanSystemModel fan(m);

  fan.setName("My FanSystemModel");

  // Availability Schedule Name: Required Object
  ScheduleConstant sch(m);
  sch.setName("Fan Avail Schedule");
  EXPECT_TRUE(fan.setAvailabilitySchedule(sch));
  EXPECT_EQ(sch, fan.availabilitySchedule());

  // Design Maximum Air Flow Rate: Required Double
  EXPECT_TRUE(fan.setDesignMaximumAirFlowRate(0.1));

  // Speed Control Method: Required String
  EXPECT_TRUE(fan.setSpeedControlMethod("Discrete"));

  // Electric Power Minimum Flow Rate Fraction: Required Double
  EXPECT_TRUE(fan.setElectricPowerMinimumFlowRateFraction(0.775));

  // Design Pressure Rise: Required Double
  EXPECT_TRUE(fan.setDesignPressureRise(121.5));

  // Motor Efficiency: Required Double
  EXPECT_TRUE(fan.setMotorEfficiency(0.54));

  // Motor In Air Stream Fraction: Required Double
  EXPECT_TRUE(fan.setMotorInAirStreamFraction(0.87));

  // Design Electric Power Consumption: Required Double
  EXPECT_TRUE(fan.setDesignElectricPowerConsumption(3112.8));

  // Design Power Sizing Method: Required String
  EXPECT_TRUE(fan.setDesignPowerSizingMethod("TotalEfficiencyAndPressure"));

  // Electric Power Per Unit Flow Rate: Required Double
  EXPECT_TRUE(fan.setElectricPowerPerUnitFlowRate(1254.0));

  // Electric Power Per Unit Flow Rate Per Unit Pressure: Required Double
  EXPECT_TRUE(fan.setElectricPowerPerUnitFlowRatePerUnitPressure(1.345));

  // Fan Total Efficiency: Required Double
  EXPECT_TRUE(fan.setFanTotalEfficiency(0.59));

  // Electric Power Function of Flow Fraction Curve Name: Optional Object
  CurveCubic fanPowerFuncFlowCurve(m);
  fanPowerFuncFlowCurve.setName("Fan Curve");
  EXPECT_TRUE(fan.setElectricPowerFunctionofFlowFractionCurve(fanPowerFuncFlowCurve));

  // Night Ventilation Mode Pressure Rise: Optional Double
  EXPECT_TRUE(fan.setNightVentilationModePressureRise(356.0));

  // Night Ventilation Mode Flow Fraction: Optional Double
  EXPECT_TRUE(fan.setNightVentilationModeFlowFraction(0.37));

  // Motor Loss Zone Name: Optional Object
  ThermalZone z(m);
  z.setName("TZ");

  EXPECT_TRUE(fan.setMotorLossZone(z));

  // Motor Loss Radiative Fraction: Required Double
  EXPECT_TRUE(fan.setMotorLossRadiativeFraction(0.15));

  // End-Use Subcategory: Required String
  EXPECT_TRUE(fan.setEndUseSubcategory("My Fan"));

  // Speeds
  EXPECT_TRUE(fan.setSpeeds({FanSystemModelSpeed(0.25, 0.1), FanSystemModelSpeed(0.5, 0.3), FanSystemModelSpeed(0.75, 0.7)}));

  // Not assign to any loop, or hvac/zonehvac component
  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::Fan_SystemModel));
    EXPECT_EQ(0u, idfObjs.size());
  }

  {
    AirLoopHVAC a(m);
    Node supplyOutletNode = a.supplyOutletNode();
    EXPECT_TRUE(fan.addToNode(supplyOutletNode));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idf_fans(w.getObjectsByType(IddObjectType::Fan_SystemModel));
    ASSERT_EQ(1u, idf_fans.size());
    WorkspaceObject idf_fan(idf_fans[0]);

    EXPECT_EQ("My FanSystemModel", idf_fan.getString(Fan_SystemModelFields::Name).get());

    // Availability Schedule Name: Required Object
    EXPECT_EQ("Fan Avail Schedule", idf_fan.getString(Fan_SystemModelFields::AvailabilityScheduleName).get());

    // Design Maximum Air Flow Rate: Required Double
    EXPECT_EQ(0.1, idf_fan.getDouble(Fan_SystemModelFields::DesignMaximumAirFlowRate).get());

    // Speed Control Method: Required String
    EXPECT_EQ("Discrete", idf_fan.getString(Fan_SystemModelFields::SpeedControlMethod).get());

    // Electric Power Minimum Flow Rate Fraction: Required Double
    EXPECT_EQ(0.775, idf_fan.getDouble(Fan_SystemModelFields::ElectricPowerMinimumFlowRateFraction).get());

    // Design Pressure Rise: Required Double
    EXPECT_EQ(121.5, idf_fan.getDouble(Fan_SystemModelFields::DesignPressureRise).get());

    // Motor Efficiency: Required Double
    EXPECT_EQ(0.54, idf_fan.getDouble(Fan_SystemModelFields::MotorEfficiency).get());

    // Motor In Air Stream Fraction: Required Double
    EXPECT_EQ(0.87, idf_fan.getDouble(Fan_SystemModelFields::MotorInAirStreamFraction).get());

    // Design Electric Power Consumption: Required Double
    EXPECT_EQ(3112.8, idf_fan.getDouble(Fan_SystemModelFields::DesignElectricPowerConsumption).get());

    // Design Power Sizing Method: Required String
    EXPECT_EQ("TotalEfficiencyAndPressure", idf_fan.getString(Fan_SystemModelFields::DesignPowerSizingMethod).get());

    // Electric Power Per Unit Flow Rate: Required Double
    EXPECT_EQ(1254.0, idf_fan.getDouble(Fan_SystemModelFields::ElectricPowerPerUnitFlowRate).get());

    // Electric Power Per Unit Flow Rate Per Unit Pressure: Required Double
    EXPECT_EQ(1.345, idf_fan.getDouble(Fan_SystemModelFields::ElectricPowerPerUnitFlowRatePerUnitPressure).get());

    // Fan Total Efficiency: Required Double
    EXPECT_EQ(0.59, idf_fan.getDouble(Fan_SystemModelFields::FanTotalEfficiency).get());

    // Electric Power Function of Flow Fraction Curve Name: Optional Object
    EXPECT_EQ("Fan Curve", idf_fan.getString(Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName).get());

    // Night Ventilation Mode Pressure Rise: Optional Double
    EXPECT_EQ(356.0, idf_fan.getDouble(Fan_SystemModelFields::NightVentilationModePressureRise).get());

    // Night Ventilation Mode Flow Fraction: Optional Double
    EXPECT_EQ(0.37, idf_fan.getDouble(Fan_SystemModelFields::NightVentilationModeFlowFraction).get());

    // Motor Loss Zone Name: Optional Object
    EXPECT_EQ("TZ", idf_fan.getString(Fan_SystemModelFields::MotorLossZoneName).get());

    // Motor Loss Radiative Fraction: Required Double
    EXPECT_TRUE(fan.setMotorLossRadiativeFraction(0.15));
    EXPECT_EQ(0.15, fan.motorLossRadiativeFraction());
    EXPECT_EQ(0.15, idf_fan.getDouble(Fan_SystemModelFields::MotorLossRadiativeFraction).get());

    // End-Use Subcategory: Required String
    EXPECT_EQ("My Fan", idf_fan.getString(Fan_SystemModelFields::EndUseSubcategory).get());

    // Number of speeds
    EXPECT_EQ(3, idf_fan.getInt(Fan_SystemModelFields::NumberofSpeeds).get());
    // Should have three branches: supply inlet, the one with the centralHP, supply outlet
    ASSERT_EQ(3u, idf_fan.extensibleGroups().size());
    // Get the Central HP one
    {
      WorkspaceExtensibleGroup w_eg = idf_fan.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
      EXPECT_EQ(0.25, w_eg.getDouble(Fan_SystemModelExtensibleFields::SpeedFlowFraction).get());
      EXPECT_EQ(0.1, w_eg.getDouble(Fan_SystemModelExtensibleFields::SpeedElectricPowerFraction).get());
    }
    {
      WorkspaceExtensibleGroup w_eg = idf_fan.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
      EXPECT_EQ(0.5, w_eg.getDouble(Fan_SystemModelExtensibleFields::SpeedFlowFraction).get());
      EXPECT_EQ(0.3, w_eg.getDouble(Fan_SystemModelExtensibleFields::SpeedElectricPowerFraction).get());
    }
    {
      WorkspaceExtensibleGroup w_eg = idf_fan.extensibleGroups()[2].cast<WorkspaceExtensibleGroup>();
      EXPECT_EQ(0.75, w_eg.getDouble(Fan_SystemModelExtensibleFields::SpeedFlowFraction).get());
      EXPECT_EQ(0.7, w_eg.getDouble(Fan_SystemModelExtensibleFields::SpeedElectricPowerFraction).get());
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_FanSystemModel_AirLoopHVAC) {

  ForwardTranslator ft;

  Model m;
  FanSystemModel fan(m);

  AirLoopHVAC a(m);
  Node supplyOutletNode = a.supplyOutletNode();
  EXPECT_TRUE(fan.addToNode(supplyOutletNode));

  Workspace w = ft.translateModel(m);
  WorkspaceObjectVector idf_fans(w.getObjectsByType(IddObjectType::Fan_SystemModel));
  ASSERT_EQ(1u, idf_fans.size());
  WorkspaceObject idf_fan(idf_fans[0]);

  EXPECT_EQ(idf_fan.getString(Fan_SystemModelFields::AirInletNodeName).get(), fan.inletModelObject().get().name());

  EXPECT_EQ(idf_fan.getString(Fan_SystemModelFields::AirOutletNodeName).get(), fan.outletModelObject().get().name());

  // Go from AirLoopHVAC to BranchList to Branch
  WorkspaceObjectVector idf_airloops(w.getObjectsByType(IddObjectType::AirLoopHVAC));

  auto _wo = w.getObjectByTypeAndName(IddObjectType::AirLoopHVAC, a.name().get());
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_airLoopHVAC = _wo.get();
  WorkspaceObject idf_brlist = idf_airLoopHVAC.getTarget(AirLoopHVACFields::BranchListName).get();

  // Should have one branch only
  ASSERT_EQ(1u, idf_brlist.extensibleGroups().size());
  WorkspaceExtensibleGroup w_eg = idf_brlist.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  WorkspaceObject idf_branch = w_eg.getTarget(BranchListExtensibleFields::BranchName).get();

  // There should be only one equipment on the branch
  ASSERT_EQ(1u, idf_branch.extensibleGroups().size());
  WorkspaceExtensibleGroup w_eg2 = idf_branch.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();

  EXPECT_EQ("Fan:SystemModel", w_eg2.getString(BranchExtensibleFields::ComponentObjectType).get());
  EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentName).get(), fan.name());

  EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentInletNodeName).get(), fan.inletModelObject().get().nameString());

  EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentOutletNodeName).get(), fan.outletModelObject().get().nameString());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_FanSystemModel) {

  ReverseTranslator reverseTranslator;

  Workspace w(StrictnessLevel::None, IddFileType::EnergyPlus);
  OptionalWorkspaceObject _i_fan = w.addObject(IdfObject(IddObjectType::Fan_SystemModel));
  ASSERT_TRUE(_i_fan);
  _i_fan->setName("Zone1FanCoilFan");

  OptionalWorkspaceObject _i_sch = w.addObject(IdfObject(IddObjectType::Schedule_Constant));
  ASSERT_TRUE(_i_sch);

  IdfObject idf_sch(IddObjectType::Schedule_Constant);
  _i_sch->setName("FanAndCoilAvailSched");
  EXPECT_TRUE(_i_sch->setDouble(Schedule_ConstantFields::HourlyValue, 1.0));
  EXPECT_TRUE(_i_fan->setPointer(Fan_SystemModelFields::AvailabilityScheduleName, _i_sch->handle()));

  EXPECT_TRUE(_i_fan->setString(Fan_SystemModelFields::DesignMaximumAirFlowRate, "AUTOSIZE"));
  EXPECT_TRUE(_i_fan->setString(Fan_SystemModelFields::SpeedControlMethod, "Discrete"));
  EXPECT_TRUE(_i_fan->setDouble(Fan_SystemModelFields::ElectricPowerMinimumFlowRateFraction, 0.0));
  EXPECT_TRUE(_i_fan->setDouble(Fan_SystemModelFields::DesignPressureRise, 75.0));
  EXPECT_TRUE(_i_fan->setDouble(Fan_SystemModelFields::MotorEfficiency, 0.9));
  EXPECT_TRUE(_i_fan->setDouble(Fan_SystemModelFields::MotorInAirStreamFraction, 1.0));
  EXPECT_TRUE(_i_fan->setString(Fan_SystemModelFields::DesignElectricPowerConsumption, "AUTOSIZE"));
  EXPECT_TRUE(_i_fan->setString(Fan_SystemModelFields::DesignPowerSizingMethod, "TotalEfficiencyAndPressure"));
  EXPECT_TRUE(_i_fan->setDouble(Fan_SystemModelFields::FanTotalEfficiency, 0.5));
  EXPECT_TRUE(_i_fan->setString(Fan_SystemModelFields::EndUseSubcategory, "FanEndUse"));
  EXPECT_TRUE(_i_fan->setInt(Fan_SystemModelFields::NumberofSpeeds, 3));

  WorkspaceExtensibleGroup eg1 = _i_fan->pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
  EXPECT_TRUE(eg1.setDouble(Fan_SystemModelExtensibleFields::SpeedFlowFraction, 0.33));
  EXPECT_TRUE(eg1.setDouble(Fan_SystemModelExtensibleFields::SpeedElectricPowerFraction, 0.12));

  WorkspaceExtensibleGroup eg2 = _i_fan->pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
  EXPECT_TRUE(eg2.setDouble(Fan_SystemModelExtensibleFields::SpeedFlowFraction, 0.66));
  EXPECT_TRUE(eg2.setDouble(Fan_SystemModelExtensibleFields::SpeedElectricPowerFraction, 0.51));

  WorkspaceExtensibleGroup eg3 = _i_fan->pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
  EXPECT_TRUE(eg3.setDouble(Fan_SystemModelExtensibleFields::SpeedFlowFraction, 1.0));
  EXPECT_TRUE(eg3.setDouble(Fan_SystemModelExtensibleFields::SpeedElectricPowerFraction, 1.0));

  //   Fan:SystemModel,
  //     Zone1FanCoilFan,         !- Name
  //     FanAndCoilAvailSched,    !- Availability Schedule Name
  //     Zone1FanCoilOAMixerOutletNode,  !- Air Inlet Node Name
  //     Zone1FanCoilFanOutletNode,  !- Air Outlet Node Name
  //     AUTOSIZE,                !- Design Maximum Air Flow Rate {m3/s}
  //     Discrete,                !- Speed Control Method
  //     0.0,                     !- Electric Power Minimum Flow Rate Fraction
  //     75.0,                    !- Design Pressure Rise {Pa}
  //     0.9,                     !- Motor Efficiency
  //     1.0,                     !- Motor In Air Stream Fraction
  //     AUTOSIZE,                !- Design Electric Power Consumption {W}
  //     TotalEfficiencyAndPressure,  !- Design Power Sizing Method
  //     ,                        !- Electric Power Per Unit Flow Rate {W/(m3/s)}
  //     ,                        !- Electric Power Per Unit Flow Rate Per Unit Pressure {W/((m3/s)-Pa)}
  //     0.50,                    !- Fan Total Efficiency
  //     ,                        !- Electric Power Function of Flow Fraction Curve Name
  //     ,                        !- Night Ventilation Mode Pressure Rise {Pa}
  //     ,                        !- Night Ventilation Mode Flow Fraction
  //     ,                        !- Motor Loss Zone Name
  //     ,                        !- Motor Loss Radiative Fraction
  //     FanEndUse,                 !- End-Use Subcategory
  //     3,                       !- Number of Speeds
  //     0.33,                    !- Speed 1 Flow Fraction
  //     0.12,                    !- Speed 1 Electric Power Fraction
  //     0.66,                    !- Speed 2 Flow Fraction
  //     0.51,                    !- Speed 2 Electric Power Fraction
  //     1.0,                     !- Speed 3 Flow Fraction
  //     1.0;                     !- Speed 3 Electric Power Fraction

  // To avoid other warnings, we add required objects
  OptionalWorkspaceObject _i_globalGeometryRules = w.addObject(IdfObject(IddObjectType::GlobalGeometryRules));
  ASSERT_TRUE(_i_globalGeometryRules);

  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::StartingVertexPosition, "UpperLeftCorner");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::VertexEntryDirection, "Counterclockwise");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::CoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::DaylightingReferencePointCoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::RectangularSurfaceCoordinateSystem, "Relative");

  OptionalWorkspaceObject _i_building = w.addObject(IdfObject(IddObjectType::Building));
  ASSERT_TRUE(_i_building);

  {
    ASSERT_NO_THROW(reverseTranslator.translateWorkspace(w));
    Model model = reverseTranslator.translateWorkspace(w);
    EXPECT_TRUE(reverseTranslator.errors().empty());
    EXPECT_TRUE(reverseTranslator.warnings().empty());

    std::vector<openstudio::model::FanSystemModel> fans = model.getModelObjects<openstudio::model::FanSystemModel>();
    ASSERT_EQ(static_cast<unsigned>(1), fans.size());
    FanSystemModel fan = fans[0];

    EXPECT_EQ("Zone1FanCoilFan", fan.nameString());
    EXPECT_EQ("FanAndCoilAvailSched", fan.availabilitySchedule().nameString());
    EXPECT_TRUE(fan.isDesignMaximumAirFlowRateAutosized());
    EXPECT_EQ("Discrete", fan.speedControlMethod());
    EXPECT_EQ(0.0, fan.electricPowerMinimumFlowRateFraction());
    EXPECT_EQ(75.0, fan.designPressureRise());
    EXPECT_EQ(0.9, fan.motorEfficiency());
    EXPECT_EQ(1.0, fan.motorInAirStreamFraction());
    EXPECT_TRUE(fan.isDesignElectricPowerConsumptionAutosized());
    EXPECT_EQ("TotalEfficiencyAndPressure", fan.designPowerSizingMethod());

    // from Ctor
    EXPECT_EQ(840.0, fan.electricPowerPerUnitFlowRate());
    EXPECT_EQ(1.66667, fan.electricPowerPerUnitFlowRatePerUnitPressure());

    EXPECT_EQ(0.5, fan.fanTotalEfficiency());
    EXPECT_FALSE(fan.electricPowerFunctionofFlowFractionCurve());
    EXPECT_FALSE(fan.nightVentilationModePressureRise());
    EXPECT_FALSE(fan.nightVentilationModeFlowFraction());
    EXPECT_FALSE(fan.motorLossZone());
    // from Ctor
    EXPECT_EQ(0.0, fan.motorLossRadiativeFraction());

    EXPECT_EQ(3, fan.numExtensibleGroups());
    EXPECT_EQ(3, fan.numberofSpeeds());
    std::vector<FanSystemModelSpeed> speeds = fan.speeds();
    EXPECT_EQ(3, speeds.size());
    EXPECT_EQ(0.33, speeds[0].flowFraction());
    EXPECT_EQ(0.12, speeds[0].electricPowerFraction());
    EXPECT_EQ(0.66, speeds[1].flowFraction());
    EXPECT_EQ(0.51, speeds[1].electricPowerFraction());
    EXPECT_EQ(1.00, speeds[2].flowFraction());
    EXPECT_EQ(1.00, speeds[2].electricPowerFraction());
  }

  WorkspaceExtensibleGroup invalid_eg = _i_fan->pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
  EXPECT_TRUE(invalid_eg.setDouble(Fan_SystemModelExtensibleFields::SpeedFlowFraction, 1.5));  // Should be [0, 1]
  EXPECT_TRUE(invalid_eg.setDouble(Fan_SystemModelExtensibleFields::SpeedElectricPowerFraction, 1.0));

  // Right now, numberofSpeeds is still set to 3, so this group will be ignored, with a warning
  {
    ASSERT_NO_THROW(reverseTranslator.translateWorkspace(w));
    Model model = reverseTranslator.translateWorkspace(w);
    EXPECT_EQ(0, reverseTranslator.errors().size());
    EXPECT_EQ(1, reverseTranslator.warnings().size());

    std::vector<openstudio::model::FanSystemModel> fans = model.getModelObjects<openstudio::model::FanSystemModel>();
    ASSERT_EQ(static_cast<unsigned>(1), fans.size());
    FanSystemModel fan = fans[0];

    EXPECT_EQ(3, fan.numExtensibleGroups());
    EXPECT_EQ(3, fan.numberofSpeeds());
  }

  // Ok, this time the invalid group will be processed
  EXPECT_TRUE(_i_fan->setInt(Fan_SystemModelFields::NumberofSpeeds, 4));
  {
    // I don't want it to throw, it should log an error, and ignore that speed
    ASSERT_NO_THROW(reverseTranslator.translateWorkspace(w));
    Model model = reverseTranslator.translateWorkspace(w);
    EXPECT_EQ(1, reverseTranslator.errors().size());
    EXPECT_TRUE(reverseTranslator.warnings().empty());

    std::vector<openstudio::model::FanSystemModel> fans = model.getModelObjects<openstudio::model::FanSystemModel>();
    ASSERT_EQ(static_cast<unsigned>(1), fans.size());
    FanSystemModel fan = fans[0];

    EXPECT_EQ(3, fan.numExtensibleGroups());
    EXPECT_EQ(3, fan.numberofSpeeds());
  }
}
