/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPump.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPump_Impl.hpp"
#include "../Curve.hpp"
#include "../TableLookup.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilWaterHeatingAirToWaterHeatPump) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  EXPECT_EXIT(
    {
      Model m;
      CoilWaterHeatingAirToWaterHeatPump coil(m);
      coil.remove();

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  {
    Model m;
    CoilWaterHeatingAirToWaterHeatPump coil(m);
    {
      auto curve = coil.heatingCapacityFunctionofTemperatureCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }

    {
      auto curve = coil.heatingCapacityFunctionofAirFlowFractionCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }

    {
      auto curve = coil.heatingCapacityFunctionofWaterFlowFractionCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }

    {
      auto curve = coil.heatingCOPFunctionofTemperatureCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }

    {
      auto curve = coil.heatingCOPFunctionofAirFlowFractionCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }

    {
      auto curve = coil.heatingCOPFunctionofWaterFlowFractionCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }

    {
      auto curve = coil.partLoadFractionCorrelationCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }
  }

  {
    Model m;

    TableLookup heatingCapacityFunctionofTemperatureCurve(m);
    TableLookup heatingCapacityFunctionofAirFlowFractionCurve(m);
    TableLookup heatingCapacityFunctionofWaterFlowFractionCurve(m);
    TableLookup heatingCOPFunctionofTemperatureCurve(m);
    TableLookup heatingCOPFunctionofAirFlowFractionCurve(m);
    TableLookup heatingCOPFunctionofWaterFlowFractionCurve(m);
    TableLookup partLoadFractionCorrelationCurve(m);

    CoilWaterHeatingAirToWaterHeatPump coil(m, heatingCapacityFunctionofTemperatureCurve, heatingCapacityFunctionofAirFlowFractionCurve,
                                            heatingCapacityFunctionofWaterFlowFractionCurve, heatingCOPFunctionofTemperatureCurve,
                                            heatingCOPFunctionofAirFlowFractionCurve, heatingCOPFunctionofWaterFlowFractionCurve,
                                            partLoadFractionCorrelationCurve);

    {
      auto curve = coil.heatingCapacityFunctionofTemperatureCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }

    {
      auto curve = coil.heatingCapacityFunctionofAirFlowFractionCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }

    {
      auto curve = coil.heatingCapacityFunctionofWaterFlowFractionCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }

    {
      auto curve = coil.heatingCOPFunctionofTemperatureCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }

    {
      auto curve = coil.heatingCOPFunctionofAirFlowFractionCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }

    {
      auto curve = coil.heatingCOPFunctionofWaterFlowFractionCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }

    {
      auto curve = coil.partLoadFractionCorrelationCurve();
      EXPECT_FALSE(curve.handle().isNull());
    }
  }
}
