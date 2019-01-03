/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../RefrigerationWalkInZoneBoundary.hpp"
#include "../RefrigerationWalkInZoneBoundary_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationWalkInZoneBoundary_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model model;
    RefrigerationWalkInZoneBoundary testObject = RefrigerationWalkInZoneBoundary(model);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, RefrigerationWalkInZoneBoundary_Remove)
{
  Model model;
  RefrigerationWalkInZoneBoundary testObject = RefrigerationWalkInZoneBoundary(model);

  std::vector<RefrigerationWalkInZoneBoundary> refrigerationWalkInZoneBoundaries = model.getModelObjects<RefrigerationWalkInZoneBoundary>();
  EXPECT_EQ(1, refrigerationWalkInZoneBoundaries.size());

  testObject.remove();

  refrigerationWalkInZoneBoundaries = model.getModelObjects<RefrigerationWalkInZoneBoundary>();
  EXPECT_EQ(0, refrigerationWalkInZoneBoundaries.size());
}

TEST_F(ModelFixture, RefrigerationWalkInZoneBoundary_CloneOneModelWithDefaultData)
{
  Model model;
  RefrigerationWalkInZoneBoundary testObject = RefrigerationWalkInZoneBoundary(model);

  RefrigerationWalkInZoneBoundary testObjectClone = testObject.clone(model).cast<RefrigerationWalkInZoneBoundary>();

  EXPECT_DOUBLE_EQ(43.4, testObjectClone.totalInsulatedSurfaceAreaFacingZone());
  EXPECT_DOUBLE_EQ(0.235, testObjectClone.insulatedSurfaceUValueFacingZone());
  EXPECT_EQ("StripCurtain", testObjectClone.stockingDoorOpeningProtectionTypeFacingZone());
  EXPECT_DOUBLE_EQ(2.0, testObjectClone.areaofStockingDoorsFacingZone());
  EXPECT_DOUBLE_EQ(2.0, testObjectClone.heightofStockingDoorsFacingZone());
}

TEST_F(ModelFixture, RefrigerationWalkInZoneBoundary_CloneOneModelWithCustomData)
{
  Model model;
  ThermalZone thermalZone(model);
  Schedule discreteSchedule = model.alwaysOnDiscreteSchedule();
  RefrigerationWalkInZoneBoundary testObject = RefrigerationWalkInZoneBoundary(model);
  testObject.setTotalInsulatedSurfaceAreaFacingZone(999.0);
  testObject.setStockingDoorOpeningProtectionTypeFacingZone("AirCurtain");
  testObject.setInsulatedSurfaceUValueFacingZone(999.0);
  testObject.setAreaofStockingDoorsFacingZone(999.0);
  testObject.setHeightofStockingDoorsFacingZone(999.0);
  testObject.setStockingDoorUValueFacingZone(999.0);
  testObject.setThermalZone(thermalZone);
  testObject.setGlassReachInDoorOpeningScheduleFacingZone(discreteSchedule);
  testObject.setStockingDoorOpeningScheduleFacingZone(discreteSchedule);

  RefrigerationWalkInZoneBoundary testObjectClone = testObject.clone(model).cast<RefrigerationWalkInZoneBoundary>();
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.totalInsulatedSurfaceAreaFacingZone());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.insulatedSurfaceUValueFacingZone());
  EXPECT_EQ("AirCurtain", testObjectClone.stockingDoorOpeningProtectionTypeFacingZone());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.areaofStockingDoorsFacingZone());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.heightofStockingDoorsFacingZone());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.stockingDoorUValueFacingZone());
  EXPECT_FALSE(testObjectClone.thermalZone());
  EXPECT_EQ(testObjectClone.glassReachInDoorOpeningScheduleFacingZone().get().handle(), testObject.glassReachInDoorOpeningScheduleFacingZone().get().handle());
  EXPECT_EQ(testObjectClone.stockingDoorOpeningScheduleFacingZone().get().handle(), testObject.stockingDoorOpeningScheduleFacingZone().get().handle());
}

TEST_F(ModelFixture, RefrigerationWalkInZoneBoundary_CloneTwoModelsWithDefaultData)
{
  Model model;
  RefrigerationWalkInZoneBoundary testObject = RefrigerationWalkInZoneBoundary(model);

  RefrigerationWalkInZoneBoundary testObjectClone = testObject.clone(model).cast<RefrigerationWalkInZoneBoundary>();

  Model model2;

  RefrigerationWalkInZoneBoundary testObjectClone2 = testObject.clone(model2).cast<RefrigerationWalkInZoneBoundary>();

  EXPECT_DOUBLE_EQ(43.4, testObjectClone2.totalInsulatedSurfaceAreaFacingZone());
  EXPECT_DOUBLE_EQ(0.235, testObjectClone2.insulatedSurfaceUValueFacingZone());
  EXPECT_EQ("StripCurtain", testObjectClone2.stockingDoorOpeningProtectionTypeFacingZone());
  EXPECT_DOUBLE_EQ(2.0, testObjectClone2.areaofStockingDoorsFacingZone());
  EXPECT_DOUBLE_EQ(2.0, testObjectClone2.heightofStockingDoorsFacingZone());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}

