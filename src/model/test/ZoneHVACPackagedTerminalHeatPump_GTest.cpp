/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"

#include "../ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../ZoneHVACPackagedTerminalHeatPump_Impl.hpp"

#include "../AirLoopHVAC.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingElectric_Impl.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../CoilCoolingDXVariableSpeed.hpp"
#include "../CoilCoolingDXVariableSpeed_Impl.hpp"
#include "../CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "../CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../CoilHeatingDXSingleSpeed_Impl.hpp"
#include "../CoilHeatingDXVariableSpeed.hpp"
#include "../CoilHeatingDXVariableSpeed_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../FanOnOff.hpp"
#include "../FanSystemModel.hpp"
#include "../ThermalZone.hpp"
#include "../PlantLoop.hpp"
#include "../BoilerHotWater.hpp"

#include <utilities/idd/OS_Curve_Biquadratic_FieldEnums.hxx>

using namespace openstudio;

using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACPackagedTerminalHeatPump_ZoneHVACPackagedTerminalHeatPump) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      model::Model m;

      model::ScheduleCompact availabilitySchedule(m);

      model::FanConstantVolume fan(m, availabilitySchedule);

      model::CoilHeatingElectric supplementalHeatingCoil(m, availabilitySchedule);

      model::CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(m);
      coolingCurveFofTemp.setCoefficient1Constant(0.42415);
      coolingCurveFofTemp.setCoefficient2x(0.04426);
      coolingCurveFofTemp.setCoefficient3xPOW2(-0.00042);
      coolingCurveFofTemp.setCoefficient4y(0.00333);
      coolingCurveFofTemp.setCoefficient5yPOW2(-0.00008);
      coolingCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
      coolingCurveFofTemp.setMinimumValueofx(17.0);
      coolingCurveFofTemp.setMaximumValueofx(22.0);
      coolingCurveFofTemp.setMinimumValueofy(13.0);
      coolingCurveFofTemp.setMaximumValueofy(46.0);

      CurveQuadratic coolingCurveFofFlow = CurveQuadratic(m);
      coolingCurveFofFlow.setCoefficient1Constant(0.77136);
      coolingCurveFofFlow.setCoefficient2x(0.34053);
      coolingCurveFofFlow.setCoefficient3xPOW2(-0.11088);
      coolingCurveFofFlow.setMinimumValueofx(0.75918);
      coolingCurveFofFlow.setMaximumValueofx(1.13877);

      CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(m);
      energyInputRatioFofTemp.setCoefficient1Constant(1.23649);
      energyInputRatioFofTemp.setCoefficient2x(-0.02431);
      energyInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
      energyInputRatioFofTemp.setCoefficient4y(-0.01434);
      energyInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
      energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
      energyInputRatioFofTemp.setMinimumValueofx(17.0);
      energyInputRatioFofTemp.setMaximumValueofx(22.0);
      energyInputRatioFofTemp.setMinimumValueofy(13.0);
      energyInputRatioFofTemp.setMaximumValueofy(46.0);

      CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(m);
      energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
      energyInputRatioFofFlow.setCoefficient2x(-0.32953);
      energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
      energyInputRatioFofFlow.setMinimumValueofx(0.75918);
      energyInputRatioFofFlow.setMaximumValueofx(1.13877);

      CurveQuadratic partLoadFraction = CurveQuadratic(m);
      partLoadFraction.setCoefficient1Constant(0.77100);
      partLoadFraction.setCoefficient2x(0.22900);
      partLoadFraction.setCoefficient3xPOW2(0.0);
      partLoadFraction.setMinimumValueofx(0.0);
      partLoadFraction.setMaximumValueofx(1.0);

      CoilCoolingDXSingleSpeed coolingCoil = CoilCoolingDXSingleSpeed(m, availabilitySchedule, coolingCurveFofTemp, coolingCurveFofFlow,
                                                                      energyInputRatioFofTemp, energyInputRatioFofFlow, partLoadFraction);

      CurveBiquadratic totalHeatingCapacityFunctionofTemperatureCurve(m);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(0.758746);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient2x(0.0);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.0);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient4y(0.027626);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient5yPOW2(0.000148716);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient6xTIMESY(0.0);

      CurveQuadratic totalHeatingCapacityFunctionofFlowFractionCurve(m);
      totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.84);
      totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.16);
      totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
      totalHeatingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
      totalHeatingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

      CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(m);
      energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(1.19248);
      energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(-0.0300438);
      energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0.00103745);
      energyInputRatioFunctionofTemperatureCurve.setCoefficient4y(0.0);
      energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
      energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(20.0);

      CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(m);
      energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.3824);
      energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.4336);
      energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0512);
      energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.0);
      energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.0);

      CurveQuadratic partLoadFractionCorrelationCurve(m);
      partLoadFractionCorrelationCurve.setCoefficient1Constant(0.85);
      partLoadFractionCorrelationCurve.setCoefficient2x(0.15);
      partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
      partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
      partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);

      CoilHeatingDXSingleSpeed heatingCoil(m, availabilitySchedule, totalHeatingCapacityFunctionofTemperatureCurve,
                                           totalHeatingCapacityFunctionofFlowFractionCurve, energyInputRatioFunctionofTemperatureCurve,
                                           energyInputRatioFunctionofFlowFractionCurve, partLoadFractionCorrelationCurve);

      model::ZoneHVACPackagedTerminalHeatPump pthp(m, availabilitySchedule, fan, heatingCoil, coolingCoil, supplementalHeatingCoil);

      pthp.availabilitySchedule();

      pthp.supplyAirFan();

      pthp.heatingCoil();

      pthp.coolingCoil();

      pthp.supplementalHeatingCoil();

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  model::Model m;
  model::CoilHeatingDXVariableSpeed heatingCoil(m);
  model::CoilCoolingDXVariableSpeed coolingCoil(m);
  model::ScheduleCompact availabilitySchedule(m);
  model::FanConstantVolume fan(m, availabilitySchedule);
  model::CoilHeatingElectric supplementalHeatingCoil(m, availabilitySchedule);

  model::ZoneHVACPackagedTerminalHeatPump pthp(m, availabilitySchedule, fan, heatingCoil, coolingCoil, supplementalHeatingCoil);

  EXPECT_TRUE(pthp.noLoadSupplyAirFlowRateControlSetToLowSpeed());
  EXPECT_TRUE(pthp.setNoLoadSupplyAirFlowRateControlSetToLowSpeed(false));
  EXPECT_FALSE(pthp.noLoadSupplyAirFlowRateControlSetToLowSpeed());
}

TEST_F(ModelFixture, ZoneHVACPackagedTerminalHeatPump_Clone) {
  model::Model m;

  model::ScheduleCompact availabilitySchedule(m);

  model::FanConstantVolume fan(m, availabilitySchedule);

  model::CoilHeatingElectric supplementalHeatingCoil(m, availabilitySchedule);

  model::CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(m);
  coolingCurveFofTemp.setCoefficient1Constant(0.42415);
  coolingCurveFofTemp.setCoefficient2x(0.04426);
  coolingCurveFofTemp.setCoefficient3xPOW2(-0.00042);
  coolingCurveFofTemp.setCoefficient4y(0.00333);
  coolingCurveFofTemp.setCoefficient5yPOW2(-0.00008);
  coolingCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
  coolingCurveFofTemp.setMinimumValueofx(17.0);
  coolingCurveFofTemp.setMaximumValueofx(22.0);
  coolingCurveFofTemp.setMinimumValueofy(13.0);
  coolingCurveFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic coolingCurveFofFlow = CurveQuadratic(m);
  coolingCurveFofFlow.setCoefficient1Constant(0.77136);
  coolingCurveFofFlow.setCoefficient2x(0.34053);
  coolingCurveFofFlow.setCoefficient3xPOW2(-0.11088);
  coolingCurveFofFlow.setMinimumValueofx(0.75918);
  coolingCurveFofFlow.setMaximumValueofx(1.13877);

  CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(m);
  energyInputRatioFofTemp.setCoefficient1Constant(1.23649);
  energyInputRatioFofTemp.setCoefficient2x(-0.02431);
  energyInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
  energyInputRatioFofTemp.setCoefficient4y(-0.01434);
  energyInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
  energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
  energyInputRatioFofTemp.setMinimumValueofx(17.0);
  energyInputRatioFofTemp.setMaximumValueofx(22.0);
  energyInputRatioFofTemp.setMinimumValueofy(13.0);
  energyInputRatioFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(m);
  energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
  energyInputRatioFofFlow.setCoefficient2x(-0.32953);
  energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
  energyInputRatioFofFlow.setMinimumValueofx(0.75918);
  energyInputRatioFofFlow.setMaximumValueofx(1.13877);

  CurveQuadratic partLoadFraction = CurveQuadratic(m);
  partLoadFraction.setCoefficient1Constant(0.77100);
  partLoadFraction.setCoefficient2x(0.22900);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  CoilCoolingDXSingleSpeed coolingCoil = CoilCoolingDXSingleSpeed(m, availabilitySchedule, coolingCurveFofTemp, coolingCurveFofFlow,
                                                                  energyInputRatioFofTemp, energyInputRatioFofFlow, partLoadFraction);

  CurveBiquadratic totalHeatingCapacityFunctionofTemperatureCurve(m);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(0.758746);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient2x(0.0);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.0);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient4y(0.027626);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient5yPOW2(0.000148716);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient6xTIMESY(0.0);

  CurveQuadratic totalHeatingCapacityFunctionofFlowFractionCurve(m);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.84);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.16);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
  totalHeatingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
  totalHeatingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

  CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(m);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(1.19248);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(-0.0300438);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0.00103745);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient4y(0.0);
  energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
  energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(20.0);

  CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(m);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.3824);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.4336);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0512);
  energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.0);
  energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.0);

  CurveQuadratic partLoadFractionCorrelationCurve(m);
  partLoadFractionCorrelationCurve.setCoefficient1Constant(0.85);
  partLoadFractionCorrelationCurve.setCoefficient2x(0.15);
  partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
  partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
  partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);

  CoilHeatingDXSingleSpeed heatingCoil(m, availabilitySchedule, totalHeatingCapacityFunctionofTemperatureCurve,
                                       totalHeatingCapacityFunctionofFlowFractionCurve, energyInputRatioFunctionofTemperatureCurve,
                                       energyInputRatioFunctionofFlowFractionCurve, partLoadFractionCorrelationCurve);

  model::ZoneHVACPackagedTerminalHeatPump pthp(m, availabilitySchedule, fan, heatingCoil, coolingCoil, supplementalHeatingCoil);

  model::Model m2;

  auto pthp2 = pthp.clone(m2).cast<model::ZoneHVACPackagedTerminalHeatPump>();

  pthp.remove();

  pthp2.remove();
}

TEST_F(ModelFixture, ZoneHVACPackagedTerminalHeatPump_CoilDXVariableSpeed) {
  model::Model m;
  model::CoilHeatingDXVariableSpeed heatingCoil(m);
  model::CoilCoolingDXVariableSpeed coolingCoil(m);
  model::ScheduleCompact availabilitySchedule(m);
  model::FanConstantVolume fan(m, availabilitySchedule);
  model::CoilHeatingElectric supplementalHeatingCoil(m, availabilitySchedule);

  model::ZoneHVACPackagedTerminalHeatPump pthp(m, availabilitySchedule, fan, heatingCoil, coolingCoil, supplementalHeatingCoil);
}

TEST_F(ModelFixture, ZoneHVACPackagedTerminalHeatPump_CoilSystemCoolingDXHeatExchangerAssisted) {
  model::Model m;
  model::CoilHeatingDXSingleSpeed heatingCoil(m);
  model::CoilSystemCoolingDXHeatExchangerAssisted coolingCoil(m);
  model::ScheduleCompact availabilitySchedule(m);
  model::FanConstantVolume fan(m, availabilitySchedule);
  model::CoilHeatingElectric supplementalHeatingCoil(m, availabilitySchedule);

  model::ZoneHVACPackagedTerminalHeatPump pthp(m, availabilitySchedule, fan, heatingCoil, coolingCoil, supplementalHeatingCoil);
}

TEST_F(ModelFixture, ZoneHVACPackagedTerminalHeatPump_SupplyAirFanOpSch) {
  model::Model m;
  model::CoilHeatingDXVariableSpeed heatingCoil(m);
  model::CoilCoolingDXVariableSpeed coolingCoil(m);
  model::CoilHeatingElectric supplementalHeatingCoil(m);
  model::FanConstantVolume fanCV(m);
  model::FanSystemModel fanSystemModel(m);
  model::FanOnOff fanOnOff(m);
  auto alwaysOn = m.alwaysOnDiscreteSchedule();
  auto alwaysOff = m.alwaysOffDiscreteSchedule();

  model::ZoneHVACPackagedTerminalHeatPump pthpCV(m, alwaysOn, fanCV, heatingCoil, coolingCoil, supplementalHeatingCoil);
  EXPECT_EQ(alwaysOn, pthpCV.supplyAirFanOperatingModeSchedule());

  model::ZoneHVACPackagedTerminalHeatPump pthpSystemModel(m, alwaysOn, fanSystemModel, heatingCoil, coolingCoil, supplementalHeatingCoil);
  EXPECT_EQ(alwaysOff, pthpSystemModel.supplyAirFanOperatingModeSchedule());

  model::ZoneHVACPackagedTerminalHeatPump pthpOnOff(m, alwaysOn, fanOnOff, heatingCoil, coolingCoil, supplementalHeatingCoil);
  EXPECT_EQ(alwaysOff, pthpOnOff.supplyAirFanOperatingModeSchedule());
}

TEST_F(ModelFixture, ZoneHVACPackagedTerminalHeatPump_HeatCoolFuelTypes) {

  Model m;

  FanSystemModel fan(m);
  CoilHeatingDXVariableSpeed hc(m);
  CoilCoolingDXVariableSpeed cc(m);
  CoilHeatingWater supHC(m);

  auto alwaysOn = m.alwaysOnDiscreteSchedule();

  ZoneHVACPackagedTerminalHeatPump pthp(m, alwaysOn, fan, hc, cc, supHC);

  ThermalZone z(m);
  EXPECT_EQ(ComponentType(ComponentType::None), z.componentType());
  EXPECT_EQ(0, z.coolingFuelTypes().size());
  EXPECT_EQ(0, z.heatingFuelTypes().size());
  EXPECT_EQ(0, z.appGHeatingFuelTypes().size());

  PlantLoop p(m);
  EXPECT_EQ(0, supHC.heatingFuelTypes().size());
  EXPECT_EQ(0, p.heatingFuelTypes().size());

  BoilerHotWater b(m);
  EXPECT_TRUE(p.addSupplyBranchForComponent(b));
  EXPECT_TRUE(p.addDemandBranchForComponent(supHC));

  EXPECT_TRUE(b.setFuelType("Propane"));
  ASSERT_EQ(1, b.heatingFuelTypes().size());
  EXPECT_EQ(FuelType(FuelType::Propane), b.heatingFuelTypes().front());
  ASSERT_EQ(1, p.heatingFuelTypes().size());
  EXPECT_EQ(FuelType(FuelType::Propane), p.heatingFuelTypes().front());

  EXPECT_EQ(ComponentType(ComponentType::Heating), supHC.componentType());
  EXPECT_EQ(0, supHC.coolingFuelTypes().size());
  ASSERT_EQ(1, supHC.heatingFuelTypes().size());
  EXPECT_EQ(FuelType(FuelType::Propane), supHC.heatingFuelTypes().front());
  ASSERT_EQ(1, supHC.appGHeatingFuelTypes().size());
  EXPECT_EQ(AppGFuelType(AppGFuelType::Fuel), supHC.appGHeatingFuelTypes().front());

  EXPECT_EQ(ComponentType(ComponentType::Cooling), cc.componentType());
  ASSERT_EQ(1, cc.coolingFuelTypes().size());
  EXPECT_EQ(FuelType(FuelType::Electricity), cc.coolingFuelTypes().front());
  EXPECT_EQ(0, cc.heatingFuelTypes().size());
  EXPECT_EQ(0, cc.appGHeatingFuelTypes().size());

  EXPECT_EQ(ComponentType(ComponentType::Heating), hc.componentType());
  EXPECT_EQ(0, hc.coolingFuelTypes().size());
  ASSERT_EQ(1, hc.heatingFuelTypes().size());
  EXPECT_EQ(FuelType(FuelType::Electricity), hc.heatingFuelTypes().front());
  ASSERT_EQ(1, hc.appGHeatingFuelTypes().size());
  EXPECT_EQ(AppGFuelType(AppGFuelType::HeatPump), hc.appGHeatingFuelTypes().front());

  EXPECT_EQ(ComponentType(ComponentType::Both), pthp.componentType());
  testFuelTypeEquality({FuelType::Electricity}, pthp.coolingFuelTypes());
  testFuelTypeEquality({FuelType::Electricity, FuelType::Propane}, pthp.heatingFuelTypes());
  testAppGFuelTypeEquality({AppGFuelType::Fuel, AppGFuelType::HeatPump}, pthp.appGHeatingFuelTypes());

  EXPECT_TRUE(pthp.addToThermalZone(z));

  EXPECT_EQ(ComponentType(ComponentType::Both), z.componentType());
  testFuelTypeEquality({FuelType::Electricity}, z.coolingFuelTypes());
  testFuelTypeEquality({FuelType::Electricity, FuelType::Propane}, z.heatingFuelTypes());
  testAppGFuelTypeEquality({AppGFuelType::Fuel, AppGFuelType::HeatPump}, z.appGHeatingFuelTypes());
}
