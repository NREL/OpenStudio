/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../GeneratorFuelCellElectricalStorage.hpp"
#include "../GeneratorFuelCellElectricalStorage_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, FuelCellElectricalStorage) {
  Model model;

  GeneratorFuelCellElectricalStorage elecStorage(model);
  EXPECT_EQ("SimpleEfficiencyWithConstraints", elecStorage.choiceofModel());
  EXPECT_EQ(1.0, elecStorage.nominalChargingEnergeticEfficiency());
  EXPECT_EQ(1.0, elecStorage.nominalDischargingEnergeticEfficiency());
  EXPECT_EQ(0, elecStorage.simpleMaximumCapacity());
  EXPECT_EQ(0, elecStorage.simpleMaximumPowerDraw());
  EXPECT_EQ(0, elecStorage.simpleMaximumPowerStore());
  EXPECT_EQ(0, elecStorage.initialChargeState());
}
