/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ZoneVentilationDesignFlowRate.hpp"
#include "../ZoneVentilationDesignFlowRate_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ZoneVentilationDesignFlowRate)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     Model m;
     ZoneVentilationDesignFlowRate zv(m);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

/** Tests that the designFlowRateCalculationMethod is set appropriately and the other inputs cleared
 * when using the setXXX methods for flow rates
 */
TEST_F(ModelFixture,ZoneVentilationDesignFlowRate_setFlow)
{
  Model m;

  ZoneVentilationDesignFlowRate zv(m);

  // Flow/Zone
  ASSERT_TRUE(zv.setDesignFlowRate(12.05));
  EXPECT_DOUBLE_EQ(12.05, zv.designFlowRate());
  ASSERT_EQ("Flow/Zone", zv.designFlowRateCalculationMethod());
  ASSERT_FALSE(zv.flowRateperZoneFloorArea());
  ASSERT_FALSE(zv.flowRateperPerson());
  ASSERT_FALSE(zv.airChangesperHour());

  // Flow/Area
  ASSERT_TRUE(zv.setFlowRateperZoneFloorArea(10.05));
  EXPECT_DOUBLE_EQ(10.05, zv.flowRateperZoneFloorArea());
  ASSERT_EQ("Flow/Area", zv.designFlowRateCalculationMethod());
  ASSERT_FALSE(zv.designFlowRate());
  ASSERT_FALSE(zv.flowRateperPerson());
  ASSERT_FALSE(zv.airChangesperHour());

  // Flow/Person
  ASSERT_TRUE(zv.setFlowRateperPerson(15.5));
  EXPECT_DOUBLE_EQ(15.5, zv.flowRateperPerson());
  ASSERT_EQ("Flow/Person", zv.designFlowRateCalculationMethod());
  ASSERT_FALSE(zv.designFlowRate());
  ASSERT_FALSE(zv.flowRateperZoneFloorArea());
  ASSERT_FALSE(zv.airChangesperHour());

  // AirChanges/Hour
  ASSERT_TRUE(zv.setAirChangesperHour(1.05));
  EXPECT_DOUBLE_EQ(1.05, zv.airChangesperHour());
  ASSERT_EQ("AirChanges/Hour", zv.designFlowRateCalculationMethod());
  ASSERT_FALSE(zv.designFlowRate());
  ASSERT_FALSE(zv.flowRateperZoneFloorArea());
  ASSERT_FALSE(zv.flowRateperPerson());
}


