/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SetpointManagerFollowGroundTemperature.hpp"
#include "../../model/SetpointManagerFollowGroundTemperature_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/PlantLoop.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/SetpointManager_FollowGroundTemperature_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SetpointManagerFollowGroundTemperature) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerFollowGroundTemperature testObject(m);
  testObject.setControlVariable("MinimumTemperature");
  testObject.setReferenceGroundTemperatureObjectType("Site:GroundTemperature:Shallow");
  testObject.addToNode(outletNode);
  testObject.setOffsetTemperatureDifference(999.9);
  testObject.setMaximumSetpointTemperature(999.9);
  testObject.setMinimumSetpointTemperature(999.9);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);
  EXPECT_EQ(0u, forwardTranslator.errors().size());

  WorkspaceObjectVector spmIdfs = workspace.getObjectsByType(IddObjectType::SetpointManager_FollowGroundTemperature);
  ASSERT_EQ(1u, spmIdfs.size());
  WorkspaceObject& spmIdf(spmIdfs[0]);

  EXPECT_EQ("MinimumTemperature", spmIdf.getString(SetpointManager_FollowGroundTemperatureFields::ControlVariable, true).get());
  EXPECT_EQ("Site:GroundTemperature:Shallow",
            spmIdf.getString(SetpointManager_FollowGroundTemperatureFields::ReferenceGroundTemperatureObjectType, true).get());
  EXPECT_DOUBLE_EQ(999.9, spmIdf.getDouble(SetpointManager_FollowGroundTemperatureFields::OffsetTemperatureDifference, true).get());
  EXPECT_DOUBLE_EQ(999.9, spmIdf.getDouble(SetpointManager_FollowGroundTemperatureFields::MaximumSetpointTemperature, true).get());
  EXPECT_DOUBLE_EQ(999.9, spmIdf.getDouble(SetpointManager_FollowGroundTemperatureFields::MinimumSetpointTemperature, true).get());
}
