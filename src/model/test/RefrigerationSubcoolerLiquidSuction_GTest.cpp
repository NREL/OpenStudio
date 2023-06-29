/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../RefrigerationSubcoolerLiquidSuction.hpp"
#include "../RefrigerationSubcoolerLiquidSuction_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationSubcoolerLiquidSuction_RefrigerationSubcoolerLiquidSuction) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, RefrigerationSubcoolerLiquidSuction_iddObjectType) {
  Model m;
  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);

  EXPECT_EQ(refrigerationSubcoolerLiquidSuction.iddObjectType(), IddObjectType::OS_Refrigeration_Subcooler_LiquidSuction);
}

TEST_F(ModelFixture, RefrigerationSubcoolerLiquidSuction_LiquidSuctionDesignSubcoolingTemperatureDifference) {
  Model m;
  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);

  EXPECT_EQ(6.0, refrigerationSubcoolerLiquidSuction.liquidSuctionDesignSubcoolingTemperatureDifference().get());  // Brian's value

  refrigerationSubcoolerLiquidSuction.setLiquidSuctionDesignSubcoolingTemperatureDifference(15.0);
  EXPECT_EQ(15.0, refrigerationSubcoolerLiquidSuction.liquidSuctionDesignSubcoolingTemperatureDifference().get());

  refrigerationSubcoolerLiquidSuction.setLiquidSuctionDesignSubcoolingTemperatureDifference(-15.0);
  EXPECT_EQ(-15.0, refrigerationSubcoolerLiquidSuction.liquidSuctionDesignSubcoolingTemperatureDifference().get());
}

TEST_F(ModelFixture, RefrigerationSubcoolerLiquidSuction_DesignLiquidInletTemperature) {
  Model m;
  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);

  EXPECT_EQ(16.0, refrigerationSubcoolerLiquidSuction.designLiquidInletTemperature().get());  // Brian's value

  refrigerationSubcoolerLiquidSuction.setDesignLiquidInletTemperature(15.0);
  EXPECT_EQ(15.0, refrigerationSubcoolerLiquidSuction.designLiquidInletTemperature().get());

  refrigerationSubcoolerLiquidSuction.setDesignLiquidInletTemperature(-15.0);
  EXPECT_EQ(-15.0, refrigerationSubcoolerLiquidSuction.designLiquidInletTemperature().get());
}

TEST_F(ModelFixture, RefrigerationSubcoolerLiquidSuction_DesignVaporInletTemperature) {
  Model m;
  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);

  EXPECT_EQ(0.0, refrigerationSubcoolerLiquidSuction.designVaporInletTemperature().get());  // Brian's value

  refrigerationSubcoolerLiquidSuction.setDesignVaporInletTemperature(15.0);
  EXPECT_EQ(15.0, refrigerationSubcoolerLiquidSuction.designVaporInletTemperature().get());

  refrigerationSubcoolerLiquidSuction.setDesignVaporInletTemperature(-15.0);
  EXPECT_EQ(-15.0, refrigerationSubcoolerLiquidSuction.designVaporInletTemperature().get());
}

TEST_F(ModelFixture, RefrigerationSubcoolerLiquidSuction_Remove) {
  Model model;
  RefrigerationSubcoolerLiquidSuction testObject = RefrigerationSubcoolerLiquidSuction(model);

  std::vector<RefrigerationSubcoolerLiquidSuction> refrigerationLiquidSuctionSubcoolers =
    model.getConcreteModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(1, refrigerationLiquidSuctionSubcoolers.size());

  testObject.remove();

  refrigerationLiquidSuctionSubcoolers = model.getConcreteModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(0, refrigerationLiquidSuctionSubcoolers.size());
}

TEST_F(ModelFixture, RefrigerationSubcoolerLiquidSuction_CloneOneModelWithDefaultData) {
  Model m;

  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);

  auto refrigerationSubcoolerLiquidSuctionClone = refrigerationSubcoolerLiquidSuction.clone(m).cast<RefrigerationSubcoolerLiquidSuction>();

  EXPECT_NE(refrigerationSubcoolerLiquidSuctionClone.handle(), refrigerationSubcoolerLiquidSuction.handle());

  EXPECT_EQ(6.0, refrigerationSubcoolerLiquidSuctionClone.liquidSuctionDesignSubcoolingTemperatureDifference().get());
  EXPECT_EQ(16.0, refrigerationSubcoolerLiquidSuctionClone.designLiquidInletTemperature().get());
  EXPECT_EQ(0.0, refrigerationSubcoolerLiquidSuctionClone.designVaporInletTemperature().get());
}

TEST_F(ModelFixture, RefrigerationSubcoolerLiquidSuction_CloneOneModelWithCustomData) {
  Model m;

  RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = RefrigerationSubcoolerLiquidSuction(m);
  refrigerationSubcoolerLiquidSuction.setLiquidSuctionDesignSubcoolingTemperatureDifference(10.0);
  refrigerationSubcoolerLiquidSuction.setDesignLiquidInletTemperature(20.0);
  refrigerationSubcoolerLiquidSuction.setDesignVaporInletTemperature(10.0);

  auto refrigerationSubcoolerLiquidSuctionClone = refrigerationSubcoolerLiquidSuction.clone(m).cast<RefrigerationSubcoolerLiquidSuction>();

  EXPECT_NE(refrigerationSubcoolerLiquidSuctionClone.handle(), refrigerationSubcoolerLiquidSuction.handle());

  EXPECT_EQ(10.0, refrigerationSubcoolerLiquidSuctionClone.liquidSuctionDesignSubcoolingTemperatureDifference().get());
  EXPECT_EQ(20.0, refrigerationSubcoolerLiquidSuctionClone.designLiquidInletTemperature().get());
  EXPECT_EQ(10.0, refrigerationSubcoolerLiquidSuctionClone.designVaporInletTemperature().get());
}
