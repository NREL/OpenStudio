/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "../PlantEquipmentOperationCoolingLoad.hpp"
#include "../PlantEquipmentOperationCoolingLoad_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../BoilerHotWater.hpp"
#include "../BoilerHotWater_Impl.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,PlantEquipmentOperationCoolingLoad) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
    {
      model::Model m;
      model::PlantEquipmentOperationCoolingLoad plantOperationScheme(m);
      exit(0);
    } ,
    ::testing::ExitedWithCode(0), "" );

  {
    model::Model m;
    model::PlantEquipmentOperationCoolingLoad plantOperationScheme(m);
    BoilerHotWater boiler(m);

    auto upperLimits = plantOperationScheme.loadRangeUpperLimits();
    EXPECT_EQ(1u,upperLimits.size());
    EXPECT_TRUE(plantOperationScheme.equipment(upperLimits.back()).empty());

    EXPECT_TRUE(plantOperationScheme.addEquipment(boiler));
    EXPECT_EQ(1u,plantOperationScheme.equipment(upperLimits.back()).size());
    EXPECT_FALSE(plantOperationScheme.addEquipment(boiler));

    std::vector<model::HVACComponent> equipment = {boiler};
    EXPECT_TRUE(plantOperationScheme.addLoadRange(1000.0,equipment));
    upperLimits = plantOperationScheme.loadRangeUpperLimits();
    EXPECT_EQ(2u,upperLimits.size());
    EXPECT_FLOAT_EQ(1000.0,upperLimits[0]);
    EXPECT_EQ(1u,plantOperationScheme.equipment(upperLimits[0]).size());
    EXPECT_EQ(1u,plantOperationScheme.equipment(upperLimits[1]).size());

    auto result = plantOperationScheme.removeLoadRange(1000.0);
    EXPECT_EQ(equipment,result);
    upperLimits = plantOperationScheme.loadRangeUpperLimits();
    EXPECT_EQ(1u,upperLimits.size());

    BoilerHotWater boiler2(m);
    EXPECT_TRUE(plantOperationScheme.addEquipment(boiler2));
    EXPECT_EQ(2u,plantOperationScheme.equipment(upperLimits.back()).size());

    equipment = {boiler2};
    EXPECT_TRUE(plantOperationScheme.addLoadRange(1000.0,equipment));
    upperLimits = plantOperationScheme.loadRangeUpperLimits();
    EXPECT_EQ(2u,upperLimits.size());
    EXPECT_FLOAT_EQ(1000.0,upperLimits[0]);
    EXPECT_EQ(1u,plantOperationScheme.equipment(upperLimits[0]).size());
    EXPECT_EQ(2u,plantOperationScheme.equipment(upperLimits[1]).size());

    EXPECT_FALSE(plantOperationScheme.removeEquipment(1000.0,boiler));
    EXPECT_TRUE(plantOperationScheme.removeEquipment(1000.0,boiler2));
    EXPECT_EQ(0u,plantOperationScheme.equipment(upperLimits[0]).size());
  }
}

