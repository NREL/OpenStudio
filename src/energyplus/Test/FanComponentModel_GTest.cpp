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

  ScheduleConstant sch(m);
  sch.setName("Fan Avail Schedule");
  fanComponentModel.setName("My FanComponentModel");
  EXPECT_TRUE(fanComponentModel.setMaximumFlowRate(1.5));
  EXPECT_TRUE(fanComponentModel.setMinimumFlowRate(0.05));
  EXPECT_TRUE(fanComponentModel.setFanSizingFactor(1.0));
  EXPECT_TRUE(fanComponentModel.setFanWheelDiameter(0.3048));
  EXPECT_TRUE(fanComponentModel.setFanOutletArea(0.0873288576));
  EXPECT_TRUE(fanComponentModel.setMaximumFanStaticEfficiency(0.514));
  EXPECT_TRUE(fanComponentModel.setEulerNumberatMaximumFanStaticEfficiency(9.76));
  EXPECT_TRUE(fanComponentModel.setMaximumDimensionlessFanAirflow(0.160331811647483));
  EXPECT_TRUE(fanComponentModel.setMotorFanPulleyRatio(0.96));
  EXPECT_TRUE(fanComponentModel.setBeltMaximumTorque(10.0));
  EXPECT_TRUE(fanComponentModel.setBeltSizingFactor(0.98));
  EXPECT_TRUE(fanComponentModel.setBeltFractionalTorqueTransition(0.167));
  EXPECT_TRUE(fanComponentModel.setMotorMaximumSpeed(1800.0));
  EXPECT_TRUE(fanComponentModel.setMaximumMotorOutputPower(10000.0));
  EXPECT_TRUE(fanComponentModel.setMotorSizingFactor(0.99));
  EXPECT_TRUE(fanComponentModel.setMotorInAirstreamFraction(0.5));
  EXPECT_TRUE(fanComponentModel.setVFDEfficiencyType("Power"));
  EXPECT_TRUE(fanComponentModel.setMaximumVFDOutputPower(11000.0));
  EXPECT_TRUE(fanComponentModel.setVFDSizingFactor(0.95));

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
    EXPECT_EQ(1.0, idf_fan.getDouble(Fan_ComponentModelFields::FanSizingFactor).get());
    EXPECT_EQ(0.3048, idf_fan.getDouble(Fan_ComponentModelFields::FanWheelDiameter).get());
    EXPECT_EQ(0.0873288576, idf_fan.getDouble(Fan_ComponentModelFields::FanOutletArea).get());
    EXPECT_EQ(0.514, idf_fan.getDouble(Fan_ComponentModelFields::MaximumFanStaticEfficiency).get());
    EXPECT_EQ(9.76, idf_fan.getDouble(Fan_ComponentModelFields::EulerNumberatMaximumFanStaticEfficiency).get());
    EXPECT_EQ(0.160331811647483, idf_fan.getDouble(Fan_ComponentModelFields::MaximumDimensionlessFanAirflow).get());
    EXPECT_EQ(0.96, idf_fan.getDouble(Fan_ComponentModelFields::MotorFanPulleyRatio).get());
    EXPECT_EQ(10.0, idf_fan.getDouble(Fan_ComponentModelFields::BeltMaximumTorque).get());
    EXPECT_EQ(0.98, idf_fan.getDouble(Fan_ComponentModelFields::BeltSizingFactor).get());
    EXPECT_EQ(0.167, idf_fan.getDouble(Fan_ComponentModelFields::BeltFractionalTorqueTransition).get());
    EXPECT_EQ(1800.0, idf_fan.getDouble(Fan_ComponentModelFields::MotorMaximumSpeed).get());
    EXPECT_EQ(10000.0, idf_fan.getDouble(Fan_ComponentModelFields::MaximumMotorOutputPower).get());
    EXPECT_EQ(0.99, idf_fan.getDouble(Fan_ComponentModelFields::MotorSizingFactor).get());
    EXPECT_EQ(0.5, idf_fan.getDouble(Fan_ComponentModelFields::MotorInAirstreamFraction).get());
    EXPECT_EQ(11000.0, idf_fan.getDouble(Fan_ComponentModelFields::MaximumVFDOutputPower).get());
    EXPECT_EQ(0.95, idf_fan.getDouble(Fan_ComponentModelFields::VFDSizingFactor).get());

    EXPECT_EQ("Power", idf_fan.getString(Fan_ComponentModelFields::VFDEfficiencyType).get());
    EXPECT_EQ("VSD Example", idf_fan.getString(Fan_ComponentModelFields::FanPressureRiseCurveName).get());
    EXPECT_EQ("DiagnosticSPR", idf_fan.getString(Fan_ComponentModelFields::DuctStaticPressureResetCurveName).get());
    EXPECT_EQ("FanEff120CPLANormal", idf_fan.getString(Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameNonStallRegion).get());
    EXPECT_EQ("FanEff120CPLAStall", idf_fan.getString(Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameStallRegion).get());
    EXPECT_EQ("FanDimFlowNormal", idf_fan.getString(Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameNonStallRegion).get());
    EXPECT_EQ("FanDimFlowStall", idf_fan.getString(Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameStallRegion).get());
    EXPECT_EQ("BeltMaxEffMedium", idf_fan.getString(Fan_ComponentModelFields::MaximumBeltEfficiencyCurveName).get());
    EXPECT_EQ("BeltPartLoadRegion1", idf_fan.getString(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion1).get());
    EXPECT_EQ("BeltPartLoadRegion2", idf_fan.getString(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion2).get());
    EXPECT_EQ("BeltPartLoadRegion3", idf_fan.getString(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion3).get());
    EXPECT_EQ("MotorMaxEffAvg", idf_fan.getString(Fan_ComponentModelFields::MaximumMotorEfficiencyCurveName).get());
    EXPECT_EQ("MotorPartLoad", idf_fan.getString(Fan_ComponentModelFields::NormalizedMotorEfficiencyCurveName).get());
    EXPECT_EQ("VFDPartLoad", idf_fan.getString(Fan_ComponentModelFields::VFDEfficiencyCurveName).get());

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

  EXPECT_EQ(idf_fan.getString(Fan_ComponentModelFields::AirInletNodeName).get(), fan.inletModelObject().get().name());

  EXPECT_EQ(idf_fan.getString(Fan_ComponentModelFields::AirOutletNodeName).get(), fan.outletModelObject().get().name());

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

  EXPECT_EQ("Fan:ComponentModel", w_eg2.getString(BranchExtensibleFields::ComponentObjectType).get());
  EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentName).get(), fan.name());

  EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentInletNodeName).get(), fan.inletModelObject().get().nameString());

  EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentOutletNodeName).get(), fan.outletModelObject().get().nameString());
}
