/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirTerminalSingleDuctVAVHeatAndCoolReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/AirLoopHVAC.hpp"

#include <utilities/idd/AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirTerminalSingleDuctVAVHeatAndCoolReheat) {
  Model m;

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);

  Schedule sch = m.alwaysOnDiscreteSchedule();
  CoilHeatingGas coil(m, sch);
  AirTerminalSingleDuctVAVHeatAndCoolReheat atu(m, coil);
  atu.setName("ATU SingleDuct VAV HeatAndCool Reheat");
  atu.setAvailabilitySchedule(sch);
  atu.setMaximumAirFlowRate(0.1);
  atu.setZoneMinimumAirFlowFraction(0.2);
  atu.setMaximumHotWaterorSteamFlowRate(0.3);
  atu.setMinimumHotWaterorSteamFlowRate(0.4);
  atu.setConvergenceTolerance(0.5);
  atu.setMaximumReheatAirTemperature(0.6);
  atu.setMinimumAirFlowTurndownSchedule(sch);

  AirLoopHVAC a(m);
  a.addBranchForZone(z, atu);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_atus(w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat));
  ASSERT_EQ(1u, idf_atus.size());
  WorkspaceObject idf_atu(idf_atus[0]);

  EXPECT_EQ("ATU SingleDuct VAV HeatAndCool Reheat", idf_atu.nameString());

  boost::optional<WorkspaceObject> woAvailabilitySchedule(
    idf_atu.getTarget(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AvailabilityScheduleName));
  EXPECT_TRUE(woAvailabilitySchedule);
  EXPECT_EQ(woAvailabilitySchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woAvailabilitySchedule->nameString());

  EXPECT_EQ("ATU SingleDuct VAV HeatAndCool Reheat Damper Outlet Node",
            idf_atu.getString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::DamperAirOutletNodeName).get());
  EXPECT_EQ(atu.inletModelObject()->nameString(), idf_atu.getString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AirInletNodeName).get());

  EXPECT_EQ("Coil:Heating:Fuel", idf_atu.getString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ReheatCoilObjectType).get());

  boost::optional<WorkspaceObject> woReheatCoil(idf_atu.getTarget(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ReheatCoilName));
  EXPECT_TRUE(woReheatCoil);
  EXPECT_EQ(woReheatCoil->iddObject().type(), IddObjectType::Coil_Heating_Fuel);
  EXPECT_EQ("Coil Heating Gas 1", woReheatCoil->nameString());

  EXPECT_EQ(0.1, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumAirFlowRate).get());
  EXPECT_EQ(0.2, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ZoneMinimumAirFlowFraction).get());
  EXPECT_EQ(0.3, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumHotWaterorSteamFlowRate).get());
  EXPECT_EQ(0.4, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MinimumHotWaterorSteamFlowRate).get());
  EXPECT_EQ(0.5, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ConvergenceTolerance).get());
  EXPECT_EQ(0.6, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumReheatAirTemperature).get());

  boost::optional<WorkspaceObject> woTurndownSchedule(
    idf_atu.getTarget(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MinimumAirFlowTurndownScheduleName));
  EXPECT_TRUE(woTurndownSchedule);
  EXPECT_EQ(woTurndownSchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woTurndownSchedule->nameString());
}
