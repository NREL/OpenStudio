/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SetpointManagerSystemNodeResetTemperature.hpp"
#include "../../model/SetpointManagerSystemNodeResetTemperature_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/PlantLoop.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/SetpointManager_SystemNodeReset_Temperature_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SetpointManagerSystemNodeResetTemperature) {
  Model m;
  AirLoopHVAC airloop(m);
  Node supplyOutlet = airloop.supplyOutletNode();
  supplyOutlet.setName("Supply Outlet Node");
  Node supplyInlet = airloop.supplyInletNode();
  supplyInlet.setName("Supply Inlet Node");

  SetpointManagerSystemNodeResetTemperature spm(m);

  EXPECT_TRUE(spm.setControlVariable("MinimumTemperature"));
  EXPECT_TRUE(spm.setSetpointatLowReferenceTemperature(80.0));
  EXPECT_TRUE(spm.setSetpointatHighReferenceTemperature(60.0));
  EXPECT_TRUE(spm.setLowReferenceTemperature(-10.0));
  EXPECT_TRUE(spm.setHighReferenceTemperature(10.0));

  EXPECT_TRUE(spm.addToNode(supplyOutlet));
  EXPECT_TRUE(spm.setReferenceNode(supplyInlet));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);
  EXPECT_EQ(0u, forwardTranslator.errors().size());

  WorkspaceObjectVector spmIdfs = workspace.getObjectsByType(IddObjectType::SetpointManager_SystemNodeReset_Temperature);
  ASSERT_EQ(1u, spmIdfs.size());
  auto& spmIdf = spmIdfs.front();

  EXPECT_EQ(spm.nameString(), spmIdf.nameString());
  EXPECT_EQ("MinimumTemperature", spmIdf.getString(SetpointManager_SystemNodeReset_TemperatureFields::ControlVariable).get());
  EXPECT_EQ(80.0, spmIdf.getDouble(SetpointManager_SystemNodeReset_TemperatureFields::SetpointatLowReferenceTemperature).get());
  EXPECT_EQ(60.0, spmIdf.getDouble(SetpointManager_SystemNodeReset_TemperatureFields::SetpointatHighReferenceTemperature).get());
  EXPECT_EQ(-10.0, spmIdf.getDouble(SetpointManager_SystemNodeReset_TemperatureFields::LowReferenceTemperature).get());
  EXPECT_EQ(10.0, spmIdf.getDouble(SetpointManager_SystemNodeReset_TemperatureFields::HighReferenceTemperature).get());
  EXPECT_EQ("Supply Inlet Node", spmIdf.getString(SetpointManager_SystemNodeReset_TemperatureFields::ReferenceNodeName).get());
  EXPECT_EQ("Supply Outlet Node", spmIdf.getString(SetpointManager_SystemNodeReset_TemperatureFields::SetpointNodeorNodeListName).get());
}
