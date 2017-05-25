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

#include "../AirLoopHVACUnitarySystem.hpp"
#include "../AirLoopHVACUnitarySystem_Impl.hpp"
#include "../UnitarySystemPerformanceMultispeed.hpp"
#include "../UnitarySystemPerformanceMultispeed_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, UnitarySystemPerformanceMultispeed_DefaultConstructors)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    UnitarySystemPerformanceMultispeed testObject = UnitarySystemPerformanceMultispeed(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, UnitarySystemPerformanceMultispeed_GetterSetters)
{
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

TEST_F(ModelFixture, UnitarySystemPerformanceMultispeed_SupplyAirflowRatioField)
{
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

TEST_F(ModelFixture, UnitarySystemPerformanceMultispeed_addToAirLoopHVACUnitarySystem)
{
  Model m;
  UnitarySystemPerformanceMultispeed perf = UnitarySystemPerformanceMultispeed(m);
  AirLoopHVACUnitarySystem airloopsys = AirLoopHVACUnitarySystem(m);

  EXPECT_TRUE(airloopsys.setDesignSpecificationMultispeedObject(perf));
  ASSERT_TRUE(airloopsys.designSpecificationMultispeedObject());
  EXPECT_EQ(airloopsys.designSpecificationMultispeedObject().get().handle(), perf.handle());
  airloopsys.resetDesignSpecificationMultispeedObject();
  ASSERT_FALSE(airloopsys.designSpecificationMultispeedObject());

}
