/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "../CoilWaterHeatingAirToWaterHeatPump.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPump_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../TableMultiVariableLookup.hpp"
#include "../TableMultiVariableLookup_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,CoilWaterHeatingAirToWaterHeatPump)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  EXPECT_EXIT ( 
  {  
    Model m; 
    CoilWaterHeatingAirToWaterHeatPump coil(m); 
    coil.remove();

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );

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

    TableMultiVariableLookup heatingCapacityFunctionofTemperatureCurve(m,1);
    TableMultiVariableLookup heatingCapacityFunctionofAirFlowFractionCurve(m,1);
    TableMultiVariableLookup heatingCapacityFunctionofWaterFlowFractionCurve(m,1);
    TableMultiVariableLookup heatingCOPFunctionofTemperatureCurve(m,1);
    TableMultiVariableLookup heatingCOPFunctionofAirFlowFractionCurve(m,1);
    TableMultiVariableLookup heatingCOPFunctionofWaterFlowFractionCurve(m,1);
    TableMultiVariableLookup partLoadFractionCorrelationCurve(m,1);

    CoilWaterHeatingAirToWaterHeatPump coil(m,
      heatingCapacityFunctionofTemperatureCurve,
      heatingCapacityFunctionofAirFlowFractionCurve,
      heatingCapacityFunctionofWaterFlowFractionCurve,
      heatingCOPFunctionofTemperatureCurve,
      heatingCOPFunctionofAirFlowFractionCurve,
      heatingCOPFunctionofWaterFlowFractionCurve,
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

    m.save("/Users/kbenne/tabletest.idf",true);
  }
}


