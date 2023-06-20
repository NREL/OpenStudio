/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirTerminalSingleDuctVAVNoReheat.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"

#include <utilities/idd/IddObject.hpp>
#include <utilities/idd/AirTerminal_SingleDuct_VAV_NoReheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirTerminalSingleDuctVAVNoReheat) {
  Model m;

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);
  DesignSpecificationOutdoorAir dsoa(m);
  EXPECT_TRUE(s.setDesignSpecificationOutdoorAir(dsoa));

  Schedule sch = m.alwaysOnDiscreteSchedule();
  AirTerminalSingleDuctVAVNoReheat atu(m, sch);
  atu.setName("ATU SingleDuct VAV No Reheat");
  atu.setAvailabilitySchedule(sch);
  atu.setMaximumAirFlowRate(0.1);
  atu.setZoneMinimumAirFlowInputMethod("FixedFlowRate");
  atu.setConstantMinimumAirFlowFraction(0.2);
  atu.setFixedMinimumAirFlowRate(0.3);
  atu.setMinimumAirFlowFractionSchedule(sch);
  atu.setMinimumAirFlowTurndownSchedule(sch);
  atu.setControlForOutdoorAir(true);

  AirLoopHVAC a(m);
  a.addBranchForZone(z, atu);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_atus(w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_VAV_NoReheat));
  ASSERT_EQ(1u, idf_atus.size());
  WorkspaceObject idf_atu(idf_atus[0]);

  EXPECT_EQ("ATU SingleDuct VAV No Reheat", idf_atu.nameString());

  boost::optional<WorkspaceObject> woAvailabilitySchedule(idf_atu.getTarget(AirTerminal_SingleDuct_VAV_NoReheatFields::AvailabilityScheduleName));
  EXPECT_TRUE(woAvailabilitySchedule);
  EXPECT_EQ(woAvailabilitySchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woAvailabilitySchedule->nameString());

  EXPECT_EQ(atu.outletModelObject()->nameString(), idf_atu.getString(AirTerminal_SingleDuct_VAV_NoReheatFields::AirOutletNodeName).get());
  EXPECT_EQ(atu.inletModelObject()->nameString(), idf_atu.getString(AirTerminal_SingleDuct_VAV_NoReheatFields::AirInletNodeName).get());

  EXPECT_EQ(0.1, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate).get());
  EXPECT_EQ("FixedFlowRate", idf_atu.getString(AirTerminal_SingleDuct_VAV_NoReheatFields::ZoneMinimumAirFlowInputMethod).get());
  EXPECT_EQ(0.2, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::ConstantMinimumAirFlowFraction).get());
  EXPECT_EQ(0.3, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::FixedMinimumAirFlowRate).get());

  boost::optional<WorkspaceObject> woFractionSchedule(
    idf_atu.getTarget(AirTerminal_SingleDuct_VAV_NoReheatFields::MinimumAirFlowTurndownScheduleName));
  EXPECT_TRUE(woFractionSchedule);
  EXPECT_EQ(woFractionSchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woFractionSchedule->nameString());

  boost::optional<WorkspaceObject> woTurndownSchedule(
    idf_atu.getTarget(AirTerminal_SingleDuct_VAV_NoReheatFields::MinimumAirFlowTurndownScheduleName));
  EXPECT_TRUE(woTurndownSchedule);
  EXPECT_EQ(woTurndownSchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woTurndownSchedule->nameString());

  boost::optional<WorkspaceObject> idf_dsoa(idf_atu.getTarget(AirTerminal_SingleDuct_VAV_NoReheatFields::DesignSpecificationOutdoorAirObjectName));
  EXPECT_TRUE(idf_dsoa);
  EXPECT_EQ(idf_dsoa->iddObject().type(), IddObjectType::DesignSpecification_OutdoorAir);
}
