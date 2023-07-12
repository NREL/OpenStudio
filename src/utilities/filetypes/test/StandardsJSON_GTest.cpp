/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
