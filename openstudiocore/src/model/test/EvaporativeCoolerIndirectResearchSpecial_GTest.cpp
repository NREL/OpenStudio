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
#include "../EvaporativeCoolerIndirectResearchSpecial.hpp"
#include "../EvaporativeCoolerIndirectResearchSpecial_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture,EvaporativeCoolerIndirectResearchSpecial)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  
  ASSERT_EXIT ( 
  {  
    Model m; 
    EvaporativeCoolerIndirectResearchSpecial idec(m);
    
    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );

  {
    Model m;
    AirLoopHVAC airLoopHVAC(m); 
    EvaporativeCoolerIndirectResearchSpecial idec(m);

    Node supplyOutletNode = airLoopHVAC.supplyOutletNode();
    EXPECT_TRUE(idec.addToNode(supplyOutletNode));

    EXPECT_EQ(3u,airLoopHVAC.supplyComponents().size());

    Node demandInletNode = airLoopHVAC.demandInletNode();
    EXPECT_FALSE(idec.addToNode(demandInletNode));

    EXPECT_EQ(3u,airLoopHVAC.supplyComponents().size());

    Node node(m);
    EXPECT_TRUE(idec.getImpl<model::detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->setReliefAirInletNode(node));

    ASSERT_TRUE(idec.getImpl<model::detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->reliefAirInletNode());
    EXPECT_EQ(node,idec.getImpl<model::detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->reliefAirInletNode().get());
  }

  {
    Model m;
    PlantLoop plant(m);

    EvaporativeCoolerIndirectResearchSpecial idec(m);
    Node node = plant.supplyOutletNode();
    EXPECT_FALSE(idec.addToNode(node));
  }
}

