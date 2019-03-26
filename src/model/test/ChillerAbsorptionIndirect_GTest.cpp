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
#include "../ChillerAbsorptionIndirect.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../Node_Impl.hpp"
#include "../Node.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ChillerAbsorptionIndirect)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     Model m;
     ChillerAbsorptionIndirect chiller(m);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );

  {
    Model m;
    ChillerAbsorptionIndirect chiller(m);

    {
      PlantLoop chilledPlant(m);
      auto node = chilledPlant.supplyOutletNode();
      EXPECT_TRUE(chiller.addToNode(node));
      auto plant = chiller.plantLoop();
      EXPECT_TRUE(plant);
      if( plant ) {
        EXPECT_EQ(chilledPlant.handle(),plant->handle());
      }

      EXPECT_EQ(7u,chilledPlant.supplyComponents().size());
    }

    {
      PlantLoop condenserPlant(m);
      auto node = condenserPlant.demandInletNode();
      EXPECT_TRUE(chiller.addToNode(node));
      auto plant = chiller.secondaryPlantLoop();
      EXPECT_TRUE(plant);
      if( plant ) {
        EXPECT_EQ(condenserPlant.handle(),plant->handle());
      }

      EXPECT_EQ(7u,condenserPlant.demandComponents().size());
    }

    {
      PlantLoop generatorPlant(m);
      auto node = generatorPlant.demandInletNode();
      EXPECT_TRUE(chiller.addToTertiaryNode(node));
      auto plant = chiller.tertiaryPlantLoop();
      EXPECT_TRUE(plant);
      if( plant ) {
        EXPECT_EQ(generatorPlant.handle(),plant->handle());
      }

      EXPECT_EQ(7u,generatorPlant.demandComponents().size());

      EXPECT_TRUE( chiller.removeFromTertiaryPlantLoop() );
      plant = chiller.tertiaryPlantLoop();
      EXPECT_FALSE(plant);
      EXPECT_EQ(5u,generatorPlant.demandComponents().size());
    }
  }
}
