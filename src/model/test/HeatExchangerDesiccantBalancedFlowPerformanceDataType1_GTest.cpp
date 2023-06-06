/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include <string>
#include "ModelFixture.hpp"

#include "../HeatExchangerDesiccantBalancedFlowPerformanceDataType1.hpp"
#include "../HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl.hpp"

#include "../HeatExchangerDesiccantBalancedFlow.hpp"
#include "../HeatExchangerDesiccantBalancedFlow_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, HeatExchangerDesiccantBalancedFlowPerformanceDataType1_HeatExchangerDesiccantBalancedFlowPerformanceDataType1) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p(m);

  EXPECT_TRUE(p.isNominalAirFlowRateAutosized());
  EXPECT_TRUE(p.isNominalAirFaceVelocityAutosized());
  EXPECT_EQ(0.0, p.nominalElectricPower());
  EXPECT_EQ(-7.18302E+00, p.temperatureEquationCoefficient1());
  EXPECT_EQ(-1.84967E+02, p.temperatureEquationCoefficient2());
  EXPECT_EQ(1.00051E+00, p.temperatureEquationCoefficient3());
  EXPECT_EQ(1.16033E+04, p.temperatureEquationCoefficient4());
  EXPECT_EQ(-5.07550E+01, p.temperatureEquationCoefficient5());
  EXPECT_EQ(-1.68467E-02, p.temperatureEquationCoefficient6());
  EXPECT_EQ(5.82213E+01, p.temperatureEquationCoefficient7());
  EXPECT_EQ(5.98863E-01, p.temperatureEquationCoefficient8());
  EXPECT_EQ(0.007143, p.minimumRegenerationInletAirHumidityRatioforTemperatureEquation());
  EXPECT_EQ(0.024286, p.maximumRegenerationInletAirHumidityRatioforTemperatureEquation());
  EXPECT_EQ(17.83333, p.minimumRegenerationInletAirTemperatureforTemperatureEquation());
  EXPECT_EQ(48.88889, p.maximumRegenerationInletAirTemperatureforTemperatureEquation());
  EXPECT_EQ(0.005000, p.minimumProcessInletAirHumidityRatioforTemperatureEquation());
  EXPECT_EQ(0.015714, p.maximumProcessInletAirHumidityRatioforTemperatureEquation());
  EXPECT_EQ(4.583333, p.minimumProcessInletAirTemperatureforTemperatureEquation());
  EXPECT_EQ(21.83333, p.maximumProcessInletAirTemperatureforTemperatureEquation());
  EXPECT_EQ(2.286, p.minimumRegenerationAirVelocityforTemperatureEquation());
  EXPECT_EQ(4.826, p.maximumRegenerationAirVelocityforTemperatureEquation());
  EXPECT_EQ(16.66667, p.minimumRegenerationOutletAirTemperatureforTemperatureEquation());
  EXPECT_EQ(46.11111, p.maximumRegenerationOutletAirTemperatureforTemperatureEquation());
  EXPECT_EQ(10.0, p.minimumRegenerationInletAirRelativeHumidityforTemperatureEquation());
  EXPECT_EQ(100.0, p.maximumRegenerationInletAirRelativeHumidityforTemperatureEquation());
  EXPECT_EQ(80.0, p.minimumProcessInletAirRelativeHumidityforTemperatureEquation());
  EXPECT_EQ(100.0, p.maximumProcessInletAirRelativeHumidityforTemperatureEquation());
  EXPECT_EQ(3.13878E-03, p.humidityRatioEquationCoefficient1());
  EXPECT_EQ(1.09689E+00, p.humidityRatioEquationCoefficient2());
  EXPECT_EQ(-2.63341E-05, p.humidityRatioEquationCoefficient3());
  EXPECT_EQ(-6.33885E+00, p.humidityRatioEquationCoefficient4());
  EXPECT_EQ(9.38196E-03, p.humidityRatioEquationCoefficient5());
  EXPECT_EQ(5.21186E-05, p.humidityRatioEquationCoefficient6());
  EXPECT_EQ(6.70354E-02, p.humidityRatioEquationCoefficient7());
  EXPECT_EQ(-1.60823E-04, p.humidityRatioEquationCoefficient8());
  EXPECT_EQ(0.007143, p.minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation());
  EXPECT_EQ(0.024286, p.maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation());
  EXPECT_EQ(17.83333, p.minimumRegenerationInletAirTemperatureforHumidityRatioEquation());
  EXPECT_EQ(48.88889, p.maximumRegenerationInletAirTemperatureforHumidityRatioEquation());
  EXPECT_EQ(0.005000, p.minimumProcessInletAirHumidityRatioforHumidityRatioEquation());
  EXPECT_EQ(0.015714, p.maximumProcessInletAirHumidityRatioforHumidityRatioEquation());
  EXPECT_EQ(4.583333, p.minimumProcessInletAirTemperatureforHumidityRatioEquation());
  EXPECT_EQ(21.83333, p.maximumProcessInletAirTemperatureforHumidityRatioEquation());
  EXPECT_EQ(2.286, p.minimumRegenerationAirVelocityforHumidityRatioEquation());
  EXPECT_EQ(4.826, p.maximumRegenerationAirVelocityforHumidityRatioEquation());
  EXPECT_EQ(0.007811, p.minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation());
  EXPECT_EQ(0.026707, p.maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation());
  EXPECT_EQ(10.0, p.minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation());
  EXPECT_EQ(100.0, p.maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation());
  EXPECT_EQ(80.0, p.minimumProcessInletAirRelativeHumidityforHumidityRatioEquation());
  EXPECT_EQ(100.0, p.maximumProcessInletAirRelativeHumidityforHumidityRatioEquation());
  EXPECT_EQ(0, p.heatExchangerDesiccantBalancedFlows().size());
}

TEST_F(ModelFixture, HeatExchangerDesiccantBalancedFlowPerformanceDataType1_GettersSetters) {
  Model m;
  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p(m);
  HeatExchangerDesiccantBalancedFlow hx(m, p);

  EXPECT_TRUE(p.setNominalAirFlowRate(1.0));
  EXPECT_TRUE(p.setNominalAirFaceVelocity(2.0));
  EXPECT_TRUE(p.setNominalElectricPower(3.0));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient1(0.001));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient2(0.002));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient3(0.003));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient4(0.004));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient5(0.005));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient6(0.006));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient7(0.007));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient8(0.008));
  EXPECT_TRUE(p.setMinimumRegenerationInletAirHumidityRatioforTemperatureEquation(0.009));
  EXPECT_TRUE(p.setMaximumRegenerationInletAirHumidityRatioforTemperatureEquation(0.0001));
  EXPECT_TRUE(p.setMinimumRegenerationInletAirTemperatureforTemperatureEquation(0.0002));
  EXPECT_TRUE(p.setMaximumRegenerationInletAirTemperatureforTemperatureEquation(0.0003));
  EXPECT_TRUE(p.setMinimumProcessInletAirHumidityRatioforTemperatureEquation(0.0004));
  EXPECT_TRUE(p.setMaximumProcessInletAirHumidityRatioforTemperatureEquation(0.0005));
  EXPECT_TRUE(p.setMinimumProcessInletAirTemperatureforTemperatureEquation(0.0006));
  EXPECT_TRUE(p.setMaximumProcessInletAirTemperatureforTemperatureEquation(0.0007));
  EXPECT_TRUE(p.setMinimumRegenerationAirVelocityforTemperatureEquation(0.0008));
  EXPECT_TRUE(p.setMaximumRegenerationAirVelocityforTemperatureEquation(0.0009));
  EXPECT_TRUE(p.setMinimumRegenerationOutletAirTemperatureforTemperatureEquation(0.0001));
  EXPECT_TRUE(p.setMaximumRegenerationOutletAirTemperatureforTemperatureEquation(0.0002));
  EXPECT_TRUE(p.setMinimumRegenerationInletAirRelativeHumidityforTemperatureEquation(0.0003));
  EXPECT_TRUE(p.setMaximumRegenerationInletAirRelativeHumidityforTemperatureEquation(0.0004));
  EXPECT_TRUE(p.setMinimumProcessInletAirRelativeHumidityforTemperatureEquation(0.0005));
  EXPECT_TRUE(p.setMaximumProcessInletAirRelativeHumidityforTemperatureEquation(0.0006));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient1(0.0007));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient2(0.0008));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient3(0.0009));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient4(0.001));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient5(0.002));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient6(0.003));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient7(0.004));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient8(0.005));
  EXPECT_TRUE(p.setMinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation(0.006));
  EXPECT_TRUE(p.setMaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation(0.007));
  EXPECT_TRUE(p.setMinimumRegenerationInletAirTemperatureforHumidityRatioEquation(0.008));
  EXPECT_TRUE(p.setMaximumRegenerationInletAirTemperatureforHumidityRatioEquation(0.009));
  EXPECT_TRUE(p.setMinimumProcessInletAirHumidityRatioforHumidityRatioEquation(0.01));
  EXPECT_TRUE(p.setMaximumProcessInletAirHumidityRatioforHumidityRatioEquation(0.02));
  EXPECT_TRUE(p.setMinimumProcessInletAirTemperatureforHumidityRatioEquation(0.03));
  EXPECT_TRUE(p.setMaximumProcessInletAirTemperatureforHumidityRatioEquation(0.04));
  EXPECT_TRUE(p.setMinimumRegenerationAirVelocityforHumidityRatioEquation(0.05));
  EXPECT_TRUE(p.setMaximumRegenerationAirVelocityforHumidityRatioEquation(0.06));
  EXPECT_TRUE(p.setMinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(0.07));
  EXPECT_TRUE(p.setMaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(0.08));
  EXPECT_TRUE(p.setMinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(0.09));
  EXPECT_TRUE(p.setMaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(0.01));
  EXPECT_TRUE(p.setMinimumProcessInletAirRelativeHumidityforHumidityRatioEquation(0.02));
  EXPECT_TRUE(p.setMaximumProcessInletAirRelativeHumidityforHumidityRatioEquation(0.03));

  EXPECT_FALSE(p.isNominalAirFlowRateAutosized());
  ASSERT_TRUE(p.nominalAirFlowRate());
  EXPECT_EQ(1.0, p.nominalAirFlowRate().get());
  EXPECT_FALSE(p.isNominalAirFaceVelocityAutosized());
  ASSERT_TRUE(p.nominalAirFaceVelocity());
  EXPECT_EQ(2.0, p.nominalAirFaceVelocity().get());
  EXPECT_EQ(3.0, p.nominalElectricPower());
  EXPECT_EQ(0.001, p.temperatureEquationCoefficient1());
  EXPECT_EQ(0.002, p.temperatureEquationCoefficient2());
  EXPECT_EQ(0.003, p.temperatureEquationCoefficient3());
  EXPECT_EQ(0.004, p.temperatureEquationCoefficient4());
  EXPECT_EQ(0.005, p.temperatureEquationCoefficient5());
  EXPECT_EQ(0.006, p.temperatureEquationCoefficient6());
  EXPECT_EQ(0.007, p.temperatureEquationCoefficient7());
  EXPECT_EQ(0.008, p.temperatureEquationCoefficient8());
  EXPECT_EQ(0.009, p.minimumRegenerationInletAirHumidityRatioforTemperatureEquation());
  EXPECT_EQ(0.0001, p.maximumRegenerationInletAirHumidityRatioforTemperatureEquation());
  EXPECT_EQ(0.0002, p.minimumRegenerationInletAirTemperatureforTemperatureEquation());
  EXPECT_EQ(0.0003, p.maximumRegenerationInletAirTemperatureforTemperatureEquation());
  EXPECT_EQ(0.0004, p.minimumProcessInletAirHumidityRatioforTemperatureEquation());
  EXPECT_EQ(0.0005, p.maximumProcessInletAirHumidityRatioforTemperatureEquation());
  EXPECT_EQ(0.0006, p.minimumProcessInletAirTemperatureforTemperatureEquation());
  EXPECT_EQ(0.0007, p.maximumProcessInletAirTemperatureforTemperatureEquation());
  EXPECT_EQ(0.0008, p.minimumRegenerationAirVelocityforTemperatureEquation());
  EXPECT_EQ(0.0009, p.maximumRegenerationAirVelocityforTemperatureEquation());
  EXPECT_EQ(0.0001, p.minimumRegenerationOutletAirTemperatureforTemperatureEquation());
  EXPECT_EQ(0.0002, p.maximumRegenerationOutletAirTemperatureforTemperatureEquation());
  EXPECT_EQ(0.0003, p.minimumRegenerationInletAirRelativeHumidityforTemperatureEquation());
  EXPECT_EQ(0.0004, p.maximumRegenerationInletAirRelativeHumidityforTemperatureEquation());
  EXPECT_EQ(0.0005, p.minimumProcessInletAirRelativeHumidityforTemperatureEquation());
  EXPECT_EQ(0.0006, p.maximumProcessInletAirRelativeHumidityforTemperatureEquation());
  EXPECT_EQ(0.0007, p.humidityRatioEquationCoefficient1());
  EXPECT_EQ(0.0008, p.humidityRatioEquationCoefficient2());
  EXPECT_EQ(0.0009, p.humidityRatioEquationCoefficient3());
  EXPECT_EQ(0.001, p.humidityRatioEquationCoefficient4());
  EXPECT_EQ(0.002, p.humidityRatioEquationCoefficient5());
  EXPECT_EQ(0.003, p.humidityRatioEquationCoefficient6());
  EXPECT_EQ(0.004, p.humidityRatioEquationCoefficient7());
  EXPECT_EQ(0.005, p.humidityRatioEquationCoefficient8());
  EXPECT_EQ(0.006, p.minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation());
  EXPECT_EQ(0.007, p.maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation());
  EXPECT_EQ(0.008, p.minimumRegenerationInletAirTemperatureforHumidityRatioEquation());
  EXPECT_EQ(0.009, p.maximumRegenerationInletAirTemperatureforHumidityRatioEquation());
  EXPECT_EQ(0.01, p.minimumProcessInletAirHumidityRatioforHumidityRatioEquation());
  EXPECT_EQ(0.02, p.maximumProcessInletAirHumidityRatioforHumidityRatioEquation());
  EXPECT_EQ(0.03, p.minimumProcessInletAirTemperatureforHumidityRatioEquation());
  EXPECT_EQ(0.04, p.maximumProcessInletAirTemperatureforHumidityRatioEquation());
  EXPECT_EQ(0.05, p.minimumRegenerationAirVelocityforHumidityRatioEquation());
  EXPECT_EQ(0.06, p.maximumRegenerationAirVelocityforHumidityRatioEquation());
  EXPECT_EQ(0.07, p.minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation());
  EXPECT_EQ(0.08, p.maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation());
  EXPECT_EQ(0.09, p.minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation());
  EXPECT_EQ(0.01, p.maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation());
  EXPECT_EQ(0.02, p.minimumProcessInletAirRelativeHumidityforHumidityRatioEquation());
  EXPECT_EQ(0.03, p.maximumProcessInletAirRelativeHumidityforHumidityRatioEquation());
  EXPECT_EQ(1, p.heatExchangerDesiccantBalancedFlows().size());
}

TEST_F(ModelFixture, HeatExchangerDesiccantBalancedFlowPerformanceDataType1_remove) {
  Model m;
  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p(m);
  HeatExchangerDesiccantBalancedFlow hx(m, p);

  // This shouldn't work, it's going to put the HeatExchangerDesiccantBalancedFlow in a broken state
  auto rmed = p.remove();
  EXPECT_EQ(0u, rmed.size());
  EXPECT_NO_THROW(hx.heatExchangerPerformance());
  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p2(m);
  EXPECT_TRUE(hx.setHeatExchangerPerformance(p2));
  EXPECT_EQ(1u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlow>().size());
  EXPECT_EQ(2u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlowPerformanceDataType1>().size());

  rmed = p.remove();
  EXPECT_EQ(1u, rmed.size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlow>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlowPerformanceDataType1>().size());
}

TEST_F(ModelFixture, HeatExchangerDesiccantBalancedFlowPerformanceDataType1_clone) {
  Model m;
  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p(m);
  p.setNominalAirFlowRate(5);
  HeatExchangerDesiccantBalancedFlow hx(m, p);

  EXPECT_EQ(1u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlow>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlowPerformanceDataType1>().size());

  auto pClone = p.clone(m).cast<HeatExchangerDesiccantBalancedFlowPerformanceDataType1>();
  EXPECT_EQ(p, hx.heatExchangerPerformance());
  EXPECT_EQ(5, p.nominalAirFlowRate().get());
  EXPECT_EQ(5, pClone.nominalAirFlowRate().get());
  EXPECT_EQ(1u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlow>().size());
  EXPECT_EQ(2u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlowPerformanceDataType1>().size());
}
