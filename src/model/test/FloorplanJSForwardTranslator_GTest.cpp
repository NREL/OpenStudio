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

#include "../FloorplanJSForwardTranslator.hpp"
#include "../ThreeJSReverseTranslator.hpp"
#include "../ModelMerger.hpp"

#include "../Model.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../BuildingStory.hpp"
#include "../BuildingStory_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../BuildingUnit.hpp"
#include "../BuildingUnit_Impl.hpp"
#include "../DefaultConstructionSet.hpp"
#include "../DefaultConstructionSet_Impl.hpp"
#include "../RenderingColor.hpp"
#include "../RenderingColor_Impl.hpp"

#include "../../utilities/geometry/FloorplanJS.hpp"
#include "../../utilities/geometry/ThreeJS.hpp"

#include <json/json.h>

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture, FloorplanJSForwardTranslator) {
  std::string json("{\"application\":{\"currentSelections\":{\"story\":{\"id\":\"1\",\"name\":\"Story 1\",\"handle\":null,\"geometry_id\":\"2\",\"below_floor_plenum_height\":0,\"floor_to_ceiling_height\":0,\"multiplier\":0,\"spaces\":[{\"id\":\"3\",\"name\":\"Space 1 - 1\",\"color\":\"#007373\",\"handle\":null,\"face_id\":\"12\",\"daylighting_controls\":[],\"building_unit_id\":null,\"thermal_zone_id\":null,\"space_type_id\":null,\"construction_set_id\":null,\"type\":\"space\"}],\"windows\":[],\"shading\":[],\"images\":[]},\"space\":{\"id\":\"3\",\"name\":\"Space 1 - 1\",\"color\":\"#007373\",\"handle\":null,\"face_id\":\"12\",\"daylighting_controls\":[],\"building_unit_id\":null,\"thermal_zone_id\":null,\"space_type_id\":null,\"construction_set_id\":null,\"type\":\"space\"},\"shading\":null,\"image\":null,\"building_unit\":null,\"thermal_zone\":null,\"space_type\":null,\"tool\":\"Rectangle\",\"mode\":\"spaces\"},\"modes\":[\"spaces\",\"shading\",\"building_units\",\"thermal_zones\",\"space_types\",\"images\"],\"tools\":[\"Pan\",\"Drag\",\"Rectangle\",\"Polygon\",\"Eraser\",\"Select\",\"Map\",\"Fill\"],\"scale\":{}},\"project\":{\"config\":{\"units\":\"m\",\"language\":\"EN-US\",\"north_axis\":0},\"grid\":{\"visible\":true,\"spacing\":50},\"view\":{\"min_x\":-502.5817565917968,\"min_y\":-194.06196213425125,\"max_x\":497.4182434082031,\"max_y\":194.06196213425122},\"map\":{\"initialized\":true,\"enabled\":true,\"visible\":true,\"latitude\":39.7653,\"longitude\":-104.9863,\"zoom\":4.5,\"rotation\":0,\"elevation\":0},\"previous_story\":{\"visible\":true}},\"stories\":[{\"id\":\"1\",\"name\":\"Story 1\",\"handle\":null,\"below_floor_plenum_height\":0,\"floor_to_ceiling_height\":0,\"multiplier\":0,\"spaces\":[{\"id\":\"3\",\"name\":\"Space 1 - 1\",\"color\":\"#007373\",\"handle\":null,\"face_id\":\"12\",\"daylighting_controls\":[],\"building_unit_id\":null,\"thermal_zone_id\":null,\"space_type_id\":null,\"construction_set_id\":null,\"type\":\"space\"}],\"windows\":[],\"shading\":[],\"images\":[],\"geometry\":{\"id\":\"2\",\"vertices\":[{\"id\":\"4\",\"x\":-152.22119140625,\"y\":0.5,\"edge_ids\":[\"8\",\"11\"]},{\"id\":\"5\",\"x\":99.5,\"y\":0.5,\"edge_ids\":[\"8\",\"9\"]},{\"id\":\"6\",\"x\":99.5,\"y\":-99.5,\"edge_ids\":[\"9\",\"10\"]},{\"id\":\"7\",\"x\":-152.22119140625,\"y\":-99.5,\"edge_ids\":[\"10\",\"11\"]}],\"edges\":[{\"id\":\"8\",\"vertex_ids\":[\"4\",\"5\"],\"face_ids\":[\"12\"]},{\"id\":\"9\",\"vertex_ids\":[\"5\",\"6\"],\"face_ids\":[\"12\"]},{\"id\":\"10\",\"vertex_ids\":[\"6\",\"7\"],\"face_ids\":[\"12\"]},{\"id\":\"11\",\"vertex_ids\":[\"7\",\"4\"],\"face_ids\":[\"12\"]}],\"faces\":[{\"id\":\"12\",\"edge_ids\":[\"8\",\"9\",\"10\",\"11\"],\"edge_order\":[1,1,1,1]}]}}],\"building_units\":[],\"thermal_zones\":[],\"space_types\":[],\"construction_sets\":[],\"windows\":[],\"daylighting_controls\":[]}");

  boost::optional<FloorplanJS> floorplan = FloorplanJS::load(json);
  ASSERT_TRUE(floorplan);

  Json::CharReaderBuilder rbuilder;
  std::istringstream ss(json);
  Json::Value value;
  ASSERT_TRUE(Json::parseFromStream(rbuilder, ss, &value, nullptr));

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

  ASSERT_TRUE(value.isMember("thermal_zones"));
  ASSERT_TRUE(value["thermal_zones"].isArray());
  EXPECT_EQ(0u, value["thermal_zones"].size());

  ASSERT_TRUE(value.isMember("building_units"));
  ASSERT_TRUE(value["building_units"].isArray());
  EXPECT_EQ(0u, value["building_units"].size());

  ASSERT_TRUE(value.isMember("construction_sets"));
  ASSERT_TRUE(value["construction_sets"].isArray());
  EXPECT_EQ(0u, value["construction_sets"].size());

  // translate the floorplan to a model
  boost::optional<Model> model;
  {
    ThreeScene threeScene = floorplan->toThreeScene(false);
    std::string j = threeScene.toJSON(false);

    bool found = false;
    for (const auto& child : threeScene.object().children()){
      if (child.userData().spaceName() == "Space 1 - 1"){
        found = true;
        EXPECT_EQ("", child.userData().spaceHandle());
        EXPECT_EQ("Story 1", child.userData().buildingStoryName());
        EXPECT_EQ("", child.userData().buildingStoryHandle());
        EXPECT_EQ("", child.userData().spaceTypeName());
        EXPECT_EQ("", child.userData().spaceTypeHandle());
        EXPECT_EQ("", child.userData().thermalZoneName());
        EXPECT_EQ("", child.userData().thermalZoneHandle());
        EXPECT_EQ("", child.userData().buildingUnitName());
        EXPECT_EQ("", child.userData().buildingUnitHandle());
        EXPECT_EQ("", child.userData().constructionSetName());
        EXPECT_EQ("", child.userData().constructionSetHandle());
      }
    }
    EXPECT_TRUE(found);

    ThreeJSReverseTranslator rt;
    model = rt.modelFromThreeJS(threeScene);
  }
  ASSERT_TRUE(model);

  // check the model
  ASSERT_EQ(1u, model->getConcreteModelObjects<Space>().size());
  Space space = model->getConcreteModelObjects<Space>()[0];
  EXPECT_EQ("Space 1 - 1", space.nameString());

  ASSERT_EQ(1u, model->getConcreteModelObjects<BuildingStory>().size());
  BuildingStory buildingStory = model->getConcreteModelObjects<BuildingStory>()[0];
  EXPECT_EQ("Story 1", buildingStory.nameString());

  EXPECT_EQ(0u, model->getConcreteModelObjects<SpaceType>().size());
  EXPECT_EQ(0u, model->getConcreteModelObjects<ThermalZone>().size());
  EXPECT_EQ(0u, model->getConcreteModelObjects<BuildingUnit>().size());
  EXPECT_EQ(0u, model->getConcreteModelObjects<DefaultConstructionSet>().size());

  SpaceType spaceType(*model);
  EXPECT_TRUE(spaceType.setName("SpaceType 1"));

  ThermalZone thermalZone(*model);
  EXPECT_TRUE(thermalZone.setName("ThermalZone 1"));

  BuildingUnit buildingUnit(*model);
  EXPECT_TRUE(buildingUnit.setName("BuildingUnit 1"));

  DefaultConstructionSet defaultConstructionSet(*model);
  EXPECT_TRUE(defaultConstructionSet.setName("DefaultConstructionSet 1"));

  EXPECT_EQ(1u, model->getConcreteModelObjects<SpaceType>().size());
  EXPECT_EQ(1u, model->getConcreteModelObjects<ThermalZone>().size());
  EXPECT_EQ(1u, model->getConcreteModelObjects<BuildingUnit>().size());
  EXPECT_EQ(1u, model->getConcreteModelObjects<DefaultConstructionSet>().size());

  // update the floorplan with objects from the model
  // should match up the space and story, add the other new objects
  model::FloorplanJSForwardTranslator ft;
  FloorplanJS floorplan2 = ft.updateFloorplanJS(*floorplan, *model, false);

  std::stringstream ss2(floorplan2.toJSON());
  ASSERT_TRUE(Json::parseFromStream(rbuilder, ss2, &value, nullptr));
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

  ASSERT_TRUE(value.isMember("thermal_zones"));
  ASSERT_TRUE(value["thermal_zones"].isArray());
  ASSERT_EQ(1u, value["thermal_zones"].size());
  EXPECT_EQ("ThermalZone 1", value["thermal_zones"][0].get("name", "").asString());

  ASSERT_TRUE(value.isMember("building_units"));
  ASSERT_TRUE(value["building_units"].isArray());
  ASSERT_EQ(1u, value["building_units"].size());
  EXPECT_EQ("BuildingUnit 1", value["building_units"][0].get("name", "").asString());

  ASSERT_TRUE(value.isMember("construction_sets"));
  ASSERT_TRUE(value["construction_sets"].isArray());
  ASSERT_EQ(1u, value["construction_sets"].size());
  EXPECT_EQ("DefaultConstructionSet 1", value["construction_sets"][0].get("name", "").asString());

  {
    std::string f = floorplan2.toJSON();

    ThreeScene threeScene = floorplan2.toThreeScene(false);
    std::string j = threeScene.toJSON(false);

    // the space is not hooked up to this stuff
    bool found = false;
    for (const auto& child : threeScene.object().children()){
      if (child.userData().spaceName() == "Space 1 - 1"){
        found = true;
        EXPECT_EQ(toString(space.handle()), child.userData().spaceHandle());
        EXPECT_EQ("Story 1", child.userData().buildingStoryName());
        EXPECT_EQ(toString(buildingStory.handle()), child.userData().buildingStoryHandle());
        EXPECT_EQ("", child.userData().spaceTypeName());
        EXPECT_EQ("", child.userData().spaceTypeHandle());
        EXPECT_EQ("", child.userData().thermalZoneName());
        EXPECT_EQ("", child.userData().thermalZoneHandle());
        EXPECT_EQ("", child.userData().buildingUnitName());
        EXPECT_EQ("", child.userData().buildingUnitHandle());
        EXPECT_EQ("", child.userData().constructionSetName());
        EXPECT_EQ("", child.userData().constructionSetHandle());
      }
    }
    EXPECT_TRUE(found);
  }

  // hoook space up to other objects
  space.setSpaceType(spaceType);
  space.setThermalZone(thermalZone);
  space.setBuildingUnit(buildingUnit);
  space.setDefaultConstructionSet(defaultConstructionSet);

  FloorplanJS floorplan3 = ft.updateFloorplanJS(floorplan2, *model, false);

  std::stringstream ss3(floorplan3.toJSON());
  ASSERT_TRUE(Json::parseFromStream(rbuilder, ss3, &value, nullptr));
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

  ASSERT_TRUE(value.isMember("thermal_zones"));
  ASSERT_TRUE(value["thermal_zones"].isArray());
  ASSERT_EQ(1u, value["thermal_zones"].size());
  EXPECT_EQ("ThermalZone 1", value["thermal_zones"][0].get("name", "").asString());

  ASSERT_TRUE(value.isMember("building_units"));
  ASSERT_TRUE(value["building_units"].isArray());
  ASSERT_EQ(1u, value["building_units"].size());
  EXPECT_EQ("BuildingUnit 1", value["building_units"][0].get("name", "").asString());

  ASSERT_TRUE(value.isMember("construction_sets"));
  ASSERT_TRUE(value["construction_sets"].isArray());
  ASSERT_EQ(1u, value["construction_sets"].size());
  EXPECT_EQ("DefaultConstructionSet 1", value["construction_sets"][0].get("name", "").asString());

{
    std::string f = floorplan3.toJSON();

    ThreeScene threeScene = floorplan3.toThreeScene(false);
    std::string j = threeScene.toJSON(false);

    // the space is now hooked up to this stuff
    bool found = false;
    for (const auto& child : threeScene.object().children()){
      if (child.userData().spaceName() == "Space 1 - 1"){
        found = true;
        EXPECT_EQ(toString(space.handle()), child.userData().spaceHandle());
        EXPECT_EQ("Story 1", child.userData().buildingStoryName());
        EXPECT_EQ(toString(buildingStory.handle()), child.userData().buildingStoryHandle());
        EXPECT_EQ(spaceType.nameString(), child.userData().spaceTypeName());
        EXPECT_EQ(toString(spaceType.handle()), child.userData().spaceTypeHandle());
        EXPECT_EQ(thermalZone.nameString(), child.userData().thermalZoneName());
        EXPECT_EQ(toString(thermalZone.handle()), child.userData().thermalZoneHandle());
        EXPECT_EQ(buildingUnit.nameString(), child.userData().buildingUnitName());
        EXPECT_EQ(toString(buildingUnit.handle()), child.userData().buildingUnitHandle());
        EXPECT_EQ(defaultConstructionSet.nameString(), child.userData().constructionSetName());
        EXPECT_EQ(toString(defaultConstructionSet.handle()), child.userData().constructionSetHandle());
      }
    }
    EXPECT_TRUE(found);
  }

  // remove some resource objects
  spaceType.remove();
  thermalZone.remove();
  buildingUnit.remove();
  defaultConstructionSet.remove();

  FloorplanJS floorplan4 = ft.updateFloorplanJS(floorplan3, *model, true);

  std::stringstream ss4(floorplan4.toJSON());
  ASSERT_TRUE(Json::parseFromStream(rbuilder, ss4, &value, nullptr));
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

  ASSERT_TRUE(value.isMember("thermal_zones"));
  ASSERT_TRUE(value["thermal_zones"].isArray());
  EXPECT_EQ(0u, value["thermal_zones"].size());

  ASSERT_TRUE(value.isMember("building_units"));
  ASSERT_TRUE(value["building_units"].isArray());
  EXPECT_EQ(0u, value["building_units"].size());

  ASSERT_TRUE(value.isMember("construction_sets"));
  ASSERT_TRUE(value["construction_sets"].isArray());
  EXPECT_EQ(0u, value["construction_sets"].size());
}


TEST_F(ModelFixture, FloorplanJSForwardTranslator_Colors) {

  Model model;

  RenderingColor red(model);
  red.setRenderingRedValue(255);
  red.setRenderingGreenValue(0);
  red.setRenderingBlueValue(0);

  RenderingColor blue(model);
  blue.setRenderingRedValue(0);
  blue.setRenderingGreenValue(0);
  blue.setRenderingBlueValue(255);

  SpaceType redType(model);
  redType.setName("Red Type");
  redType.setRenderingColor(red);

  SpaceType blueType(model);
  blueType.setName("Blue Type");
  blueType.setRenderingColor(blue);

  // import current model content as library
  FloorplanJS floorplan;
  model::FloorplanJSForwardTranslator ft;
  floorplan = ft.updateFloorplanJS(floorplan, model, false);

  Json::CharReaderBuilder rbuilder;
  std::istringstream ss(floorplan.toJSON());
  std::string formattedErrors;
  Json::Value value;
  ASSERT_TRUE(Json::parseFromStream(rbuilder, ss, &value, &formattedErrors));

  ASSERT_TRUE(value.isMember("space_types"));
  ASSERT_TRUE(value["space_types"].isArray());

  ASSERT_EQ(2u, value["space_types"].size());

  // model.getSpaceTypes[0].name => "Blue Type"

  EXPECT_EQ("1", value["space_types"][0].get("id", "").asString());
  EXPECT_EQ("2", value["space_types"][1].get("id", "").asString());

  unsigned redIndex = std::numeric_limits<unsigned>::max();
  unsigned blueIndex = std::numeric_limits<unsigned>::max();
  if ("Red Type" == value["space_types"][0].get("name", "").asString()) {
    redIndex = 0;
    blueIndex = 1;
  } else {
    redIndex = 1;
    blueIndex = 0;
  }

  EXPECT_EQ("Red Type", value["space_types"][redIndex].get("name", "").asString());
  EXPECT_EQ("#FF0000", value["space_types"][redIndex].get("color", "").asString());
  EXPECT_EQ(toString(redType.handle()), value["space_types"][redIndex].get("handle", "").asString());

  EXPECT_EQ("Blue Type", value["space_types"][blueIndex].get("name", "").asString());
  EXPECT_EQ("#0000FF", value["space_types"][blueIndex].get("color", "").asString());
  EXPECT_EQ(toString(blueType.handle()), value["space_types"][blueIndex].get("handle", "").asString());

  Json::Value greenSpaceTypeValue(Json::objectValue);
  greenSpaceTypeValue["id"] = "3";
  greenSpaceTypeValue["name"] = "Green Type";
  greenSpaceTypeValue["color"] = "#00FF00";
  value["space_types"].append(greenSpaceTypeValue);

  Json::StreamWriterBuilder wbuilder;
  const std::string json_string = Json::writeString(wbuilder, value);
  FloorplanJS floorplan2(json_string);

  ThreeScene scene2 = floorplan2.toThreeScene(true);

  model::ThreeJSReverseTranslator rt;
  boost::optional<model::Model> model2 = rt.modelFromThreeJS(scene2);
  std::map<UUID, UUID> handleMapping = rt.handleMapping();

  ASSERT_TRUE(model2);
  EXPECT_EQ(3u, model2->getConcreteModelObjects<SpaceType>().size());
  ASSERT_TRUE(model2->getConcreteModelObjectByName<SpaceType>("Red Type"));
  ASSERT_TRUE(model2->getConcreteModelObjectByName<SpaceType>("Red Type")->renderingColor());
  EXPECT_EQ("#FF0000", model2->getConcreteModelObjectByName<SpaceType>("Red Type")->renderingColor()->colorString());
  ASSERT_TRUE(model2->getConcreteModelObjectByName<SpaceType>("Blue Type"));
  ASSERT_TRUE(model2->getConcreteModelObjectByName<SpaceType>("Blue Type")->renderingColor());
  EXPECT_EQ("#0000FF", model2->getConcreteModelObjectByName<SpaceType>("Blue Type")->renderingColor()->colorString());
  ASSERT_TRUE(model2->getConcreteModelObjectByName<SpaceType>("Green Type"));
  ASSERT_TRUE(model2->getConcreteModelObjectByName<SpaceType>("Green Type")->renderingColor());
  EXPECT_EQ("#00FF00", model2->getConcreteModelObjectByName<SpaceType>("Green Type")->renderingColor()->colorString());

  model::ModelMerger mm;
  mm.mergeModels(model, *model2, handleMapping);

  EXPECT_EQ(3u, model.getConcreteModelObjects<SpaceType>().size());
  ASSERT_TRUE(model.getConcreteModelObjectByName<SpaceType>("Red Type"));
  ASSERT_TRUE(model.getConcreteModelObjectByName<SpaceType>("Red Type")->renderingColor());
  EXPECT_EQ("#FF0000", model.getConcreteModelObjectByName<SpaceType>("Red Type")->renderingColor()->colorString());
  ASSERT_TRUE(model.getConcreteModelObjectByName<SpaceType>("Blue Type"));
  ASSERT_TRUE(model.getConcreteModelObjectByName<SpaceType>("Blue Type")->renderingColor());
  EXPECT_EQ("#0000FF", model.getConcreteModelObjectByName<SpaceType>("Blue Type")->renderingColor()->colorString());
  ASSERT_TRUE(model.getConcreteModelObjectByName<SpaceType>("Green Type"));
  ASSERT_TRUE(model.getConcreteModelObjectByName<SpaceType>("Green Type")->renderingColor());
  EXPECT_EQ("#00FF00", model.getConcreteModelObjectByName<SpaceType>("Green Type")->renderingColor()->colorString());

}
