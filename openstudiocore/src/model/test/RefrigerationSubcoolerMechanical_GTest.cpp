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

#include <model/test/ModelFixture.hpp>

#include <model/RefrigerationSystem.hpp>
#include <model/RefrigerationSubcoolerMechanical.hpp>
#include <model/RefrigerationSubcoolerMechanical_Impl.hpp>

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
	EXPECT_TRUE(refrigerationSystem2 != NULL);
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

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_CloneOneModelWithDefaultData)
{
	Model m; 
	
	RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);
	RefrigerationSystem refrigerationSystem = RefrigerationSystem(m);

	refrigerationSubcoolerMechanical.setCapacityProvidingSystem(refrigerationSystem);

	RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanicalClone = refrigerationSubcoolerMechanical.clone(m).cast<RefrigerationSubcoolerMechanical>();

	EXPECT_NE(refrigerationSubcoolerMechanicalClone.handle(), refrigerationSubcoolerMechanical.handle());
	
	EXPECT_EQ(refrigerationSubcoolerMechanicalClone.capacityProvidingSystem().get().handle(), refrigerationSubcoolerMechanical.capacityProvidingSystem().get().handle());
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

	// capacityProvidingSystem handle is the same
	EXPECT_EQ(refrigerationSubcoolerMechanicalClone.capacityProvidingSystem().get().handle(), refrigerationSubcoolerMechanical.capacityProvidingSystem().get().handle());
	EXPECT_EQ(15.0, refrigerationSubcoolerMechanicalClone.outletControlTemperature());
}
