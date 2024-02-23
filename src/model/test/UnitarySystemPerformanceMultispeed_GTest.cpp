/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirLoopHVACUnitarySystem.hpp"
#include "../AirLoopHVACUnitarySystem_Impl.hpp"

#include "../ZoneHVACWaterToAirHeatPump.hpp"
#include "../ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "../ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "../UnitarySystemPerformanceMultispeed.hpp"
#include "../UnitarySystemPerformanceMultispeed_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingElectric_Impl.hpp"
#include "../FanOnOff.hpp"
#include "../FanOnOff_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, UnitarySystemPerformanceMultispeed_DefaultConstructors) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      UnitarySystemPerformanceMultispeed testObject = UnitarySystemPerformanceMultispeed(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, UnitarySystemPerformanceMultispeed_GetterSetters) {
  Model m;
  UnitarySystemPerformanceMultispeed testObject = UnitarySystemPerformanceMultispeed(m);

  // Single Mode
  EXPECT_FALSE(testObject.singleModeOperation());
  EXPECT_TRUE(testObject.setSingleModeOperation(true));
  EXPECT_TRUE(testObject.singleModeOperation());
  EXPECT_TRUE(testObject.setSingleModeOperation(false));
  EXPECT_FALSE(testObject.singleModeOperation());
  EXPECT_TRUE(testObject.setSingleModeOperation(true));
  testObject.resetSingleModeOperation();
  EXPECT_FALSE(testObject.singleModeOperation());

  // No Load
  EXPECT_EQ(1.0, testObject.noLoadSupplyAirflowRateRatio());
  EXPECT_TRUE(testObject.setNoLoadSupplyAirflowRateRatio(0.5));
  EXPECT_EQ(0.5, testObject.noLoadSupplyAirflowRateRatio());

  // Supply Airflow Ratios
  EXPECT_TRUE(testObject.addSupplyAirflowRatioField(1.0, 2.0));
  EXPECT_TRUE(testObject.addSupplyAirflowRatioField(SupplyAirflowRatioField::fromCoolingRatio(3.0)));
  EXPECT_TRUE(testObject.addSupplyAirflowRatioField(SupplyAirflowRatioField()));
  std::vector<SupplyAirflowRatioField> airflowRatioFields = testObject.supplyAirflowRatioFields();
  EXPECT_EQ(airflowRatioFields.size(), 3);
  EXPECT_DOUBLE_EQ(airflowRatioFields[0].coolingRatio().get(), 2.0);
  EXPECT_TRUE(airflowRatioFields[1].isHeatingRatioAutosized());
  EXPECT_TRUE(airflowRatioFields[2].isCoolingRatioAutosized());
  testObject.resetSupplyAirflowRatioFields();
  EXPECT_EQ(testObject.supplyAirflowRatioFields().size(), 0);
  airflowRatioFields.pop_back();
  EXPECT_TRUE(testObject.setSupplyAirflowRatioFields(airflowRatioFields));
  EXPECT_EQ(testObject.supplyAirflowRatioFields().size(), 2);
}

void testDblOptionalEq(boost::optional<double> op, double value) {
  EXPECT_TRUE(op);
  if (op) {
    EXPECT_DOUBLE_EQ(value, *op);
  }
}

TEST_F(ModelFixture, UnitarySystemPerformanceMultispeed_SupplyAirflowRatioField) {
  SupplyAirflowRatioField a;
  EXPECT_TRUE(a.isCoolingRatioAutosized());
  EXPECT_TRUE(a.isHeatingRatioAutosized());
  EXPECT_FALSE(a.heatingRatio());
  EXPECT_FALSE(a.coolingRatio());

  SupplyAirflowRatioField b(1.0, 2.0);
  EXPECT_FALSE(b.isHeatingRatioAutosized());
  EXPECT_FALSE(b.isCoolingRatioAutosized());
  testDblOptionalEq(b.coolingRatio(), 2.0);
  testDblOptionalEq(b.heatingRatio(), 1.0);

  SupplyAirflowRatioField c = SupplyAirflowRatioField::fromCoolingRatio(3.0);
  EXPECT_FALSE(c.isCoolingRatioAutosized());
  EXPECT_TRUE(c.isHeatingRatioAutosized());
  EXPECT_FALSE(c.heatingRatio());
  testDblOptionalEq(c.coolingRatio(), 3.0);

  SupplyAirflowRatioField d = SupplyAirflowRatioField::fromHeatingRatio(4.0);
  EXPECT_FALSE(d.isHeatingRatioAutosized());
  EXPECT_TRUE(d.isCoolingRatioAutosized());
  EXPECT_FALSE(d.coolingRatio());
  testDblOptionalEq(d.heatingRatio(), 4.0);
}

TEST_F(ModelFixture, UnitarySystemPerformanceMultispeed_addToAirLoopHVACUnitarySystem) {
  Model m;
  UnitarySystemPerformanceMultispeed perf(m);
  AirLoopHVACUnitarySystem airloopsys = AirLoopHVACUnitarySystem(m);

  ASSERT_FALSE(airloopsys.designSpecificationMultispeedObject());
  EXPECT_TRUE(airloopsys.setDesignSpecificationMultispeedObject(perf));
  ASSERT_TRUE(airloopsys.designSpecificationMultispeedObject());
  EXPECT_EQ(airloopsys.designSpecificationMultispeedObject().get().handle(), perf.handle());
  airloopsys.resetDesignSpecificationMultispeedObject();
  ASSERT_FALSE(airloopsys.designSpecificationMultispeedObject());
}

TEST_F(ModelFixture, UnitarySystemPerformanceMultispeed_addZoneHVACWaterToAirHeatPump) {
  Model m;
  UnitarySystemPerformanceMultispeed perf(m);
  Schedule availabilitySched = m.alwaysOnDiscreteSchedule();
  FanOnOff supplyFan(m, availabilitySched);
  CoilHeatingWaterToAirHeatPumpEquationFit coilHeatingWaterToAirHP(m);
  CoilCoolingWaterToAirHeatPumpEquationFit coilCoolingWaterToAirHP(m);
  CoilHeatingElectric supplementalHC(m, availabilitySched);
  ZoneHVACWaterToAirHeatPump zonehvac(m, availabilitySched, supplyFan, coilHeatingWaterToAirHP, coilCoolingWaterToAirHP, supplementalHC);

  ASSERT_FALSE(zonehvac.designSpecificationMultispeedObject());
  EXPECT_TRUE(zonehvac.setDesignSpecificationMultispeedObject(perf));
  ASSERT_TRUE(zonehvac.designSpecificationMultispeedObject());
  EXPECT_EQ(zonehvac.designSpecificationMultispeedObject().get().handle(), perf.handle());
  zonehvac.resetDesignSpecificationMultispeedObject();
  ASSERT_FALSE(zonehvac.designSpecificationMultispeedObject());
}

TEST_F(ModelFixture, UnitarySystemPerformanceMultispeed_addZoneHVACTerminalUnitVariableRefrigerantFlow) {
  Model m;
  UnitarySystemPerformanceMultispeed perf(m);
  ZoneHVACTerminalUnitVariableRefrigerantFlow zonehvac(m);

  ASSERT_FALSE(zonehvac.designSpecificationMultispeedObject());
  EXPECT_TRUE(zonehvac.setDesignSpecificationMultispeedObject(perf));
  ASSERT_TRUE(zonehvac.designSpecificationMultispeedObject());
  EXPECT_EQ(zonehvac.designSpecificationMultispeedObject().get().handle(), perf.handle());
  zonehvac.resetDesignSpecificationMultispeedObject();
  ASSERT_FALSE(zonehvac.designSpecificationMultispeedObject());
}
