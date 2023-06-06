/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirTerminalSingleDuctInletSideMixer.hpp"

#include "../../model/ZoneHVACFourPipeFanCoil.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/CoilCoolingWater.hpp"
#include "../../model/CoilHeatingWater.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/Space.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/Lights.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/AirTerminal_SingleDuct_Mixer_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirTerminalSingleDuctInletSideMixer) {

  ForwardTranslator ft;

  // Create a one space, one zone model
  Model m;

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);

  LightsDefinition ld(m);
  Lights l(ld);
  l.setSpace(s);
  DesignSpecificationOutdoorAir dsoa(m);
  dsoa.setName("Space DSOA");
  EXPECT_TRUE(s.setDesignSpecificationOutdoorAir(dsoa));

  // Create an ATU
  AirTerminalSingleDuctInletSideMixer atu(m);
  EXPECT_TRUE(atu.setControlForOutdoorAir(true));
  EXPECT_TRUE(atu.setPerPersonVentilationRateMode("DesignOccupancy"));

  // Connect it to an AirLoopHVAC
  AirLoopHVAC a(m);
  a.addBranchForZone(z, atu);

  // TODO: should we even allow FT if the ATU InletSideMixer isn't also connected to a secondary node?

  Schedule availabilitySchedule = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m, availabilitySchedule);
  CoilHeatingWater heatingCoil(m, availabilitySchedule);
  CoilCoolingWater coolingCoil(m, availabilitySchedule);
  ZoneHVACFourPipeFanCoil fc(m, availabilitySchedule, fan, coolingCoil, heatingCoil);

  // They must be connected to a PlantLoop too
  PlantLoop chw_p(m);
  chw_p.addDemandBranchForComponent(heatingCoil);

  PlantLoop hw_p(m);
  hw_p.addDemandBranchForComponent(coolingCoil);

  auto n = atu.outletModelObject()->cast<Node>();
  EXPECT_TRUE(fc.addToNode(n));

  z.zoneAirNode().setName("Zone Air Node");
  atu.inletModelObject()->setName("ATU InletSideMixer Inlet Node");
  atu.outletModelObject()->setName("ATU InletSideMixer Outlet to FC Inlet Node");
  fc.outletNode()->setName("FC Outlet Node");
  z.returnAirModelObjects()[0].setName("Zone Return Air Node");
  z.exhaustPortList().modelObjects()[0].setName("Zone Exhaust Air Node");

  // This time, it should have been translated
  {
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(0, ft.warnings().size()) << [&ft]() {
      std::string s;
      bool firstTime = true;
      for (auto& l : ft.warnings()) {
        if (!firstTime) {
          s += '\n';
          firstTime = false;
        }
        s += l.logMessage();
      }
      return s;
    }();
    EXPECT_EQ(0, ft.errors().size());

    // Check the ATU
    auto idfObjs = w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_Mixer);
    EXPECT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_atu = idfObjs[0];

    EXPECT_EQ(atu.nameString(), idf_atu.nameString());

    EXPECT_EQ("ZoneHVAC:FourPipeFanCoil", idf_atu.getString(AirTerminal_SingleDuct_MixerFields::ZoneHVACUnitObjectType).get());

    EXPECT_EQ(fc.nameString(), idf_atu.getString(AirTerminal_SingleDuct_MixerFields::ZoneHVACUnitObjectName).get());

    // Nodes

    EXPECT_EQ("ATU InletSideMixer Outlet to FC Inlet Node", idf_atu.getString(AirTerminal_SingleDuct_MixerFields::MixerOutletNodeName).get());
    EXPECT_EQ(atu.outletModelObject()->nameString(), idf_atu.getString(AirTerminal_SingleDuct_MixerFields::MixerOutletNodeName).get());

    EXPECT_EQ("ATU InletSideMixer Inlet Node", idf_atu.getString(AirTerminal_SingleDuct_MixerFields::MixerPrimaryAirInletNodeName).get());
    EXPECT_EQ(atu.inletModelObject()->nameString(), idf_atu.getString(AirTerminal_SingleDuct_MixerFields::MixerPrimaryAirInletNodeName).get());

    EXPECT_EQ("Zone Exhaust Air Node", idf_atu.getString(AirTerminal_SingleDuct_MixerFields::MixerSecondaryAirInletNodeName).get());
    EXPECT_EQ(z.exhaustPortList().modelObjects()[0].nameString(),
              idf_atu.getString(AirTerminal_SingleDuct_MixerFields::MixerSecondaryAirInletNodeName).get());

    // Control for Outdoor Air => DSOA should be filled
    auto target_dsoa_ = idf_atu.getTarget(AirTerminal_SingleDuct_MixerFields::DesignSpecificationOutdoorAirObjectName);
    ASSERT_TRUE(target_dsoa_);
    EXPECT_EQ(dsoa.nameString(), target_dsoa_->nameString());

    EXPECT_EQ("DesignOccupancy", idf_atu.getString(AirTerminal_SingleDuct_MixerFields::PerPersonVentilationRateMode).get());

    // Check the ADU
    auto idfObjs2 = w.getObjectsByType(IddObjectType::ZoneHVAC_AirDistributionUnit);
    EXPECT_EQ(1u, idfObjs2.size());
    WorkspaceObject idf_adu = idfObjs2[0];

    // Check node match
    EXPECT_EQ("ATU InletSideMixer Outlet to FC Inlet Node",
              idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName).get());
    EXPECT_EQ(idf_atu.getString(AirTerminal_SingleDuct_MixerFields::MixerOutletNodeName).get(),
              idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName).get());

    EXPECT_EQ("AirTerminal:SingleDuct:Mixer", idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType).get());
    EXPECT_EQ(idf_atu.nameString(), idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName).get());
  }
}
