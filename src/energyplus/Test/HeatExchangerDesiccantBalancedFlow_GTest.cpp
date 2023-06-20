/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/HeatExchangerDesiccantBalancedFlow.hpp"
#include "../../model/HeatExchangerDesiccantBalancedFlowPerformanceDataType1.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/CoilHeatingDXSingleSpeed.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "../../model/FanSystemModel.hpp"
#include "../../model/HVACTemplates.hpp"

#include "../../utilities/idd/IddObject.hpp"

#include <utilities/idd/HeatExchanger_Desiccant_BalancedFlow_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_HeatExchangerDesiccantBalancedFlow) {
  Model m;

  auto loop = addSystemType3(m).cast<AirLoopHVAC>();

  AirLoopHVACOutdoorAirSystem oaSystem = loop.airLoopHVACOutdoorAirSystem().get();

  Node oaNode = oaSystem.outboardOANode().get();

  HeatExchangerDesiccantBalancedFlow hx(m);
  EXPECT_TRUE(hx.addToNode(oaNode));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfHXs(w.getObjectsByType(IddObjectType::HeatExchanger_Desiccant_BalancedFlow));
  ASSERT_EQ(1u, idfHXs.size());
  WorkspaceObject idfHX(idfHXs[0]);

  boost::optional<WorkspaceObject> woAvailabilitySchedule(idfHX.getTarget(HeatExchanger_Desiccant_BalancedFlowFields::AvailabilityScheduleName));
  EXPECT_TRUE(woAvailabilitySchedule);
  if (woAvailabilitySchedule) {
    EXPECT_EQ(woAvailabilitySchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  }
  EXPECT_EQ(hx.primaryAirInletModelObject()->nameString(),
            idfHX.getString(HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirInletNodeName).get());
  EXPECT_EQ(hx.primaryAirOutletModelObject()->nameString(),
            idfHX.getString(HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirOutletNodeName).get());
  EXPECT_EQ(hx.secondaryAirInletModelObject()->nameString(),
            idfHX.getString(HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirInletNodeName).get());
  EXPECT_EQ(hx.secondaryAirOutletModelObject()->nameString(),
            idfHX.getString(HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirOutletNodeName).get());
  EXPECT_EQ("HeatExchanger:Desiccant:BalancedFlow:PerformanceDataType1",
            idfHX.getString(HeatExchanger_Desiccant_BalancedFlowFields::HeatExchangerPerformanceObjectType, false).get());
  boost::optional<WorkspaceObject> woPerformance(idfHX.getTarget(HeatExchanger_Desiccant_BalancedFlowFields::HeatExchangerPerformanceName));
  EXPECT_TRUE(woPerformance);
  if (woPerformance) {
    EXPECT_EQ(woPerformance->iddObject().type(), IddObjectType::HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1);
  }
  EXPECT_EQ("No", idfHX.getString(HeatExchanger_Desiccant_BalancedFlowFields::EconomizerLockout, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilSystemCoolingDXHeatExchangerAssisted_HeatExchangerDesiccantBalancedFlow) {

  // Test that the nodes are written correctly in this very complex scenario: the AirLoopHVAC has a Unitary,
  // which has a CoilSystemCoolingDXHeatExchangerAssisted as a Cooling Coil, and that has a HeatExchangerDesiccantBalancedFlow as a HX.

  Model m;
  AirLoopHVAC a(m);

  HeatExchangerDesiccantBalancedFlow hx(m);
  CoilSystemCoolingDXHeatExchangerAssisted coilSystem(m, hx);
  coilSystem.setName("CoilSystemDXHX");
  EXPECT_EQ(hx, coilSystem.heatExchanger());
  coilSystem.coolingCoil().setName("CoilSystemDXHX CoolingCoil");

  AirLoopHVACUnitarySystem unitary(m);
  EXPECT_TRUE(unitary.setCoolingCoil(coilSystem));
  CoilHeatingDXSingleSpeed dxHeating(m);
  EXPECT_TRUE(unitary.setHeatingCoil(dxHeating));
  CoilHeatingElectric supplHeating(m);
  EXPECT_TRUE(unitary.setSupplementalHeatingCoil(supplHeating));

  FanSystemModel fan(m);
  EXPECT_TRUE(unitary.setSupplyFan(fan));

  Node supplyInletNode = a.supplyInletNode();
  EXPECT_TRUE(unitary.addToNode(supplyInletNode));

  a.supplyInletNode().setName("AirLoop Supply Inlet Node");
  a.supplyOutletNode().setName("AirLoop Supply Outlet Node");

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  auto getFirst = [&w](auto iddObjectType) -> WorkspaceObject {
    WorkspaceObjectVector objs(w.getObjectsByType(iddObjectType));
    return objs[0];
  };

  auto idf_hx = getFirst(IddObjectType::HeatExchanger_Desiccant_BalancedFlow);
  auto idf_dxHeating = getFirst(IddObjectType::Coil_Heating_DX_SingleSpeed);
  auto idf_dxCoolingCoilSystem = getFirst(IddObjectType::Coil_Cooling_DX_SingleSpeed);
  auto idf_a = getFirst(IddObjectType::AirLoopHVAC);

  EXPECT_EQ(idf_a.getString(AirLoopHVACFields::SupplySideInletNodeName).get(),
            idf_hx.getString(HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirInletNodeName).get());
  EXPECT_EQ("AirLoop Supply Inlet Node", idf_hx.getString(HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirInletNodeName).get());

  EXPECT_EQ(idf_dxCoolingCoilSystem.getString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName).get(),
            idf_hx.getString(HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirOutletNodeName).get());
  EXPECT_EQ("CoilSystemDXHX HX Supply Air Outlet - Cooling Inlet Node",
            idf_hx.getString(HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirOutletNodeName).get());

  EXPECT_EQ(idf_dxCoolingCoilSystem.getString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName).get(),
            idf_hx.getString(HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirInletNodeName).get());
  EXPECT_EQ("CoilSystemDXHX HX Exhaust Air Inlet - Cooling Outlet Node",
            idf_hx.getString(HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirInletNodeName).get());

  EXPECT_EQ(idf_dxHeating.getString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName).get(),
            idf_hx.getString(HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirOutletNodeName).get());
  EXPECT_EQ("Air Loop HVAC Unitary System 1 Cooling Coil - Heating Coil Node",
            idf_hx.getString(HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirOutletNodeName).get());
}
