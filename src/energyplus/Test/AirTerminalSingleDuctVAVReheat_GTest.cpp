/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirTerminalSingleDuctVAVReheat.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"

#include <utilities/idd/IddObject.hpp>
#include <utilities/idd/AirTerminal_SingleDuct_VAV_Reheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirTerminalSingleDuctVAVReheat) {
  Model m;

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);
  DesignSpecificationOutdoorAir dsoa(m);
  EXPECT_TRUE(s.setDesignSpecificationOutdoorAir(dsoa));

  Schedule sch = m.alwaysOnDiscreteSchedule();
  CoilHeatingElectric coil = CoilHeatingElectric(m, sch);
  AirTerminalSingleDuctVAVReheat atu(m, sch, coil);
  atu.setName("ATU SingleDuct VAV Reheat");
  atu.setAvailabilitySchedule(sch);
  atu.setMaximumAirFlowRate(0.1);
  atu.setZoneMinimumAirFlowInputMethod("FixedFlowRate");
  atu.setConstantMinimumAirFlowFraction(0.2);
  atu.setFixedMinimumAirFlowRate(0.3);
  atu.setMinimumAirFlowFractionSchedule(sch);
  atu.setMaximumHotWaterOrSteamFlowRate(0.4);
  atu.setMinimumHotWaterOrStreamFlowRate(0.5);
  atu.setConvergenceTolerance(0.6);
  atu.setDamperHeatingAction("Reverse");
  atu.setMaximumFlowPerZoneFloorAreaDuringReheat(0.7);
  atu.setMaximumFlowFractionDuringReheat(0.8);
  atu.setMaximumReheatAirTemperature(0.9);
  atu.setControlForOutdoorAir(true);
  atu.setMinimumAirFlowTurndownSchedule(sch);

  AirLoopHVAC a(m);
  a.addBranchForZone(z, atu);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_atus(w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_VAV_Reheat));
  ASSERT_EQ(1u, idf_atus.size());
  WorkspaceObject idf_atu(idf_atus[0]);

  EXPECT_EQ("ATU SingleDuct VAV Reheat", idf_atu.nameString());

  boost::optional<WorkspaceObject> woAvailabilitySchedule(idf_atu.getTarget(AirTerminal_SingleDuct_VAV_ReheatFields::AvailabilityScheduleName));
  EXPECT_TRUE(woAvailabilitySchedule);
  EXPECT_EQ(woAvailabilitySchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woAvailabilitySchedule->nameString());

  EXPECT_EQ("ATU SingleDuct VAV Reheat Damper Outlet", idf_atu.getString(AirTerminal_SingleDuct_VAV_ReheatFields::DamperAirOutletNodeName).get());
  EXPECT_EQ(atu.inletModelObject()->nameString(), idf_atu.getString(AirTerminal_SingleDuct_VAV_ReheatFields::AirInletNodeName).get());

  EXPECT_EQ("Coil:Heating:Electric", idf_atu.getString(AirTerminal_SingleDuct_VAV_ReheatFields::ReheatCoilObjectType).get());

  boost::optional<WorkspaceObject> woReheatCoil(idf_atu.getTarget(AirTerminal_SingleDuct_VAV_ReheatFields::ReheatCoilName));
  EXPECT_TRUE(woReheatCoil);
  EXPECT_EQ(woReheatCoil->iddObject().type(), IddObjectType::Coil_Heating_Electric);
  EXPECT_EQ("Coil Heating Electric 1", woReheatCoil->nameString());

  EXPECT_EQ(0.1, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate).get());
  EXPECT_EQ("FixedFlowRate", idf_atu.getString(AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod).get());
  EXPECT_EQ(0.2, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::ConstantMinimumAirFlowFraction).get());
  EXPECT_EQ(0.3, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::FixedMinimumAirFlowRate).get());

  boost::optional<WorkspaceObject> woFractionSchedule(idf_atu.getTarget(AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName));
  EXPECT_TRUE(woFractionSchedule);
  EXPECT_EQ(woFractionSchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woFractionSchedule->nameString());

  EXPECT_EQ(0.4, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate).get());
  EXPECT_EQ(0.5, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MinimumHotWaterorSteamFlowRate).get());
  EXPECT_EQ(0.6, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::ConvergenceTolerance).get());
  EXPECT_EQ("Reverse", idf_atu.getString(AirTerminal_SingleDuct_VAV_ReheatFields::DamperHeatingAction).get());
  EXPECT_EQ(0.7, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat).get());
  EXPECT_EQ(0.8, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat).get());
  EXPECT_EQ(0.9, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumReheatAirTemperature).get());

  boost::optional<WorkspaceObject> idf_dsoa(idf_atu.getTarget(AirTerminal_SingleDuct_VAV_ReheatFields::DesignSpecificationOutdoorAirObjectName));
  EXPECT_TRUE(idf_dsoa);
  EXPECT_EQ(idf_dsoa->iddObject().type(), IddObjectType::DesignSpecification_OutdoorAir);

  boost::optional<WorkspaceObject> woTurndownSchedule(idf_atu.getTarget(AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowTurndownScheduleName));
  EXPECT_TRUE(woTurndownSchedule);
  EXPECT_EQ(woTurndownSchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woTurndownSchedule->nameString());
}
