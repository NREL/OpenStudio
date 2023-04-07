/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
