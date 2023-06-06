/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SetpointManagerSystemNodeResetHumidity.hpp"
#include "../../model/SetpointManagerSystemNodeResetHumidity_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/PlantLoop.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/SetpointManager_SystemNodeReset_Humidity_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SetpointManagerSystemNodeResetHumidity) {
  Model m;
  AirLoopHVAC airloop(m);
  Node supplyOutlet = airloop.supplyOutletNode();
  supplyOutlet.setName("Supply Outlet Node");
  Node supplyInlet = airloop.supplyInletNode();
  supplyInlet.setName("Supply Inlet Node");

  SetpointManagerSystemNodeResetHumidity spm(m);

  EXPECT_TRUE(spm.setControlVariable("MinimumHumidityRatio"));
  EXPECT_TRUE(spm.setSetpointatLowReferenceHumidityRatio(0.002));
  EXPECT_TRUE(spm.setSetpointatHighReferenceHumidityRatio(0.001));
  EXPECT_TRUE(spm.setLowReferenceHumidityRatio(0.004));
  EXPECT_TRUE(spm.setHighReferenceHumidityRatio(0.003));

  EXPECT_TRUE(spm.addToNode(supplyOutlet));
  EXPECT_TRUE(spm.setReferenceNode(supplyInlet));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);
  EXPECT_EQ(0u, forwardTranslator.errors().size());

  WorkspaceObjectVector spmIdfs = workspace.getObjectsByType(IddObjectType::SetpointManager_SystemNodeReset_Humidity);
  ASSERT_EQ(1u, spmIdfs.size());
  auto& spmIdf = spmIdfs.front();

  EXPECT_EQ(spm.nameString(), spmIdf.nameString());
  EXPECT_EQ("MinimumHumidityRatio", spmIdf.getString(SetpointManager_SystemNodeReset_HumidityFields::ControlVariable).get());
  EXPECT_EQ(0.002, spmIdf.getDouble(SetpointManager_SystemNodeReset_HumidityFields::SetpointatLowReferenceHumidityRatio).get());
  EXPECT_EQ(0.001, spmIdf.getDouble(SetpointManager_SystemNodeReset_HumidityFields::SetpointatHighReferenceHumidityRatio).get());
  EXPECT_EQ(0.004, spmIdf.getDouble(SetpointManager_SystemNodeReset_HumidityFields::LowReferenceHumidityRatio).get());
  EXPECT_EQ(0.003, spmIdf.getDouble(SetpointManager_SystemNodeReset_HumidityFields::HighReferenceHumidityRatio).get());
  EXPECT_EQ("Supply Inlet Node", spmIdf.getString(SetpointManager_SystemNodeReset_HumidityFields::ReferenceNodeName).get());
  EXPECT_EQ("Supply Outlet Node", spmIdf.getString(SetpointManager_SystemNodeReset_HumidityFields::SetpointNodeorNodeListName).get());
}
