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

#include "ModelFixture.hpp"

#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../AirflowNetworkDistributionNode.hpp"
#include "../AirflowNetworkDistributionNode_Impl.hpp"
#include "../AirflowNetworkDistributionLinkage.hpp"
#include "../AirflowNetworkDistributionLinkage_Impl.hpp"
#include "../AirflowNetworkEquivalentDuct.hpp"
#include "../AirflowNetworkEquivalentDuct_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_EquivalentDuct)
{
  Model model;

  Node node0(model);
  Node node1(model);

  AirflowNetworkDistributionNode afnnode0 = node0.getAirflowNetworkDistributionNode();
  AirflowNetworkDistributionNode afnnode1 = node1.getAirflowNetworkDistributionNode();

  CoilCoolingDXSingleSpeed coilCooling(model);

  AirflowNetworkEquivalentDuct coolingComponent = coilCooling.getAirflowNetworkEquivalentDuct(0.1, 1.0);

  boost::optional<AirflowNetworkEquivalentDuct> opteqd = coilCooling.airflowNetworkEquivalentDuct();

  ASSERT_TRUE(opteqd);
  EXPECT_EQ(coolingComponent, opteqd.get());

  AirflowNetworkDistributionLinkage link(model, afnnode0, afnnode1, coolingComponent);

  // Test component here?

  EXPECT_TRUE(link.setComponent(coolingComponent));
  EXPECT_TRUE(link.component().componentModelObject());
}


