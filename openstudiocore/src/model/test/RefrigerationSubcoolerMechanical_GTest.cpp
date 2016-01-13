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

#include "../RefrigerationSystem.hpp"
#include "../RefrigerationSystem_Impl.hpp"
#include "../RefrigerationSubcoolerMechanical.hpp"
#include "../RefrigerationSubcoolerMechanical_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,RefrigerationSubcoolerMechanical_RefrigerationSubcoolerMechanical)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  
  ASSERT_EXIT ( 
  {  
     Model m; 
     RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,RefrigerationSubcoolerMechanical_iddObjectType)
{    
  Model m; 
  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);

  EXPECT_EQ( refrigerationSubcoolerMechanical.iddObjectType(), IddObjectType::OS_Refrigeration_Subcooler_Mechanical);
}

TEST_F(ModelFixture,RefrigerationSubcoolerMechanical_CapacityProvidingSystem)
{    
  Model m; 
  
  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(m);

  refrigerationSubcoolerMechanical.setCapacityProvidingSystem(refrigerationSystem);

  boost::optional<RefrigerationSystem> refrigerationSystem2 = refrigerationSubcoolerMechanical.capacityProvidingSystem();
  EXPECT_TRUE(refrigerationSystem2 != nullptr);
  EXPECT_EQ( refrigerationSystem.handle(), refrigerationSystem2.get().handle());
}

TEST_F(ModelFixture,RefrigerationSubcoolerMechanical_OutletControlTemperature)
{    
  Model m; 
  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);

  EXPECT_EQ( 10.0, refrigerationSubcoolerMechanical.outletControlTemperature() );  // Brian's value
  
  refrigerationSubcoolerMechanical.setOutletControlTemperature(15.0);
  EXPECT_EQ( 15.0, refrigerationSubcoolerMechanical.outletControlTemperature() );

  refrigerationSubcoolerMechanical.setOutletControlTemperature(-15.0);
  EXPECT_EQ( -15.0, refrigerationSubcoolerMechanical.outletControlTemperature() );
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_Remove)
{
  Model model;
  RefrigerationSubcoolerMechanical testObject = RefrigerationSubcoolerMechanical(model);

  std::vector<RefrigerationSubcoolerMechanical> refrigerationMechanicalSubcoolers = model.getModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(1, refrigerationMechanicalSubcoolers.size());

  testObject.remove();

  refrigerationMechanicalSubcoolers = model.getModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(0, refrigerationMechanicalSubcoolers.size());
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_RemoveRefrigerationSystem)
{
  Model model;
  RefrigerationSubcoolerMechanical testObject = RefrigerationSubcoolerMechanical(model);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(model);

  testObject.setCapacityProvidingSystem(refrigerationSystem);

  ASSERT_TRUE(testObject.capacityProvidingSystem());
  EXPECT_EQ(testObject.capacityProvidingSystem().get().handle(), refrigerationSystem.handle());

  refrigerationSystem.remove();

  ASSERT_FALSE(testObject.capacityProvidingSystem());
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_CloneOneModelWithDefaultData)
{
  Model m; 
  
  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(m);

  refrigerationSubcoolerMechanical.setCapacityProvidingSystem(refrigerationSystem);

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanicalClone = refrigerationSubcoolerMechanical.clone(m).cast<RefrigerationSubcoolerMechanical>();

  EXPECT_NE(refrigerationSubcoolerMechanicalClone.handle(), refrigerationSubcoolerMechanical.handle());

  EXPECT_FALSE(refrigerationSubcoolerMechanicalClone.capacityProvidingSystem());
  EXPECT_EQ(10.0, refrigerationSubcoolerMechanical.outletControlTemperature());
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_CloneOneModelWithCustomData)
{
  Model m; 
  
  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(m);
  refrigerationSubcoolerMechanical.setCapacityProvidingSystem(refrigerationSystem);
  refrigerationSubcoolerMechanical.setOutletControlTemperature(15.0);

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanicalClone = refrigerationSubcoolerMechanical.clone(m).cast<RefrigerationSubcoolerMechanical>();

  EXPECT_NE(refrigerationSubcoolerMechanicalClone.handle(), refrigerationSubcoolerMechanical.handle());

  EXPECT_FALSE(refrigerationSubcoolerMechanicalClone.capacityProvidingSystem());
  EXPECT_EQ(15.0, refrigerationSubcoolerMechanicalClone.outletControlTemperature());
}


TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_CloneTwoModelWithDefaultData)
{
  Model model; 
  
  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(model);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(model);

  refrigerationSubcoolerMechanical.setCapacityProvidingSystem(refrigerationSystem);

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanicalClone = refrigerationSubcoolerMechanical.clone(model).cast<RefrigerationSubcoolerMechanical>();

  Model model2;
  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanicalClone2 = refrigerationSubcoolerMechanical.clone(model2).cast<RefrigerationSubcoolerMechanical>();

  std::vector<RefrigerationSystem> refrigerationSystems = model.getModelObjects<RefrigerationSystem>();
    ASSERT_EQ(1, refrigerationSystems.size());

    refrigerationSystems = model2.getModelObjects<RefrigerationSystem>();
    ASSERT_EQ(0, refrigerationSystems.size());

  EXPECT_NE(refrigerationSubcoolerMechanicalClone.handle(), refrigerationSubcoolerMechanical.handle());
  EXPECT_NE(refrigerationSubcoolerMechanicalClone2.handle(), refrigerationSubcoolerMechanical.handle());
  EXPECT_NE(refrigerationSubcoolerMechanicalClone2.handle(), refrigerationSubcoolerMechanicalClone.handle());
  
  EXPECT_FALSE(refrigerationSubcoolerMechanicalClone.capacityProvidingSystem());
  EXPECT_FALSE(refrigerationSubcoolerMechanicalClone2.capacityProvidingSystem());
  EXPECT_EQ(10.0, refrigerationSubcoolerMechanicalClone2.outletControlTemperature());
}
