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

#include "../../model/ControllerOutdoorAir.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/Node.hpp"
#include "../../model/ControllerMechanicalVentilation.hpp"

#include <utilities/idd/Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include "../../utilities/core/Logger.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ControllerOutdoorAir) {
  // Test for #3984
  Model m;
  ForwardTranslator ft;

  ControllerOutdoorAir controller_oa(m);

  // Not used: not translated
  {
    ASSERT_NO_THROW(ft.translateModel(m));
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idf_controller_oas(w.getObjectsByType(IddObjectType::Controller_OutdoorAir));
    EXPECT_EQ(0u, idf_controller_oas.size());
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::AirLoopHVAC).size());
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::AirLoopHVAC_OutdoorAirSystem).size());
  }

  AirLoopHVAC a(m);
  AirLoopHVACOutdoorAirSystem oa_sys(m, controller_oa);

  // Not used since the OutdoorAirSystem isn't used itself: not translated
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idf_controller_oas(w.getObjectsByType(IddObjectType::Controller_OutdoorAir));
    EXPECT_EQ(0u, idf_controller_oas.size());
    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC).size());
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::AirLoopHVAC_OutdoorAirSystem).size());
  }

  Node supplyOutlet = a.supplyOutletNode();
  EXPECT_TRUE(oa_sys.addToNode(supplyOutlet));
  // Not used: should be translated
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idf_controller_oas(w.getObjectsByType(IddObjectType::Controller_OutdoorAir));
    EXPECT_EQ(1u, idf_controller_oas.size());
    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC).size());
    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC_OutdoorAirSystem).size());
  }
}
