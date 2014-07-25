/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "../CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Splitter.hpp"
#include "../Splitter_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,CoilHeatingWaterToAirHeatPumpEquationFit_Test) {
  Model model;
  // test constructor.
  CoilHeatingWaterToAirHeatPumpEquationFit coilHeatingWaterToAirHPEquationFit(model);

  // test rated air flow rate getter and setter
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedAirFlowRate(0.1));
  boost::optional <double> testRatedAirFlowRate = coilHeatingWaterToAirHPEquationFit.ratedAirFlowRate();
  EXPECT_EQ(*testRatedAirFlowRate, 0.1);

  // test if rated air flow rate is defaulted to autosize
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.isRatedAirFlowRateDefaulted());
  coilHeatingWaterToAirHPEquationFit.resetRatedAirFlowRate();
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.ratedAirFlowRate());
  coilHeatingWaterToAirHPEquationFit.autosizeRatedAirFlowRate();
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.isRatedAirFlowRateAutosized());


 // test rated water flow rate getter and setter
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedWaterFlowRate(0.1));
  boost::optional <double> testRatedWaterFlowRate = coilHeatingWaterToAirHPEquationFit.ratedWaterFlowRate();
  EXPECT_EQ(*testRatedWaterFlowRate, 0.1);

  // test if rated water flow rate autosize
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.isRatedWaterFlowRateDefaulted());
  coilHeatingWaterToAirHPEquationFit.resetRatedWaterFlowRate();
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.ratedWaterFlowRate());
  coilHeatingWaterToAirHPEquationFit.autosizeRatedWaterFlowRate();
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.isRatedWaterFlowRateAutosized());

 // test rated heating capacity
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedHeatingCapacity(200));
  boost::optional <double> testRatedHeatingCapacity = coilHeatingWaterToAirHPEquationFit.ratedHeatingCapacity();
  EXPECT_EQ(*testRatedHeatingCapacity, 200);

  // test if rated heating capacity autosize
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.isRatedHeatingCapacityDefaulted());
  coilHeatingWaterToAirHPEquationFit.resetRatedHeatingCapacity();
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.ratedHeatingCapacity());
  coilHeatingWaterToAirHPEquationFit.autosizeRatedHeatingCapacity();
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.isRatedHeatingCapacityAutosized());


 // test rated heatingCOP
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedHeatingCoefficientofPerformance(4));
  boost::optional <double> testRatedHeatingCOP = coilHeatingWaterToAirHPEquationFit.ratedHeatingCoefficientofPerformance();
  EXPECT_EQ(*testRatedHeatingCOP, 4);

  // test if rated HeatingCOP default
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.isRatedHeatingCoefficientofPerformanceDefaulted());
  coilHeatingWaterToAirHPEquationFit.resetRatedHeatingCoefficientofPerformance();
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.ratedHeatingCoefficientofPerformance());
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.isRatedHeatingCoefficientofPerformanceDefaulted());

}




