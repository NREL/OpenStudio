/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../GeneratorFuelCellAuxiliaryHeater.hpp"
#include "../GeneratorFuelCellAuxiliaryHeater_Impl.hpp"

#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, FuelCellAuxilliaryHeater) {
  Model model;

  GeneratorFuelCellAuxiliaryHeater auxHeater(model);
  EXPECT_EQ(0, auxHeater.excessAirRatio());
  EXPECT_EQ(0, auxHeater.ancillaryPowerConstantTerm());
  EXPECT_EQ(0, auxHeater.ancillaryPowerLinearTerm());
  EXPECT_EQ(0.5, auxHeater.skinLossUFactorTimesAreaValue());
  EXPECT_EQ("AirInletForFuelCell", auxHeater.skinLossDestination());
  EXPECT_EQ("Watts", auxHeater.heatingCapacityUnits());
  EXPECT_EQ(0, auxHeater.maximumHeatingCapacityinWatts());
  EXPECT_EQ(0, auxHeater.minimumHeatingCapacityinWatts());
  ASSERT_FALSE(auxHeater.zonetoReceiveSkinLosses());
  ThermalZone zone(model);
  ASSERT_TRUE(auxHeater.setZonetoReceiveSkinLosses(zone));
  EXPECT_EQ(zone, auxHeater.zonetoReceiveSkinLosses().get());
  EXPECT_EQ("SurroundingZone", auxHeater.skinLossDestination());
  auxHeater.resetZonetoReceiveSkinLosses();
  ASSERT_FALSE(auxHeater.zonetoReceiveSkinLosses());
  EXPECT_EQ("AirInletForFuelCell", auxHeater.skinLossDestination());
}
