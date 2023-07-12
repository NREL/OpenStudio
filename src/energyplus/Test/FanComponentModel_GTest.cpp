/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/FanComponentModel.hpp"
#include "../../model/FanComponentModel_Impl.hpp"

#include "../../model/Model.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Curve.hpp"
#include "../../model/CurveExponentialSkewNormal.hpp"
#include "../../model/CurveQuartic.hpp"
#include "../../model/CurveFanPressureRise.hpp"
#include "../../model/CurveRectangularHyperbola2.hpp"
#include "../../model/CurveSigmoid.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/CurveRectangularHyperbola1.hpp"
#include "../../model/CurveExponentialDecay.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Fan_ComponentModel_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/BranchList_FieldEnums.hxx>
#include <utilities/idd/Branch_FieldEnums.hxx>
#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>

#include <utilities/idd/Curve_ExponentialSkewNormal_FieldEnums.hxx>
#include <utilities/idd/Curve_Quartic_FieldEnums.hxx>
#include <utilities/idd/Curve_FanPressureRise_FieldEnums.hxx>
#include <utilities/idd/Curve_RectangularHyperbola2_FieldEnums.hxx>
#include <utilities/idd/Curve_Sigmoid_FieldEnums.hxx>
#include <utilities/idd/Curve_Linear_FieldEnums.hxx>
#include <utilities/idd/Curve_RectangularHyperbola1_FieldEnums.hxx>
#include <utilities/idd/Curve_ExponentialDecay_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_FanComponentModel) {

  ForwardTranslator ft;

  // Taken from EnergyPlus/testfiles/5ZoneWarmestVFD_FCMAuto.idf, v9.4.0
  Model m;
  FanComponentModel fanComponentModel(m);

  fanComponentModel.setName("My FanComponentModel");
  ScheduleConstant sch(m);
  sch.setName("Fan Avail Schedule");
  EXPECT_TRUE(fanComponentModel.setAvailabilitySchedule(sch));
  EXPECT_TRUE(fanComponentModel.setMaximumFlowRate(1.5));
  EXPECT_TRUE(fanComponentModel.setMinimumFlowRate(0.05));
  EXPECT_TRUE(fanComponentModel.setFanSizingFactor(1.01));
  EXPECT_TRUE(fanComponentModel.setFanWheelDiameter(0.3048));
  EXPECT_TRUE(fanComponentModel.setFanOutletArea(0.0873288576));
  EXPECT_TRUE(fanComponentModel.setMaximumFanStaticEfficiency(0.514));
  EXPECT_TRUE(fanComponentModel.setEulerNumberatMaximumFanStaticEfficiency(9.76));
  EXPECT_TRUE(fanComponentModel.setMaximumDimensionlessFanAirflow(0.160331811647483));
  EXPECT_TRUE(fanComponentModel.setMotorFanPulleyRatio(0.96));
  EXPECT_TRUE(fanComponentModel.setBeltMaximumTorque(10.0));
  EXPECT_TRUE(fanComponentModel.setBeltSizingFactor(1.98));
  EXPECT_TRUE(fanComponentModel.setBeltFractionalTorqueTransition(0.167));
  EXPECT_TRUE(fanComponentModel.setMotorMaximumSpeed(1800.0));
  EXPECT_TRUE(fanComponentModel.setMaximumMotorOutputPower(10000.0));
  EXPECT_TRUE(fanComponentModel.setMotorSizingFactor(1.99));
  EXPECT_TRUE(fanComponentModel.setMotorInAirstreamFraction(0.5));
  EXPECT_TRUE(fanComponentModel.setVFDEfficiencyType("Power"));
  EXPECT_TRUE(fanComponentModel.setMaximumVFDOutputPower(11000.0));
  EXPECT_TRUE(fanComponentModel.setVFDSizingFactor(1.95));

  CurveFanPressureRise vSDExample(m);
  vSDExample.setName("VSD Example");
  EXPECT_TRUE(fanComponentModel.setFanPressureRiseCurve(vSDExample));

  CurveLinear diagnosticSPR(m);
  diagnosticSPR.setName("DiagnosticSPR");
  EXPECT_TRUE(fanComponentModel.setDuctStaticPressureResetCurve(diagnosticSPR));

  CurveExponentialSkewNormal fanEff120CPLANormal(m);
  fanEff120CPLANormal.setName("FanEff120CPLANormal");
  EXPECT_TRUE(fanComponentModel.setNormalizedFanStaticEfficiencyCurveNonStallRegion(fanEff120CPLANormal));

  CurveExponentialSkewNormal fanEff120CPLAStall(m);
  fanEff120CPLAStall.setName("FanEff120CPLAStall");
  EXPECT_TRUE(fanComponentModel.setNormalizedFanStaticEfficiencyCurveStallRegion(fanEff120CPLAStall));

  CurveSigmoid fanDimFlowNormal(m);
  fanDimFlowNormal.setName("FanDimFlowNormal");
  EXPECT_TRUE(fanComponentModel.setNormalizedDimensionlessAirflowCurveNonStallRegion(fanDimFlowNormal));

  CurveSigmoid fanDimFlowStall(m);
  fanDimFlowStall.setName("FanDimFlowStall");
  EXPECT_TRUE(fanComponentModel.setNormalizedDimensionlessAirflowCurveStallRegion(fanDimFlowStall));

  CurveQuartic beltMaxEffMedium(m);
  beltMaxEffMedium.setName("BeltMaxEffMedium");
  EXPECT_TRUE(fanComponentModel.setMaximumBeltEfficiencyCurve(beltMaxEffMedium));

  CurveRectangularHyperbola2 beltPartLoadRegion1(m);
  beltPartLoadRegion1.setName("BeltPartLoadRegion1");
  EXPECT_TRUE(fanComponentModel.setNormalizedBeltEfficiencyCurveRegion1(beltPartLoadRegion1));

  CurveExponentialDecay beltPartLoadRegion2(m);
  beltPartLoadRegion2.setName("BeltPartLoadRegion2");
  EXPECT_TRUE(fanComponentModel.setNormalizedBeltEfficiencyCurveRegion2(beltPartLoadRegion2));

  CurveRectangularHyperbola2 beltPartLoadRegion3(m);
  beltPartLoadRegion3.setName("BeltPartLoadRegion3");
  EXPECT_TRUE(fanComponentModel.setNormalizedBeltEfficiencyCurveRegion3(beltPartLoadRegion3));

  CurveRectangularHyperbola1 motorMaxEffAvg(m);
  motorMaxEffAvg.setName("MotorMaxEffAvg");
  EXPECT_TRUE(fanComponentModel.setMaximumMotorEfficiencyCurve(motorMaxEffAvg));

  CurveRectangularHyperbola2 motorPartLoad(m);
  motorPartLoad.setName("MotorPartLoad");
  EXPECT_TRUE(fanComponentModel.setNormalizedMotorEfficiencyCurve(motorPartLoad));

  CurveRectangularHyperbola2 vFDPartLoad(m);
  vFDPartLoad.setName("VFDPartLoad");
  EXPECT_TRUE(fanComponentModel.setVFDEfficiencyCurve(vFDPartLoad));

  fanComponentModel.setEndUseSubcategory("Fans");

  // Not assigned to any loop, or hvac/zonehvac component
  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::Fan_ComponentModel));
    EXPECT_EQ(0u, idfObjs.size());
  }

  {
    AirLoopHVAC a(m);
    Node supplyOutletNode = a.supplyOutletNode();
    EXPECT_TRUE(fanComponentModel.addToNode(supplyOutletNode));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idf_fans(w.getObjectsByType(IddObjectType::Fan_ComponentModel));
    ASSERT_EQ(1u, idf_fans.size());
    WorkspaceObject idf_fan(idf_fans[0]);

    EXPECT_EQ("My FanComponentModel", idf_fan.getString(Fan_ComponentModelFields::Name).get());

    // Availability Schedule Name: Required Object
    EXPECT_EQ("Fan Avail Schedule", idf_fan.getString(Fan_ComponentModelFields::AvailabilityScheduleName).get());

    EXPECT_EQ(1.5, idf_fan.getDouble(Fan_ComponentModelFields::MaximumFlowRate).get());
    EXPECT_EQ(0.05, idf_fan.getDouble(Fan_ComponentModelFields::MinimumFlowRate).get());
    EXPECT_EQ(1.01, idf_fan.getDouble(Fan_ComponentModelFields::FanSizingFactor).get());
    EXPECT_EQ(0.3048, idf_fan.getDouble(Fan_ComponentModelFields::FanWheelDiameter).get());
    EXPECT_EQ(0.0873288576, idf_fan.getDouble(Fan_ComponentModelFields::FanOutletArea).get());
    EXPECT_EQ(0.514, idf_fan.getDouble(Fan_ComponentModelFields::MaximumFanStaticEfficiency).get());
    EXPECT_EQ(9.76, idf_fan.getDouble(Fan_ComponentModelFields::EulerNumberatMaximumFanStaticEfficiency).get());
    EXPECT_EQ(0.160331811647483, idf_fan.getDouble(Fan_ComponentModelFields::MaximumDimensionlessFanAirflow).get());
    EXPECT_EQ(0.96, idf_fan.getDouble(Fan_ComponentModelFields::MotorFanPulleyRatio).get());
    EXPECT_EQ(10.0, idf_fan.getDouble(Fan_ComponentModelFields::BeltMaximumTorque).get());
    EXPECT_EQ(1.98, idf_fan.getDouble(Fan_ComponentModelFields::BeltSizingFactor).get());
    EXPECT_EQ(0.167, idf_fan.getDouble(Fan_ComponentModelFields::BeltFractionalTorqueTransition).get());
    EXPECT_EQ(1800.0, idf_fan.getDouble(Fan_ComponentModelFields::MotorMaximumSpeed).get());
    EXPECT_EQ(10000.0, idf_fan.getDouble(Fan_ComponentModelFields::MaximumMotorOutputPower).get());
    EXPECT_EQ(1.99, idf_fan.getDouble(Fan_ComponentModelFields::MotorSizingFactor).get());
    EXPECT_EQ(0.5, idf_fan.getDouble(Fan_ComponentModelFields::MotorInAirstreamFraction).get());
    EXPECT_EQ(11000.0, idf_fan.getDouble(Fan_ComponentModelFields::MaximumVFDOutputPower).get());
    EXPECT_EQ(1.95, idf_fan.getDouble(Fan_ComponentModelFields::VFDSizingFactor).get());

    EXPECT_EQ("Power", idf_fan.getString(Fan_ComponentModelFields::VFDEfficiencyType).get());
    EXPECT_EQ(vSDExample.nameString(), idf_fan.getString(Fan_ComponentModelFields::FanPressureRiseCurveName).get());
    EXPECT_EQ(diagnosticSPR.nameString(), idf_fan.getString(Fan_ComponentModelFields::DuctStaticPressureResetCurveName).get());
    EXPECT_EQ(fanEff120CPLANormal.nameString(),
              idf_fan.getString(Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameNonStallRegion).get());
    EXPECT_EQ(fanEff120CPLAStall.nameString(), idf_fan.getString(Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameStallRegion).get());
    EXPECT_EQ(fanDimFlowNormal.nameString(),
              idf_fan.getString(Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameNonStallRegion).get());
    EXPECT_EQ(fanDimFlowStall.nameString(), idf_fan.getString(Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameStallRegion).get());
    EXPECT_EQ(beltMaxEffMedium.nameString(), idf_fan.getString(Fan_ComponentModelFields::MaximumBeltEfficiencyCurveName).get());
    EXPECT_EQ(beltPartLoadRegion1.nameString(), idf_fan.getString(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion1).get());
    EXPECT_EQ(beltPartLoadRegion2.nameString(), idf_fan.getString(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion2).get());
    EXPECT_EQ(beltPartLoadRegion3.nameString(), idf_fan.getString(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion3).get());
    EXPECT_EQ(motorMaxEffAvg.nameString(), idf_fan.getString(Fan_ComponentModelFields::MaximumMotorEfficiencyCurveName).get());
    EXPECT_EQ(motorPartLoad.nameString(), idf_fan.getString(Fan_ComponentModelFields::NormalizedMotorEfficiencyCurveName).get());
    EXPECT_EQ(vFDPartLoad.nameString(), idf_fan.getString(Fan_ComponentModelFields::VFDEfficiencyCurveName).get());

    EXPECT_EQ("Fans", idf_fan.getString(Fan_ComponentModelFields::EndUseSubcategory).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_FanComponentModel_AirLoopHVAC) {

  ForwardTranslator ft;

  Model m;
  FanComponentModel fan(m);

  AirLoopHVAC a(m);
  Node supplyOutletNode = a.supplyOutletNode();
  EXPECT_TRUE(fan.addToNode(supplyOutletNode));

  Workspace w = ft.translateModel(m);
  WorkspaceObjectVector idf_fans(w.getObjectsByType(IddObjectType::Fan_ComponentModel));
  ASSERT_EQ(1u, idf_fans.size());
  WorkspaceObject idf_fan(idf_fans[0]);

  EXPECT_EQ(idf_fan.getString(Fan_ComponentModelFields::AirInletNodeName).get(), fan.inletModelObject().get().nameString());

  EXPECT_EQ(idf_fan.getString(Fan_ComponentModelFields::AirOutletNodeName).get(), fan.outletModelObject().get().nameString());

  // Go from AirLoopHVAC to BranchList to Branch
  WorkspaceObjectVector idf_airloops(w.getObjectsByType(IddObjectType::AirLoopHVAC));

  auto _wo = w.getObjectByTypeAndName(IddObjectType::AirLoopHVAC, a.name().get());
  ASSERT_TRUE(_wo.is_initialized());
  WorkspaceObject idf_airLoopHVAC = _wo.get();
  WorkspaceObject idf_brlist = idf_airLoopHVAC.getTarget(AirLoopHVACFields::BranchListName).get();

  // Should have one branch only
  ASSERT_EQ(1u, idf_brlist.extensibleGroups().size());
  auto w_eg = idf_brlist.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  WorkspaceObject idf_branch = w_eg.getTarget(BranchListExtensibleFields::BranchName).get();

  // There should be only one equipment on the branch
  ASSERT_EQ(1u, idf_branch.extensibleGroups().size());
  auto w_eg2 = idf_branch.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();

  EXPECT_EQ("Fan:ComponentModel", w_eg2.getString(BranchExtensibleFields::ComponentObjectType).get());
  EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentName).get(), fan.nameString());

  EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentInletNodeName).get(), fan.inletModelObject().get().nameString());

  EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentOutletNodeName).get(), fan.outletModelObject().get().nameString());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_FanComponentModel) {

  ReverseTranslator reverseTranslator;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);
  OptionalWorkspaceObject _i_fan = w.addObject(IdfObject(IddObjectType::Fan_ComponentModel));
  ASSERT_TRUE(_i_fan);
  _i_fan->setName("My FanComponentModel");

  OptionalWorkspaceObject _i_sch = w.addObject(IdfObject(IddObjectType::Schedule_Constant));
  ASSERT_TRUE(_i_sch);
  _i_sch->setName("FanAvailSched");
  EXPECT_TRUE(_i_sch->setDouble(Schedule_ConstantFields::HourlyValue, 1.0));
  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::AvailabilityScheduleName, _i_sch->handle()));

  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::MaximumFlowRate, 1.5));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::MinimumFlowRate, 0.05));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::FanSizingFactor, 1.01));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::FanWheelDiameter, 0.334));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::FanOutletArea, 0.010));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::MaximumFanStaticEfficiency, 0.816));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::EulerNumberatMaximumFanStaticEfficiency, 10.76));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::MaximumDimensionlessFanAirflow, 0.19));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::MotorFanPulleyRatio, 0.96));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::BeltMaximumTorque, 10.0));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::BeltSizingFactor, 1.98));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::BeltFractionalTorqueTransition, 0.14));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::MotorMaximumSpeed, 1802.0));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::MaximumMotorOutputPower, 10000.0));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::MotorSizingFactor, 1.99));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::MotorInAirstreamFraction, 0.5));

  EXPECT_TRUE(_i_fan->setString(Fan_ComponentModelFields::VFDEfficiencyType, "Speed"));

  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::MaximumVFDOutputPower, 11000.0));
  EXPECT_TRUE(_i_fan->setDouble(Fan_ComponentModelFields::VFDSizingFactor, 1.95));

  OptionalWorkspaceObject _i_vSDExample = w.addObject(IdfObject(IddObjectType::Curve_FanPressureRise));
  ASSERT_TRUE(_i_vSDExample);
  EXPECT_TRUE(_i_vSDExample->setString(Curve_FanPressureRiseFields::Name, "IDF VSD Example"));
  EXPECT_TRUE(_i_vSDExample->setDouble(Curve_FanPressureRiseFields::Coefficient1C1, 1446.75833497653));
  EXPECT_TRUE(_i_vSDExample->setDouble(Curve_FanPressureRiseFields::Coefficient2C2, 0.0));
  EXPECT_TRUE(_i_vSDExample->setDouble(Curve_FanPressureRiseFields::Coefficient3C3, 0.0));
  EXPECT_TRUE(_i_vSDExample->setDouble(Curve_FanPressureRiseFields::Coefficient4C4, 1.0));
  EXPECT_TRUE(_i_vSDExample->setDouble(Curve_FanPressureRiseFields::MinimumValueofQfan, 0.0));
  EXPECT_TRUE(_i_vSDExample->setDouble(Curve_FanPressureRiseFields::MaximumValueofQfan, 100.0));
  EXPECT_TRUE(_i_vSDExample->setDouble(Curve_FanPressureRiseFields::MinimumValueofPsm, 62.5));
  EXPECT_TRUE(_i_vSDExample->setDouble(Curve_FanPressureRiseFields::MaximumValueofPsm, 300.0));
  EXPECT_TRUE(_i_vSDExample->setDouble(Curve_FanPressureRiseFields::MinimumCurveOutput, 0.0));
  EXPECT_TRUE(_i_vSDExample->setDouble(Curve_FanPressureRiseFields::MaximumCurveOutput, 5000.0));

  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::FanPressureRiseCurveName, _i_vSDExample->handle()));

  OptionalWorkspaceObject _i_diagnosticSPR = w.addObject(IdfObject(IddObjectType::Curve_Linear));
  ASSERT_TRUE(_i_diagnosticSPR);
  EXPECT_TRUE(_i_diagnosticSPR->setString(Curve_LinearFields::Name, "IDF DiagnosticSPR"));
  EXPECT_TRUE(_i_diagnosticSPR->setDouble(Curve_LinearFields::Coefficient1Constant, 248.84));
  EXPECT_TRUE(_i_diagnosticSPR->setDouble(Curve_LinearFields::Coefficient2x, 0.0));
  EXPECT_TRUE(_i_diagnosticSPR->setDouble(Curve_LinearFields::MinimumValueofx, 0.0));
  EXPECT_TRUE(_i_diagnosticSPR->setDouble(Curve_LinearFields::MaximumValueofx, 100.0));
  EXPECT_TRUE(_i_diagnosticSPR->setDouble(Curve_LinearFields::MinimumCurveOutput, 62.5));
  EXPECT_TRUE(_i_diagnosticSPR->setDouble(Curve_LinearFields::MaximumCurveOutput, 248.84));
  // EXPECT_TRUE(_i_diagnosticSPR->setDouble(Curve_LinearFields::InputUnitTypeforX, ""));
  // EXPECT_TRUE(_i_diagnosticSPR->setDouble(Curve_LinearFields::OutputUnitType, ""));

  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::DuctStaticPressureResetCurveName, _i_diagnosticSPR->handle()));

  OptionalWorkspaceObject _i_fanEff120CPLANormal = w.addObject(IdfObject(IddObjectType::Curve_ExponentialSkewNormal));
  ASSERT_TRUE(_i_fanEff120CPLANormal);
  EXPECT_TRUE(_i_fanEff120CPLANormal->setString(Curve_ExponentialSkewNormalFields::Name, "IDF FanEff120CPLANormal"));
  EXPECT_TRUE(_i_fanEff120CPLANormal->setDouble(Curve_ExponentialSkewNormalFields::Coefficient1C1, 0.072613));
  EXPECT_TRUE(_i_fanEff120CPLANormal->setDouble(Curve_ExponentialSkewNormalFields::Coefficient2C2, 0.833213));
  EXPECT_TRUE(_i_fanEff120CPLANormal->setDouble(Curve_ExponentialSkewNormalFields::Coefficient3C3, 0.0));
  EXPECT_TRUE(_i_fanEff120CPLANormal->setDouble(Curve_ExponentialSkewNormalFields::Coefficient4C4, 0.013911));
  EXPECT_TRUE(_i_fanEff120CPLANormal->setDouble(Curve_ExponentialSkewNormalFields::MinimumValueofx, -4.0));
  EXPECT_TRUE(_i_fanEff120CPLANormal->setDouble(Curve_ExponentialSkewNormalFields::MaximumValueofx, 5.0));
  EXPECT_TRUE(_i_fanEff120CPLANormal->setDouble(Curve_ExponentialSkewNormalFields::MinimumCurveOutput, 0.1));
  EXPECT_TRUE(_i_fanEff120CPLANormal->setDouble(Curve_ExponentialSkewNormalFields::MaximumCurveOutput, 1.0));
  // EXPECT_TRUE(_i_fanEff120CPLANormal->setDouble(Curve_ExponentialSkewNormalFields::InputUnitTypeforx, ""));
  // EXPECT_TRUE(_i_fanEff120CPLANormal->setDouble(Curve_ExponentialSkewNormalFields::OutputUnitType, ""));

  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameNonStallRegion, _i_fanEff120CPLANormal->handle()));

  OptionalWorkspaceObject _i_fanEff120CPLAStall = w.addObject(IdfObject(IddObjectType::Curve_ExponentialSkewNormal));
  ASSERT_TRUE(_i_fanEff120CPLAStall);
  EXPECT_TRUE(_i_fanEff120CPLAStall->setString(Curve_ExponentialSkewNormalFields::Name, "IDF FanEff120CPLAStall"));
  EXPECT_TRUE(_i_fanEff120CPLAStall->setDouble(Curve_ExponentialSkewNormalFields::Coefficient1C1, -1.674931));
  EXPECT_TRUE(_i_fanEff120CPLAStall->setDouble(Curve_ExponentialSkewNormalFields::Coefficient2C2, 1.980182));
  EXPECT_TRUE(_i_fanEff120CPLAStall->setDouble(Curve_ExponentialSkewNormalFields::Coefficient3C3, 0.0));
  EXPECT_TRUE(_i_fanEff120CPLAStall->setDouble(Curve_ExponentialSkewNormalFields::Coefficient4C4, 1.84495));
  EXPECT_TRUE(_i_fanEff120CPLAStall->setDouble(Curve_ExponentialSkewNormalFields::MinimumValueofx, -4.0));
  EXPECT_TRUE(_i_fanEff120CPLAStall->setDouble(Curve_ExponentialSkewNormalFields::MaximumValueofx, 5.0));
  EXPECT_TRUE(_i_fanEff120CPLAStall->setDouble(Curve_ExponentialSkewNormalFields::MinimumCurveOutput, 0.1));
  EXPECT_TRUE(_i_fanEff120CPLAStall->setDouble(Curve_ExponentialSkewNormalFields::MaximumCurveOutput, 1.0));
  // EXPECT_TRUE(_i_fanEff120CPLAStall->setDouble(Curve_ExponentialSkewNormalFields::InputUnitTypeforx, ""));
  // EXPECT_TRUE(_i_fanEff120CPLAStall->setDouble(Curve_ExponentialSkewNormalFields::OutputUnitType, ""));

  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameStallRegion, _i_fanEff120CPLAStall->handle()));

  OptionalWorkspaceObject _i_fanDimFlowNormal = w.addObject(IdfObject(IddObjectType::Curve_Sigmoid));
  ASSERT_TRUE(_i_fanDimFlowNormal);
  EXPECT_TRUE(_i_fanDimFlowNormal->setString(Curve_SigmoidFields::Name, "IDF FanDimFlowNormal"));
  EXPECT_TRUE(_i_fanDimFlowNormal->setDouble(Curve_SigmoidFields::Coefficient1C1, 0.0));
  EXPECT_TRUE(_i_fanDimFlowNormal->setDouble(Curve_SigmoidFields::Coefficient2C2, 1.001423));
  EXPECT_TRUE(_i_fanDimFlowNormal->setDouble(Curve_SigmoidFields::Coefficient3C3, 0.123935));
  EXPECT_TRUE(_i_fanDimFlowNormal->setDouble(Curve_SigmoidFields::Coefficient4C4, -0.476026));
  EXPECT_TRUE(_i_fanDimFlowNormal->setDouble(Curve_SigmoidFields::Coefficient5C5, 1.0));
  EXPECT_TRUE(_i_fanDimFlowNormal->setDouble(Curve_SigmoidFields::MinimumValueofx, -4.0));
  EXPECT_TRUE(_i_fanDimFlowNormal->setDouble(Curve_SigmoidFields::MaximumValueofx, 5.0));
  EXPECT_TRUE(_i_fanDimFlowNormal->setDouble(Curve_SigmoidFields::MinimumCurveOutput, 0.05));
  EXPECT_TRUE(_i_fanDimFlowNormal->setDouble(Curve_SigmoidFields::MaximumCurveOutput, 1.0));
  // EXPECT_TRUE(_i_fanDimFlowNormal->setDouble(Curve_SigmoidFields::InputUnitTypeforx, ""));
  // EXPECT_TRUE(_i_fanDimFlowNormal->setDouble(Curve_SigmoidFields::OutputUnitType, ""));

  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameStallRegion, _i_fanDimFlowNormal->handle()));

  OptionalWorkspaceObject _i_fanDimFlowStall = w.addObject(IdfObject(IddObjectType::Curve_Sigmoid));
  ASSERT_TRUE(_i_fanDimFlowStall);
  EXPECT_TRUE(_i_fanDimFlowStall->setString(Curve_SigmoidFields::Name, "IDF FanDimFlowStall"));
  EXPECT_TRUE(_i_fanDimFlowStall->setDouble(Curve_SigmoidFields::Coefficient1C1, 0.0));
  EXPECT_TRUE(_i_fanDimFlowStall->setDouble(Curve_SigmoidFields::Coefficient2C2, 5.924993));
  EXPECT_TRUE(_i_fanDimFlowStall->setDouble(Curve_SigmoidFields::Coefficient3C3, -1.91636));
  EXPECT_TRUE(_i_fanDimFlowStall->setDouble(Curve_SigmoidFields::Coefficient4C4, -0.851779));
  EXPECT_TRUE(_i_fanDimFlowStall->setDouble(Curve_SigmoidFields::Coefficient5C5, 1.0));
  EXPECT_TRUE(_i_fanDimFlowStall->setDouble(Curve_SigmoidFields::MinimumValueofx, -4.0));
  EXPECT_TRUE(_i_fanDimFlowStall->setDouble(Curve_SigmoidFields::MaximumValueofx, 5.0));
  EXPECT_TRUE(_i_fanDimFlowStall->setDouble(Curve_SigmoidFields::MinimumCurveOutput, 0.05));
  EXPECT_TRUE(_i_fanDimFlowStall->setDouble(Curve_SigmoidFields::MaximumCurveOutput, 1.0));
  // EXPECT_TRUE(_i_fanDimFlowStall->setDouble(Curve_SigmoidFields::InputUnitTypeforx, ""));
  // EXPECT_TRUE(_i_fanDimFlowStall->setDouble(Curve_SigmoidFields::OutputUnitType, ""));

  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameNonStallRegion, _i_fanDimFlowStall->handle()));

  OptionalWorkspaceObject _i_beltMaxEffMedium = w.addObject(IdfObject(IddObjectType::Curve_Quartic));
  ASSERT_TRUE(_i_beltMaxEffMedium);
  EXPECT_TRUE(_i_beltMaxEffMedium->setString(Curve_QuarticFields::Name, "IDF BeltMaxEffMedium"));
  EXPECT_TRUE(_i_beltMaxEffMedium->setDouble(Curve_QuarticFields::Coefficient1Constant, -0.09504));
  EXPECT_TRUE(_i_beltMaxEffMedium->setDouble(Curve_QuarticFields::Coefficient2x, 0.03415));
  EXPECT_TRUE(_i_beltMaxEffMedium->setDouble(Curve_QuarticFields::Coefficient3x_POW_2, -0.008897));
  EXPECT_TRUE(_i_beltMaxEffMedium->setDouble(Curve_QuarticFields::Coefficient4x_POW_3, 0.001159));
  EXPECT_TRUE(_i_beltMaxEffMedium->setDouble(Curve_QuarticFields::Coefficient5x_POW_4, -6.132e-05));
  EXPECT_TRUE(_i_beltMaxEffMedium->setDouble(Curve_QuarticFields::MinimumValueofx, -1.2));
  EXPECT_TRUE(_i_beltMaxEffMedium->setDouble(Curve_QuarticFields::MaximumValueofx, 6.2));
  EXPECT_TRUE(_i_beltMaxEffMedium->setDouble(Curve_QuarticFields::MinimumCurveOutput, -4.6));
  EXPECT_TRUE(_i_beltMaxEffMedium->setDouble(Curve_QuarticFields::MaximumCurveOutput, 0.0));
  // EXPECT_TRUE(_i_beltMaxEffMedium->setDouble(Curve_QuarticFields::InputUnitTypeforX, ""));
  // EXPECT_TRUE(_i_beltMaxEffMedium->setDouble(Curve_QuarticFields::OutputUnitType, ""));

  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::MaximumBeltEfficiencyCurveName, _i_beltMaxEffMedium->handle()));

  OptionalWorkspaceObject _i_beltPartLoadRegion1 = w.addObject(IdfObject(IddObjectType::Curve_RectangularHyperbola2));
  ASSERT_TRUE(_i_beltPartLoadRegion1);
  EXPECT_TRUE(_i_beltPartLoadRegion1->setString(Curve_RectangularHyperbola2Fields::Name, "IDF BeltPartLoadRegion1"));
  EXPECT_TRUE(_i_beltPartLoadRegion1->setDouble(Curve_RectangularHyperbola2Fields::Coefficient1C1, 0.920797));
  EXPECT_TRUE(_i_beltPartLoadRegion1->setDouble(Curve_RectangularHyperbola2Fields::Coefficient2C2, 0.0262686));
  EXPECT_TRUE(_i_beltPartLoadRegion1->setDouble(Curve_RectangularHyperbola2Fields::Coefficient3C3, 0.151594));
  EXPECT_TRUE(_i_beltPartLoadRegion1->setDouble(Curve_RectangularHyperbola2Fields::MinimumValueofx, 0.0));
  EXPECT_TRUE(_i_beltPartLoadRegion1->setDouble(Curve_RectangularHyperbola2Fields::MaximumValueofx, 1.0));
  EXPECT_TRUE(_i_beltPartLoadRegion1->setDouble(Curve_RectangularHyperbola2Fields::MinimumCurveOutput, 0.01));
  EXPECT_TRUE(_i_beltPartLoadRegion1->setDouble(Curve_RectangularHyperbola2Fields::MaximumCurveOutput, 1.0));
  // EXPECT_TRUE(_i_beltPartLoadRegion1->setDouble(Curve_RectangularHyperbola2Fields::InputUnitTypeforx, ""));
  // EXPECT_TRUE(_i_beltPartLoadRegion1->setDouble(Curve_RectangularHyperbola2Fields::OutputUnitType, ""));

  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion1, _i_beltPartLoadRegion1->handle()));

  OptionalWorkspaceObject _i_beltPartLoadRegion2 = w.addObject(IdfObject(IddObjectType::Curve_ExponentialDecay));
  ASSERT_TRUE(_i_beltPartLoadRegion2);
  EXPECT_TRUE(_i_beltPartLoadRegion2->setString(Curve_ExponentialDecayFields::Name, "IDF BeltPartLoadRegion2"));
  EXPECT_TRUE(_i_beltPartLoadRegion2->setDouble(Curve_ExponentialDecayFields::Coefficient1C1, 1.011965));
  EXPECT_TRUE(_i_beltPartLoadRegion2->setDouble(Curve_ExponentialDecayFields::Coefficient2C2, -0.339038));
  EXPECT_TRUE(_i_beltPartLoadRegion2->setDouble(Curve_ExponentialDecayFields::Coefficient3C3, -3.43626));
  EXPECT_TRUE(_i_beltPartLoadRegion2->setDouble(Curve_ExponentialDecayFields::MinimumValueofx, 0.0));
  EXPECT_TRUE(_i_beltPartLoadRegion2->setDouble(Curve_ExponentialDecayFields::MaximumValueofx, 1.0));
  EXPECT_TRUE(_i_beltPartLoadRegion2->setDouble(Curve_ExponentialDecayFields::MinimumCurveOutput, 0.01));
  EXPECT_TRUE(_i_beltPartLoadRegion2->setDouble(Curve_ExponentialDecayFields::MaximumCurveOutput, 1.0));
  // EXPECT_TRUE(_i_beltPartLoadRegion2->setDouble(Curve_ExponentialDecayFields::InputUnitTypeforx, ""));
  // EXPECT_TRUE(_i_beltPartLoadRegion2->setDouble(Curve_ExponentialDecayFields::OutputUnitType, ""));

  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion2, _i_beltPartLoadRegion2->handle()));

  OptionalWorkspaceObject _i_beltPartLoadRegion3 = w.addObject(IdfObject(IddObjectType::Curve_RectangularHyperbola2));
  ASSERT_TRUE(_i_beltPartLoadRegion3);
  EXPECT_TRUE(_i_beltPartLoadRegion3->setString(Curve_RectangularHyperbola2Fields::Name, "IDF BeltPartLoadRegion3"));
  EXPECT_TRUE(_i_beltPartLoadRegion3->setDouble(Curve_RectangularHyperbola2Fields::Coefficient1C1, 1.037778));
  EXPECT_TRUE(_i_beltPartLoadRegion3->setDouble(Curve_RectangularHyperbola2Fields::Coefficient2C2, 0.0103068));
  EXPECT_TRUE(_i_beltPartLoadRegion3->setDouble(Curve_RectangularHyperbola2Fields::Coefficient3C3, -0.0268146));
  EXPECT_TRUE(_i_beltPartLoadRegion3->setDouble(Curve_RectangularHyperbola2Fields::MinimumValueofx, 0.0));
  EXPECT_TRUE(_i_beltPartLoadRegion3->setDouble(Curve_RectangularHyperbola2Fields::MaximumValueofx, 1.0));
  EXPECT_TRUE(_i_beltPartLoadRegion3->setDouble(Curve_RectangularHyperbola2Fields::MinimumCurveOutput, 0.01));
  EXPECT_TRUE(_i_beltPartLoadRegion3->setDouble(Curve_RectangularHyperbola2Fields::MaximumCurveOutput, 1.0));
  // EXPECT_TRUE(_i_beltPartLoadRegion3->setDouble(Curve_RectangularHyperbola2Fields::InputUnitTypeforx, ""));
  // EXPECT_TRUE(_i_beltPartLoadRegion3->setDouble(Curve_RectangularHyperbola2Fields::OutputUnitType, ""));

  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion3, _i_beltPartLoadRegion3->handle()));

  OptionalWorkspaceObject _i_motorMaxEffAvg = w.addObject(IdfObject(IddObjectType::Curve_RectangularHyperbola1));
  ASSERT_TRUE(_i_motorMaxEffAvg);
  EXPECT_TRUE(_i_motorMaxEffAvg->setString(Curve_RectangularHyperbola1Fields::Name, "IDF MotorMaxEffAvg"));
  EXPECT_TRUE(_i_motorMaxEffAvg->setDouble(Curve_RectangularHyperbola1Fields::Coefficient1C1, 0.29228));
  EXPECT_TRUE(_i_motorMaxEffAvg->setDouble(Curve_RectangularHyperbola1Fields::Coefficient2C2, 3.368739));
  EXPECT_TRUE(_i_motorMaxEffAvg->setDouble(Curve_RectangularHyperbola1Fields::Coefficient3C3, 0.762471));
  EXPECT_TRUE(_i_motorMaxEffAvg->setDouble(Curve_RectangularHyperbola1Fields::MinimumValueofx, 0.0));
  EXPECT_TRUE(_i_motorMaxEffAvg->setDouble(Curve_RectangularHyperbola1Fields::MaximumValueofx, 7.6));
  EXPECT_TRUE(_i_motorMaxEffAvg->setDouble(Curve_RectangularHyperbola1Fields::MinimumCurveOutput, 0.01));
  EXPECT_TRUE(_i_motorMaxEffAvg->setDouble(Curve_RectangularHyperbola1Fields::MaximumCurveOutput, 1.0));
  // EXPECT_TRUE(_i_motorMaxEffAvg->setDouble(Curve_RectangularHyperbola1Fields::InputUnitTypeforx, ""));
  // EXPECT_TRUE(_i_motorMaxEffAvg->setDouble(Curve_RectangularHyperbola1Fields::OutputUnitType, ""));

  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::MaximumMotorEfficiencyCurveName, _i_motorMaxEffAvg->handle()));

  OptionalWorkspaceObject _i_motorPartLoad = w.addObject(IdfObject(IddObjectType::Curve_RectangularHyperbola2));
  ASSERT_TRUE(_i_motorPartLoad);
  EXPECT_TRUE(_i_motorPartLoad->setString(Curve_RectangularHyperbola2Fields::Name, "IDF MotorPartLoad"));
  EXPECT_TRUE(_i_motorPartLoad->setDouble(Curve_RectangularHyperbola2Fields::Coefficient1C1, 1.137209));
  EXPECT_TRUE(_i_motorPartLoad->setDouble(Curve_RectangularHyperbola2Fields::Coefficient2C2, 0.0502359));
  EXPECT_TRUE(_i_motorPartLoad->setDouble(Curve_RectangularHyperbola2Fields::Coefficient3C3, -0.0891503));
  EXPECT_TRUE(_i_motorPartLoad->setDouble(Curve_RectangularHyperbola2Fields::MinimumValueofx, 0.0));
  EXPECT_TRUE(_i_motorPartLoad->setDouble(Curve_RectangularHyperbola2Fields::MaximumValueofx, 1.0));
  EXPECT_TRUE(_i_motorPartLoad->setDouble(Curve_RectangularHyperbola2Fields::MinimumCurveOutput, 0.01));
  EXPECT_TRUE(_i_motorPartLoad->setDouble(Curve_RectangularHyperbola2Fields::MaximumCurveOutput, 1.0));
  // EXPECT_TRUE(_i_motorPartLoad->setDouble(Curve_RectangularHyperbola2Fields::InputUnitTypeforx, ""));
  // EXPECT_TRUE(_i_motorPartLoad->setDouble(Curve_RectangularHyperbola2Fields::OutputUnitType, ""));

  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::NormalizedMotorEfficiencyCurveName, _i_motorPartLoad->handle()));

  OptionalWorkspaceObject _i_vFDPartLoad = w.addObject(IdfObject(IddObjectType::Curve_RectangularHyperbola2));
  ASSERT_TRUE(_i_vFDPartLoad);
  EXPECT_TRUE(_i_vFDPartLoad->setString(Curve_RectangularHyperbola2Fields::Name, "IDF VFDPartLoad"));
  EXPECT_TRUE(_i_vFDPartLoad->setDouble(Curve_RectangularHyperbola2Fields::Coefficient1C1, 0.987405));
  EXPECT_TRUE(_i_vFDPartLoad->setDouble(Curve_RectangularHyperbola2Fields::Coefficient2C2, 0.0155361));
  EXPECT_TRUE(_i_vFDPartLoad->setDouble(Curve_RectangularHyperbola2Fields::Coefficient3C3, -0.0059365));
  EXPECT_TRUE(_i_vFDPartLoad->setDouble(Curve_RectangularHyperbola2Fields::MinimumValueofx, 0.0));
  EXPECT_TRUE(_i_vFDPartLoad->setDouble(Curve_RectangularHyperbola2Fields::MaximumValueofx, 1.0));
  EXPECT_TRUE(_i_vFDPartLoad->setDouble(Curve_RectangularHyperbola2Fields::MinimumCurveOutput, 0.01));
  EXPECT_TRUE(_i_vFDPartLoad->setDouble(Curve_RectangularHyperbola2Fields::MaximumCurveOutput, 1.0));
  // EXPECT_TRUE(_i_vFDPartLoad->setDouble(Curve_RectangularHyperbola2Fields::InputUnitTypeforx, ""));
  // EXPECT_TRUE(_i_vFDPartLoad->setDouble(Curve_RectangularHyperbola2Fields::OutputUnitType, ""));

  EXPECT_TRUE(_i_fan->setPointer(Fan_ComponentModelFields::VFDEfficiencyCurveName, _i_vFDPartLoad->handle()));

  // Fan:ComponentModel,
  //   My FanComponentModel,                   !- Name
  //   ,                                       !- Air Inlet Node Name
  //   ,                                       !- Air Outlet Node Name
  //   FanAvailSched,                          !- Availability Schedule Name
  //   1.5,                                    !- Maximum Flow Rate {m3/s}
  //   0.05,                                   !- Minimum Flow Rate {m3/s}
  //   1.01,                                   !- Fan Sizing Factor
  //   0.334,                                  !- Fan Wheel Diameter {m}
  //   0.01,                                   !- Fan Outlet Area {m2}
  //   0.816,                                  !- Maximum Fan Static Efficiency
  //   10.76,                                  !- Euler Number at Maximum Fan Static Efficiency
  //   0.19,                                   !- Maximum Dimensionless Fan Airflow
  //   0.96,                                   !- Motor Fan Pulley Ratio
  //   10,                                     !- Belt Maximum Torque {N-m}
  //   1.98,                                   !- Belt Sizing Factor
  //   0.14,                                   !- Belt Fractional Torque Transition
  //   1802,                                   !- Motor Maximum Speed {rev/min}
  //   10000,                                  !- Maximum Motor Output Power {W}
  //   1.99,                                   !- Motor Sizing Factor
  //   0.5,                                    !- Motor In Airstream Fraction
  //   Speed,                                  !- VFD Efficiency Type
  //   11000,                                  !- Maximum VFD Output Power {W}
  //   1.95,                                   !- VFD Sizing Factor
  //   IDF VSD Example,                        !- Fan Pressure Rise Curve Name
  //   IDF DiagnosticSPR,                      !- Duct Static Pressure Reset Curve Name
  //   IDF FanEff120CPLANormal,                !- Normalized Fan Static Efficiency Curve Name-Non-Stall Region
  //   IDF FanEff120CPLAStall,                 !- Normalized Fan Static Efficiency Curve Name-Stall Region
  //   IDF FanDimFlowStall,                    !- Normalized Dimensionless Airflow Curve Name-Non-Stall Region
  //   IDF FanDimFlowNormal,                   !- Normalized Dimensionless Airflow Curve Name-Stall Region
  //   IDF BeltMaxEffMedium,                   !- Maximum Belt Efficiency Curve Name
  //   IDF BeltPartLoadRegion1,                !- Normalized Belt Efficiency Curve Name - Region 1
  //   IDF BeltPartLoadRegion2,                !- Normalized Belt Efficiency Curve Name - Region 2
  //   IDF BeltPartLoadRegion3,                !- Normalized Belt Efficiency Curve Name - Region 3
  //   IDF MotorMaxEffAvg,                     !- Maximum Motor Efficiency Curve Name
  //   IDF MotorPartLoad,                      !- Normalized Motor Efficiency Curve Name
  //   IDF VFDPartLoad;                        !- VFD Efficiency Curve Name

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

    std::vector<openstudio::model::FanComponentModel> fans = model.getConcreteModelObjects<openstudio::model::FanComponentModel>();
    ASSERT_EQ(static_cast<unsigned>(1), fans.size());
    FanComponentModel fan = fans[0];

    EXPECT_EQ("My FanComponentModel", fan.nameString());
    EXPECT_EQ("FanAvailSched", fan.availabilitySchedule().nameString());
    ASSERT_FALSE(fan.isMaximumFlowRateAutosized());
    EXPECT_EQ(1.5, fan.maximumFlowRate().get());
    ASSERT_FALSE(fan.isMinimumFlowRateAutosized());
    EXPECT_EQ(0.05, fan.minimumFlowRate().get());
    EXPECT_EQ(1.01, fan.fanSizingFactor());
    EXPECT_EQ(0.334, fan.fanWheelDiameter());
    EXPECT_EQ(0.010, fan.fanOutletArea());
    EXPECT_EQ(0.816, fan.maximumFanStaticEfficiency());
    EXPECT_EQ(10.76, fan.eulerNumberatMaximumFanStaticEfficiency());
    EXPECT_EQ(0.19, fan.maximumDimensionlessFanAirflow());

    ASSERT_FALSE(fan.isMotorFanPulleyRatioAutosized());
    EXPECT_EQ(0.96, fan.motorFanPulleyRatio().get());

    ASSERT_FALSE(fan.isBeltMaximumTorqueAutosized());
    EXPECT_EQ(10.0, fan.beltMaximumTorque().get());

    EXPECT_EQ(1.98, fan.beltSizingFactor());
    EXPECT_EQ(0.14, fan.beltFractionalTorqueTransition());
    EXPECT_EQ(1802.0, fan.motorMaximumSpeed());

    ASSERT_FALSE(fan.isMaximumMotorOutputPowerAutosized());
    EXPECT_EQ(10000.0, fan.maximumMotorOutputPower().get());

    EXPECT_EQ(1.99, fan.motorSizingFactor());
    EXPECT_EQ(0.5, fan.motorInAirstreamFraction());

    EXPECT_TRUE(openstudio::istringEqual(fan.vFDEfficiencyType(), "Speed"));

    ASSERT_FALSE(fan.isMaximumVFDOutputPowerAutosized());
    EXPECT_EQ(11000.0, fan.maximumVFDOutputPower().get());
    EXPECT_EQ(1.95, fan.vFDSizingFactor());

    // Required curves
    EXPECT_TRUE(openstudio::istringEqual(fan.fanPressureRiseCurve().nameString(), "IDF VSD Example"));
    EXPECT_TRUE(openstudio::istringEqual(fan.ductStaticPressureResetCurve().nameString(), "IDF DiagnosticSPR"));
    EXPECT_TRUE(openstudio::istringEqual(fan.normalizedFanStaticEfficiencyCurveNonStallRegion().nameString(), "IDF FanEff120CPLANormal"));
    EXPECT_TRUE(openstudio::istringEqual(fan.normalizedFanStaticEfficiencyCurveStallRegion().nameString(), "IDF FanEff120CPLAStall"));
    EXPECT_TRUE(openstudio::istringEqual(fan.normalizedDimensionlessAirflowCurveNonStallRegion().nameString(), "IDF FanDimFlowStall"));
    EXPECT_TRUE(openstudio::istringEqual(fan.normalizedDimensionlessAirflowCurveStallRegion().nameString(), "IDF FanDimFlowNormal"));

    // Optional curves
    ASSERT_TRUE(fan.maximumBeltEfficiencyCurve());
    EXPECT_TRUE(openstudio::istringEqual(fan.maximumBeltEfficiencyCurve()->nameString(), "IDF BeltMaxEffMedium"));
    ASSERT_TRUE(fan.normalizedBeltEfficiencyCurveRegion1());
    EXPECT_TRUE(openstudio::istringEqual(fan.normalizedBeltEfficiencyCurveRegion1()->nameString(), "IDF BeltPartLoadRegion1"));
    ASSERT_TRUE(fan.normalizedBeltEfficiencyCurveRegion2());
    EXPECT_TRUE(openstudio::istringEqual(fan.normalizedBeltEfficiencyCurveRegion2()->nameString(), "IDF BeltPartLoadRegion2"));
    ASSERT_TRUE(fan.normalizedBeltEfficiencyCurveRegion3());
    EXPECT_TRUE(openstudio::istringEqual(fan.normalizedBeltEfficiencyCurveRegion3()->nameString(), "IDF BeltPartLoadRegion3"));
    ASSERT_TRUE(fan.maximumMotorEfficiencyCurve());
    EXPECT_TRUE(openstudio::istringEqual(fan.maximumMotorEfficiencyCurve()->nameString(), "IDF MotorMaxEffAvg"));
    ASSERT_TRUE(fan.normalizedMotorEfficiencyCurve());
    EXPECT_TRUE(openstudio::istringEqual(fan.normalizedMotorEfficiencyCurve()->nameString(), "IDF MotorPartLoad"));
    ASSERT_TRUE(fan.vFDEfficiencyCurve());
    EXPECT_TRUE(openstudio::istringEqual(fan.vFDEfficiencyCurve()->nameString(), "IDF VFDPartLoad"));
  }
}
