/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <model/test/ModelFixture.hpp>

#include <model/EvaporativeCoolerIndirectResearchSpecial.hpp>
#include <model/EvaporativeCoolerIndirectResearchSpecial_Impl.hpp>
#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>

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
  }

  {
    Model m;
    PlantLoop plant(m);

    EvaporativeCoolerIndirectResearchSpecial idec(m);
    Node node = plant.supplyOutletNode();
    EXPECT_FALSE(idec.addToNode(node));
  }
}

