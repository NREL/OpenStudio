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
#include "../CoilWaterHeatingAirToWaterHeatPump.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPump_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../TableMultiVariableLookup.hpp"
#include "../TableMultiVariableLookup_Impl.hpp"

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

    TableMultiVariableLookup heatingCapacityFunctionofTemperatureCurve(m, 1);
    TableMultiVariableLookup heatingCapacityFunctionofAirFlowFractionCurve(m, 1);
    TableMultiVariableLookup heatingCapacityFunctionofWaterFlowFractionCurve(m, 1);
    TableMultiVariableLookup heatingCOPFunctionofTemperatureCurve(m, 1);
    TableMultiVariableLookup heatingCOPFunctionofAirFlowFractionCurve(m, 1);
    TableMultiVariableLookup heatingCOPFunctionofWaterFlowFractionCurve(m, 1);
    TableMultiVariableLookup partLoadFractionCorrelationCurve(m, 1);

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
