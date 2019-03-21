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

TEST_F(EnergyPlusFixture, ForwardTranslator_SetpointManagerFollowGroundTemperature)
{
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
  WorkspaceObject &spmIdf(spmIdfs[0]);

  EXPECT_EQ("MinimumTemperature", spmIdf.getString(SetpointManager_FollowGroundTemperatureFields::ControlVariable, true).get());
  EXPECT_EQ("Site:GroundTemperature:Shallow", spmIdf.getString(SetpointManager_FollowGroundTemperatureFields::ReferenceGroundTemperatureObjectType, true).get());
  EXPECT_DOUBLE_EQ(999.9, spmIdf.getDouble(SetpointManager_FollowGroundTemperatureFields::OffsetTemperatureDifference, true).get());
  EXPECT_DOUBLE_EQ(999.9, spmIdf.getDouble(SetpointManager_FollowGroundTemperatureFields::MaximumSetpointTemperature, true).get());
  EXPECT_DOUBLE_EQ(999.9, spmIdf.getDouble(SetpointManager_FollowGroundTemperatureFields::MinimumSetpointTemperature, true).get());

}


