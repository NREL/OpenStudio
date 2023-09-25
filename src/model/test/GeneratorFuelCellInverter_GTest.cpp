/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../GeneratorFuelCellInverter.hpp"
#include "../GeneratorFuelCellInverter_Impl.hpp"

#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, FuelCellInverter) {
  Model model;

  // check default Inverter
  GeneratorFuelCellInverter inverter(model);
  EXPECT_EQ("Constant", inverter.inverterEfficiencyCalculationMode());
  EXPECT_EQ(1.0, inverter.inverterEfficiency());
  boost::optional<CurveQuadratic> electricCurve = inverter.efficiencyFunctionofDCPowerCurve();
  ASSERT_TRUE(electricCurve);
  inverter.setInverterEfficiency(0.5);
  EXPECT_EQ(0.5, inverter.inverterEfficiency());
  inverter.resetInverterEfficiency();
  EXPECT_EQ(1.0, inverter.inverterEfficiency());
}

TEST_F(ModelFixture, FuelCellInverter2) {
  Model model;

  // check default Inverter
  CurveQuadratic electricCurve(model);
  GeneratorFuelCellInverter inverter(model, electricCurve);
  EXPECT_EQ("Quadratic", inverter.inverterEfficiencyCalculationMode());
  EXPECT_EQ(1.0, inverter.inverterEfficiency());
  EXPECT_EQ(electricCurve, inverter.efficiencyFunctionofDCPowerCurve().get());
  EXPECT_EQ(1.0, inverter.inverterEfficiency());
  //check reset calc mode
  inverter.resetInverterEfficiencyCalculationMode();
  EXPECT_EQ("Constant", inverter.inverterEfficiencyCalculationMode());
  EXPECT_EQ(1.0, inverter.inverterEfficiency());
}
