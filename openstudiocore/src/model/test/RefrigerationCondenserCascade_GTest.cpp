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

#include "../RefrigerationCondenserCascade.hpp"
#include "../RefrigerationCondenserCascade_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,RefrigerationCondenserCascade_RefrigerationCondenserCascade)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  
  ASSERT_EXIT ( 
  {  
     Model m; 
     RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}


TEST_F(ModelFixture,RefrigerationCondenserCascade_iddObjectType)
{    
  Model m; 
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  EXPECT_EQ( refrigerationCondenserCascade.iddObjectType(), IddObjectType::OS_Refrigeration_Condenser_Cascade);
}


//  -4.0,                    !- Rated Condensing Temperature {C}
TEST_F(ModelFixture,RefrigerationCondenserCascade_RatedCondensingTemperature)
{    
  Model m; 
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  EXPECT_EQ( -4.0, refrigerationCondenserCascade.ratedCondensingTemperature());  // Brian's value
  
  refrigerationCondenserCascade.setRatedCondensingTemperature(15.0);
  EXPECT_EQ( 15.0, refrigerationCondenserCascade.ratedCondensingTemperature() );
}


//  3.,                      !- Rated Approach Temperature Difference {DeltaC}
TEST_F(ModelFixture,RefrigerationCondenserCascade_RatedApproachTemperatureDifference)
{    
  Model m; 
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);
  
  EXPECT_EQ( 3.0, refrigerationCondenserCascade.ratedApproachTemperatureDifference());  // Brian's value
  
  EXPECT_TRUE(refrigerationCondenserCascade.setRatedApproachTemperatureDifference(15.0));
  EXPECT_EQ( 15.0, refrigerationCondenserCascade.ratedApproachTemperatureDifference() );

  refrigerationCondenserCascade.resetRatedApproachTemperatureDifference();
  EXPECT_EQ( 3.0, refrigerationCondenserCascade.ratedApproachTemperatureDifference() );

  EXPECT_TRUE( refrigerationCondenserCascade.setRatedApproachTemperatureDifference(1.0) );
  EXPECT_FALSE( refrigerationCondenserCascade.setRatedApproachTemperatureDifference(0.0) );
  EXPECT_FALSE( refrigerationCondenserCascade.setRatedApproachTemperatureDifference(-1.0) );
}


//  20000.,                  !- Rated Effective Total Heat Rejection Rate {W}
TEST_F(ModelFixture,RefrigerationCondenserCascade_RatedEffectiveTotalHeatRejectionRate)
{    
  Model m; 
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  EXPECT_EQ( 20000.0, refrigerationCondenserCascade.ratedEffectiveTotalHeatRejectionRate());  // Brian's value
  
  EXPECT_TRUE( refrigerationCondenserCascade.setRatedEffectiveTotalHeatRejectionRate(1500.0) );
  EXPECT_EQ( 1500.0, refrigerationCondenserCascade.ratedEffectiveTotalHeatRejectionRate() );

  EXPECT_TRUE( refrigerationCondenserCascade.setRatedEffectiveTotalHeatRejectionRate(1.0) );
  EXPECT_FALSE( refrigerationCondenserCascade.setRatedEffectiveTotalHeatRejectionRate(0.0) );
  EXPECT_FALSE( refrigerationCondenserCascade.setRatedEffectiveTotalHeatRejectionRate(-1.0) );
}

//  Fixed;                   !- Condensing Temperature Control Type  
TEST_F(ModelFixture,RefrigerationCondenserCascade_CondensingTemperatureControlType)
{    
  Model m; 
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  EXPECT_EQ("Fixed", refrigerationCondenserCascade.condensingTemperatureControlType());

  EXPECT_TRUE(refrigerationCondenserCascade.setCondensingTemperatureControlType("Float"));
  EXPECT_EQ("Float", refrigerationCondenserCascade.condensingTemperatureControlType());

  EXPECT_TRUE(refrigerationCondenserCascade.setCondensingTemperatureControlType("Fixed"));
  EXPECT_EQ("Fixed", refrigerationCondenserCascade.condensingTemperatureControlType());

  refrigerationCondenserCascade.resetCondensingTemperatureControlType();
  EXPECT_EQ("Fixed", refrigerationCondenserCascade.condensingTemperatureControlType());
}

TEST_F(ModelFixture,RefrigerationCondenserCascade_CondenserRefrigerantOperatingChargeInventory)
{
  Model m; 
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);
  
  refrigerationCondenserCascade.setCondenserRefrigerantOperatingChargeInventory(10.0);
  EXPECT_EQ(10.0, refrigerationCondenserCascade.condenserRefrigerantOperatingChargeInventory());

  refrigerationCondenserCascade.setCondenserRefrigerantOperatingChargeInventory(-10.0);
  EXPECT_EQ(-10.0, refrigerationCondenserCascade.condenserRefrigerantOperatingChargeInventory());
}

TEST_F(ModelFixture,RefrigerationCondenserCascade_CondensateReceiverRefrigerantInventory)
{
  Model m; 
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  refrigerationCondenserCascade.setCondensateReceiverRefrigerantInventory(10.0);
  EXPECT_EQ(10.0, refrigerationCondenserCascade.condensateReceiverRefrigerantInventory());

  refrigerationCondenserCascade.setCondensateReceiverRefrigerantInventory(-10.0);
  EXPECT_EQ(-10.0, refrigerationCondenserCascade.condensateReceiverRefrigerantInventory());
}

TEST_F(ModelFixture,RefrigerationCondenserCascade_CondensatePipingRefrigerantInventory)
{
  Model m; 
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  refrigerationCondenserCascade.setCondensatePipingRefrigerantInventory(10.0);
  EXPECT_EQ(10.0, refrigerationCondenserCascade.condensatePipingRefrigerantInventory());

  refrigerationCondenserCascade.setCondensatePipingRefrigerantInventory(-10.0);
  EXPECT_EQ(-10.0, refrigerationCondenserCascade.condensatePipingRefrigerantInventory());
}

TEST_F(ModelFixture, RefrigerationCondenserCascade_Remove)
{
  Model model;
  RefrigerationCondenserCascade testObject = RefrigerationCondenserCascade(model);

  std::vector<RefrigerationCondenserCascade> refrigerationCascadeCondensers = model.getModelObjects<RefrigerationCondenserCascade>();
  EXPECT_EQ(1, refrigerationCascadeCondensers.size());

  testObject.remove();

  refrigerationCascadeCondensers = model.getModelObjects<RefrigerationCondenserCascade>();
  EXPECT_EQ(0, refrigerationCascadeCondensers.size());
}

TEST_F(ModelFixture, RefrigerationCondenserCascade_CloneOneModelWithDefaultData)
{
  Model m; 
  
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  RefrigerationCondenserCascade refrigerationCondenserCascadeClone = refrigerationCondenserCascade.clone(m).cast<RefrigerationCondenserCascade>();

  EXPECT_NE(refrigerationCondenserCascadeClone.handle(), refrigerationCondenserCascade.handle());
  
  EXPECT_EQ(-4.0, refrigerationCondenserCascadeClone.ratedCondensingTemperature());
  EXPECT_EQ(3.0, refrigerationCondenserCascadeClone.ratedApproachTemperatureDifference());
  EXPECT_EQ(20000.0, refrigerationCondenserCascadeClone.ratedEffectiveTotalHeatRejectionRate());
  EXPECT_EQ("Fixed", refrigerationCondenserCascadeClone.condensingTemperatureControlType());
}

TEST_F(ModelFixture, RefrigerationCondenserCascade_CloneOneModelWithCustomData)
{
  Model m; 
  
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);
  refrigerationCondenserCascade.setRatedCondensingTemperature(-3.0);
  refrigerationCondenserCascade.setRatedApproachTemperatureDifference(4.0);
  refrigerationCondenserCascade.setRatedEffectiveTotalHeatRejectionRate(21000.0);
  refrigerationCondenserCascade.setCondensingTemperatureControlType("Float");

  RefrigerationCondenserCascade refrigerationCondenserCascadeClone = refrigerationCondenserCascade.clone(m).cast<RefrigerationCondenserCascade>();

  EXPECT_NE(refrigerationCondenserCascadeClone.handle(), refrigerationCondenserCascade.handle());

  EXPECT_EQ(-3.0, refrigerationCondenserCascadeClone.ratedCondensingTemperature());
  EXPECT_EQ(4.0, refrigerationCondenserCascadeClone.ratedApproachTemperatureDifference());
  EXPECT_EQ(21000.0, refrigerationCondenserCascadeClone.ratedEffectiveTotalHeatRejectionRate());
  EXPECT_EQ("Float", refrigerationCondenserCascadeClone.condensingTemperatureControlType());
}
