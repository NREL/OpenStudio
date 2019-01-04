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
