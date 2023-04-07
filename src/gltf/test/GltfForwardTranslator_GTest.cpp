/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "GltfFixture.hpp"

#include "../GltfForwardTranslator.hpp"
#include "../GltfMetaData.hpp"
#include "../GltfUserData.hpp"
#include "../GltfModelObjectMetaData.hpp"
#include "../GltfUtils.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/ConstructionAirBoundary.hpp"
#include "../../model/Construction.hpp"

#include "../../model/PlanarSurface.hpp"
#include "../../model/PlanarSurface_Impl.hpp"
#include "../../model/PlanarSurfaceGroup.hpp"
#include "../../model/PlanarSurfaceGroup_Impl.hpp"
#include "../../model/BuildingStory.hpp"
#include "../../model/BuildingStory_Impl.hpp"
#include "../../model/BuildingUnit.hpp"
#include "../../model/BuildingUnit_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/DefaultConstructionSet.hpp"
#include "../../model/DefaultConstructionSet_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include "../../osversion/VersionTranslator.hpp"

#include "../../utilities/core/Json.hpp"

#include <resources.hxx>
#include <OpenStudio.hxx>

#include <json/json.h>

#include <algorithm>
#include <json/value.h>

using namespace openstudio::gltf;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(GltfFixture, GltfForwardTranslator_ExampleModel_FullTest) {
  GltfForwardTranslator ft;

  Model m = exampleModel();
  std::string s = ft.modelToGLTFString(m);

  auto planarSurfaces = m.getModelObjects<PlanarSurface>();
  auto nPlanarSurfaces = planarSurfaces.size();

  // auto planarSurfaceGroups = m.getModelObjects<PlanarSurfaceGroup>();
  // auto nPlanarSurfaceGroups = planarSurfaceGroups.size();

  auto buildingStories = m.getConcreteModelObjects<BuildingStory>();
  auto nBuildingStories = buildingStories.size();
  EXPECT_EQ(1, nBuildingStories);

  auto buildingUnits = m.getConcreteModelObjects<BuildingUnit>();
  auto nBuildingUnits = buildingUnits.size();
  EXPECT_EQ(0, nBuildingUnits);

  auto thermalZones = m.getConcreteModelObjects<ThermalZone>();
  auto nThermalZones = thermalZones.size();
  EXPECT_EQ(1, nThermalZones);

  auto airLoopHVACs = m.getConcreteModelObjects<AirLoopHVAC>();
  auto nAirLoopHVACs = airLoopHVACs.size();
  EXPECT_EQ(1, nAirLoopHVACs);

  auto spaceTypes = m.getConcreteModelObjects<SpaceType>();
  auto nSpaceTypes = spaceTypes.size();
  EXPECT_EQ(1, nSpaceTypes);

  auto defaultConstructionSets = m.getConcreteModelObjects<DefaultConstructionSet>();
  auto nDefaultConstructionSets = defaultConstructionSets.size();
  EXPECT_EQ(1, nDefaultConstructionSets);

  auto spaces = m.getConcreteModelObjects<Space>();
  auto nSpaces = spaces.size();
  EXPECT_EQ(4, nSpaces);

  auto nMetaDataSize = nBuildingStories + nBuildingUnits + nThermalZones + nAirLoopHVACs + nSpaceTypes + nDefaultConstructionSets + nSpaces;

  Json::Reader reader;
  Json::Value root;
  bool parsingSuccessful = reader.parse(s, root);
  ASSERT_TRUE(parsingSuccessful);

  std::vector<std::pair<std::string, Json::ValueType>> allKeysAndTypes{
    {"accessors", Json::arrayValue}, {"asset", Json::objectValue}, {"bufferViews", Json::arrayValue}, {"buffers", Json::arrayValue},
    {"materials", Json::arrayValue}, {"meshes", Json::arrayValue}, {"nodes", Json::arrayValue},       {"scenes", Json::arrayValue},
  };
  for (const auto& [key, keyType] : allKeysAndTypes) {
    assertKeyAndType(root, key, keyType);
  }

  {
    // Accessors

    // We except 3 accessors for each surfaces:
    // * 1 SCALAR for indices
    // * 1 VEC3 for Coordinates
    // * 1 VEC3 for Normals
    auto accessors = root.get("accessors", Json::arrayValue);
    EXPECT_EQ(nPlanarSurfaces * 3, accessors.size());
    EXPECT_EQ(nPlanarSurfaces, std::count_if(accessors.begin(), accessors.end(),
                                             [](const auto& accessor) { return accessor.get("type", "").asString() == "SCALAR"; }));
    EXPECT_EQ(nPlanarSurfaces * 2,
              std::count_if(accessors.begin(), accessors.end(), [](const auto& accessor) { return accessor.get("type", "").asString() == "VEC3"; }));
  }

  {
    // Asset
    assertKeyAndType(root, "asset", Json::objectValue);
    auto asset = root.get("asset", Json::objectValue);
    assertKeyAndType(asset, "generator", Json::stringValue);
    EXPECT_EQ("OpenStudio", asset.get("generator", Json::stringValue).asString());
    assertKeyAndType(asset, "version", Json::stringValue);
    EXPECT_EQ("2.0", asset.get("version", Json::stringValue).asString());
  }

  {
    // BufferViews
    auto bufferViews = root.get("bufferViews", Json::arrayValue);
    {
      // First one
      auto bufferView = bufferViews[0];
      assertKeyAndType(bufferView, "buffer", Json::intValue);
      EXPECT_EQ(0, bufferView.get("buffer", -999).asInt());
      assertKeyAndType(bufferView, "byteLength", Json::intValue);
      EXPECT_EQ(268, bufferView.get("byteLength", -999).asInt());
      assertKeyAndType(bufferView, "target", Json::intValue);
      EXPECT_EQ(34963, bufferView.get("target", -999).asInt());

      // Not needed for the first one
      EXPECT_FALSE(checkKey(bufferView, "byteOffset"));
      EXPECT_FALSE(checkKey(bufferView, "byteStride"));
    }
    {
      // Second one
      auto bufferView = bufferViews[1];
      assertKeyAndType(bufferView, "buffer", Json::intValue);
      EXPECT_EQ(0, bufferView.get("buffer", -999).asInt());
      assertKeyAndType(bufferView, "byteLength", Json::intValue);
      EXPECT_EQ(3072, bufferView.get("byteLength", -999).asInt());
      assertKeyAndType(bufferView, "target", Json::intValue);
      EXPECT_EQ(34962, bufferView.get("target", -999).asInt());

      assertKeyAndType(bufferView, "byteOffset", Json::intValue);
      EXPECT_EQ(268, bufferView.get("byteOffset", -999).asInt());
      assertKeyAndType(bufferView, "byteStride", Json::intValue);
      EXPECT_EQ(12, bufferView.get("byteStride", -999).asInt());
    }
  }

  {
    // Buffers: there's only one
    assertKeyAndType(root, "buffers", Json::arrayValue);
    auto buffers = root.get("buffers", Json::arrayValue);
    ASSERT_EQ(1, buffers.size());
    auto buffer = buffers[0];
    ASSERT_TRUE(buffer.isObject());

    assertKeyAndType(buffer, "byteLength", Json::intValue);
    EXPECT_EQ(3340, buffer.get("byteLength", -999).asInt());

    std::string startsWith = "data:application/octet-stream;base64,";
    assertKeyAndType(buffer, "uri", Json::stringValue);
    auto uri = buffer.get("uri", "").asString();
    EXPECT_EQ(startsWith, uri.substr(0, startsWith.size()));
  }

  {
    // Materials
    auto materials = root.get("materials", Json::arrayValue);
    EXPECT_EQ(9, materials.size());
    // This is already sorted
    std::vector<std::string> expectedMaterialNames{"BuildingShading", "Door", "Floor", "InteriorPartitionSurface", "RoofCeiling", "SiteShading",
                                                   "SpaceShading",    "Wall", "Window"};
    std::vector<std::string> foundMaterialNames;
    std::vector<std::pair<std::string, Json::ValueType>> theKeysAndType{
      {"doubleSided", Json::booleanValue},
      {"emissiveFactor", Json::arrayValue},
      {"name", Json::stringValue},
      {"pbrMetallicRoughness", Json::objectValue},
    };
    for (auto& material : materials) {
      for (const auto& [key, keyType] : theKeysAndType) {
        assertKeyAndType(material, key, keyType);
      }
      foundMaterialNames.emplace_back(material.get("name", "").asString());
    }
    std::sort(foundMaterialNames.begin(), foundMaterialNames.end());
    EXPECT_EQ(expectedMaterialNames, foundMaterialNames);
  }

  {
    // Meshes: one per Surface
    auto meshes = root.get("meshes", Json::arrayValue);
    ASSERT_EQ(nPlanarSurfaces, meshes.size());

    std::vector<std::pair<std::string, Json::ValueType>> primitiveKeysAndTypes{
      {"attributes", Json::objectValue},
      {"indices", Json::intValue},
      {"material", Json::intValue},
      {"mode", Json::intValue},
    };

    std::vector<std::string> foundMeshNames;
    for (auto& mesh : meshes) {
      assertKeyAndType(mesh, "name", Json::stringValue);
      foundMeshNames.emplace_back(mesh.get("name", "").asString());

      assertKeyAndType(mesh, "primitives", Json::arrayValue);
      auto primitives = mesh.get("primitives", Json::arrayValue);
      ASSERT_EQ(1, primitives.size());
      auto primitive = primitives[0];
      ASSERT_TRUE(primitive.isObject());
      for (const auto& [key, keyType] : primitiveKeysAndTypes) {
        assertKeyAndType(primitive, key, keyType);
      }
    }

    std::vector<std::string> expectedMeshNames;
    for (const auto& planarSurface : planarSurfaces) {
      expectedMeshNames.emplace_back(planarSurface.nameString());
    }

    std::sort(expectedMeshNames.begin(), expectedMeshNames.end());
    std::sort(foundMeshNames.begin(), foundMeshNames.end());
    EXPECT_EQ(expectedMeshNames, foundMeshNames);
  }

  {

    // Nodes: the first one is the topNode, and it has all the other ones as children
    auto nodes = root.get("nodes", Json::arrayValue);
    EXPECT_EQ(nPlanarSurfaces + 1, nodes.size());
    std::vector<std::pair<std::string, Json::ValueType>> theKeysAndType{
      {"matrix", Json::arrayValue},
      {"name", Json::stringValue},
    };
    auto topNode = nodes[0];
    for (const auto& [key, keyType] : theKeysAndType) {
      assertKeyAndType(topNode, key, keyType);
    }
    EXPECT_EQ("Z_UP", topNode.get("name", "").asString());
    assertKeyAndType(topNode, "children", Json::arrayValue);
    auto children = topNode.get("children", Json::arrayValue);
    ASSERT_EQ(30, children.size());
    for (int i = 0; i < 30; ++i) {
      EXPECT_EQ(i + 1, children[i].asInt());
    }
    EXPECT_FALSE(checkKey(topNode, "extras"));

    std::vector<std::pair<std::string, Json::ValueType>> extrasKeysAndType{
      // TODO: THESE SHOULD BE AS AN ARRAY OF OBJECTS
      {"airLoopHVACHandles", Json::objectValue},
      {"airLoopHVACMaterialNames", Json::objectValue},
      {"airLoopHVACNames", Json::objectValue},
      // Real
      {"illuminanceSetpoint", Json::realValue},
      // Booleans
      {"airWall", Json::booleanValue},
      {"coincidentWithOutsideObject", Json::booleanValue},
      // Strings
      {"boundaryMaterialName", Json::stringValue},
      {"buildingStoryHandle", Json::stringValue},
      {"buildingStoryMaterialName", Json::stringValue},
      {"buildingStoryName", Json::stringValue},
      {"buildingUnitHandle", Json::stringValue},
      {"buildingUnitMaterialName", Json::stringValue},
      {"buildingUnitName", Json::stringValue},
      {"constructionHandle", Json::stringValue},
      {"constructionMaterialName", Json::stringValue},
      {"constructionName", Json::stringValue},
      {"constructionSetHandle", Json::stringValue},
      {"constructionSetMaterialName", Json::stringValue},
      {"constructionSetName", Json::stringValue},
      {"handle", Json::stringValue},
      {"name", Json::stringValue},
      {"outsideBoundaryCondition", Json::stringValue},
      {"outsideBoundaryConditionObjectHandle", Json::stringValue},
      {"outsideBoundaryConditionObjectName", Json::stringValue},
      {"shadingHandle", Json::stringValue},
      {"shadingName", Json::stringValue},
      {"spaceHandle", Json::stringValue},
      {"spaceName", Json::stringValue},
      {"spaceTypeHandle", Json::stringValue},
      {"spaceTypeMaterialName", Json::stringValue},
      {"spaceTypeName", Json::stringValue},
      {"subSurfaceHandle", Json::stringValue},
      {"subSurfaceName", Json::stringValue},
      {"sunExposure", Json::stringValue},
      {"surfaceHandle", Json::stringValue},
      {"surfaceName", Json::stringValue},
      {"surfaceType", Json::stringValue},
      {"surfaceTypeMaterialName", Json::stringValue},
      {"thermalZoneHandle", Json::stringValue},
      {"thermalZoneMaterialName", Json::stringValue},
      {"thermalZoneName", Json::stringValue},
      {"windExposure", Json::stringValue},
    };
    bool isTopNode = true;
    for (auto& node : nodes) {
      if (isTopNode) {
        isTopNode = false;
        continue;
      }

      assertKeyAndType(node, "extras", Json::objectValue);
      auto extras = node.get("extras", Json::objectValue);
      for (const auto& [key, keyType] : extrasKeysAndType) {
        if (keyType != Json::objectValue) {
          assertKeyAndType(extras, key, keyType);
        } else {
          checkKey(extras, key);
          EXPECT_TRUE(checkType(extras, key, Json::objectValue) || checkType(extras, key, Json::nullValue));
        }
      }
    }

    // TODO: should probably at least spot check one of the node's extras
    // TODO: There's an (existing) mistake with respect to materials... The node extras define materials that aren't actually in the "material" key
  }

  {
    // Scenes: there's only one
    auto scenes = root.get("scenes", Json::arrayValue);
    ASSERT_EQ(1, scenes.size());
    auto scene = scenes[0];
    ASSERT_TRUE(scene.isObject());

    // "nodes" : [0]
    assertKeyAndType(scene, "nodes", Json::arrayValue);
    auto nodes = scene.get("nodes", Json::arrayValue);
    ASSERT_EQ(1, nodes.size());
    ASSERT_EQ(0, nodes[0].asInt());

    // extras: that's the main dish
    assertKeyAndType(scene, "extras", Json::objectValue);
    auto extras = scene.get("extras", Json::objectValue);
    std::vector<std::string> allExtrasKeys{"boundingbox", "buildingStoryNames", "generator", "modelObjectMetaData", "northAxis", "type", "version"};
    for (const auto& key : allExtrasKeys) {
      assertKey(extras, key);
    }
    // Strings
    assertKeyAndType(extras, "generator", Json::stringValue);
    EXPECT_EQ("OpenStudio", extras.get("generator", Json::stringValue).asString());
    assertKeyAndType(extras, "type", Json::stringValue);
    EXPECT_EQ("Object", extras.get("type", Json::stringValue).asString());
    assertKeyAndType(extras, "version", Json::stringValue);
    EXPECT_EQ(openStudioVersion(), extras.get("version", Json::stringValue).asString());

    // North Axis: float
    assertKeyAndType(extras, "northAxis", Json::realValue);
    EXPECT_DOUBLE_EQ(0.0, extras.get("northAxis", -999).asDouble());

    {
      assertKeyAndType(extras, "boundingbox", Json::objectValue);
      auto boundingbox = extras.get("boundingbox", Json::objectValue);
      std::vector<std::string> boundingBoxKeys{"lookAtR", "lookAtX", "lookAtY", "lookAtZ", "maxX", "maxY", "maxZ", "minX", "minY", "minZ"};
      for (const auto& key : boundingBoxKeys) {
        assertKeyAndType(boundingbox, key, Json::realValue);
      }

      EXPECT_DOUBLE_EQ(20.615528128088304, boundingbox.get("lookAtR", -999).asDouble());
      EXPECT_DOUBLE_EQ(0.0, boundingbox.get("lookAtX", -999).asDouble());
      EXPECT_DOUBLE_EQ(0.0, boundingbox.get("lookAtY", -999).asDouble());
      EXPECT_DOUBLE_EQ(0.0, boundingbox.get("lookAtZ", -999).asDouble());
      EXPECT_DOUBLE_EQ(20.55, boundingbox.get("maxX", -999).asDouble());
      EXPECT_DOUBLE_EQ(20.0, boundingbox.get("maxY", -999).asDouble());
      EXPECT_DOUBLE_EQ(20.0, boundingbox.get("maxZ", -999).asDouble());
      EXPECT_DOUBLE_EQ(-30.0, boundingbox.get("minX", -999).asDouble());
      EXPECT_DOUBLE_EQ(-1.0, boundingbox.get("minY", -999).asDouble());
      EXPECT_DOUBLE_EQ(0.0, boundingbox.get("minZ", -999).asDouble());
    }
    {
      // TODO: this should be an array!!!
      assertKeyAndType(extras, "buildingStoryNames", Json::objectValue);
      auto buildingStoryNames = extras.get("buildingStoryNames", Json::objectValue);
      assertKeyAndType(buildingStoryNames, "0", Json::stringValue);
      EXPECT_EQ("Building Story 1", buildingStoryNames.get("0", Json::stringValue).asString());
    }

    {
      // TODO: this should be an array!!!
      assertKeyAndType(extras, "modelObjectMetaData", Json::objectValue);
      auto modelObjectMetaData = extras.get("modelObjectMetaData", Json::objectValue);

      std::vector<std::pair<std::string, Json::ValueType>> theKeysAndType{
        {"color", Json::stringValue},
        {"handle", Json::stringValue},
        {"iddObjectType", Json::stringValue},
        {"multiplier", Json::intValue},
        {"name", Json::stringValue},
        {"nominal_floorCeiling_Height", Json::realValue},
        {"nominal_z_coordinate", Json::realValue},
        {"open_to_below", Json::booleanValue},
      };

      for (size_t i = 0; i < nMetaDataSize; ++i) {
        assertKeyAndType(modelObjectMetaData, std::to_string(i), Json::objectValue);
        auto theMetaData = modelObjectMetaData.get(std::to_string(i), Json::objectValue);
        for (const auto& [key, keyType] : theKeysAndType) {
          assertKeyAndType(theMetaData, key, keyType);
        }
      }

      // TODO: Should at least spot check one
    }
  }
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 9 materials
// 30 meshes
// 31 nodes
// 30 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_ExampleModel) {
  GltfForwardTranslator ft;
  openstudio::path outputPath;
  openstudio::path inputPath;
  outputPath = resourcesPath() / toPath("utilities/Geometry/exampleModel.gltf");
  // Create OSM
  Model model = exampleModel();
  model.save(resourcesPath() / toPath("model/exampleModel.osm"), true);
  // Generate glTF
  bool isExported = ft.modelToGLTF(model, outputPath);
  ASSERT_TRUE(isExported);
  inputPath = outputPath;
  // Load glTF
  bool isLoaded = ft.loadGLTF(inputPath);
  ASSERT_TRUE(isLoaded);
  GltfMetaData glTFMetaData = ft.getMetaData();

  EXPECT_EQ("OpenStudio", glTFMetaData.generator());
  EXPECT_DOUBLE_EQ(0.0, glTFMetaData.northAxis());
  EXPECT_EQ("Object", glTFMetaData.type());
  EXPECT_EQ(openStudioVersion(), glTFMetaData.version());

  GltfBoundingBox glTFBoundingBox = glTFMetaData.glTFBoundingBox();
  // TODO: add delta during assertion
  ASSERT_DOUBLE_EQ(glTFBoundingBox.lookAtR(), 20.615528128088304);
  ASSERT_EQ(glTFBoundingBox.lookAtX(), 0.0);
  ASSERT_EQ(glTFBoundingBox.lookAtY(), 0.0);
  ASSERT_EQ(glTFBoundingBox.lookAtZ(), 0.0);
  ASSERT_EQ(glTFBoundingBox.maxX(), 20.55);
  ASSERT_EQ(glTFBoundingBox.maxY(), 20);
  ASSERT_EQ(glTFBoundingBox.maxZ(), 20);
  ASSERT_EQ(glTFBoundingBox.minX(), -30);
  ASSERT_EQ(glTFBoundingBox.minY(), -1);
  ASSERT_EQ(glTFBoundingBox.minZ(), 0.0);

  auto buildingStoreys = glTFMetaData.buildingStoryNames();
  ASSERT_EQ(1, buildingStoreys.size());
  ASSERT_TRUE("Building Story 1" == buildingStoreys.front());

  auto _modelObjectMetaDataCollection = glTFMetaData.glTFModelObjectMetaDataVector();
  ASSERT_EQ(9, _modelObjectMetaDataCollection.size());
  // TODO: Assert few of the attributes from one of the modelObjectMetaData Collection
  GltfModelObjectMetaData glTFModelObjectMetadata = _modelObjectMetaDataCollection.front();
  std::string name0 = _modelObjectMetaDataCollection[0].name();
  std::string name1 = _modelObjectMetaDataCollection[1].name();
  ASSERT_FALSE(name0 == name1);
  /* std::string name2 = _modelObjectMetaDataCollection[2].name();
  std::string name3 = _modelObjectMetaDataCollection[3].name();
  std::string name4 = _modelObjectMetaDataCollection[4].name();
  std::string name5 = _modelObjectMetaDataCollection[5].name();
  std::string name6 = _modelObjectMetaDataCollection[6].name();
  std::string name7 = _modelObjectMetaDataCollection[7].name();
  std::string name8 = _modelObjectMetaDataCollection[8].name();*/

  ASSERT_EQ(glTFMetaData.buildingStoryCount(), 1);
  ASSERT_EQ(glTFMetaData.thermalZoneCount(), 1);
  ASSERT_EQ(glTFMetaData.spaceCount(), 4);
  ASSERT_EQ(glTFMetaData.spaceTypeCount(), 1);
  ASSERT_EQ(glTFMetaData.constructionSetCount(), 1);
  ASSERT_EQ(glTFMetaData.airLoopCount(), 1);

  std::vector<GltfUserData> glTFUserDataVector = ft.getUserDataCollection();
  ASSERT_EQ(glTFUserDataVector.size(), 30);
  boost::optional<GltfUserData> glTFUserData = ft.getUserDataBySurfaceName("Surface 1");
  ASSERT_TRUE(glTFUserData);
  ASSERT_TRUE(glTFUserData->surfaceType() == "Floor");
  ASSERT_TRUE(glTFUserData->constructionMaterialName() == "Construction_Slab");
  ASSERT_TRUE(glTFUserData->thermalZoneName() == "Thermal Zone 1");
  ASSERT_TRUE(glTFUserData->sunExposure() == "NoSun");
  ASSERT_TRUE(glTFUserData->windExposure() == "NoWind");
  ASSERT_TRUE(glTFUserData->illuminanceSetpoint() == 0.0);
  ASSERT_TRUE(glTFUserData->outsideBoundaryCondition() == "Ground");
  ASSERT_TRUE(glTFUserData->boundaryMaterialName() == "Boundary_Ground");
}

TEST_F(GltfFixture, GltfForwardTranslator_ParkUnder_Retail_Office_C2) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/ParkUnder_Retail_Office_C2.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/ParkUnder_Retail_Office_C2.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 5 materials
// 750 meshes
// 751 nodes
// 750 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_ASHRAECourthouse) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/ASHRAECourthouse.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/ASHRAECourthouse.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 4 materials
// 481 meshes
// 482 nodes
// 481 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_RefBldgSecondarySchoolNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/Sample_DOE-RefBldgSecondarySchoolNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/Sample_DOE-RefBldgSecondarySchoolNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 5 materials
// 443 meshes
// 444 nodes
// 443 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_RefBldgHospitalNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/Sample_DOE-RefBldgHospitalNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/Sample_DOE-RefBldgHospitalNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Validation report
// Format: glTF 2.0
// 0 animations
// 5 materials
// 871 meshes
// 872 nodes
// 871 primitives
// 0 textures
// Extensions: None
// FIXED : 1742 Validation issues
// starting from :
// ACCESSOR_TOTAL_OFFSET_ALIGNMENT	Accessor's total byteOffset 7162 isn't a multiple of componentType length 4.
// /accessors/1
TEST_F(GltfFixture, GltfForwardTranslator_RefBldgOutPatientNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/Sample_DOE-RefBldgOutPatientNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/Sample_DOE-RefBldgOutPatientNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 5 materials
// 589 meshes
// 590 nodes
// 589 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_RefBldgSmallHotelNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/Sample_DOE-RefBldgSmallHotelNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/Sample_DOE-RefBldgSmallHotelNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 4 materials
// 21 meshes
// 22 nodes
// 21 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_RefBldgFullServiceRestaurantNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/RefBldgFullServiceRestaurantNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/RefBldgFullServiceRestaurantNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 5 materials
// 208 meshes
// 209 nodes
// 208 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_RefBldgLargeHotelNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/RefBldgLargeHotelNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/RefBldgLargeHotelNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 4 materials
// 142 meshes
// 143 nodes
// 142 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_RefBldgLargeOfficeNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/RefBldgLargeOfficeNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/RefBldgLargeOfficeNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 4 materials
// 80 meshes
// 81 nodes
// 80 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_Space_SurfaceMatch_LargeTest) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/Space_SurfaceMatch_LargeTest.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/Space_SurfaceMatch_LargeTest.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 3 materials
// 1515 meshes
// 1516 nodes
// 1515 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_15023_Model12) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/15023_Model12.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/15023_Model12.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 3 materials
// 21 meshes
// 22 nodes
// 21 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_split_level) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/split_level.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/split_level.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 3 materials
// 24 meshes
// 25 nodes
// 24 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_open_to_below) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/open_to_below.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/open_to_below.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Load Test with and without bin file
TEST_F(GltfFixture, GltfForwardTranslator_LoadTest) {
  GltfForwardTranslator ft;
  openstudio::path inputPath;
  openstudio::path inputNonEmbededPath;
  inputPath = resourcesPath() / toPath("utilities/Geometry/minimal_GLTF_File.gltf");
  inputNonEmbededPath = resourcesPath() / toPath("utilities/Geometry/input.gltf");
  bool result = ft.loadGLTF(inputPath, inputNonEmbededPath);
  ASSERT_TRUE(result);
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 1 materials
// 1 meshes
// 1 nodes
// 1 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_CreateTriangleGLTFTest) {
  GltfForwardTranslator ft;
  openstudio::path output;
  openstudio::path output_2;
  output = resourcesPath() / toPath("utilities/Geometry/triangle.gltf");
  // Passed Raw buffer data
  bool result = createTriangleGLTF(output);
  ASSERT_TRUE(result);
  output_2 = resourcesPath() / toPath("utilities/Geometry/triangle_2.gltf");
  // Creates Raw buffer data from Point3dVector
  bool result_2 = createTriangleGLTFFromPoint3DVector(output_2);
  ASSERT_TRUE(result_2);
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 4 materials
// 3915 meshes
// 3916 nodes
// 3915 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_7_7_Windows_Complete) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/7-7_Windows_Complete.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/7-7_Windows_Complete.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// VersionTranslator.cpp@590 : Unable to retrieve OpenStudio Version 3.2.2 IDD from the IddFactory."
// thrown in the test body.
TEST_F(GltfFixture, GltfForwardTranslator_story_multipliers) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/story_multipliers.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/story_multipliers.osm");
  //model::OptionalModel model = translator.loadModel(modelPath);
  //bool result = ft.modelToGLTF(model.get(), output);
  //ASSERT_TRUE(result);
}

//  Assertion failed: this->is_initialized() | Optional.hpp 1212
TEST_F(GltfFixture, GltfForwardTranslator_story_space_heights) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/story_space_heights.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/story_space_heights.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  /*bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);*/
}

// Validation report
// Format: glTF 2.0
// Stats:
// 0 animations
// 3 materials
// 227 meshes
// 228 nodes
// 227 primitives
// 0 textures
// Extensions: None
TEST_F(GltfFixture, GltfForwardTranslator_floorplan_school) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/floorplan_school.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/floorplan_school.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

// Assertion failed: this->is_initialized() | Optional.hpp 1212
TEST_F(GltfFixture, GltfForwardTranslator_two_stories_pre_intersect) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/two_stories_pre_intersect.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/two_stories_pre_intersect.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  /*bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);*/
}

// Assertion failed: this->is_initialized() | Optinoal.hpp 1212
TEST_F(GltfFixture, GltfForwardTranslator_TwoStoryOffice_Trane) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/TwoStoryOffice_Trane.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/TwoStoryOffice_Trane.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  /*bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);*/
}
