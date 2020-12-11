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

#include "../GeneratorFuelCellWaterSupply.hpp"
#include "../GeneratorFuelCellWaterSupply_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../SiteWaterMainsTemperature.hpp"
#include "../SiteWaterMainsTemperature_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, FuelCellWaterSupply) {
  Model model;

  // check default WaterSupply
  GeneratorFuelCellWaterSupply watersupply(model);
  CurveQuadratic curveQuadratic = watersupply.reformerWaterFlowRateFunctionofFuelRateCurve();
  EXPECT_EQ(0, curveQuadratic.coefficient1Constant());
  EXPECT_EQ(0, curveQuadratic.coefficient2x());
  EXPECT_EQ(0, curveQuadratic.coefficient3xPOW2());
  EXPECT_EQ(-1.0e10, curveQuadratic.minimumValueofx());
  EXPECT_EQ(1.0e10, curveQuadratic.maximumValueofx());
  CurveCubic curveCubic = watersupply.reformerWaterPumpPowerFunctionofFuelRateCurve();
  EXPECT_EQ(0, curveCubic.coefficient1Constant());
  EXPECT_EQ(0, curveCubic.coefficient2x());
  EXPECT_EQ(0, curveCubic.coefficient3xPOW2());
  EXPECT_EQ(0, curveCubic.coefficient4xPOW3());
  EXPECT_EQ(-1.0e10, curveCubic.minimumValueofx());
  EXPECT_EQ(1.0e10, curveCubic.maximumValueofx());
  EXPECT_EQ(0.0, watersupply.pumpHeatLossFactor());
  EXPECT_EQ("TemperatureFromSchedule", watersupply.waterTemperatureModelingMode());
  ASSERT_TRUE(watersupply.waterTemperatureSchedule());
  //change default curves
  CurveQuadratic curveQuadratic2(model);
  CurveCubic curveCubic2(model);
  ASSERT_TRUE(watersupply.setReformerWaterFlowRateFunctionofFuelRateCurve(curveQuadratic2));
  ASSERT_TRUE(watersupply.setReformerWaterPumpPowerFunctionofFuelRateCurve(curveCubic2));
  EXPECT_EQ(curveQuadratic2, watersupply.reformerWaterFlowRateFunctionofFuelRateCurve());
  EXPECT_EQ(curveCubic2, watersupply.reformerWaterPumpPowerFunctionofFuelRateCurve());
  //check node
  ASSERT_FALSE(watersupply.waterTemperatureReferenceNode());
  Node node(model);
  ASSERT_TRUE(watersupply.setWaterTemperatureReferenceNode(node));
  EXPECT_EQ(node, watersupply.waterTemperatureReferenceNode());
  watersupply.resetWaterTemperatureReferenceNode();
  ASSERT_FALSE(watersupply.waterTemperatureReferenceNode());
}

TEST_F(ModelFixture, FuelCellWaterSupply2) {
  Model model;

  CurveQuadratic curveQuadratic(model);
  CurveCubic curveCubic(model);
  ScheduleConstant schedule(model);
  GeneratorFuelCellWaterSupply watersupply(model, curveQuadratic, curveCubic, schedule);
  EXPECT_EQ(curveQuadratic, watersupply.reformerWaterFlowRateFunctionofFuelRateCurve());
  EXPECT_EQ(curveCubic, watersupply.reformerWaterPumpPowerFunctionofFuelRateCurve());
  EXPECT_EQ(schedule.optionalCast<Schedule>(), watersupply.waterTemperatureSchedule());
  EXPECT_EQ("TemperatureFromSchedule", watersupply.waterTemperatureModelingMode());
}

TEST_F(ModelFixture, FuelCellWaterSupply3) {
  Model model;

  CurveQuadratic curveQuadratic(model);
  CurveCubic curveCubic(model);
  Node node(model);
  GeneratorFuelCellWaterSupply watersupply(model, curveQuadratic, curveCubic, node, "TemperatureFromAirNode");
  EXPECT_EQ(curveQuadratic, watersupply.reformerWaterFlowRateFunctionofFuelRateCurve());
  EXPECT_EQ(curveCubic, watersupply.reformerWaterPumpPowerFunctionofFuelRateCurve());
  EXPECT_EQ(node, watersupply.waterTemperatureReferenceNode());
  EXPECT_EQ("TemperatureFromAirNode", watersupply.waterTemperatureModelingMode());
  ASSERT_FALSE(watersupply.waterTemperatureSchedule());
}

TEST_F(ModelFixture, FuelCellWaterSupply4) {
  Model model;
  EXPECT_FALSE(model.getOptionalUniqueModelObject<SiteWaterMainsTemperature>());
  CurveQuadratic curveQuadratic(model);
  CurveCubic curveCubic(model);
  Node node(model);
  GeneratorFuelCellWaterSupply watersupply(model, "MainsWaterTemperature");
  EXPECT_EQ("MainsWaterTemperature", watersupply.waterTemperatureModelingMode());
  ASSERT_FALSE(watersupply.waterTemperatureSchedule());
  EXPECT_TRUE(model.getOptionalUniqueModelObject<SiteWaterMainsTemperature>());
  EXPECT_EQ(9.69, model.getUniqueModelObject<SiteWaterMainsTemperature>().annualAverageOutdoorAirTemperature());
}

TEST_F(ModelFixture, FuelCellWaterSupply5) {
  Model model;
  SiteWaterMainsTemperature wmt = model.getUniqueModelObject<SiteWaterMainsTemperature>();
  wmt.setAnnualAverageOutdoorAirTemperature(10);
  wmt.setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(11);
  CurveQuadratic curveQuadratic(model);
  CurveCubic curveCubic(model);
  Node node(model);
  GeneratorFuelCellWaterSupply watersupply(model, "MainsWaterTemperature");
  EXPECT_EQ("MainsWaterTemperature", watersupply.waterTemperatureModelingMode());
  ASSERT_FALSE(watersupply.waterTemperatureSchedule());
  EXPECT_TRUE(model.getOptionalUniqueModelObject<SiteWaterMainsTemperature>());
  EXPECT_EQ(10.0, model.getUniqueModelObject<SiteWaterMainsTemperature>().annualAverageOutdoorAirTemperature());
  EXPECT_EQ(11.0, model.getUniqueModelObject<SiteWaterMainsTemperature>().maximumDifferenceInMonthlyAverageOutdoorAirTemperatures());
}
