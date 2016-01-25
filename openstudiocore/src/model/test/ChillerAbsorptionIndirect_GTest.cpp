/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
