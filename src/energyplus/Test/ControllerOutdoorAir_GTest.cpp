/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
