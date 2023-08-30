/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../WaterHeaterHeatPump.hpp"
#include "../CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "../ModelObjectList.hpp"
#include "../ModelObjectList_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_CoilWaterHeatingAirToWaterHeatPumpVariableSpeed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilWaterHeatingAirToWaterHeatPumpVariableSpeed coil(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed coil(m);

  EXPECT_EQ(1, coil.nominalSpeedLevel());
  EXPECT_EQ(4000.0, coil.ratedWaterHeatingCapacity());
  EXPECT_EQ(29.44, coil.ratedEvaporatorInletAirDryBulbTemperature());
  EXPECT_EQ(22.22, coil.ratedEvaporatorInletAirWetBulbTemperature());
  EXPECT_EQ(55.72, coil.ratedCondenserInletWaterTemperature());
  EXPECT_FALSE(coil.ratedEvaporatorAirFlowRate());
  EXPECT_TRUE(coil.isRatedEvaporatorAirFlowRateAutocalculated());
  EXPECT_FALSE(coil.ratedCondenserWaterFlowRate());
  EXPECT_TRUE(coil.isRatedCondenserWaterFlowRateAutocalculated());
  EXPECT_EQ("Yes", coil.evaporatorFanPowerIncludedinRatedCOP());
  EXPECT_EQ("No", coil.condenserPumpPowerIncludedinRatedCOP());
  EXPECT_EQ("No", coil.condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP());
  EXPECT_EQ(0.2, coil.fractionofCondenserPumpHeattoWater());
  EXPECT_EQ(0, coil.crankcaseHeaterCapacity());
  EXPECT_EQ(10, coil.maximumAmbientTemperatureforCrankcaseHeaterOperation());
  EXPECT_EQ("WetBulbTemperature", coil.evaporatorAirTemperatureTypeforCurveObjects());
  EXPECT_TRUE(coil.partLoadFractionCorrelationCurve().optionalCast<CurveQuadratic>());
}

TEST_F(ModelFixture, CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_SetGetFields) {
  Model m;

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed coil(m);

  EXPECT_TRUE(coil.setNominalSpeedLevel(2));
  EXPECT_TRUE(coil.setRatedWaterHeatingCapacity(1800.0));
  EXPECT_TRUE(coil.setRatedEvaporatorInletAirDryBulbTemperature(20.0));
  EXPECT_TRUE(coil.setRatedEvaporatorInletAirWetBulbTemperature(15.0));
  EXPECT_TRUE(coil.setRatedCondenserInletWaterTemperature(60.0));
  EXPECT_TRUE(coil.setRatedEvaporatorAirFlowRate(100.0));
  EXPECT_TRUE(coil.setRatedCondenserWaterFlowRate(200.0));
  EXPECT_TRUE(coil.setEvaporatorFanPowerIncludedinRatedCOP("No"));
  EXPECT_TRUE(coil.setCondenserPumpPowerIncludedinRatedCOP("Yes"));
  EXPECT_TRUE(coil.setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP("Yes"));
  EXPECT_TRUE(coil.setFractionofCondenserPumpHeattoWater(0.3));
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacity(50));
  EXPECT_TRUE(coil.setMaximumAmbientTemperatureforCrankcaseHeaterOperation(75));
  EXPECT_TRUE(coil.setEvaporatorAirTemperatureTypeforCurveObjects("DryBulbTemperature"));
  auto curve = CurveQuadratic(m);
  EXPECT_TRUE(coil.setPartLoadFractionCorrelationCurve(curve));

  EXPECT_EQ(2, coil.nominalSpeedLevel());
  EXPECT_EQ(1800.0, coil.ratedWaterHeatingCapacity());
  EXPECT_EQ(20.0, coil.ratedEvaporatorInletAirDryBulbTemperature());
  EXPECT_EQ(15.0, coil.ratedEvaporatorInletAirWetBulbTemperature());
  EXPECT_EQ(60.0, coil.ratedCondenserInletWaterTemperature());
  ASSERT_TRUE(coil.ratedEvaporatorAirFlowRate());
  EXPECT_EQ(100.0, coil.ratedEvaporatorAirFlowRate().get());
  EXPECT_FALSE(coil.isRatedEvaporatorAirFlowRateAutocalculated());
  ASSERT_TRUE(coil.ratedCondenserWaterFlowRate());
  EXPECT_EQ(200.0, coil.ratedCondenserWaterFlowRate().get());
  EXPECT_FALSE(coil.isRatedCondenserWaterFlowRateAutocalculated());
  EXPECT_EQ("No", coil.evaporatorFanPowerIncludedinRatedCOP());
  EXPECT_EQ("Yes", coil.condenserPumpPowerIncludedinRatedCOP());
  EXPECT_EQ("Yes", coil.condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP());
  EXPECT_EQ(0.3, coil.fractionofCondenserPumpHeattoWater());
  EXPECT_EQ(50, coil.crankcaseHeaterCapacity());
  EXPECT_EQ(75, coil.maximumAmbientTemperatureforCrankcaseHeaterOperation());
  EXPECT_EQ("DryBulbTemperature", coil.evaporatorAirTemperatureTypeforCurveObjects());
  EXPECT_TRUE(coil.partLoadFractionCorrelationCurve().optionalCast<CurveQuadratic>());

  coil.autocalculateRatedEvaporatorAirFlowRate();
  coil.autocalculateRatedCondenserWaterFlowRate();

  EXPECT_FALSE(coil.ratedEvaporatorAirFlowRate());
  EXPECT_TRUE(coil.isRatedEvaporatorAirFlowRateAutocalculated());
  EXPECT_FALSE(coil.ratedCondenserWaterFlowRate());
  EXPECT_TRUE(coil.isRatedCondenserWaterFlowRateAutocalculated());
}

TEST_F(ModelFixture, CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Clone) {
  Model m;

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed coil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData speed1(m);
  coil.addSpeed(speed1);
  coil.setRatedWaterHeatingCapacity(1900.0);

  EXPECT_EQ(1, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<ModelObjectList>().size());

  auto coilClone = coil.clone(m).cast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>();
  ASSERT_EQ(1900.0, coilClone.ratedWaterHeatingCapacity());

  EXPECT_EQ(2, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<ModelObjectList>().size());

  Model m2;
  auto coilClone2 = coil.clone(m2).cast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>();
  ASSERT_EQ(1900.0, coilClone2.ratedWaterHeatingCapacity());
  EXPECT_EQ(2, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<ModelObjectList>().size());
  EXPECT_EQ(1, m2.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());
  EXPECT_EQ(1, m2.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>().size());
  EXPECT_EQ(1, m2.getConcreteModelObjects<ModelObjectList>().size());
}

TEST_F(ModelFixture, CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Remove) {
  Model m;
  auto count = m.modelObjects().size();

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed coil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData speed1(m);
  coil.addSpeed(speed1);
  EXPECT_EQ(1, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<ModelObjectList>().size());

  coil.remove();

  EXPECT_EQ(0, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<ModelObjectList>().size());

  auto curves = m.getModelObjects<model::Curve>();

  EXPECT_EQ(count, m.modelObjects().size() - curves.size());
}

TEST_F(ModelFixture, CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Speeds) {
  Model m;

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed coil(m);

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData speed1(m);
  coil.addSpeed(speed1);

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData speed2(m);
  coil.addSpeed(speed2);

  EXPECT_EQ(2u, coil.speeds().size());
  EXPECT_EQ(2u, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>().size());
}

TEST_F(ModelFixture, CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_containingHVACComponent_WaterHeaterHeatPumpPumpedCondenser) {

  Model m;

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed coil(m);
  EXPECT_FALSE(coil.containingHVACComponent());

  WaterHeaterHeatPump hpwh(m);
  EXPECT_TRUE(hpwh.setDXCoil(coil));
  EXPECT_EQ(coil.handle(), hpwh.dXCoil().handle());

  // Test containingHVAC
  ASSERT_TRUE(coil.containingHVACComponent());
  EXPECT_EQ(hpwh.handle(), coil.containingHVACComponent()->handle());
}

TEST_F(ModelFixture, CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_containingHVACComponent_CoilSystemIntegratedHeatPumpAirSource) {

  Model m;

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed coil(m);
  EXPECT_FALSE(coil.containingHVACComponent());

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m);
  EXPECT_TRUE(coilSystem.setDedicatedWaterHeatingCoil(coil));
  EXPECT_EQ(coil.handle(), coilSystem.dedicatedWaterHeatingCoil().handle());

  // Test containingHVAC
  ASSERT_TRUE(coil.containingHVACComponent());
  EXPECT_EQ(coilSystem.handle(), coil.containingHVACComponent()->handle());
}
