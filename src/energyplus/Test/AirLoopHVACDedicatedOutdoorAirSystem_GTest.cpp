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
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirLoopHVACDedicatedOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACDedicatedOutdoorAirSystem_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/ControllerOutdoorAir_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include "../../model/AirLoopHVACZoneMixer_Impl.hpp"
#include "../../model/AirLoopHVACZoneSplitter.hpp"
#include "../../model/AirLoopHVACZoneSplitter_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/AirLoopHVAC_DedicatedOutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_Mixer_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_Splitter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACDedicatedOutdoorAirSystem) {
  Model m;
  ControllerOutdoorAir controller(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controller);
  oaSystem.setName("Outdoor Air System 1");
  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(m, oaSystem);
  doaSystem.setName("Dedicated Outdoor Air System 1");
  AirLoopHVAC airLoop(m);
  airLoop.setName("Air Loop 1");
  Node supplyOutlet = airLoop.supplyOutletNode();
  oaSystem.addToNode(supplyOutlet);
  doaSystem.addAirLoop(airLoop);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfContrls(w.getObjectsByType(IddObjectType::Controller_OutdoorAir));
  ASSERT_EQ(1u, idfContrls.size());

  WorkspaceObjectVector idfOASs(w.getObjectsByType(IddObjectType::AirLoopHVAC_OutdoorAirSystem));
  ASSERT_EQ(1u, idfOASs.size());

  WorkspaceObjectVector idfAirLoops(w.getObjectsByType(IddObjectType::AirLoopHVAC));
  ASSERT_EQ(1u, idfAirLoops.size());

  WorkspaceObjectVector idfDOASs(w.getObjectsByType(IddObjectType::AirLoopHVAC_DedicatedOutdoorAirSystem));
  ASSERT_EQ(1u, idfDOASs.size());
  WorkspaceObject idfDOAS(idfDOASs[0]);

  EXPECT_EQ("Outdoor Air System 1", idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_OutdoorAirSystemName, false).get());
  EXPECT_EQ("", idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ("Dedicated Outdoor Air System 1 Mixer", idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_MixerName, false).get());
  EXPECT_EQ("Dedicated Outdoor Air System 1 Splitter", idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_SplitterName, false).get());
  EXPECT_EQ(4.5, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignTemperature, false).get());
  EXPECT_EQ(0.004, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignHumidityRatio, false).get());
  EXPECT_EQ(17.5, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignTemperature, false).get());
  EXPECT_EQ(0.012, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignHumidityRatio, false).get());
  EXPECT_EQ(1, idfDOAS.getInt(AirLoopHVAC_DedicatedOutdoorAirSystemFields::NumberofAirLoopHVAC, false).get());
  EXPECT_EQ(1u, idfDOAS.numExtensibleGroups());
  WorkspaceExtensibleGroup w_eg = idfDOAS.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ("Air Loop 1", w_eg.getString(AirLoopHVAC_DedicatedOutdoorAirSystemExtensibleFields::AirLoopHVACName, false).get());

  WorkspaceObjectVector idfMixers(w.getObjectsByType(IddObjectType::AirLoopHVAC_Mixer));
  ASSERT_EQ(1u, idfMixers.size());
  WorkspaceObject idfMixer(idfMixers[0]);

  EXPECT_EQ("Dedicated Outdoor Air System 1 Mixer", idfMixer.getString(AirLoopHVAC_MixerFields::Name, false).get());
  EXPECT_EQ("", idfMixer.getString(AirLoopHVAC_MixerFields::OutletNodeName, false).get());
  EXPECT_EQ(1u, idfMixer.numExtensibleGroups());
  WorkspaceExtensibleGroup w_egMixer = idfMixer.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ(airLoop.zoneMixer().inletModelObjects()[0].nameString(), w_egMixer.getString(AirLoopHVAC_MixerExtensibleFields::InletNodeName, false).get());

  WorkspaceObjectVector idfSplitters(w.getObjectsByType(IddObjectType::AirLoopHVAC_Splitter));
  ASSERT_EQ(1u, idfSplitters.size());
  WorkspaceObject idfSplitter(idfSplitters[0]);

  EXPECT_EQ("Dedicated Outdoor Air System 1 Splitter", idfSplitter.getString(AirLoopHVAC_SplitterFields::Name, false).get());
  EXPECT_EQ("", idfSplitter.getString(AirLoopHVAC_SplitterFields::InletNodeName, false).get());
  EXPECT_EQ(1u, idfSplitter.numExtensibleGroups());
  WorkspaceExtensibleGroup w_egSplitter = idfSplitter.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ(airLoop.zoneSplitter().outletModelObjects()[0].nameString(), w_egSplitter.getString(AirLoopHVAC_SplitterExtensibleFields::OutletNodeName, false).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_AirLoopHVACDedicatedOutdoorAirSystem) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idf_oas(openstudio::IddObjectType::AirLoopHVAC_OutdoorAirSystem);
  idf_oas.setString(AirLoopHVAC_OutdoorAirSystemFields::Name, "Outdoor Air System 1");

  openstudio::WorkspaceObject epOAS = workspace.addObject(idf_oas).get();

  openstudio::IdfObject idf_doas(openstudio::IddObjectType::AirLoopHVAC_DedicatedOutdoorAirSystem);
  idf_doas.setString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::Name, "Dedicated Outdoor Air System 1");
  idf_doas.setString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_OutdoorAirSystemName, "Outdoor Air System 1");

  openstudio::WorkspaceObject epDOAS = workspace.addObject(idf_doas).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<AirLoopHVACDedicatedOutdoorAirSystem> doass = model.getModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>();
  ASSERT_EQ(1u, doass.size());
}
