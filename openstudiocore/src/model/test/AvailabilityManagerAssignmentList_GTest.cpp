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
#include "../Model.hpp"

#include "../AvailabilityManagerAssignmentList.hpp"
#include "../AvailabilityManagerAssignmentList_Impl.hpp"

#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Loop.hpp"


// For getting the right AVM from a plantLoop/AirLoop directly (no public one yet)
#include "../AirLoopHVAC_Impl.hpp"
#include "../PlantLoop_Impl.hpp"

#include "../AvailabilityManagerLowTemperatureTurnOn.hpp"
#include "../AvailabilityManagerLowTemperatureTurnOff.hpp"
#include "../AvailabilityManagerHighTemperatureTurnOn.hpp"
#include "../AvailabilityManagerHighTemperatureTurnOff.hpp"
#include "../AvailabilityManagerDifferentialThermostat.hpp"
//#include "../AvailabilityManagerScheduled.hpp"
#include "../AvailabilityManagerNightCycle.hpp"
#include "../AvailabilityManagerHybridVentilation.hpp"
#include "../AvailabilityManagerNightVentilation.hpp"
#include "../AvailabilityManagerOptimumStart.hpp"

using namespace openstudio;
using namespace model;

TEST_F(ModelFixture,AvailabilityManagerAssignmentList)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
    {
      Model m;

      PlantLoop p(m);
      AvailabilityManagerAssignmentList avmList(p);

      exit(0);
    } ,
    ::testing::ExitedWithCode(0), "" );

  ASSERT_EXIT (
    {
      Model m;

      AirLoopHVAC a(m);
      AvailabilityManagerAssignmentList avmList(a);

      // avmList.availabilityManagerScheduled();

      // avmList.availabilityManagerNightCycle();

      exit(0);
    } ,
    ::testing::ExitedWithCode(0), "" );
}

/* Test the class AVMList itself */
TEST_F(ModelFixture, AvailabilityManagerAssignmentList_TestClass)
{
  Model (m);
  PlantLoop p(m);
  p.setName("Plant Loop");

  AvailabilityManagerAssignmentList avmList(p);
  ASSERT_EQ(0u, avmList.availabilityManagers().size());

  // This could change, and we don't really care
  EXPECT_EQ(p.name().get() + " AvailabilityManagerAssignmentList", avmList.name().get());
  EXPECT_TRUE(avmList.setName("My Custom AVMList"));
  EXPECT_EQ("My Custom AVMList", avmList.name().get());

  // Test regular add (at end)
  AvailabilityManagerLowTemperatureTurnOn aLTOn(m);
  ASSERT_TRUE(avmList.addAvailabilityManager(aLTOn));
  ASSERT_EQ(1u, avmList.availabilityManagers().size());
  ASSERT_EQ(aLTOn, avmList.availabilityManagers()[0]);

  AvailabilityManagerHighTemperatureTurnOn aHTOn(m);
  ASSERT_TRUE(avmList.addAvailabilityManager(aHTOn));
  ASSERT_EQ(2u, avmList.availabilityManagers().size());
  ASSERT_EQ(aLTOn, avmList.availabilityManagers()[0]);
  ASSERT_EQ(aHTOn, avmList.availabilityManagers()[1]);

  // Test add at priority
  // Correct priority
  AvailabilityManagerLowTemperatureTurnOff aLTOff(m);
  ASSERT_TRUE(avmList.addAvailabilityManager(aLTOff, 2u));
  ASSERT_EQ(3u, avmList.availabilityManagers().size());
  ASSERT_EQ(aLTOn, avmList.availabilityManagers()[0]);
  ASSERT_EQ(aLTOff, avmList.availabilityManagers()[1]);
  ASSERT_EQ(aHTOn, avmList.availabilityManagers()[2]);

  // Low priority (below 1), reset to 1
  AvailabilityManagerHighTemperatureTurnOff aHTOff(m);
  ASSERT_TRUE(avmList.addAvailabilityManager(aHTOff, 0u));
  ASSERT_EQ(4u, avmList.availabilityManagers().size());
  ASSERT_EQ(aHTOff, avmList.availabilityManagers()[0]);
  ASSERT_EQ(aLTOn, avmList.availabilityManagers()[1]);
  ASSERT_EQ(aLTOff, avmList.availabilityManagers()[2]);
  ASSERT_EQ(aHTOn, avmList.availabilityManagers()[3]);

  // Very high priority, reset to add at last
  AvailabilityManagerDifferentialThermostat aDiffTstat(m);
  ASSERT_TRUE(avmList.addAvailabilityManager(aDiffTstat, 100u));
  ASSERT_EQ(5u, avmList.availabilityManagers().size());
  ASSERT_EQ(aHTOff, avmList.availabilityManagers()[0]);
  ASSERT_EQ(aLTOn, avmList.availabilityManagers()[1]);
  ASSERT_EQ(aLTOff, avmList.availabilityManagers()[2]);
  ASSERT_EQ(aHTOn, avmList.availabilityManagers()[3]);
  ASSERT_EQ(aDiffTstat, avmList.availabilityManagers()[4]);

  // Test priority
  ASSERT_EQ(1u, avmList.availabilityManagerPriority(aHTOff));
  ASSERT_EQ(3u, avmList.availabilityManagerPriority(aLTOff));

  // Test setAvailabilityManagerPriority
  ASSERT_TRUE(avmList.setAvailabilityManagerPriority(aDiffTstat, 2u));
  ASSERT_EQ(2u, avmList.availabilityManagerPriority(aDiffTstat));
  ASSERT_EQ(5u, avmList.availabilityManagers().size());
  ASSERT_EQ(aHTOff, avmList.availabilityManagers()[0]);
  ASSERT_EQ(aDiffTstat, avmList.availabilityManagers()[1]);
  ASSERT_EQ(aLTOn, avmList.availabilityManagers()[2]);
  ASSERT_EQ(aLTOff, avmList.availabilityManagers()[3]);
  ASSERT_EQ(aHTOn, avmList.availabilityManagers()[4]);


  // Test remove AVM
  // by AVM
  ASSERT_TRUE(avmList.removeAvailabilityManager(aLTOff));
  ASSERT_EQ(4u, avmList.availabilityManagers().size());
  ASSERT_EQ(aHTOff, avmList.availabilityManagers()[0]);
  ASSERT_EQ(aDiffTstat, avmList.availabilityManagers()[1]);
  ASSERT_EQ(aLTOn, avmList.availabilityManagers()[2]);
  ASSERT_EQ(aHTOn, avmList.availabilityManagers()[3]);

  // by Priority
  ASSERT_TRUE(avmList.removeAvailabilityManager(2));
  ASSERT_EQ(3u, avmList.availabilityManagers().size());
  ASSERT_EQ(aHTOff, avmList.availabilityManagers()[0]);
  ASSERT_EQ(aLTOn, avmList.availabilityManagers()[1]);
  ASSERT_EQ(aHTOn, avmList.availabilityManagers()[2]);

  // Test reset (void)
  avmList.resetAvailabilityManagers();
  ASSERT_EQ(0u, avmList.availabilityManagers().size());

  // test setAvailabilityManagers
  std::vector<AvailabilityManager> avmVector;
  avmVector.push_back(aDiffTstat);
  avmVector.push_back(aLTOn);
  avmVector.push_back(aHTOff);
  ASSERT_TRUE(avmList.setAvailabilityManagers(avmVector));
  ASSERT_EQ(3u, avmList.availabilityManagers().size());
  ASSERT_EQ(aDiffTstat, avmList.availabilityManagers()[0]);
  ASSERT_EQ(aLTOn, avmList.availabilityManagers()[1]);
  ASSERT_EQ(aHTOff, avmList.availabilityManagers()[2]);

}


// Check that the Loops do have an availabilityManagerAssigmentList
// Check that remove also removes the AVMList
// Check that the Base class Loop does have an access to avmList
TEST_F(ModelFixture, AvailabilityManagerAssignmentList_Loop_Ctor_Dtor)
{
  Model m;
  ASSERT_EQ(0u, m.getModelObjects<AvailabilityManagerAssignmentList>().size());

  // Check that there is one avm created when creating a plantLoop
  PlantLoop p(m);
  ASSERT_EQ(1u, m.getModelObjects<AvailabilityManagerAssignmentList>().size());
  // AvailabilityManagerAssignmentList p_avmList =  m.getModelObjects<AvailabilityManagerAssignmentList>()[0];
  AvailabilityManagerAssignmentList p_avmList = p.getImpl<openstudio::model::detail::PlantLoop_Impl>()->availabilityManagerAssignmentList();
  ASSERT_TRUE(p_avmList.loop());
  ASSERT_TRUE(p_avmList.plantLoop());
  ASSERT_EQ(p.handle(), p_avmList.plantLoop().get().handle());

  // Check that there is one avm created when creating a plantLoop
  AirLoopHVAC a(m);
  ASSERT_EQ(2u, m.getModelObjects<AvailabilityManagerAssignmentList>().size());
  // AvailabilityManagerAssignmentList a_avmList =  m.getModelObjects<AvailabilityManagerAssignmentList>()[1];
  AvailabilityManagerAssignmentList a_avmList = a.getImpl<openstudio::model::detail::AirLoopHVAC_Impl>()->availabilityManagerAssignmentList();
  ASSERT_TRUE(a_avmList.loop());
  ASSERT_TRUE(a_avmList.airLoopHVAC());
  ASSERT_EQ(a.handle(), a_avmList.airLoopHVAC().get().handle());

  // Check that the loop itself has an availabiltyManagerList in its Impl (pure virtual)
  Loop a_loop = a_avmList.loop().get();
  AvailabilityManagerAssignmentList a_loop_avmList = a_loop.getImpl<openstudio::model::detail::Loop_Impl>()->availabilityManagerAssignmentList();
  ASSERT_EQ(a_loop_avmList.handle(), a_avmList.handle());

  // Check that removing a loop, removes the AVMList
  a.remove();
  ASSERT_EQ(1u, m.getModelObjects<AvailabilityManagerAssignmentList>().size());

  // Test the convenience methods
  ASSERT_EQ(0u, p.availabilityManagers().size());
  ASSERT_EQ(0u, p_avmList.availabilityManagers().size());


  // Test regular add (at end)
  AvailabilityManagerLowTemperatureTurnOn aLTOn(m);
  ASSERT_TRUE(p.addAvailabilityManager(aLTOn));
  ASSERT_EQ(1u, p.availabilityManagers().size());
  ASSERT_EQ(1u, p_avmList.availabilityManagers().size());
  ASSERT_EQ(aLTOn, p.availabilityManagers()[0]);
  ASSERT_EQ(aLTOn, p_avmList.availabilityManagers()[0]);

  AvailabilityManagerHighTemperatureTurnOn aHTOn(m);
  ASSERT_TRUE(p.addAvailabilityManager(aHTOn));
  ASSERT_EQ(2u, p.availabilityManagers().size());
  ASSERT_EQ(2u, p_avmList.availabilityManagers().size());
  ASSERT_EQ(aHTOn, p.availabilityManagers()[1]);
  ASSERT_EQ(aHTOn, p_avmList.availabilityManagers()[1]);

  // Test add at priority
  // Correct priority
  AvailabilityManagerLowTemperatureTurnOff aLTOff(m);
  ASSERT_TRUE(p.addAvailabilityManager(aLTOff, 2u));
  ASSERT_EQ(3u, p.availabilityManagers().size());
  ASSERT_EQ(3u, p_avmList.availabilityManagers().size());
  ASSERT_EQ(aLTOn, p.availabilityManagers()[0]);
  ASSERT_EQ(aLTOff, p.availabilityManagers()[1]);
  ASSERT_EQ(aHTOn, p.availabilityManagers()[2]);


  // Test priority
  ASSERT_EQ(1u, p.availabilityManagerPriority(aLTOn));
  ASSERT_EQ(2u, p_avmList.availabilityManagerPriority(aLTOff));
  ASSERT_EQ(3u, p_avmList.availabilityManagerPriority(aHTOn));


  // Test setAvailabilityManagerPriority
  ASSERT_TRUE(p.setAvailabilityManagerPriority(aHTOn, 2u));
  ASSERT_EQ(2u, p.availabilityManagerPriority(aHTOn));
  ASSERT_EQ(3u, p_avmList.availabilityManagers().size());
  ASSERT_EQ(aLTOn, p.availabilityManagers()[0]);
  ASSERT_EQ(aHTOn, p.availabilityManagers()[1]);
  ASSERT_EQ(aLTOff, p.availabilityManagers()[2]);


   // Test remove AVM
  // by AVM
  ASSERT_TRUE(p.removeAvailabilityManager(aHTOn));
  ASSERT_EQ(2u, p.availabilityManagers().size());
  ASSERT_EQ(2u, p_avmList.availabilityManagers().size());
  ASSERT_EQ(aLTOn, p.availabilityManagers()[0]);
  ASSERT_EQ(aLTOff, p.availabilityManagers()[1]);

  // by Priority
  ASSERT_TRUE(p.removeAvailabilityManager(1));
  ASSERT_EQ(1u, p.availabilityManagers().size());
  ASSERT_EQ(1u, p_avmList.availabilityManagers().size());
  ASSERT_EQ(aLTOff, p.availabilityManagers()[0]);

  // Test reset (void)
  p.resetAvailabilityManagers();
  ASSERT_EQ(0u, p.availabilityManagers().size());
  ASSERT_EQ(0u, p_avmList.availabilityManagers().size());


  // test setAvailabilityManagers
  std::vector<AvailabilityManager> avmVector;
  avmVector.push_back(aLTOff);
  avmVector.push_back(aLTOn);
  avmVector.push_back(aHTOn);
  ASSERT_TRUE(p.setAvailabilityManagers(avmVector));
  ASSERT_EQ(3u, p.availabilityManagers().size());
  ASSERT_EQ(aLTOff, p.availabilityManagers()[0]);
  ASSERT_EQ(aLTOn, p.availabilityManagers()[1]);
  ASSERT_EQ(aHTOn, p.availabilityManagers()[2]);

}
