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

#include "../FloorplanJSForwardTranslator.hpp"
#include "../Model.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SpaceType.hpp"

#include "../../utilities/geometry/FloorplanJS.hpp"
#include "../../utilities/geometry/ThreeJS.hpp"

#include <jsoncpp/json.h>

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture, FloorplanJSForwardTranslator) {
  std::string json("{\"application\":{\"currentSelections\":{\"story\":{\"id\":\"1\",\"name\":\"Story 1\",\"handle\":null,\"geometry_id\":\"2\",\"below_floor_plenum_height\":0,\"floor_to_ceiling_height\":0,\"multiplier\":0,\"spaces\":[{\"id\":\"3\",\"name\":\"Space 1 - 1\",\"color\":\"#007373\",\"handle\":null,\"face_id\":\"12\",\"daylighting_controls\":[],\"building_unit_id\":null,\"thermal_zone_id\":null,\"space_type_id\":null,\"construction_set_id\":null,\"type\":\"space\"}],\"windows\":[],\"shading\":[],\"images\":[]},\"space\":{\"id\":\"3\",\"name\":\"Space 1 - 1\",\"color\":\"#007373\",\"handle\":null,\"face_id\":\"12\",\"daylighting_controls\":[],\"building_unit_id\":null,\"thermal_zone_id\":null,\"space_type_id\":null,\"construction_set_id\":null,\"type\":\"space\"},\"shading\":null,\"image\":null,\"building_unit\":null,\"thermal_zone\":null,\"space_type\":null,\"tool\":\"Rectangle\",\"mode\":\"spaces\"},\"modes\":[\"spaces\",\"shading\",\"building_units\",\"thermal_zones\",\"space_types\",\"images\"],\"tools\":[\"Pan\",\"Drag\",\"Rectangle\",\"Polygon\",\"Eraser\",\"Select\",\"Map\",\"Fill\"],\"scale\":{}},\"project\":{\"config\":{\"units\":\"m\",\"language\":\"EN-US\",\"north_axis\":0},\"grid\":{\"visible\":true,\"spacing\":50},\"view\":{\"min_x\":-502.5817565917968,\"min_y\":-194.06196213425125,\"max_x\":497.4182434082031,\"max_y\":194.06196213425122},\"map\":{\"initialized\":true,\"enabled\":true,\"visible\":true,\"latitude\":39.7653,\"longitude\":-104.9863,\"zoom\":4.5,\"rotation\":0,\"elevation\":0},\"previous_story\":{\"visible\":true}},\"stories\":[{\"id\":\"1\",\"name\":\"Story 1\",\"handle\":null,\"below_floor_plenum_height\":0,\"floor_to_ceiling_height\":0,\"multiplier\":0,\"spaces\":[{\"id\":\"3\",\"name\":\"Space 1 - 1\",\"color\":\"#007373\",\"handle\":null,\"face_id\":\"12\",\"daylighting_controls\":[],\"building_unit_id\":null,\"thermal_zone_id\":null,\"space_type_id\":null,\"construction_set_id\":null,\"type\":\"space\"}],\"windows\":[],\"shading\":[],\"images\":[],\"geometry\":{\"id\":\"2\",\"vertices\":[{\"id\":\"4\",\"x\":-152.22119140625,\"y\":0.5,\"edge_ids\":[\"8\",\"11\"]},{\"id\":\"5\",\"x\":99.5,\"y\":0.5,\"edge_ids\":[\"8\",\"9\"]},{\"id\":\"6\",\"x\":99.5,\"y\":-99.5,\"edge_ids\":[\"9\",\"10\"]},{\"id\":\"7\",\"x\":-152.22119140625,\"y\":-99.5,\"edge_ids\":[\"10\",\"11\"]}],\"edges\":[{\"id\":\"8\",\"vertex_ids\":[\"4\",\"5\"],\"face_ids\":[\"12\"]},{\"id\":\"9\",\"vertex_ids\":[\"5\",\"6\"],\"face_ids\":[\"12\"]},{\"id\":\"10\",\"vertex_ids\":[\"6\",\"7\"],\"face_ids\":[\"12\"]},{\"id\":\"11\",\"vertex_ids\":[\"7\",\"4\"],\"face_ids\":[\"12\"]}],\"faces\":[{\"id\":\"12\",\"edge_ids\":[\"8\",\"9\",\"10\",\"11\"],\"edge_order\":[1,1,1,1]}]}}],\"building_units\":[],\"thermal_zones\":[],\"space_types\":[],\"construction_sets\":[],\"windows\":[],\"daylighting_controls\":[]}");

  boost::optional<FloorplanJS> floorplan = FloorplanJS::load(json);
  ASSERT_TRUE(floorplan);

  Json::Reader reader;
  Json::Value value;
  ASSERT_TRUE(reader.parse(json, value));
  ASSERT_TRUE(value.isMember("stories"));
  ASSERT_TRUE(value["stories"].isArray());
  ASSERT_EQ(1u, value["stories"].size());
  EXPECT_EQ("Story 1", value["stories"][0].get("name", "").asString());

  ASSERT_TRUE(value["stories"][0].isMember("spaces"));
  ASSERT_TRUE(value["stories"][0]["spaces"].isArray());
  ASSERT_EQ(1u, value["stories"][0]["spaces"].size());
  EXPECT_EQ("Space 1 - 1", value["stories"][0]["spaces"][0].get("name", "").asString());

  ASSERT_TRUE(value.isMember("space_types"));
  ASSERT_TRUE(value["space_types"].isArray());
  EXPECT_EQ(0u, value["space_types"].size());
  

  ThreeScene threeScene = floorplan->toThreeScene(false);
  std::string j = threeScene.toJSON(false);

  bool found = false;
  for (const auto& child : threeScene.object().children()){
    if (child.userData().spaceName() == "Space 1 - 1"){
      found = true;
      EXPECT_EQ("Story 1", child.userData().buildingStoryName());
    }
  }
  EXPECT_TRUE(found);

  // create model with a space type
  Model model;

  SpaceType spaceType(model);
  EXPECT_TRUE(spaceType.setName("SpaceType 1"));

  model::FloorplanJSForwardTranslator ft;
  FloorplanJS floorplan2 = ft.updateFloorplanJSResources(*floorplan, model);

  ASSERT_TRUE(reader.parse(floorplan2.toJSON(), value));
  ASSERT_TRUE(value.isMember("stories"));
  ASSERT_TRUE(value["stories"].isArray());
  ASSERT_EQ(1u, value["stories"].size());
  EXPECT_EQ("Story 1", value["stories"][0].get("name", "").asString());

  ASSERT_TRUE(value["stories"][0].isMember("spaces"));
  ASSERT_TRUE(value["stories"][0]["spaces"].isArray());
  ASSERT_EQ(1u, value["stories"][0]["spaces"].size());
  EXPECT_EQ("Space 1 - 1", value["stories"][0]["spaces"][0].get("name", "").asString());

  ASSERT_TRUE(value.isMember("space_types"));
  ASSERT_TRUE(value["space_types"].isArray());
  ASSERT_EQ(1u, value["space_types"].size());
  EXPECT_EQ("SpaceType 1", value["space_types"][0].get("name", "").asString());
}
