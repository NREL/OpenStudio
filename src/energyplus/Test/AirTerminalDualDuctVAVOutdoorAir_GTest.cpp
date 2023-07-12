/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirTerminalDualDuctVAVOutdoorAir.hpp"
#include "../../model/AirTerminalDualDuctVAVOutdoorAir_Impl.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/Lights.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/AirTerminal_DualDuct_VAV_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirTerminalDualDuctVAVOutdoorAir) {

  Model m;

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);

  LightsDefinition ld(m);
  Lights l(ld);
  l.setSpace(s);

  // Set a DSOA for the space, will be used if controlForOutdoorAir is true
  DesignSpecificationOutdoorAir dsoa(m);
  dsoa.setOutdoorAirFlowRate(0.1);
  dsoa.setName("My DSOA");
  EXPECT_TRUE(s.setDesignSpecificationOutdoorAir(dsoa));

  AirTerminalDualDuctVAVOutdoorAir atu(m);
  ScheduleConstant sch(m);

  EXPECT_TRUE(atu.setControlForOutdoorAir(true));
  EXPECT_TRUE(atu.setPerPersonVentilationRateMode("DesignOccupancy"));

  EXPECT_TRUE(atu.setAvailabilitySchedule(sch));

  AirLoopHVAC a(m, true);
  ASSERT_TRUE(a.isDualDuct());
  a.addBranchForZone(z, atu);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  // Check the ATU
  WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::AirTerminal_DualDuct_VAV_OutdoorAir);
  EXPECT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_atu(idfObjs[0]);

  EXPECT_EQ(atu.name().get(), idf_atu.name().get());

  EXPECT_EQ(atu.availabilitySchedule().name().get(), idf_atu.getString(AirTerminal_DualDuct_VAV_OutdoorAirFields::AvailabilityScheduleName).get());
  EXPECT_EQ(sch.name().get(), idf_atu.getString(AirTerminal_DualDuct_VAV_OutdoorAirFields::AvailabilityScheduleName).get());

  EXPECT_EQ(atu.outdoorAirInletNode().get().name().get(),
            idf_atu.getString(AirTerminal_DualDuct_VAV_OutdoorAirFields::OutdoorAirInletNodeName).get());
  EXPECT_EQ(atu.recirculatedAirInletNode().get().name().get(),
            idf_atu.getString(AirTerminal_DualDuct_VAV_OutdoorAirFields::RecirculatedAirInletNodeName).get());

  EXPECT_TRUE(openstudio::istringEqual("autosize", idf_atu.getString(AirTerminal_DualDuct_VAV_OutdoorAirFields::MaximumTerminalAirFlowRate).get()));

  // per Person Ventilation Rate Mode
  EXPECT_EQ("DesignOccupancy", idf_atu.getString(AirTerminal_DualDuct_VAV_OutdoorAirFields::PerPersonVentilationRateMode).get());

  // DSOA
  EXPECT_EQ(dsoa.name().get(), idf_atu.getString(AirTerminal_DualDuct_VAV_OutdoorAirFields::DesignSpecificationOutdoorAirObjectName).get());

  // Check the ADU
  WorkspaceObjectVector idfObjs2 = w.getObjectsByType(IddObjectType::ZoneHVAC_AirDistributionUnit);
  EXPECT_EQ(1u, idfObjs2.size());
  WorkspaceObject idf_adu(idfObjs2[0]);

  // Check node match
  EXPECT_EQ(idf_atu.getString(AirTerminal_DualDuct_VAV_OutdoorAirFields::AirOutletNodeName).get(),
            idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName).get());

  EXPECT_EQ("AirTerminal:DualDuct:VAV:OutdoorAir", idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType).get());
  EXPECT_EQ(idf_atu.name().get(), idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName).get());

  // hardsize max flow rate, setControlForOutdoorAir(false), PerPersonVentilationRateMode to the other one
  // translate, and check if set correctly
  EXPECT_TRUE(atu.setMaximumTerminalAirFlowRate(0.2));
  ASSERT_TRUE(atu.maximumTerminalAirFlowRate());
  EXPECT_TRUE(atu.setPerPersonVentilationRateMode("CurrentOccupancy"));
  EXPECT_TRUE(atu.setControlForOutdoorAir(false));
  EXPECT_FALSE(atu.controlForOutdoorAir());

  w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs3 = w.getObjectsByType(IddObjectType::AirTerminal_DualDuct_VAV_OutdoorAir);
  EXPECT_EQ(1u, idfObjs3.size());
  WorkspaceObject idf_atu3(idfObjs3[0]);

  EXPECT_DOUBLE_EQ(atu.maximumTerminalAirFlowRate().get(),
                   idf_atu3.getDouble(AirTerminal_DualDuct_VAV_OutdoorAirFields::MaximumTerminalAirFlowRate).get());

  EXPECT_EQ("CurrentOccupancy", idf_atu3.getString(AirTerminal_DualDuct_VAV_OutdoorAirFields::PerPersonVentilationRateMode).get());
  EXPECT_EQ("", idf_atu3.getString(AirTerminal_DualDuct_VAV_OutdoorAirFields::DesignSpecificationOutdoorAirObjectName).get());
}
