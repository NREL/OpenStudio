/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilCoolingDX.hpp"
#include "../../model/CoilCoolingDX_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode_Impl.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/Node.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Coil_Cooling_DX_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_CurveFit_Performance_FieldEnums.hxx>
#include <utilities/idd/CoilSystem_Cooling_DX_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/BranchList_FieldEnums.hxx>
#include <utilities/idd/Branch_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_UnitarySystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDX_Unitary) {
  Model m;
  CoilCoolingDXCurveFitOperatingMode operatingMode(m);
  CoilCoolingDXCurveFitPerformance performance(m, operatingMode);
  CoilCoolingDX dx(m, performance);
  dx.setName("CoilCoolingDX");

  // put it inside a Unitary, and put Unitary on an AirLoopHVAC so it gets translated
  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(dx);
  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  // Test custom setting of one condenser node name
  // EXPECT_TRUE(dx.setCondenserOutletNodeName("My Custom Condenser Outlet Node"));
  // EXPECT_TRUE(dx.condenserOutletNodeName());

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfUnitarys(w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem));
  ASSERT_EQ(1u, idfUnitarys.size());
  WorkspaceObject idfUnitary(idfUnitarys[0]);

  WorkspaceObjectVector idfDXs(w.getObjectsByType(IddObjectType::Coil_Cooling_DX));
  ASSERT_EQ(1u, idfDXs.size());
  WorkspaceObject idfDX(idfDXs[0]);

  // No CoilSystem:Cooling:DX wrapper needed, it's inside a unitary
  EXPECT_EQ(0, w.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX).size());

  // Check that the Unitary ends up with the CoilCoolingDX
  EXPECT_EQ("Coil:Cooling:DX", idfUnitary.getString(AirLoopHVAC_UnitarySystemFields::CoolingCoilObjectType).get());
  EXPECT_EQ(idfDX.nameString(), idfUnitary.getString(AirLoopHVAC_UnitarySystemFields::CoolingCoilName).get());

  // Check Node connections: Evap by the Unitary
  EXPECT_NE("", idfDX.getString(Coil_Cooling_DXFields::EvaporatorInletNodeName).get());
  EXPECT_NE("", idfDX.getString(Coil_Cooling_DXFields::EvaporatorOutletNodeName).get());

  // Since the Unitary only has a Cooling Coil (no fan, not HC / Suppl HC), the nodes should match
  EXPECT_EQ(idfUnitary.getString(AirLoopHVAC_UnitarySystemFields::AirInletNodeName).get(),
            idfDX.getString(Coil_Cooling_DXFields::EvaporatorInletNodeName).get());

  EXPECT_EQ(idfUnitary.getString(AirLoopHVAC_UnitarySystemFields::AirOutletNodeName).get(),
            idfDX.getString(Coil_Cooling_DXFields::EvaporatorOutletNodeName).get());

  // Condenser Inlet Node: defaulted, so there should also be an OutdoorAir:NodeList corresponding, so that node conditions are set from weather file
  // and E+ doesn't complain
  EXPECT_EQ(dx.nameString() + " Condenser Inlet Node", idfDX.getString(Coil_Cooling_DXFields::CondenserInletNodeName).get());
  WorkspaceObjectVector woOANodeLists(w.getObjectsByType(IddObjectType::OutdoorAir_NodeList));
  ASSERT_EQ(1u, woOANodeLists.size());
  WorkspaceObject woOANodeList(woOANodeLists[0]);
  EXPECT_EQ(dx.nameString() + " Condenser Inlet Node", woOANodeList.getString(0).get());

  // EXPECT_EQ("My Custom Condenser Outlet Node", idfDX.getString(Coil_Cooling_DXFields::CondenserOutletNodeName).get());
  EXPECT_EQ(dx.nameString() + " Condenser Outlet Node", idfDX.getString(Coil_Cooling_DXFields::CondenserOutletNodeName).get());

  boost::optional<WorkspaceObject> woCoilCoolingDXCurveFitPerformance(idfDX.getTarget(Coil_Cooling_DXFields::PerformanceObjectName));
  EXPECT_TRUE(woCoilCoolingDXCurveFitPerformance);
  if (woCoilCoolingDXCurveFitPerformance) {
    EXPECT_EQ(woCoilCoolingDXCurveFitPerformance->iddObject().type(), IddObjectType::Coil_Cooling_DX_CurveFit_Performance);
  }

  WorkspaceObjectVector idfPerformances(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_Performance));
  ASSERT_EQ(1u, idfPerformances.size());
  WorkspaceObject idfPerformance(idfPerformances[0]);

  EXPECT_EQ(woCoilCoolingDXCurveFitPerformance.get(), idfPerformance);

  boost::optional<WorkspaceObject> woBaseOperatingMode(idfPerformance.getTarget(Coil_Cooling_DX_CurveFit_PerformanceFields::BaseOperatingMode));
  EXPECT_TRUE(woBaseOperatingMode);
  if (woBaseOperatingMode) {
    EXPECT_EQ(woBaseOperatingMode->iddObject().type(), IddObjectType::Coil_Cooling_DX_CurveFit_OperatingMode);
  }
  boost::optional<WorkspaceObject> woAlternativeOperatingMode1(
    idfPerformance.getTarget(Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode1));
  EXPECT_FALSE(woAlternativeOperatingMode1);
  boost::optional<WorkspaceObject> woAlternativeOperatingMode2(
    idfPerformance.getTarget(Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode2));
  EXPECT_FALSE(woAlternativeOperatingMode2);

  WorkspaceObjectVector idfOperatingModes(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_OperatingMode));
  ASSERT_EQ(1u, idfOperatingModes.size());
  WorkspaceObject idfOperatingMode(idfOperatingModes[0]);

  EXPECT_EQ(woBaseOperatingMode.get(), idfOperatingMode);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDX_AirLoopHVAC) {

  ForwardTranslator ft;

  Model m;
  CoilCoolingDXCurveFitOperatingMode operatingMode(m);
  CoilCoolingDXCurveFitPerformance performance(m, operatingMode);
  CoilCoolingDX dx(m, performance);
  dx.setName("CoilCoolingDX");

  AirLoopHVAC a(m);
  Node supplyOutletNode = a.supplyOutletNode();
  EXPECT_TRUE(dx.addToNode(supplyOutletNode));

  Workspace w = ft.translateModel(m);
  WorkspaceObjectVector idf_dxs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX);
  ASSERT_EQ(1u, idf_dxs.size());
  WorkspaceObject idf_dx(idf_dxs[0]);

  EXPECT_EQ(idf_dx.getString(Coil_Cooling_DXFields::EvaporatorInletNodeName).get(), dx.inletModelObject().get().nameString());
  EXPECT_EQ(idf_dx.getString(Coil_Cooling_DXFields::EvaporatorOutletNodeName).get(), dx.outletModelObject().get().nameString());

  EXPECT_EQ(1, w.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX).size());

  // Go from AirLoopHVAC to BranchList to Branch
  WorkspaceObjectVector idf_airloops = w.getObjectsByType(IddObjectType::AirLoopHVAC);
  ASSERT_EQ(1u, idf_airloops.size());

  WorkspaceObject& idf_airLoopHVAC = idf_airloops.front();
  WorkspaceObject idf_brlist = idf_airLoopHVAC.getTarget(AirLoopHVACFields::BranchListName).get();

  // Should have one branch only
  ASSERT_EQ(1u, idf_brlist.extensibleGroups().size());
  auto w_eg = idf_brlist.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  WorkspaceObject idf_branch = w_eg.getTarget(BranchListExtensibleFields::BranchName).get();

  // There should be only one equipment on the branch
  ASSERT_EQ(1u, idf_branch.extensibleGroups().size());
  auto w_eg2 = idf_branch.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();

  EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentInletNodeName).get(), dx.inletModelObject().get().nameString());
  EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentOutletNodeName).get(), dx.outletModelObject().get().nameString());

  EXPECT_EQ("CoilSystem:Cooling:DX", w_eg2.getString(BranchExtensibleFields::ComponentObjectType).get());
  auto idf_coilSystem = w_eg2.getTarget(BranchExtensibleFields::ComponentName).get();
  // CoilSystem:Cooling:DX wrapper needed, it's not inside a unitary
  EXPECT_EQ(1, w.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX).size());

  EXPECT_EQ("Always On Discrete", idf_coilSystem.getString(CoilSystem_Cooling_DXFields::AvailabilityScheduleName).get());
  EXPECT_EQ(dx.inletModelObject().get().nameString(), idf_coilSystem.getString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemInletNodeName).get());
  EXPECT_EQ(dx.outletModelObject().get().nameString(),
            idf_coilSystem.getString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemOutletNodeName).get());
  EXPECT_EQ(dx.outletModelObject().get().nameString(),
            idf_coilSystem.getString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemSensorNodeName).get());
  EXPECT_EQ("Coil:Cooling:DX", idf_coilSystem.getString(CoilSystem_Cooling_DXFields::CoolingCoilObjectType).get());
  EXPECT_EQ(dx.nameString(), idf_coilSystem.getString(CoilSystem_Cooling_DXFields::CoolingCoilName).get());
  EXPECT_EQ(idf_dx, idf_coilSystem.getTarget(CoilSystem_Cooling_DXFields::CoolingCoilName).get());
}
