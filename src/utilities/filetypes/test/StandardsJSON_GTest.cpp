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

#include "../StandardsJSON.hpp"
#include "../StandardsJSON_Impl.hpp"

// Note JM 2018-11-09: he two embedded standards file in model are tested already by SpaceType_GTest and MasslessOpaqueMaterial_GTest

TEST(Filetypes, StandardsJSON_Load_Valid) {
  std::string test_json = R"(
    {
      "space_types": [
        {
          "template": "189.1-2009",
          "building_type": "FullServiceRestaurant",
          "space_type": "Dining",
          "electric_equipment_per_area": 4.080001756670937,
          "cooling_setpoint_schedule": "FullServiceRestaurant ClgSetp",
          "service_water_heating_peak_flow_rate": null,
          "notes": null
        },
        {
          "template": "189.1-2009",
          "building_type": "FullServiceRestaurant",
          "space_type": "Kitchen",
          "electric_equipment_per_area": 25.500010979193352,
          "cooling_setpoint_schedule": "FullServiceRestaurant ClgSetp Kitchen",
          "service_water_heating_peak_flow_rate": 133.0,
          "notes": null
        }
      ]
    })";

  // We use the factory method to avoid gtest throwing
  EXPECT_TRUE(openstudio::StandardsJSON::load(test_json));
}

TEST(Filetypes, StandardsJSON_Load_NotRootObject) {
  std::string test_json = R"(
    [
      {
        "template": "189.1-2009",
        "building_type": "FullServiceRestaurant",
        "space_type": "Dining",
        "electric_equipment_per_area": 4.080001756670937,
        "cooling_setpoint_schedule": "FullServiceRestaurant ClgSetp",
        "service_water_heating_peak_flow_rate": null,
        "notes": null
      },
      {
        "template": "189.1-2009",
        "building_type": "FullServiceRestaurant",
        "space_type": "Kitchen",
        "electric_equipment_per_area": 25.500010979193352,
        "cooling_setpoint_schedule": "FullServiceRestaurant ClgSetp Kitchen",
        "service_water_heating_peak_flow_rate": 133.0,
        "notes": null
      }
    ]
  )";

  // We use the factory method to avoid gtest throwing
  EXPECT_FALSE(openstudio::StandardsJSON::load(test_json));
}

TEST(Filetypes, StandardsJSON_Load_EmptyArr) {
  std::string test_json = R"(
    {
      "space_types": [ ]
    })";

  // We use the factory method to avoid gtest throwing
  EXPECT_FALSE(openstudio::StandardsJSON::load(test_json));
}

TEST(Filetypes, StandardsJSON_Load_NestedObjects) {
  std::string test_json = R"(
    {
      "space_types": [
        {
          "template": "189.1-2009",
          "building_type": "FullServiceRestaurant",
          "space_type": "Dining",
          "electric_equipment_per_area": 4.080001756670937,
          "cooling_setpoint_schedule": "FullServiceRestaurant ClgSetp",
          "service_water_heating_peak_flow_rate": null,
          "Nested_object": ["a", "b"],
          "notes": null
        },
        {
          "template": "189.1-2009",
          "building_type": "FullServiceRestaurant",
          "space_type": "Kitchen",
          "electric_equipment_per_area": 25.500010979193352,
          "cooling_setpoint_schedule": "FullServiceRestaurant ClgSetp Kitchen",
          "service_water_heating_peak_flow_rate": 133.0,
          "notes": null
        }
      ]
    })";

  // We use the factory method to avoid gtest throwing
  EXPECT_FALSE(openstudio::StandardsJSON::load(test_json));
}
