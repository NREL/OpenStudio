/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/AirTerminalDualDuctConstantVolume.hpp"
#include "../../model/AirTerminalDualDuctConstantVolume_Impl.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/Lights.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/AirTerminal_DualDuct_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirTerminalDualDuctConstantVolume) {

  Model m;

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);

  LightsDefinition ld(m);
  Lights l(ld);
  l.setSpace(s);

  AirTerminalDualDuctConstantVolume atu(m);
  ScheduleConstant sch(m);
  EXPECT_TRUE(atu.setAvailabilitySchedule(sch));

  AirLoopHVAC a(m);
  a.addBranchForZone(z, atu);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  // Check the ATU
  WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::AirTerminal_DualDuct_ConstantVolume);
  EXPECT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_atu(idfObjs[0]);

  EXPECT_EQ(atu.name().get(), idf_atu.name().get());

  EXPECT_EQ(atu.availabilitySchedule().name().get(), idf_atu.getString(AirTerminal_DualDuct_ConstantVolumeFields::AvailabilityScheduleName).get());
  EXPECT_EQ(sch.name().get(), idf_atu.getString(AirTerminal_DualDuct_ConstantVolumeFields::AvailabilityScheduleName).get());

  EXPECT_EQ(atu.hotAirInletNode().get().name().get(), idf_atu.getString(AirTerminal_DualDuct_ConstantVolumeFields::HotAirInletNodeName).get());
  EXPECT_EQ(atu.coldAirInletNode().get().name().get(), idf_atu.getString(AirTerminal_DualDuct_ConstantVolumeFields::ColdAirInletNodeName).get());
  EXPECT_TRUE(openstudio::istringEqual("autosize", idf_atu.getString(AirTerminal_DualDuct_ConstantVolumeFields::MaximumAirFlowRate).get()));

  // Check the ADU
  WorkspaceObjectVector idfObjs2 = w.getObjectsByType(IddObjectType::ZoneHVAC_AirDistributionUnit);
  EXPECT_EQ(1u, idfObjs2.size());
  WorkspaceObject idf_adu(idfObjs2[0]);

  // Check node match
  EXPECT_EQ(idf_atu.getString(AirTerminal_DualDuct_ConstantVolumeFields::AirOutletNodeName).get(),
            idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName).get());

  EXPECT_EQ("AirTerminal:DualDuct:ConstantVolume", idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType).get());
  EXPECT_EQ(idf_atu.name().get(), idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName).get());

  // hardsize max flow rate, translate, and check if set correctly
  atu.setMaximumAirFlowRate(0.2);
  ASSERT_TRUE(atu.maximumAirFlowRate());
  w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs3 = w.getObjectsByType(IddObjectType::AirTerminal_DualDuct_ConstantVolume);
  EXPECT_EQ(1u, idfObjs3.size());
  WorkspaceObject idf_atu3(idfObjs3[0]);

  EXPECT_DOUBLE_EQ(atu.maximumAirFlowRate().get(), idf_atu3.getDouble(AirTerminal_DualDuct_ConstantVolumeFields::MaximumAirFlowRate).get());
}
