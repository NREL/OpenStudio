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
#include "../../model/AvailabilityManagerLowTemperatureTurnOn.hpp"
#include "../../model/Node.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/AirLoopHVAC.hpp"

#include <utilities/idd/AvailabilityManager_LowTemperatureTurnOn_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AvailabilityManagerLowTemperatureTurnOn) {

  Model m;

  AvailabilityManagerLowTemperatureTurnOn avm(m);

  // Assign Sensor Node and Temperature
  Node n(m);
  EXPECT_TRUE(avm.setSensorNode(n));
  EXPECT_TRUE(avm.setTemperature(60.19));

  // Assign it to a plant loop
  PlantLoop p(m);
  p.setAvailabilityManager(avm);

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::AvailabilityManager_LowTemperatureTurnOn));
  EXPECT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_avm(idfObjs[0]);

  // Sensor Node Name
  ASSERT_EQ(n.name().get(), idf_avm.getString(AvailabilityManager_LowTemperatureTurnOnFields::SensorNodeName).get());

  // Temperature
  EXPECT_DOUBLE_EQ(avm.temperature(), idf_avm.getDouble(AvailabilityManager_LowTemperatureTurnOnFields::Temperature).get());
}

/*
 * The Forward Translator should throw if the AVM is missing the required Sensor Node (E+ crash on 8.8.0)
 */
TEST_F(EnergyPlusFixture, ForwardTranslator_AvailabilityManagerLowTemperatureTurnOn_NoSensorNode) {

  Model m;

  AvailabilityManagerLowTemperatureTurnOn avm(m);
  // No Sensor Node
  ASSERT_FALSE(avm.sensorNode());

  // Assign it to a plant loop (otherwise it's purely not translated anyways...)
  PlantLoop p(m);
  p.setAvailabilityManager(avm);

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);

  EXPECT_EQ(1u, forwardTranslator.errors().size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AvailabilityManagerLowTemperatureTurnOn_ConnectPlantLoopAirLoop) {

  Model m;

  AvailabilityManagerLowTemperatureTurnOn avm(m);

  // Assign it to a plant loop
  PlantLoop p(m);
  ASSERT_TRUE(p.setAvailabilityManager(avm));

  // Assign it to an Air loop
  AirLoopHVAC a(m);
  ASSERT_TRUE(a.setAvailabilityManager(avm));
}
