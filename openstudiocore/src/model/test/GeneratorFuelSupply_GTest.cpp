/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include <model/test/ModelFixture.hpp>

#include "../GeneratorFuelSupply.hpp"
#include "../GeneratorFuelSupply_Impl.hpp"

#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeRegistry.hpp"


using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, FuelCellFuelSupply) {
  Model model;

  // check default GeneratorFuelSupply
  GeneratorFuelSupply fuelsupply(model);
  EXPECT_EQ("Scheduled", fuelsupply.fuelTemperatureModelingMode());
  boost::optional<Schedule> schedule = fuelsupply.fuelTemperatureSchedule();
  EXPECT_EQ(20, schedule.get().cast<ScheduleConstant>().value());

  CurveCubic curveCubic = fuelsupply.compressorPowerMultiplierFunctionofFuelRateCurve();
  EXPECT_EQ(0, curveCubic.coefficient1Constant());
  EXPECT_EQ(0, curveCubic.coefficient2x());
  EXPECT_EQ(0, curveCubic.coefficient3xPOW2());
  EXPECT_EQ(0, curveCubic.coefficient4xPOW3());
  EXPECT_EQ(-1.0e10, curveCubic.minimumValueofx());
  EXPECT_EQ(1.0e10, curveCubic.maximumValueofx());
  
  EXPECT_EQ(1, fuelsupply.compressorHeatLossFactor());
  EXPECT_EQ("LiquidGeneric", fuelsupply.fuelType());
  fuelsupply.resetFuelType();
  EXPECT_EQ("LiquidGeneric", fuelsupply.fuelType());
  EXPECT_EQ(43100, fuelsupply.liquidGenericFuelLowerHeatingValue().get());
  fuelsupply.resetLiquidGenericFuelLowerHeatingValue();
  EXPECT_FALSE(fuelsupply.liquidGenericFuelLowerHeatingValue());
  EXPECT_EQ(46200, fuelsupply.liquidGenericFuelHigherHeatingValue().get());
  fuelsupply.resetLiquidGenericFuelHigherHeatingValue();
  EXPECT_FALSE(fuelsupply.liquidGenericFuelHigherHeatingValue());
  EXPECT_EQ(1, fuelsupply.liquidGenericFuelMolecularWeight().get());
  EXPECT_EQ(0, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  //should fail since name is wrong
  ASSERT_FALSE(fuelsupply.addConstituent("MadeUp", 0.0092));
  EXPECT_EQ(0, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("CarbonDioxide", 0.0003));
  EXPECT_EQ(1, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Nitrogen", 0.7728));
  EXPECT_EQ(2, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Oxygen", 0.2073));
  EXPECT_EQ(3, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Water", 0.0104));
  EXPECT_EQ(4, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Argon", 0.0092));
  EXPECT_EQ(5, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Hydrogen", 0.00));
  EXPECT_EQ(6, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Methane", 0.00));
  EXPECT_EQ(7, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Ethane", 0.00));
  EXPECT_EQ(8, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Propane", 0.00));
  EXPECT_EQ(9, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Butane", 0.00));
  EXPECT_EQ(10, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Pentane", 0.00));
  EXPECT_EQ(11, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Hexane", 0.00));
  EXPECT_EQ(12, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  //should fail since only 12 allowed
  ASSERT_FALSE(fuelsupply.addConstituent("Methanol", 0.00));
  fuelsupply.removeConstituent(11);
  EXPECT_EQ(11, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  ASSERT_TRUE(fuelsupply.addConstituent("Methanol", 0.00));
  EXPECT_EQ(12, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  fuelsupply.removeConstituent(11);
  ASSERT_TRUE(fuelsupply.addConstituent("Ethanol", 0.00));
  EXPECT_EQ(12, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  fuelsupply.removeAllConstituents();
  EXPECT_EQ(0, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());
  fuelsupply.removeConstituent(0);
  EXPECT_EQ(0, fuelsupply.numberofConstituentsinGaseousConstituentFuelSupply().get());

  CurveCubic curveCubic2(model);
  fuelsupply.setCompressorPowerMultiplierFunctionofFuelRateCurve(curveCubic2);
  EXPECT_EQ(curveCubic2, fuelsupply.compressorPowerMultiplierFunctionofFuelRateCurve());
}


