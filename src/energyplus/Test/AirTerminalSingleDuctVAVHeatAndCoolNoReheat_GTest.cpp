/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirTerminalSingleDuctVAVHeatAndCoolNoReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/AirLoopHVAC.hpp"

#include <utilities/idd/AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirTerminalSingleDuctVAVHeatAndCoolNoReheat) {
  Model m;

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);

  Schedule sch = m.alwaysOnDiscreteSchedule();
  AirTerminalSingleDuctVAVHeatAndCoolNoReheat atu(m);
  atu.setAvailabilitySchedule(sch);
  atu.setName("ATU SingleDuct VAV HeatAndCool No Reheat");
  atu.setMaximumAirFlowRate(0.1);
  atu.setZoneMinimumAirFlowFraction(0.2);
  atu.setMinimumAirFlowTurndownSchedule(sch);

  AirLoopHVAC a(m);
  a.addBranchForZone(z, atu);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_atus(w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat));
  ASSERT_EQ(1u, idf_atus.size());
  WorkspaceObject idf_atu(idf_atus[0]);

  EXPECT_EQ("ATU SingleDuct VAV HeatAndCool No Reheat", idf_atu.nameString());

  boost::optional<WorkspaceObject> woAvailabilitySchedule(
    idf_atu.getTarget(AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilityScheduleName));
  EXPECT_TRUE(woAvailabilitySchedule);
  EXPECT_EQ(woAvailabilitySchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woAvailabilitySchedule->nameString());

  EXPECT_EQ(atu.outletModelObject()->nameString(), idf_atu.getString(AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AirOutletNodeName).get());
  EXPECT_EQ(atu.inletModelObject()->nameString(), idf_atu.getString(AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AirInletNodeName).get());

  EXPECT_EQ(0.1, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate).get());
  EXPECT_EQ(0.2, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::ZoneMinimumAirFlowFraction).get());

  boost::optional<WorkspaceObject> woTurndownSchedule(
    idf_atu.getTarget(AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MinimumAirFlowTurndownScheduleName));
  EXPECT_TRUE(woTurndownSchedule);
  EXPECT_EQ(woTurndownSchedule->iddObject().type(), IddObjectType::Schedule_Constant);
}
