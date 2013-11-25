/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <model/AirLoopHVAC.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/CoilHeatingWater_Impl.hpp>
#include <model/ScheduleCompact.hpp>

using namespace openstudio;

TEST(CoilHeatingWater,CoilHeatingWater_CoilHeatingWater)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     model::Model m; 

     model::ScheduleCompact s(m);

     model::CoilHeatingWater coil(m,s); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST(CoilHeatingWater,CoilHeatingWater_addToNode)
{
  model::Model m; 

  model::ScheduleCompact s(m);
  
  model::CoilHeatingWater coil(m,s); 

  model::AirLoopHVAC airLoop(m);

  model::Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);

  ASSERT_EQ( (unsigned)3, airLoop.supplyComponents().size() );
}

// Add CoilHeatingWater to AirLoopHVAC and PlantLoop
// Test CoilHeatingWater::remove and make sure that the loops are intact
TEST(CoilHeatingWater,CoilHeatingWater_remove)
{
  model::Model m; 

  model::ScheduleCompact s(m);
  
  model::CoilHeatingWater coil(m,s); 

  model::AirLoopHVAC airLoop(m);

  model::Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);

  coil.remove();

  ASSERT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  ASSERT_TRUE( m.getModelObjects<model::CoilHeatingWater>().empty() );

  model::CoilHeatingWater coil2(m,s); 

  coil2.addToNode(supplyOutletNode);

  model::PlantLoop plant(m);

  plant.addDemandBranchForComponent(coil2);

  ASSERT_EQ( (unsigned)1, m.getModelObjects<model::CoilHeatingWater>().size() );

  ASSERT_EQ( (unsigned)1, plant.demandComponents(model::CoilHeatingWater::iddObjectType()).size() );

  coil2.remove();

  ASSERT_TRUE( m.getModelObjects<model::CoilHeatingWater>().empty() );
}

// Add ONLY to PlantLoop
// Test CoilHeatingWater::remove and make sure that the plant is intact
TEST(CoilHeatingWater,CoilHeatingWater_remove2)
{
  model::Model m; 

  model::ScheduleCompact s(m);
  
  model::CoilHeatingWater coil(m,s); 

  model::PlantLoop plant(m);

  plant.addDemandBranchForComponent(coil);

  ASSERT_EQ( (unsigned)1, m.getModelObjects<model::CoilHeatingWater>().size() );

  ASSERT_EQ( (unsigned)1, plant.demandComponents(model::CoilHeatingWater::iddObjectType()).size() );

  coil.remove();

  ASSERT_TRUE( m.getModelObjects<model::CoilHeatingWater>().empty() );
}

// Add ONLY to PlantLoop
// This time use removeDemandBranchWithComponent
TEST(CoilHeatingWater,CoilHeatingWater_remove3)
{
  model::Model m; 

  model::ScheduleCompact s(m);
  
  model::CoilHeatingWater coil(m,s); 

  model::PlantLoop plant(m);

  plant.addDemandBranchForComponent(coil);

  ASSERT_EQ( (unsigned)1, m.getModelObjects<model::CoilHeatingWater>().size() );

  ASSERT_EQ( (unsigned)1, plant.demandComponents(model::CoilHeatingWater::iddObjectType()).size() );

  plant.removeDemandBranchWithComponent(coil);

  ASSERT_TRUE( plant.demandComponents(model::CoilHeatingWater::iddObjectType()).empty() );

  ASSERT_TRUE( m.getModelObjects<model::CoilHeatingWater>().empty() );
}

// Add CoilHeatingWater to AirLoopHVAC and PlantLoop
// Test PlantLoop::removeDemandBranchWithComponent and make sure that the loops are intact
// Test that the coil is still in the model and hooked up to AirLoopHVAC
TEST(CoilHeatingWater,CoilHeatingWater_remove4)
{
  model::Model m; 

  model::ScheduleCompact s(m);
  
  model::CoilHeatingWater coil(m,s); 

  model::AirLoopHVAC airLoop(m);

  model::Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);

  model::PlantLoop plant(m);

  plant.addDemandBranchForComponent(coil);

  ASSERT_EQ( (unsigned)3, airLoop.supplyComponents().size() );

  ASSERT_EQ( (unsigned)1, m.getModelObjects<model::CoilHeatingWater>().size() );

  ASSERT_EQ( (unsigned)1, plant.demandComponents(model::CoilHeatingWater::iddObjectType()).size() );

  plant.removeDemandBranchWithComponent(coil);

  ASSERT_TRUE( plant.demandComponents(model::CoilHeatingWater::iddObjectType()).empty() );

  ASSERT_EQ( (unsigned)3, airLoop.supplyComponents().size() );

  ASSERT_EQ( (unsigned)1, m.getModelObjects<model::CoilHeatingWater>().size() );
}


