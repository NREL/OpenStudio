/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../SetpointManagerSystemNodeResetTemperature.hpp"
#include "../SetpointManagerSystemNodeResetTemperature_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../Node.hpp"
#include "../Node_Impl.hpp"

#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerSystemNodeResetTemperature_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  SetpointManagerSystemNodeResetTemperature setpointManagerSystemNodeResetTemperature(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // SetpointManagerSystemNodeResetTemperature setpointManagerSystemNodeResetTemperature = m.getUniqueModelObject<SetpointManagerSystemNodeResetTemperature>();

  setpointManagerSystemNodeResetTemperature.setName("My SetpointManagerSystemNodeResetTemperature");

  // Control Variable: Required String
  EXPECT_TRUE(setpointManagerSystemNodeResetTemperature.setControlVariable("Temperature"));
  EXPECT_EQ("Temperature", setpointManagerSystemNodeResetTemperature.controlVariable());
  // Bad Value
  EXPECT_FALSE(setpointManagerSystemNodeResetTemperature.setControlVariable("BADENUM"));
  EXPECT_EQ("Temperature", setpointManagerSystemNodeResetTemperature.controlVariable());

  // Setpoint at Low Reference Temperature: Required Double
  EXPECT_TRUE(setpointManagerSystemNodeResetTemperature.setSetpointatLowReferenceTemperature(0.4));
  EXPECT_EQ(0.4, setpointManagerSystemNodeResetTemperature.setpointatLowReferenceTemperature());

  // Setpoint at High Reference Temperature: Required Double
  EXPECT_TRUE(setpointManagerSystemNodeResetTemperature.setSetpointatHighReferenceTemperature(0.5));
  EXPECT_EQ(0.5, setpointManagerSystemNodeResetTemperature.setpointatHighReferenceTemperature());

  // Low Reference Temperature: Required Double
  EXPECT_TRUE(setpointManagerSystemNodeResetTemperature.setLowReferenceTemperature(0.6));
  EXPECT_EQ(0.6, setpointManagerSystemNodeResetTemperature.lowReferenceTemperature());

  // High Reference Temperature: Required Double
  EXPECT_TRUE(setpointManagerSystemNodeResetTemperature.setHighReferenceTemperature(0.7));
  EXPECT_EQ(0.7, setpointManagerSystemNodeResetTemperature.highReferenceTemperature());

  // Reference Node Name: Optional Object
  boost::optional<Node> obj(m);
  EXPECT_TRUE(setpointManagerSystemNodeResetTemperature.setReferenceNode(obj));
  ASSERT_TRUE(setpointManagerSystemNodeResetTemperature.referenceNode());
  EXPECT_EQ(obj, setpointManagerSystemNodeResetTemperature.referenceNode().get());

  // Setpoint Node or NodeList Name: Optional Object
  boost::optional<Node> obj(m);
  EXPECT_TRUE(setpointManagerSystemNodeResetTemperature.setSetpointNodeorNodeList(obj));
  ASSERT_TRUE(setpointManagerSystemNodeResetTemperature.setpointNodeorNodeList());
  EXPECT_EQ(obj, setpointManagerSystemNodeResetTemperature.setpointNodeorNodeList().get());
}
