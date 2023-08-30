/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirTerminalDualDuctVAV.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"

#include <utilities/idd/IddObject.hpp>
#include <utilities/idd/AirTerminal_DualDuct_VAV_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirTerminalDualDuctVAV) {
  Model m;

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);

  Schedule sch = m.alwaysOnDiscreteSchedule();
  AirTerminalDualDuctVAV atu(m);
  atu.setName("ATU DualDuct VAV");
  EXPECT_TRUE(atu.setAvailabilitySchedule(sch));
  EXPECT_TRUE(atu.setMinimumAirFlowTurndownSchedule(sch));
  EXPECT_TRUE(atu.setMaximumDamperAirFlowRate(0.15));
  DesignSpecificationOutdoorAir dsoa(m);
  EXPECT_TRUE(atu.setDesignSpecificationOutdoorAirObject(dsoa));
  EXPECT_TRUE(atu.setZoneMinimumAirFlowFraction(0.35));

  // Make a DUAL DUCT AirLoopHVAC
  AirLoopHVAC a(m, true);
  a.addBranchForZone(z, atu);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_atus(w.getObjectsByType(IddObjectType::AirTerminal_DualDuct_VAV));
  ASSERT_EQ(1u, idf_atus.size());
  WorkspaceObject idf_atu(idf_atus[0]);

  EXPECT_EQ("ATU DualDuct VAV", idf_atu.nameString());

  boost::optional<WorkspaceObject> woAvailabilitySchedule(idf_atu.getTarget(AirTerminal_DualDuct_VAVFields::AvailabilityScheduleName));
  EXPECT_TRUE(woAvailabilitySchedule);
  EXPECT_EQ(woAvailabilitySchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woAvailabilitySchedule->nameString());

  EXPECT_EQ(atu.outletModelObject()->nameString(), idf_atu.getString(AirTerminal_DualDuct_VAVFields::AirOutletNodeName).get());
  EXPECT_EQ(atu.hotAirInletNode()->nameString(), idf_atu.getString(AirTerminal_DualDuct_VAVFields::HotAirInletNodeName).get());
  EXPECT_EQ(atu.coldAirInletNode()->nameString(), idf_atu.getString(AirTerminal_DualDuct_VAVFields::ColdAirInletNodeName).get());

  EXPECT_EQ(0.15, idf_atu.getDouble(AirTerminal_DualDuct_VAVFields::MaximumDamperAirFlowRate).get());
  EXPECT_EQ(0.35, idf_atu.getDouble(AirTerminal_DualDuct_VAVFields::ZoneMinimumAirFlowFraction).get());

  boost::optional<WorkspaceObject> idf_dsoa(idf_atu.getTarget(AirTerminal_DualDuct_VAVFields::DesignSpecificationOutdoorAirObjectName));
  EXPECT_TRUE(idf_dsoa);
  EXPECT_EQ(idf_dsoa->iddObject().type(), IddObjectType::DesignSpecification_OutdoorAir);

  boost::optional<WorkspaceObject> woTurndownSchedule(idf_atu.getTarget(AirTerminal_DualDuct_VAVFields::MinimumAirFlowTurndownScheduleName));
  EXPECT_TRUE(woTurndownSchedule);
  EXPECT_EQ(woTurndownSchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woTurndownSchedule->nameString());
}
