/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ZoneCapacitanceMultiplierResearchSpecial.hpp"
#include "../ZoneCapacitanceMultiplierResearchSpecial_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneCapacitanceMultiplierResearchSpecial_GettersSetters) {

  Model m;
  auto zcap = m.getUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>();

  // Temperature Capacity Multiplier:  Double
  // Check Idd default: 1.0
  EXPECT_TRUE(zcap.isTemperatureCapacityMultiplierDefaulted());
  EXPECT_EQ(1.0, zcap.temperatureCapacityMultiplier());
  EXPECT_TRUE(zcap.setTemperatureCapacityMultiplier(0.5));
  EXPECT_FALSE(zcap.isTemperatureCapacityMultiplierDefaulted());
  EXPECT_EQ(0.5, zcap.temperatureCapacityMultiplier());
  // Test reset
  zcap.resetTemperatureCapacityMultiplier();
  EXPECT_EQ(1.0, zcap.temperatureCapacityMultiplier());
  EXPECT_TRUE(zcap.isTemperatureCapacityMultiplierDefaulted());

  // Humidity Capacity Multiplier:  Double
  // Check Idd default: 1.0
  EXPECT_TRUE(zcap.isHumidityCapacityMultiplierDefaulted());
  EXPECT_EQ(1.0, zcap.humidityCapacityMultiplier());
  EXPECT_TRUE(zcap.setHumidityCapacityMultiplier(0.5));
  EXPECT_FALSE(zcap.isHumidityCapacityMultiplierDefaulted());
  EXPECT_EQ(0.5, zcap.humidityCapacityMultiplier());
  // Test reset
  zcap.resetHumidityCapacityMultiplier();
  EXPECT_EQ(1.0, zcap.humidityCapacityMultiplier());
  EXPECT_TRUE(zcap.isHumidityCapacityMultiplierDefaulted());

  // Carbon Dioxide Capacity Multiplier:  Double
  // Check Idd default: 1.0
  EXPECT_TRUE(zcap.isCarbonDioxideCapacityMultiplierDefaulted());
  EXPECT_EQ(1.0, zcap.carbonDioxideCapacityMultiplier());
  EXPECT_TRUE(zcap.setCarbonDioxideCapacityMultiplier(0.5));
  EXPECT_FALSE(zcap.isCarbonDioxideCapacityMultiplierDefaulted());
  EXPECT_EQ(0.5, zcap.carbonDioxideCapacityMultiplier());
  // Test reset
  zcap.resetCarbonDioxideCapacityMultiplier();
  EXPECT_EQ(1.0, zcap.carbonDioxideCapacityMultiplier());
  EXPECT_TRUE(zcap.isCarbonDioxideCapacityMultiplierDefaulted());
}
