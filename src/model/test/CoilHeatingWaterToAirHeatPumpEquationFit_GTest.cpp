/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingWaterToAirHeatPumpEquationFit_Test) {
  Model model;
  // test constructor.
  CoilHeatingWaterToAirHeatPumpEquationFit coilHeatingWaterToAirHPEquationFit(model);

  // test rated air flow rate getter and setter
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedAirFlowRate(0.1));
  boost::optional<double> testRatedAirFlowRate = coilHeatingWaterToAirHPEquationFit.ratedAirFlowRate();
  EXPECT_EQ(*testRatedAirFlowRate, 0.1);

  // test if rated air flow rate is defaulted to autosize
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.isRatedAirFlowRateDefaulted());
  coilHeatingWaterToAirHPEquationFit.resetRatedAirFlowRate();
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.ratedAirFlowRate());
  coilHeatingWaterToAirHPEquationFit.autosizeRatedAirFlowRate();
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.isRatedAirFlowRateAutosized());

  // test rated water flow rate getter and setter
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedWaterFlowRate(0.1));
  boost::optional<double> testRatedWaterFlowRate = coilHeatingWaterToAirHPEquationFit.ratedWaterFlowRate();
  EXPECT_EQ(*testRatedWaterFlowRate, 0.1);

  // test if rated water flow rate autosize
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.isRatedWaterFlowRateDefaulted());
  coilHeatingWaterToAirHPEquationFit.resetRatedWaterFlowRate();
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.ratedWaterFlowRate());
  coilHeatingWaterToAirHPEquationFit.autosizeRatedWaterFlowRate();
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.isRatedWaterFlowRateAutosized());

  // test rated heating capacity
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedHeatingCapacity(200));
  boost::optional<double> testRatedHeatingCapacity = coilHeatingWaterToAirHPEquationFit.ratedHeatingCapacity();
  EXPECT_EQ(*testRatedHeatingCapacity, 200);

  // test if rated heating capacity autosize
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.isRatedHeatingCapacityDefaulted());
  coilHeatingWaterToAirHPEquationFit.resetRatedHeatingCapacity();
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.ratedHeatingCapacity());
  coilHeatingWaterToAirHPEquationFit.autosizeRatedHeatingCapacity();
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.isRatedHeatingCapacityAutosized());

  // test rated heatingCOP
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedHeatingCoefficientofPerformance(4));
  boost::optional<double> testRatedHeatingCOP = coilHeatingWaterToAirHPEquationFit.ratedHeatingCoefficientofPerformance();
  EXPECT_EQ(*testRatedHeatingCOP, 4);

  // test if rated HeatingCOP default
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.isRatedHeatingCoefficientofPerformanceDefaulted());
  coilHeatingWaterToAirHPEquationFit.resetRatedHeatingCoefficientofPerformance();
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.ratedHeatingCoefficientofPerformance());
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.isRatedHeatingCoefficientofPerformanceDefaulted());
}
