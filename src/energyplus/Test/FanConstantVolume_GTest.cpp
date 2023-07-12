/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/FanConstantVolume.hpp"

#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVAC.hpp"

#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_FanConstantVolume_EndUseSubCategory) {

  Model m;

  FanConstantVolume fan(m);

  std::string endUse("Special Fan");
  fan.setEndUseSubcategory(endUse);

  // Assign it to a loop
  AirLoopHVAC a(m);
  Node supplyOutletNode = a.supplyOutletNode();
  fan.addToNode(supplyOutletNode);

  // ForwardTranslate
  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::Fan_ConstantVolume));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_fan(idfObjs[0]);

  // End Use Subcategory
  boost::optional<std::string> _idfEndUse = idf_fan.getString(Fan_ConstantVolumeFields::EndUseSubcategory);
  ASSERT_TRUE(_idfEndUse);
  EXPECT_EQ(endUse, _idfEndUse.get());
}

// The Forward Translator should not translate fan if not used (by an AirLoopHVAC, or a ZoneHVACComponent )
TEST_F(EnergyPlusFixture, ForwardTranslator_FanConstantVolume_NotUsed) {

  Model m;

  FanConstantVolume fan(m);

  // ForwardTranslate
  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::Fan_ConstantVolume));
  EXPECT_EQ(0u, idfObjs.size());
}
