/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
  EXPECT_EQ(node, watersupply.waterTemperatureReferenceNode().get());
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
  EXPECT_EQ(schedule, watersupply.waterTemperatureSchedule().get());
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
  EXPECT_EQ(node, watersupply.waterTemperatureReferenceNode().get());
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
  EXPECT_EQ(9.69, model.getUniqueModelObject<SiteWaterMainsTemperature>().annualAverageOutdoorAirTemperature().get());
}

TEST_F(ModelFixture, FuelCellWaterSupply5) {
  Model model;
  auto wmt = model.getUniqueModelObject<SiteWaterMainsTemperature>();
  wmt.setAnnualAverageOutdoorAirTemperature(10);
  wmt.setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(11);
  CurveQuadratic curveQuadratic(model);
  CurveCubic curveCubic(model);
  Node node(model);
  GeneratorFuelCellWaterSupply watersupply(model, "MainsWaterTemperature");
  EXPECT_EQ("MainsWaterTemperature", watersupply.waterTemperatureModelingMode());
  ASSERT_FALSE(watersupply.waterTemperatureSchedule());
  EXPECT_TRUE(model.getOptionalUniqueModelObject<SiteWaterMainsTemperature>());
  EXPECT_EQ(10.0, model.getUniqueModelObject<SiteWaterMainsTemperature>().annualAverageOutdoorAirTemperature().get());
  EXPECT_EQ(11.0, model.getUniqueModelObject<SiteWaterMainsTemperature>().maximumDifferenceInMonthlyAverageOutdoorAirTemperatures().get());
}
