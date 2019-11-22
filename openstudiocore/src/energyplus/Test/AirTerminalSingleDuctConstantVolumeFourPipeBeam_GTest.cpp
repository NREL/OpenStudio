/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/AirTerminalSingleDuctConstantVolumeFourPipeBeam.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl.hpp"
#include "../../model/CoilCoolingFourPipeBeam.hpp"
#include "../../model/CoilCoolingFourPipeBeam_Impl.hpp"
#include "../../model/CoilHeatingFourPipeBeam.hpp"
#include "../../model/CoilHeatingFourPipeBeam_Impl.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/Lights.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirTerminalSingleDuctConstantVolumeFourPipeBeam) {

  ForwardTranslator ft;

  // Create a one space, one zone model
  Model m;

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);

  LightsDefinition ld(m);
  Lights l(ld);
  l.setSpace(s);

  // Create an ATU, with no coils for now
  AirTerminalSingleDuctConstantVolumeFourPipeBeam atu(m);
  ScheduleConstant sch(m);
  EXPECT_TRUE(atu.setPrimaryAirAvailabilitySchedule(sch));

  // Connect it to an AirLoopHVAC
  AirLoopHVAC a(m);
  a.addBranchForZone(z, atu);

  // It is connected to an AirLoopHVAC, but no plantLoop, so ForwardTranslation should not translate it
  {
    Workspace w = ft.translateModel(m);

    // Check the ATU isn't present
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam);
    EXPECT_EQ(0u, idfObjs.size());
  }

  // Add a Cooling Coil
  CoilCoolingFourPipeBeam cc(m);
  atu.setCoolingCoil(cc);
  // and a heating coil
  CoilHeatingFourPipeBeam hc(m);
  atu.setHeatingCoil(hc);

  // They must be connected to a PlantLoop too
  PlantLoop chw_p(m);
  chw_p.addDemandBranchForComponent(cc);

  PlantLoop hw_p(m);
  hw_p.addDemandBranchForComponent(hc);

  // Hardsize an autosize field
  atu.setDesignHotWaterVolumeFlowRate(0.005);

  // This time, it should have been translated
  {
    Workspace w = ft.translateModel(m);

    // Check the ATU
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam);
    EXPECT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_atu(idfObjs[0]);

    EXPECT_EQ(atu.name().get(), idf_atu.name().get());

    // Check that the primary air schedule was set
    EXPECT_EQ(atu.primaryAirAvailabilitySchedule().name().get(), idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirAvailabilityScheduleName).get());
    EXPECT_EQ(sch.name().get(), idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirAvailabilityScheduleName).get());

    // Check the Node connections
    // Primary Air
    EXPECT_EQ(atu.primaryAirInletNode().get().name().get(), idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirInletNodeName).get());
    EXPECT_EQ(atu.primaryAirOutletNode().get().name().get(), idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirOutletNodeName).get());

    // Chilled Water
    EXPECT_EQ(atu.chilledWaterInletNode().get().name().get(), idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ChilledWaterInletNodeName).get());
    EXPECT_EQ(atu.chilledWaterOutletNode().get().name().get(), idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ChilledWaterOutletNodeName).get());

    // Hot Water
    EXPECT_EQ(atu.hotWaterInletNode().get().name().get(), idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HotWaterInletNodeName).get());
    EXPECT_EQ(atu.hotWaterOutletNode().get().name().get(), idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HotWaterOutletNodeName).get());

    // Check autosizable fields
    EXPECT_TRUE(openstudio::istringEqual("autosize", idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate).get()));
    EXPECT_TRUE(openstudio::istringEqual("autosize", idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate).get()));
    EXPECT_EQ(0.005, idf_atu.getDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate).get());
    EXPECT_TRUE(openstudio::istringEqual("autosize", idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength).get()));

    // Check that the default value was assigned
    EXPECT_EQ(atu.ratedPrimaryAirFlowRateperBeamLength(), idf_atu.getDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::RatedPrimaryAirFlowRateperBeamLength).get());

    // Check the ADU
    WorkspaceObjectVector idfObjs2 = w.getObjectsByType(IddObjectType::ZoneHVAC_AirDistributionUnit);
    EXPECT_EQ(1u, idfObjs2.size());
    WorkspaceObject idf_adu(idfObjs2[0]);

    // Check node match
    EXPECT_EQ(idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirOutletNodeName).get(),
              idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName).get());

    EXPECT_EQ("AirTerminal:SingleDuct:ConstantVolume:FourPipeBeam", idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType).get());
    EXPECT_EQ(idf_atu.name().get(), idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName).get());

  }

}
