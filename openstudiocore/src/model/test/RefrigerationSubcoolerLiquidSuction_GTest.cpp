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

#include "../RefrigerationSubcoolerLiquidSuction.hpp"
#include "../RefrigerationSubcoolerLiquidSuction_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,RefrigerationSubcoolerLiquidSuction_RefrigerationSubcoolerLiquidSuction)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  
  ASSERT_EXIT ( 
  {  
     Model m; 
     RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}


TEST_F(ModelFixture,RefrigerationSubcoolerLiquidSuction_iddObjectType)
{    
  Model m; 
  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);

  EXPECT_EQ( refrigerationSubcoolerLiquidSuction.iddObjectType(), IddObjectType::OS_Refrigeration_Subcooler_LiquidSuction);
}


TEST_F(ModelFixture,RefrigerationSubcoolerLiquidSuction_LiquidSuctionDesignSubcoolingTemperatureDifference)
{    
  Model m; 
  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);
  
  EXPECT_EQ( 6.0, refrigerationSubcoolerLiquidSuction.liquidSuctionDesignSubcoolingTemperatureDifference() );  // Brian's value
  
  refrigerationSubcoolerLiquidSuction.setLiquidSuctionDesignSubcoolingTemperatureDifference(15.0);
  EXPECT_EQ( 15.0, refrigerationSubcoolerLiquidSuction.liquidSuctionDesignSubcoolingTemperatureDifference() );

  refrigerationSubcoolerLiquidSuction.setLiquidSuctionDesignSubcoolingTemperatureDifference(-15.0);
  EXPECT_EQ( -15.0, refrigerationSubcoolerLiquidSuction.liquidSuctionDesignSubcoolingTemperatureDifference() );
}


TEST_F(ModelFixture,RefrigerationSubcoolerLiquidSuction_DesignLiquidInletTemperature)
{    
  Model m; 
  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);

  EXPECT_EQ( 16.0, refrigerationSubcoolerLiquidSuction.designLiquidInletTemperature() );  // Brian's value
  
  refrigerationSubcoolerLiquidSuction.setDesignLiquidInletTemperature(15.0);
  EXPECT_EQ( 15.0, refrigerationSubcoolerLiquidSuction.designLiquidInletTemperature() );

  refrigerationSubcoolerLiquidSuction.setDesignLiquidInletTemperature(-15.0);
  EXPECT_EQ( -15.0, refrigerationSubcoolerLiquidSuction.designLiquidInletTemperature() );
}

TEST_F(ModelFixture,RefrigerationSubcoolerLiquidSuction_DesignVaporInletTemperature)
{    
  Model m; 
  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);

  EXPECT_EQ( 0.0, refrigerationSubcoolerLiquidSuction.designVaporInletTemperature() );  // Brian's value
  
  refrigerationSubcoolerLiquidSuction.setDesignVaporInletTemperature(15.0);
  EXPECT_EQ( 15.0, refrigerationSubcoolerLiquidSuction.designVaporInletTemperature() );

  refrigerationSubcoolerLiquidSuction.setDesignVaporInletTemperature(-15.0);
  EXPECT_EQ( -15.0, refrigerationSubcoolerLiquidSuction.designVaporInletTemperature() );
}

TEST_F(ModelFixture, RefrigerationSubcoolerLiquidSuction_Remove)
{
  Model model;
  RefrigerationSubcoolerLiquidSuction testObject = RefrigerationSubcoolerLiquidSuction(model);

  std::vector<RefrigerationSubcoolerLiquidSuction> refrigerationLiquidSuctionSubcoolers = model.getModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(1, refrigerationLiquidSuctionSubcoolers.size());

  testObject.remove();

  refrigerationLiquidSuctionSubcoolers = model.getModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(0, refrigerationLiquidSuctionSubcoolers.size());
}

TEST_F(ModelFixture, RefrigerationSubcoolerLiquidSuction_CloneOneModelWithDefaultData)
{
  Model m; 
  
  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);

  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuctionClone = refrigerationSubcoolerLiquidSuction.clone(m).cast<RefrigerationSubcoolerLiquidSuction>();

  EXPECT_NE(refrigerationSubcoolerLiquidSuctionClone.handle(), refrigerationSubcoolerLiquidSuction.handle());
  
  EXPECT_EQ(6.0, refrigerationSubcoolerLiquidSuctionClone.liquidSuctionDesignSubcoolingTemperatureDifference());
  EXPECT_EQ(16.0, refrigerationSubcoolerLiquidSuctionClone.designLiquidInletTemperature());
  EXPECT_EQ(0.0, refrigerationSubcoolerLiquidSuctionClone.designVaporInletTemperature());
}

TEST_F(ModelFixture, RefrigerationSubcoolerLiquidSuction_CloneOneModelWithCustomData)
{
  Model m; 
  
  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);
  refrigerationSubcoolerLiquidSuction.setLiquidSuctionDesignSubcoolingTemperatureDifference(10.0);
  refrigerationSubcoolerLiquidSuction.setDesignLiquidInletTemperature(20.0);
  refrigerationSubcoolerLiquidSuction.setDesignVaporInletTemperature(10.0);

  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuctionClone = refrigerationSubcoolerLiquidSuction.clone(m).cast<RefrigerationSubcoolerLiquidSuction>();

  EXPECT_NE(refrigerationSubcoolerLiquidSuctionClone.handle(), refrigerationSubcoolerLiquidSuction.handle());
  
  EXPECT_EQ(10.0, refrigerationSubcoolerLiquidSuctionClone.liquidSuctionDesignSubcoolingTemperatureDifference());
  EXPECT_EQ(20.0, refrigerationSubcoolerLiquidSuctionClone.designLiquidInletTemperature());
  EXPECT_EQ(10.0, refrigerationSubcoolerLiquidSuctionClone.designVaporInletTemperature());
}
