/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "GltfForwardTranslator.hpp"

#include "RenderingColor.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "ConstructionAirBoundary.hpp"
#include "ConstructionAirBoundary_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "Building.hpp"
#include "Building_Impl.hpp"
#include "BuildingStory.hpp"
#include "BuildingStory_Impl.hpp"
#include "BuildingUnit.hpp"
#include "BuildingUnit_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"
#include "PlanarSurfaceGroup.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "DefaultConstructionSet.hpp"
#include "DefaultConstructionSet_Impl.hpp"
#include "ShadingSurfaceGroup.hpp"
#include "InteriorPartitionSurfaceGroup.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Plane.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Polygon3d.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <stack>
#include <limits.h>
#include <tuple>

namespace openstudio {
namespace model {

  namespace GLTF = tinygltf;

  // For raw values for GLTF Materials
  struct MaterialData
  {
    std::string materialName;
    int r;     // [0, 255]
    int g;     // [0, 255]
    int b;     // [0, 255]
    double a;  // [0, 1]
    bool isDoubleSided = false;
  };

  // glTF Extras Bounding Box
  struct GltfBoundingBox
  {
    // all members will take a value of type double
    std::tuple<std::string, tinygltf::Value> lookAtR;
    std::tuple<std::string, tinygltf::Value> lookAtX;
    std::tuple<std::string, tinygltf::Value> lookAtY;
    std::tuple<std::string, tinygltf::Value> lookAtZ;
    std::tuple<std::string, tinygltf::Value> maxX;
    std::tuple<std::string, tinygltf::Value> maxY;
    std::tuple<std::string, tinygltf::Value> maxZ;
    std::tuple<std::string, tinygltf::Value> minX;
    std::tuple<std::string, tinygltf::Value> minY;
    std::tuple<std::string, tinygltf::Value> minZ;

    void setlookAtR(double value) {
      std::string key = "lookAtR";
      tinygltf::Value extraNodeValue(value);
      lookAtR = make_tuple(key, extraNodeValue);
    }

    void setlookAtX(double value) {
      std::string key = "lookAtX";
      tinygltf::Value extraNodeValue(value);
      lookAtX = make_tuple(key, extraNodeValue);
    }

    void setlookAtY(double value) {
      std::string key = "lookAtY";
      tinygltf::Value extraNodeValue(value);
      lookAtY = make_tuple(key, extraNodeValue);
    }

    void setlookAtZ(double value) {
      std::string key = "lookAtZ";
      tinygltf::Value extraNodeValue(value);
      lookAtZ = make_tuple(key, extraNodeValue);
    }

    void setmaxX(double value) {
      std::string key = "maxX";
      tinygltf::Value extraNodeValue(value);
      maxX = make_tuple(key, extraNodeValue);
    }

    void setmaxY(double value) {
      std::string key = "maxY";
      tinygltf::Value extraNodeValue(value);
      maxY = make_tuple(key, extraNodeValue);
    }

    void setmaxZ(double value) {
      std::string key = "maxZ";
      tinygltf::Value extraNodeValue(value);
      maxZ = make_tuple(key, extraNodeValue);
    }

    void setminX(double value) {
      std::string key = "minX";
      tinygltf::Value extraNodeValue(value);
      minX = make_tuple(key, extraNodeValue);
    }

    void setminY(double value) {
      std::string key = "minY";
      tinygltf::Value extraNodeValue(value);
      minY = make_tuple(key, extraNodeValue);
    }

    void setminZ(double value) {
      std::string key = "minZ";
      tinygltf::Value extraNodeValue(value);
      minZ = make_tuple(key, extraNodeValue);
    }

    // Getters
    double getlookAtR() const {
      tinygltf::Value v(std::get<1>(lookAtR));
      double valueD = v.Get<double>();
      return valueD;
    }

    double getlookAtX() const {
      tinygltf::Value v(std::get<1>(lookAtX));
      double valueD = v.Get<double>();
      return valueD;
    }

    double getlookAtY() const {
      tinygltf::Value v(std::get<1>(lookAtY));
      double valueD = v.Get<double>();
      return valueD;
    }

    double getlookAtZ() const {
      tinygltf::Value v(std::get<1>(lookAtZ));
      double valueD = v.Get<double>();
      return valueD;
    }

    double getmaxX() const {
      tinygltf::Value v(std::get<1>(maxX));
      double valueD = v.Get<double>();
      return valueD;
    }

    double getmaxY() const {
      tinygltf::Value v(std::get<1>(maxY));
      double valueD = v.Get<double>();
      return valueD;
    }

    double getmaxZ() const {
      tinygltf::Value v(std::get<1>(maxZ));
      double valueD = v.Get<double>();
      return valueD;
    }

    double getminX() const {
      tinygltf::Value v(std::get<1>(minX));
      double valueD = v.Get<double>();
      return valueD;
    }

    double getminY() const {
      tinygltf::Value v(std::get<1>(minY));
      double valueD = v.Get<double>();
      return valueD;
    }

    double getminZ() const {
      tinygltf::Value v(std::get<1>(minZ));
      double valueD = v.Get<double>();
      return valueD;
    }
  };

  class GltfModelObjectMetadata
  {
   public:
    GltfModelObjectMetadata() {
      // Initialize all attributes with defaults
      initializeAttributesWithDefaults();
    }

    void setColor(std::string value) {
      std::string key = "color";
      tinygltf::Value extraNodeValue(value);
      color = make_tuple(key, extraNodeValue);
    }

    void setHandle(std::string value) {
      std::string key = "handle";
      tinygltf::Value extraNodeValue(value);
      handle = make_tuple(key, extraNodeValue);
    }

    void setIddObjectType(std::string value) {
      std::string key = "iddObjectType";
      tinygltf::Value extraNodeValue(value);
      iddObjectType = make_tuple(key, extraNodeValue);
    }

    void setName(std::string value) {
      std::string key = "name";
      tinygltf::Value extraNodeValue(value);
      name = make_tuple(key, extraNodeValue);
    }

    void setOpen_to_below(bool value) {
      std::string key = "open_to_below";
      tinygltf::Value extraNodeValue(value);
      open_to_below = make_tuple(key, extraNodeValue);
    }

    void setNominal_z_coordinate(double value) {
      std::string key = "nominal_z_coordinate";
      tinygltf::Value extraNodeValue(value);
      nominal_z_coordinate = make_tuple(key, extraNodeValue);
    }

    void setNominal_floorCeiling_Height(double value) {
      std::string key = "nominal_floorCeiling_Height";
      tinygltf::Value extraNodeValue(value);
      nominal_floorCeiling_Height = make_tuple(key, extraNodeValue);
    }

    void setMultiplier(int value) {
      std::string key = "multiplier";
      tinygltf::Value extraNodeValue(value);
      multiplier = make_tuple(key, extraNodeValue);
    }

    //

    void setColor(tinygltf::Value value) {
      std::string key = "color";
      color = make_tuple(key, value);
    }

    void setHandle(tinygltf::Value value) {
      std::string key = "handle";
      handle = make_tuple(key, value);
    }

    void setIddObjectType(tinygltf::Value value) {
      std::string key = "iddObjectType";
      iddObjectType = make_tuple(key, value);
    }

    void setName(tinygltf::Value value) {
      std::string key = "name";
      name = make_tuple(key, value);
    }

    void setMultiplier(tinygltf::Value value) {
      std::string key = "multiplier";
      multiplier = make_tuple(key, value);
    }

    void setOpen_to_below(tinygltf::Value value) {
      std::string key = "open_to_below";
      open_to_below = make_tuple(key, value);
    }

    void setNominal_z_coordinate(tinygltf::Value value) {
      std::string key = "nominal_z_coordinate";
      nominal_z_coordinate = make_tuple(key, value);
    }

    void setNominal_floorCeiling_Height(tinygltf::Value value) {
      std::string key = "nominal_floorCeiling_Height";
      nominal_floorCeiling_Height = make_tuple(key, value);
    }

    std::map<std::string, tinygltf::Value> modelObjectMap;
    void InitializeModelObjectMap() {
      /*   tinygltf::Value extraNodeValue(toString(""));
      modelObjectMap.insert({"multiplier", extraNodeValue});*/

      //modelObjectMap.insert({std::get<0>(color), std::get<1>(color)});
      /* modelObjectMap.insert({std::get<0>(handle), std::get<1>(handle)});
      modelObjectMap.insert({std::get<0>(iddObjectType), std::get<1>(iddObjectType)});
      modelObjectMap.insert({std::get<0>(name), std::get<1>(name)});
      modelObjectMap.insert({std::get<0>(open_to_below), std::get<1>(open_to_below)});
      modelObjectMap.insert({std::get<0>(nominal_z_coordinate), std::get<1>(nominal_z_coordinate)});
      modelObjectMap.insert({std::get<0>(nominal_floorCeiling_Height), std::get<1>(nominal_floorCeiling_Height)});*/
      //modelObjectMap.insert({std::get<0>(multiplier), std::get<1>(multiplier)});

      /* auto insColor = modelObjectMap.insert(std::make_pair("color", 0));
      insColor.first->second = std::get<1>(color);*/
      /* auto insMultiplier = modelObjectMap.insert(std::make_pair("multiplier", 0));
      insMultiplier.first->second = std::get<1>(multiplier);*/

      modelObjectMap["color"] = std::get<1>(color);
      modelObjectMap["handle"] = std::get<1>(handle);
      modelObjectMap["iddObjectType"] = std::get<1>(iddObjectType);
      modelObjectMap["name"] = std::get<1>(name);
      modelObjectMap["open_to_below"] = std::get<1>(open_to_below);
      modelObjectMap["nominal_z_coordinate"] = std::get<1>(nominal_z_coordinate);
      modelObjectMap["nominal_floorCeiling_Height"] = std::get<1>(nominal_floorCeiling_Height);
      modelObjectMap["multiplier"] = std::get<1>(multiplier);

      /* if (!modelObjectMap.count("open_to_below")) {
        tinygltf::Value extraNodeValue(false);
        modelObjectMap.insert({"open_to_below", extraNodeValue});
      }*/

      /* if (!modelObjectMap.count("multiplier")) {
        tinygltf::Value extraNodeValue(toString(""));
        modelObjectMap.insert({"multiplier", extraNodeValue});
      }*/
    }

    // getters

    std::string getIddObjectType() const {
      tinygltf::Value v(std::get<1>(iddObjectType));
      std::string strVal = v.Get<std::string>().c_str();
      return strVal;
    }

   private:
    // string
    std::tuple<std::string, tinygltf::Value> color;
    std::tuple<std::string, tinygltf::Value> handle;
    std::tuple<std::string, tinygltf::Value> iddObjectType;
    std::tuple<std::string, tinygltf::Value> name;

    // bool
    std::tuple<std::string, tinygltf::Value> open_to_below;

    // double
    std::tuple<std::string, tinygltf::Value> nominal_z_coordinate;
    std::tuple<std::string, tinygltf::Value> nominal_floorCeiling_Height;

    // int
    std::tuple<std::string, tinygltf::Value> multiplier;

    void initializeAttributesWithDefaults() {
      setColor("");
      setHandle("");
      setIddObjectType("");
      setName("");
      setOpen_to_below(false);
      setNominal_z_coordinate(0.0);
      setNominal_floorCeiling_Height(0.0);
      setMultiplier(0);
    }
  };

  // MetaData as glTF Extras
  class GltfMetaData
  {
   public:
    GltfMetaData() {
      resetModelObjectMetaDataCount();
    }

    void resetModelObjectMetaDataCount() {
      storyCount = 0;
      thermalZoneCount = 0;
      spaceCount = 0;
      spaceTypeCount = 0;
      constructionSetCount = 0;
      airLoopCount = 0;
    }

    std::map<std::string, tinygltf::Value> metaData;

    double getBoundingBoxlookAtR() {
      return glTFBoundingBox.getlookAtR();
    }

    double getBoundingBoxlookAtX() {
      return glTFBoundingBox.getlookAtX();
    }

    double getBoundingBoxlookAtY() {
      return glTFBoundingBox.getlookAtY();
    }

    double getBoundingBoxlookAtZ() {
      return glTFBoundingBox.getlookAtZ();
    }

    double getBoundingBoxmaxX() {
      return glTFBoundingBox.getmaxX();
    }

    double getBoundingBoxmaxY() {
      return glTFBoundingBox.getmaxY();
    }

    double getBoundingBoxmaxZ() {
      return glTFBoundingBox.getmaxZ();
    }

    double getBoundingBoxminZ() {
      return glTFBoundingBox.getminZ();
    }

    double getBoundingBoxminX() {
      return glTFBoundingBox.getminX();
    }

    double getBoundingBoxminY() {
      return glTFBoundingBox.getminY();
    }

    std::vector<GltfModelObjectMetadata> getModelObjectMetaDataCollection() {
      return glTFModelObjectMetadataVector;
    }

    void setBoundingBox(tinygltf::Value value) {
      auto boundingBoxObj = value.Get<std::map<std::string, tinygltf::Value>>();
      for (auto const& x : boundingBoxObj) {
        std::string key = x.first;
        if (key == "lookAtR") {
          tinygltf::Value v(x.second);
          double vv = v.Get<double>();
          glTFBoundingBox.setlookAtR(vv);
        } else if (key == "lookAtX") {
          tinygltf::Value v(x.second);
          double vv = v.Get<double>();
          glTFBoundingBox.setlookAtX(vv);
        } else if (key == "lookAtY") {
          tinygltf::Value v(x.second);
          double vv = v.Get<double>();
          glTFBoundingBox.setlookAtY(vv);
        } else if (key == "lookAtZ") {
          tinygltf::Value v(x.second);
          double vv = v.Get<double>();
          glTFBoundingBox.setlookAtZ(vv);
        } else if (key == "maxX") {
          tinygltf::Value v(x.second);
          double vv = v.Get<double>();
          glTFBoundingBox.setmaxX(vv);
        } else if (key == "maxY") {
          tinygltf::Value v(x.second);
          double vv = v.Get<double>();
          glTFBoundingBox.setmaxY(vv);
        } else if (key == "maxZ") {
          tinygltf::Value v(x.second);
          double vv = v.Get<double>();
          glTFBoundingBox.setmaxZ(vv);
        } else if (key == "minX") {
          tinygltf::Value v(x.second);
          double vv = v.Get<double>();
          glTFBoundingBox.setminX(vv);
        } else if (key == "minY") {
          tinygltf::Value v(x.second);
          double vv = v.Get<double>();
          glTFBoundingBox.setminY(vv);
        } else if (key == "minZ") {
          tinygltf::Value v(x.second);
          double vv = v.Get<double>();
          glTFBoundingBox.setminZ(vv);
        }
      }
    }

    void setBoundingBox(const std::vector<PlanarSurfaceGroup>& planarSurfaceGroups, std::function<void(double)> updatePercentage, double& n,
                        std::vector<PlanarSurface>::size_type& N) {
      // Bounding Box
      std::map<std::string, tinygltf::Value> boundingBoxObject;
      BoundingBox boundingBox;
      boundingBox.addPoint(Point3d(0, 0, 0));
      boundingBox.addPoint(Point3d(1, 1, 1));
      for (const auto& group : planarSurfaceGroups) {
        boundingBox.add(group.transformation() * group.boundingBox());

        n += 1;
        updatePercentage(100.0 * n / N);
      }

      double lookAtX = 0;  // (boundingBox.minX().get() + boundingBox.maxX().get()) / 2.0
      double lookAtY = 0;  // (boundingBox.minY().get() + boundingBox.maxY().get()) / 2.0
      double lookAtZ = 0;  // (boundingBox.minZ().get() + boundingBox.maxZ().get()) / 2.0
      double lookAtR =
        sqrt(std::pow(boundingBox.maxX().get() / 2.0, 2) + std::pow(boundingBox.maxY().get() / 2.0, 2) + std::pow(boundingBox.maxZ().get() / 2.0, 2));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.minX().get() / 2.0, 2) + std::pow(boundingBox.maxY().get() / 2.0, 2)
                                       + std::pow(boundingBox.maxZ().get() / 2.0, 2)));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.maxX().get() / 2.0, 2) + std::pow(boundingBox.minY().get() / 2.0, 2)
                                       + std::pow(boundingBox.maxZ().get() / 2.0, 2)));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.maxX().get() / 2.0, 2) + std::pow(boundingBox.maxY().get() / 2.0, 2)
                                       + std::pow(boundingBox.minZ().get() / 2.0, 2)));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.minX().get() / 2.0, 2) + std::pow(boundingBox.minY().get() / 2.0, 2)
                                       + std::pow(boundingBox.maxZ().get() / 2.0, 2)));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.minX().get() / 2.0, 2) + std::pow(boundingBox.maxY().get() / 2.0, 2)
                                       + std::pow(boundingBox.minZ().get() / 2.0, 2)));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.maxX().get() / 2.0, 2) + std::pow(boundingBox.minY().get() / 2.0, 2)
                                       + std::pow(boundingBox.minZ().get() / 2.0, 2)));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.minX().get() / 2.0, 2) + std::pow(boundingBox.minY().get() / 2.0, 2)
                                       + std::pow(boundingBox.minZ().get() / 2.0, 2)));

      /*   std::map<std::string, tinygltf::Value> bbObj;*/
      /*BoundingBoxglTF boundingBoxglTF;*/
      glTFBoundingBox.setlookAtR(lookAtR);
      glTFBoundingBox.setlookAtX(lookAtX);
      glTFBoundingBox.setlookAtY(lookAtY);
      glTFBoundingBox.setlookAtZ(lookAtZ);
      glTFBoundingBox.setmaxX(boundingBox.maxX().get());
      glTFBoundingBox.setmaxY(boundingBox.maxY().get());
      glTFBoundingBox.setmaxZ(boundingBox.maxZ().get());
      glTFBoundingBox.setminX(boundingBox.minX().get());
      glTFBoundingBox.setminY(boundingBox.minY().get());
      glTFBoundingBox.setminZ(boundingBox.minZ().get());

      boundingBoxObject.insert({std::get<0>(glTFBoundingBox.lookAtR), std::get<1>(glTFBoundingBox.lookAtR)});
      boundingBoxObject.insert({std::get<0>(glTFBoundingBox.lookAtX), std::get<1>(glTFBoundingBox.lookAtX)});
      boundingBoxObject.insert({std::get<0>(glTFBoundingBox.lookAtY), std::get<1>(glTFBoundingBox.lookAtY)});
      boundingBoxObject.insert({std::get<0>(glTFBoundingBox.lookAtZ), std::get<1>(glTFBoundingBox.lookAtZ)});
      boundingBoxObject.insert({std::get<0>(glTFBoundingBox.maxX), std::get<1>(glTFBoundingBox.maxX)});
      boundingBoxObject.insert({std::get<0>(glTFBoundingBox.maxY), std::get<1>(glTFBoundingBox.maxY)});
      boundingBoxObject.insert({std::get<0>(glTFBoundingBox.maxZ), std::get<1>(glTFBoundingBox.maxZ)});
      boundingBoxObject.insert({std::get<0>(glTFBoundingBox.minX), std::get<1>(glTFBoundingBox.minX)});
      boundingBoxObject.insert({std::get<0>(glTFBoundingBox.minY), std::get<1>(glTFBoundingBox.minY)});
      boundingBoxObject.insert({std::get<0>(glTFBoundingBox.minZ), std::get<1>(glTFBoundingBox.minZ)});

      tinygltf::Value bbObj(boundingBoxObject);
      metaData.insert({"boundingbox", bbObj});
    }

    void setBuildingStoryNames(const std::vector<BuildingStory>& buildingStories, std::function<void(double)> updatePercentage, double& n,
                               std::vector<PlanarSurface>::size_type& N) {
      std::vector<std::string> buildingStoryNames;
      for (const auto& buildingStory : buildingStories) {
        buildingStoryNames.push_back(buildingStory.nameString());
      }
      std::sort(buildingStoryNames.begin(), buildingStoryNames.end(), IstringCompare());
      std::map<std::string, tinygltf::Value> bObj;
      int storyCount = 0;
      for (const auto& buildingStoryName : buildingStoryNames) {
        tinygltf::Value buildingStoryNameValue(buildingStoryName);
        bObj.insert({std::to_string(storyCount), buildingStoryNameValue});
        storyCount++;
      }
      tinygltf::Value buildingStoryNamesObj(bObj);
      metaData.insert({"buildingStoryNames", buildingStoryNamesObj});
    }

    void setGenerator(std::string value) {
      std::string key = "generator";
      tinygltf::Value generatorExtraNodeValue(static_cast<std::string>(value));
      generator = make_tuple(key, generatorExtraNodeValue);
    }

    void setType(std::string value) {
      std::string key = "type";
      tinygltf::Value typeExtraNodeValue(static_cast<std::string>(value));
      type = make_tuple(key, typeExtraNodeValue);
    }

    void setVersion(std::string value) {
      std::string key = "version";
      tinygltf::Value versionExtraNodeValue(value);  // TODO:BPS what is this version and where to get it from..
      version = make_tuple(key, versionExtraNodeValue);
    }

    void setNorthAxis(const Model& model) {
      std::string key = "northAxis";
      double northAxisValue = 0.0;
      boost::optional<Building> building = model.getOptionalUniqueModelObject<Building>();
      if (building) {
        northAxisValue = -building->northAxis();
      }
      tinygltf::Value northAxisExtraNodeValue(northAxisValue);
      northAxis = make_tuple(key, northAxisExtraNodeValue);
    }

    void setNorthAxis(double value) {
      std::string key = "northAxis";
      tinygltf::Value northAxisExtraNodeValue(value);
      northAxis = make_tuple(key, northAxisExtraNodeValue);
    }

    void setModelObjectMetaDataForStories(const std::vector<BuildingStory>& buildingStories, std::function<void(double)> updatePercentage, double& n,
                                          std::vector<PlanarSurface>::size_type& N) {
      for (const auto& buildingStory : buildingStories) {
        GltfModelObjectMetadata modelObjectMetadataglTF;
        if (buildingStory.renderingColor()) {
          modelObjectMetadataglTF.setColor(buildingStory.renderingColor()->colorString());
        }
        modelObjectMetadataglTF.setHandle(toString(buildingStory.handle()));
        modelObjectMetadataglTF.setIddObjectType(buildingStory.iddObjectType().valueDescription());
        modelObjectMetadataglTF.setName(buildingStory.nameString());
        if (buildingStory.nominalZCoordinate()) {
          modelObjectMetadataglTF.setNominal_z_coordinate(buildingStory.nominalZCoordinate().get());
        }
        // TBD: Do we have to include this ?
        if (buildingStory.nominalFloortoCeilingHeight()) {
          modelObjectMetadataglTF.setNominal_floorCeiling_Height(buildingStory.nominalFloortoCeilingHeight().get());
        }
        // TBD: How to calculate this ?
        modelObjectMetadataglTF.setOpen_to_below(false);

        if (buildingStory.nominalFloortoFloorHeight()) {
          // DLM: how to translate this?
        }

        storyCount++;
        glTFModelObjectMetadataVector.push_back(modelObjectMetadataglTF);

        for (const auto& space : buildingStory.spaces()) {
          GltfModelObjectMetadata modelObjectMetadataglTF;
          // TBD: color is always empty for Space ?
          modelObjectMetadataglTF.setColor(toString(""));
          modelObjectMetadataglTF.setHandle(toString(space.handle()));
          modelObjectMetadataglTF.setIddObjectType(space.iddObjectType().valueDescription());
          modelObjectMetadataglTF.setName(space.nameString());
          // TBD: open_to_below is always false for Space ?
          modelObjectMetadataglTF.setOpen_to_below(false);

          spaceCount++;
          glTFModelObjectMetadataVector.push_back(modelObjectMetadataglTF);
        }

        n += 1;
        updatePercentage(100.0 * n / N);
      }
    }

    void setModelObjectMetaDataForBuildingUnits(const std::vector<BuildingUnit>& buildingUnits, std::function<void(double)> updatePercentage,
                                                double& n, std::vector<PlanarSurface>::size_type& N) {
      for (const auto& buildingUnit : buildingUnits) {
        GltfModelObjectMetadata modelObjectMetadataglTF;
        if (buildingUnit.renderingColor()) {
          modelObjectMetadataglTF.setColor(buildingUnit.renderingColor()->colorString());
        }
        modelObjectMetadataglTF.setHandle(toString(buildingUnit.handle()));
        modelObjectMetadataglTF.setIddObjectType(buildingUnit.iddObjectType().valueDescription());
        modelObjectMetadataglTF.setName(buildingUnit.nameString());
        // TBD: how to calculate open_to_below for building unit ??
        modelObjectMetadataglTF.setOpen_to_below(false);

        glTFModelObjectMetadataVector.push_back(modelObjectMetadataglTF);

        n += 1;
        updatePercentage(100.0 * n / N);
      }
    }

    void setModelObjectMetaDataForThermalZones(const std::vector<ThermalZone>& thermalZones, std::function<void(double)> updatePercentage, double& n,
                                               std::vector<PlanarSurface>::size_type& N) {
      for (const auto& thermalZone : thermalZones) {
        GltfModelObjectMetadata modelObjectMetadataglTF;
        if (thermalZone.renderingColor()) {
          modelObjectMetadataglTF.setColor(thermalZone.renderingColor()->colorString());
        }
        modelObjectMetadataglTF.setHandle(toString(thermalZone.handle()));
        modelObjectMetadataglTF.setIddObjectType(thermalZone.iddObjectType().valueDescription());
        modelObjectMetadataglTF.setName(thermalZone.nameString());
        // TBD: how to calculate open_to_below for thermal Zone ??
        modelObjectMetadataglTF.setOpen_to_below(false);

        modelObjectMetadataglTF.setMultiplier(thermalZone.multiplier());

        thermalZoneCount++;
        glTFModelObjectMetadataVector.push_back(modelObjectMetadataglTF);

        n += 1;
        updatePercentage(100.0 * n / N);
      }
    }

    void setModelObjectMetaDataForSpaceTypes(const std::vector<SpaceType>& spaceTypes, std::function<void(double)> updatePercentage, double& n,
                                             std::vector<PlanarSurface>::size_type& N) {
      for (const auto& spaceType : spaceTypes) {
        GltfModelObjectMetadata modelObjectMetadataglTF;
        if (spaceType.renderingColor()) {
          modelObjectMetadataglTF.setColor(spaceType.renderingColor()->colorString());
        }
        modelObjectMetadataglTF.setHandle(toString(spaceType.handle()));
        modelObjectMetadataglTF.setIddObjectType(spaceType.iddObjectType().valueDescription());
        modelObjectMetadataglTF.setName(spaceType.nameString());
        // TBD: how to calculate open_to_below for thermal Zone ??
        modelObjectMetadataglTF.setOpen_to_below(false);

        spaceTypeCount++;
        glTFModelObjectMetadataVector.push_back(modelObjectMetadataglTF);

        n += 1;
        updatePercentage(100.0 * n / N);
      }
    }

    void setModelObjectMetaDataFordefaultConstructionSets(const std::vector<DefaultConstructionSet>& defaultConstructionSets,
                                                          std::function<void(double)> updatePercentage, double& n,
                                                          std::vector<PlanarSurface>::size_type& N) {
      for (const auto& defaultConstructionSet : defaultConstructionSets) {
        GltfModelObjectMetadata modelObjectMetadataglTF;

        modelObjectMetadataglTF.setColor(toString(""));
        modelObjectMetadataglTF.setHandle(toString(defaultConstructionSet.handle()));
        modelObjectMetadataglTF.setIddObjectType(defaultConstructionSet.iddObjectType().valueDescription());
        modelObjectMetadataglTF.setName(defaultConstructionSet.nameString());
        // TBD: how to calcualte open_to_below for defaultConstructionSet or remove this node ?
        modelObjectMetadataglTF.setOpen_to_below(false);

        constructionSetCount++;
        glTFModelObjectMetadataVector.push_back(modelObjectMetadataglTF);

        n += 1;
        updatePercentage(100.0 * n / N);
      }
    }

    void setModelObjectMetaDataForAirLoopHVACs(const std::vector<AirLoopHVAC>& airLoopHVACs, std::function<void(double)> updatePercentage, double& n,
                                               std::vector<PlanarSurface>::size_type& N) {
      for (const auto& airLoopHVAC : airLoopHVACs) {
        GltfModelObjectMetadata modelObjectMetadataglTF;

        modelObjectMetadataglTF.setColor(toString(""));
        modelObjectMetadataglTF.setHandle(toString(airLoopHVAC.handle()));
        modelObjectMetadataglTF.setIddObjectType(airLoopHVAC.iddObjectType().valueDescription());
        modelObjectMetadataglTF.setName(airLoopHVAC.nameString());
        //TBD:
        modelObjectMetadataglTF.setOpen_to_below(false);

        airLoopCount++;
        glTFModelObjectMetadataVector.push_back(modelObjectMetadataglTF);

        n += 1;
        updatePercentage(100.0 * n / N);
      }
    }

    void InitializeModelObject() {
      int modelObjectKeyCount = 0;
      std::map<std::string, tinygltf::Value> modelObj1;
      for (GltfModelObjectMetadata modelObjectglTFMetaData : glTFModelObjectMetadataVector) {
        modelObjectglTFMetaData.InitializeModelObjectMap();
        tinygltf::Value alhObj(modelObjectglTFMetaData.modelObjectMap);
        modelObj1.insert({std::to_string(modelObjectKeyCount), alhObj});
        modelObjectKeyCount++;
      }
      tinygltf::Value mmObj(modelObj1);
      metaData.insert({"modelObjectMetaData", mmObj});
      metaData.insert({std::get<0>(generator), std::get<1>(generator)});
      metaData.insert({std::get<0>(type), std::get<1>(type)});
      metaData.insert({std::get<0>(version), std::get<1>(version)});
      metaData.insert({std::get<0>(northAxis), std::get<1>(northAxis)});
    }

    void initializeFromSceneExtras(tinygltf::Scene scene) {
      metaData = scene.extras.Get<std::map<std::string, tinygltf::Value>>();
      // auto aaextra = gltf_Model.scenes[0].extras_json_string;

      for (auto const& x : metaData) {
        std::string key = x.first;
        if (key == "generator") {
          tinygltf::Value v(x.second);
          auto vv = v.Get<std::string>().c_str();
          setGenerator(vv);
        } else if (key == "type") {
          tinygltf::Value v(x.second);
          auto vv = v.Get<std::string>().c_str();
          setType(vv);
        } else if (key == "northAxis") {
          tinygltf::Value v(x.second);
          double vv = v.Get<double>();
          setNorthAxis(vv);
        } else if (key == "version") {
          tinygltf::Value v(x.second);
          auto vv = v.Get<std::string>().c_str();
          setVersion(vv);
        } else if (key == "boundingbox") {
          tinygltf::Value v(x.second);
          setBoundingBox(v);
        } else if (key == "modelObjectMetaData") {
          tinygltf::Value v(x.second);
          setModelObjectMetaData(v);
        }
      }
    }

    void setModelObjectMetaData(tinygltf::Value value) {
      auto modelObjectMetatDataVector = value.Get<tinygltf::Value::Object>();
      int countKey = 0;
      for (auto const& x : modelObjectMetatDataVector) {
        GltfModelObjectMetadata gltfModelObjectMetadata;
        auto key = x.first;
        tinygltf::Value v(x.second);
        auto modelObjectMetaData1 = v.Get<tinygltf::Value::Object>();
        for (auto const& y : modelObjectMetaData1) {
          auto key1 = y.first;
          tinygltf::Value v1(y.second);
          if (key1 == "color") {
            gltfModelObjectMetadata.setColor(y.second);
          } else if (key1 == "handle") {
            gltfModelObjectMetadata.setHandle(y.second);
          } else if (key1 == "iddObjectType") {
            gltfModelObjectMetadata.setIddObjectType(y.second);
          } else if (key1 == "name") {
            gltfModelObjectMetadata.setName(y.second);
          } else if (key1 == "multiplier") {
            gltfModelObjectMetadata.setMultiplier(y.second);
          } else if (key1 == "open_to_below") {
            gltfModelObjectMetadata.setOpen_to_below(y.second);
          } else if (key1 == "nominal_z_coordinate") {
            gltfModelObjectMetadata.setNominal_z_coordinate(y.second);
          } else if (key1 == "nominal_floorCeiling_Height") {
            gltfModelObjectMetadata.setNominal_floorCeiling_Height(y.second);
          }
        }
        std::string iddObjectTypeStr = gltfModelObjectMetadata.getIddObjectType();
        if (iddObjectTypeStr == "OS:BuildingStory") {
          storyCount++;
        } else if (iddObjectTypeStr == "OS:Space") {
          spaceCount++;
        } else if (iddObjectTypeStr == "OS:ThermalZone") {
          thermalZoneCount++;
        } else if (iddObjectTypeStr == "OS:SpaceType") {
          spaceTypeCount++;
        } else if (iddObjectTypeStr == "OS:DefaultConstructionSet") {
          constructionSetCount++;
        } else if (iddObjectTypeStr.compare("OS:AirLoopHVAC") == 0) {
          airLoopCount++;
        }
        glTFModelObjectMetadataVector.push_back(gltfModelObjectMetadata);
        countKey++;
      }
    }

    std::string getGenerator() const {
      tinygltf::Value v(std::get<1>(generator));
      std::string generatorStr = v.Get<std::string>().c_str();
      return generatorStr;
    }

    std::string getType() const {
      tinygltf::Value v(std::get<1>(type));
      std::string typeStr = v.Get<std::string>().c_str();
      return typeStr;
    }

    std::string getVersion() const {
      tinygltf::Value v(std::get<1>(version));
      std::string versionStr = v.Get<std::string>().c_str();
      return versionStr;
    }

    double getNorthAxis() const {
      tinygltf::Value v(std::get<1>(northAxis));
      double northAxisd = v.Get<double>();
      return northAxisd;
    }

    int getStoryCount() {
      return storyCount;
    }

    int getThermalZoneCount() {
      return thermalZoneCount;
    }

    int getSpaceCount() {
      return spaceCount;
    }

    int getSpaceTypeCount() {
      return spaceTypeCount;
    }

    int getConstructionSetCount() {
      return constructionSetCount;
    }

    int getAirLoopCount() {
      return airLoopCount;
    }

   private:
    std::vector<GltfModelObjectMetadata> glTFModelObjectMetadataVector;
    GltfBoundingBox glTFBoundingBox;
    std::vector<std::string> buildingStoryNames;

    // string
    std::tuple<std::string, tinygltf::Value> generator;
    std::tuple<std::string, tinygltf::Value> type;
    std::tuple<std::string, tinygltf::Value> version;
    // double
    std::tuple<std::string, tinygltf::Value> northAxis;

    int storyCount;
    int thermalZoneCount;
    int spaceCount;
    int spaceTypeCount;
    int constructionSetCount;
    int airLoopCount;
  };

  // UserData as glTF Extras
  class GltfUserData
  {
   public:
    GltfUserData() {
      // Initialize all attributes with defaults
      initializeAttributesWithDefaults();
    }
    // m_handle;
    std::string handlekey = "handle";
    void setHandle(std::string value) {
      tinygltf::Value extraNodeValue(value);
      handle = make_tuple(handlekey, extraNodeValue);
    }

    void setHandle(tinygltf::Value value) {
      handle = make_tuple(handlekey, value);
    }

    // m_name;
    std::string namekey = "name";
    void setName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      name = make_tuple(namekey, extraNodeValue);
    }

    void setName(tinygltf::Value value) {
      name = make_tuple(namekey, value);
    }

    // m_surfaceType;
    std::string surfaceTypekey = "surfaceType";
    void setSurfaceType(std::string value) {
      tinygltf::Value extraNodeValue(value);
      surfaceType = make_tuple(surfaceTypekey, extraNodeValue);
    }

    void setSurfaceType(tinygltf::Value value) {
      surfaceType = make_tuple(surfaceTypekey, value);
    }

    // m_surfaceTypeMaterialName;
    std::string surfaceTypeMaterailNamekey = "surfaceTypeMaterailName";
    void setSurfaceTypeMaterailName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      surfaceTypeMaterailName = make_tuple(surfaceTypeMaterailNamekey, extraNodeValue);
    }

    void setSurfaceTypeMaterailName(tinygltf::Value value) {
      surfaceTypeMaterailName = make_tuple(surfaceTypeMaterailNamekey, value);
    }

    // m_constructionName;
    std::string constructionNamekey = "constructionName";
    void setConstructionName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      constructionName = make_tuple(constructionNamekey, extraNodeValue);
    }

    void setConstructionName(tinygltf::Value value) {
      constructionName = make_tuple(constructionNamekey, value);
    }

    // m_constructionHandle;
    std::string constructionHandlekey = "constructionHandle";
    void setConstructionHandle(std::string value) {
      tinygltf::Value extraNodeValue(value);
      constructionHandle = make_tuple(constructionHandlekey, extraNodeValue);
    }

    void setConstructionHandle(tinygltf::Value value) {
      constructionHandle = make_tuple(constructionHandlekey, value);
    }

    // m_constructionMaterialName;
    std::string constructionMaterialNamekey = "constructionMaterialName";
    void setConstructionMaterialName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      constructionMaterialName = make_tuple(constructionMaterialNamekey, extraNodeValue);
    }

    void setConstructionMaterialName(tinygltf::Value value) {
      constructionMaterialName = make_tuple(constructionMaterialNamekey, value);
    }

    // m_surfaceName;
    std::string surfaceNamekey = "surfaceName";
    void setSurfaceName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      surfaceName = make_tuple(surfaceNamekey, extraNodeValue);
    }

    void setSurfaceName(tinygltf::Value value) {
      surfaceName = make_tuple(surfaceNamekey, value);
    }

    // m_surfaceHandle;
    std::string surfaceHandlekey = "surfaceHandle";
    void setSurfaceHandle(std::string value) {
      tinygltf::Value extraNodeValue(value);
      surfaceHandle = make_tuple(surfaceHandlekey, extraNodeValue);
    }

    void setSurfaceHandle(tinygltf::Value value) {
      surfaceHandle = make_tuple(surfaceHandlekey, value);
    }

    // m_subSurfaceName;
    std::string subSurfaceNamekey = "subSurfaceName";
    void setSubSurfaceName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      subSurfaceName = make_tuple(subSurfaceNamekey, extraNodeValue);
    }

    void setSubSurfaceName(tinygltf::Value value) {
      subSurfaceName = make_tuple(subSurfaceNamekey, value);
    }

    // m_subSurfaceHandle;
    std::string subSurfaceHandlekey = "subSurfaceHandle";
    void setSubSurfaceHandle(std::string value) {
      tinygltf::Value extraNodeValue(value);
      subSurfaceHandle = make_tuple(subSurfaceHandlekey, extraNodeValue);
    }

    void setSubSurfaceHandle(tinygltf::Value value) {
      subSurfaceHandle = make_tuple(subSurfaceHandlekey, value);
    }

    // m_spaceName;
    std::string spaceNamekey = "spaceName";
    void setSpaceName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      spaceName = make_tuple(spaceNamekey, extraNodeValue);
    }

    void setSpaceName(tinygltf::Value value) {
      spaceName = make_tuple(spaceNamekey, value);
    }

    // m_spaceHandle;
    std::string spaceHandlekey = "spaceHandle";
    void setSpaceHandle(std::string value) {
      tinygltf::Value extraNodeValue(value);
      spaceHandle = make_tuple(spaceHandlekey, extraNodeValue);
    }

    void setSpaceHandle(tinygltf::Value value) {
      spaceHandle = make_tuple(spaceHandlekey, value);
    }

    // m_shadingName
    std::string shadingNamekey = "shadingName";
    void setShadingName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      shadingName = make_tuple(shadingNamekey, extraNodeValue);
    }

    void setShadingName(tinygltf::Value value) {
      shadingName = make_tuple(shadingNamekey, value);
    }

    // m_shadingHandle;
    std::string shadingHandlekey = "shadingHandle";
    void setShadingHandle(std::string value) {
      tinygltf::Value extraNodeValue(value);
      shadingHandle = make_tuple(shadingHandlekey, extraNodeValue);
    }

    void setShadingHandle(tinygltf::Value value) {
      shadingHandle = make_tuple(shadingHandlekey, value);
    }

    // m_thermalZoneName;
    std::string thermalZoneNamekey = "thermalZoneName";
    void setThermalZoneName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      thermalZoneName = make_tuple(thermalZoneNamekey, extraNodeValue);
    }

    void setThermalZoneName(tinygltf::Value value) {
      thermalZoneName = make_tuple(thermalZoneNamekey, value);
    }

    // m_thermalZoneHandle;
    std::string thermalZoneHandlekey = "thermalZoneHandle";
    void setThermalZoneHandle(std::string value) {
      tinygltf::Value extraNodeValue(value);
      thermalZoneHandle = make_tuple(thermalZoneHandlekey, extraNodeValue);
    }

    void setThermalZoneHandle(tinygltf::Value value) {
      thermalZoneHandle = make_tuple(thermalZoneHandlekey, value);
    }

    // m_thermalZoneMaterialName;
    std::string thermalZoneMaterialNamekey = "thermalZoneMaterialName";
    void setThermalZoneMaterialName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      thermalZoneMaterialName = make_tuple(thermalZoneMaterialNamekey, extraNodeValue);
    }

    void setThermalZoneMaterialName(tinygltf::Value value) {
      thermalZoneMaterialName = make_tuple(thermalZoneMaterialNamekey, value);
    }

    // m_spaceTypeName;
    std::string spaceTypeNamekey = "spaceTypeName";
    void setSpaceTypeName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      spaceTypeName = make_tuple(spaceTypeNamekey, extraNodeValue);
    }

    void setSpaceTypeName(tinygltf::Value value) {
      spaceTypeName = make_tuple(spaceTypeNamekey, value);
    }

    // m_spaceTypeHandle;
    std::string spaceTypeHandlekey = "spaceTypeHandle";
    void setSpaceTypeHandle(std::string value) {
      tinygltf::Value extraNodeValue(value);
      spaceTypeHandle = make_tuple(spaceTypeHandlekey, extraNodeValue);
    }

    void setSpaceTypeHandle(tinygltf::Value value) {
      spaceTypeHandle = make_tuple(spaceTypeHandlekey, value);
    }

    // m_spaceTypeMaterialName;
    std::string spaceTypeMaterialNamekey = "spaceTypeMaterialName";
    void setSpaceTypeMaterialName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      spaceTypeMaterialName = make_tuple(spaceTypeMaterialNamekey, extraNodeValue);
    }

    void setSpaceTypeMaterialName(tinygltf::Value value) {
      spaceTypeMaterialName = make_tuple(spaceTypeMaterialNamekey, value);
    }

    // m_buildingStoryName;
    std::string buildingStoryNamekey = "buildingStoryName";
    void setBuildingStoryName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      buildingStoryName = make_tuple(buildingStoryNamekey, extraNodeValue);
    }

    void setBuildingStoryName(tinygltf::Value value) {
      buildingStoryName = make_tuple(buildingStoryNamekey, value);
    }

    // m_buildingStoryHandle;
    std::string buildingStoryHandlekey = "buildingStoryHandle";
    void setBuildingStoryHandle(std::string value) {
      tinygltf::Value extraNodeValue(value);
      buildingStoryHandle = make_tuple(buildingStoryHandlekey, extraNodeValue);
    }

    void setBuildingStoryHandle(tinygltf::Value value) {
      buildingStoryHandle = make_tuple(buildingStoryHandlekey, value);
    }

    // m_buildingStoryMaterialName;
    std::string buildingStoryMaterialNamekey = "buildingStoryMaterialName";
    void setBuildingStoryMaterialName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      buildingStoryMaterialName = make_tuple(buildingStoryMaterialNamekey, extraNodeValue);
    }

    void setBuildingStoryMaterialName(tinygltf::Value value) {
      buildingStoryMaterialName = make_tuple(buildingStoryMaterialNamekey, value);
    }

    // m_buildingUnitName;
    std::string buildingUnitNamekey = "buildingUnitName";
    void setBuildingUnitName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      buildingUnitName = make_tuple(buildingUnitNamekey, extraNodeValue);
    }

    void setBuildingUnitName(tinygltf::Value value) {
      buildingUnitName = make_tuple(buildingUnitNamekey, value);
    }

    // m_buildingUnitHandle;
    std::string buildingUnitHandlekey = "buildingUnitHandle";
    void setBuildingUnitHandle(std::string value) {
      tinygltf::Value extraNodeValue(value);
      buildingUnitHandle = make_tuple(buildingUnitHandlekey, extraNodeValue);
    }

    void setBuildingUnitHandle(tinygltf::Value value) {
      buildingUnitHandle = make_tuple(buildingUnitHandlekey, value);
    }

    // m_buildingUnitMaterialName;
    std::string buildingUnitMaterialNamekey = "buildingUnitMaterialName";
    void setBuildingUnitMaterialName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      buildingUnitMaterialName = make_tuple(buildingUnitMaterialNamekey, extraNodeValue);
    }

    void setBuildingUnitMaterialName(tinygltf::Value value) {
      buildingUnitMaterialName = make_tuple(buildingUnitMaterialNamekey, value);
    }

    // m_constructionSetName;
    std::string constructionSetNamekey = "constructionSetName";
    void setConstructionSetName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      constructionSetName = make_tuple(constructionSetNamekey, extraNodeValue);
    }

    void setConstructionSetName(tinygltf::Value value) {
      constructionSetName = make_tuple(constructionSetNamekey, value);
    }

    // m_constructionSetHandle;
    std::string constructionSetHandlekey = "constructionSetHandle";
    void setConstructionSetHandle(std::string value) {
      tinygltf::Value extraNodeValue(value);
      constructionSetHandle = make_tuple(constructionSetHandlekey, extraNodeValue);
    }

    void setConstructionSetHandle(tinygltf::Value value) {
      constructionSetHandle = make_tuple(constructionSetHandlekey, value);
    }

    // m_constructionSetMaterialName;
    std::string constructionSetMaterialNamekey = "constructionSetMaterialName";
    void setConstructionSetMaterialName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      constructionSetMaterialName = make_tuple(constructionSetMaterialNamekey, extraNodeValue);
    }

    void setConstructionSetMaterialName(tinygltf::Value value) {
      constructionSetMaterialName = make_tuple(constructionSetMaterialNamekey, value);
    }

    // m_outsideBoundaryCondition;
    std::string outsideBoundaryConditionkey = "outsideBoundaryCondition";
    void setOutsideBoundaryCondition(std::string value) {
      tinygltf::Value extraNodeValue(value);
      outsideBoundaryCondition = make_tuple(outsideBoundaryConditionkey, extraNodeValue);
    }

    void setOutsideBoundaryCondition(tinygltf::Value value) {
      outsideBoundaryCondition = make_tuple(outsideBoundaryConditionkey, value);
    }

    // m_outsideBoundaryConditionObjectName;
    std::string outsideBoundaryConditionObjectNamekey = "outsideBoundaryConditionObjectName";
    void setOutsideBoundaryConditionObjectName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      outsideBoundaryConditionObjectName = make_tuple(outsideBoundaryConditionObjectNamekey, extraNodeValue);
    }

    void setOutsideBoundaryConditionObjectName(tinygltf::Value value) {
      outsideBoundaryConditionObjectName = make_tuple(outsideBoundaryConditionObjectNamekey, value);
    }

    // m_outsideBoundaryConditionObjectHandle;
    std::string outsideBoundaryConditionObjectHandlekey = "outsideBoundaryConditionObjectHandle";
    void setOutsideBoundaryConditionObjectHandle(std::string value) {
      tinygltf::Value extraNodeValue(value);
      outsideBoundaryConditionObjectHandle = make_tuple(outsideBoundaryConditionObjectHandlekey, extraNodeValue);
    }

    void setOutsideBoundaryConditionObjectHandle(tinygltf::Value value) {
      outsideBoundaryConditionObjectHandle = make_tuple(outsideBoundaryConditionObjectHandlekey, value);
    }

    // m_boundaryMaterialName;
    std::string boundaryMaterialNamekey = "boundaryMaterialName";
    void setBoundaryMaterialName(std::string value) {
      tinygltf::Value extraNodeValue(value);
      boundaryMaterialName = make_tuple(boundaryMaterialNamekey, extraNodeValue);
    }

    void setBoundaryMaterialName(tinygltf::Value value) {
      boundaryMaterialName = make_tuple(boundaryMaterialNamekey, value);
    }

    // bool m_coincidentWithOutsideObject;
    std::string coincidentWithOutsideObjectkey = "coincidentWithOutsideObject";
    void setCoincidentWithOutsideObject(bool value) {
      tinygltf::Value extraNodeValue(value);
      coincidentWithOutsideObject = make_tuple(coincidentWithOutsideObjectkey, extraNodeValue);
    }

    void setCoincidentWithOutsideObject(tinygltf::Value value) {
      coincidentWithOutsideObject = make_tuple(coincidentWithOutsideObjectkey, value);
    }

    // m_sunExposure;
    std::string sunExposurekey = "sunExposure";
    void setSunExposure(std::string value) {
      tinygltf::Value extraNodeValue(value);
      sunExposure = make_tuple(sunExposurekey, extraNodeValue);
    }

    void setSunExposure(tinygltf::Value value) {
      sunExposure = make_tuple(sunExposurekey, value);
    }

    // m_windExposure;
    std::string windExposurekey = "windExposure";
    void setWindExposure(std::string value) {
      tinygltf::Value extraNodeValue(value);
      windExposure = make_tuple(windExposurekey, extraNodeValue);
    }

    void setWindExposure(tinygltf::Value value) {
      windExposure = make_tuple(windExposurekey, value);
    }

    // double m_illuminanceSetpoint;
    std::string illuminanceSetpointkey = "illuminanceSetpoint";
    void setIlluminanceSetpoint(double value) {
      tinygltf::Value extraNodeValue(value);
      illuminanceSetpoint = make_tuple(illuminanceSetpointkey, extraNodeValue);
    }

    void setIlluminanceSetpoint(tinygltf::Value value) {
      illuminanceSetpoint = make_tuple(illuminanceSetpointkey, value);
    }

    // bool m_airWall;
    std::string airWallkey = "airWall";
    void setAirWall(bool value) {
      tinygltf::Value extraNodeValue(value);
      airWall = make_tuple(airWallkey, extraNodeValue);
    }

    void setAirWall(tinygltf::Value value) {
      airWall = make_tuple(airWallkey, value);
    }

    // vector airLoopHVACNames
    std::string airLoopHVACNameskey = "airLoopHVACNames";
    void setAirLoopHVACNames(std::map<std::string, tinygltf::Value> value) {
      tinygltf::Value extraNodeValue(value);
      airLoopHVACNames = make_tuple(airLoopHVACNameskey, extraNodeValue);
    }

    void setAirLoopHVACNames(tinygltf::Value value) {
      airLoopHVACNames = make_tuple(airLoopHVACNameskey, value);
    }

    // vector airLoopHVACHandles
    std::string airLoopHVACHandleskey = "airLoopHVACHandles";
    void setAirLoopHVACHandles(std::map<std::string, tinygltf::Value> value) {
      tinygltf::Value extraNodeValue(value);
      airLoopHVACHandles = make_tuple(airLoopHVACHandleskey, extraNodeValue);
    }

    void setAirLoopHVACHandles(tinygltf::Value value) {
      airLoopHVACHandles = make_tuple(airLoopHVACHandleskey, value);
    }

    // vector airLoopHVACMaterialNames
    std::string airLoopHVACMaterialNameskey = "airLoopHVACMaterialNames";
    void setAirLoopHVACMaterialNames(std::map<std::string, tinygltf::Value> value) {
      tinygltf::Value extraNodeValue(value);
      airLoopHVACMaterialNames = make_tuple(airLoopHVACMaterialNameskey, extraNodeValue);
    }

    void setAirLoopHVACMaterialNames(tinygltf::Value value) {
      airLoopHVACMaterialNames = make_tuple(airLoopHVACMaterialNameskey, value);
    }

    std::map<std::string, tinygltf::Value> userDataObject;
    void initializeUserDataObject() {
      userDataObject.insert({std::get<0>(handle), std::get<1>(handle)});
      userDataObject.insert({std::get<0>(name), std::get<1>(name)});
      userDataObject.insert({std::get<0>(coincidentWithOutsideObject), std::get<1>(coincidentWithOutsideObject)});
      userDataObject.insert({std::get<0>(surfaceType), std::get<1>(surfaceType)});
      userDataObject.insert({std::get<0>(surfaceTypeMaterailName), std::get<1>(surfaceTypeMaterailName)});
      userDataObject.insert({std::get<0>(constructionName), std::get<1>(constructionName)});
      userDataObject.insert({std::get<0>(constructionHandle), std::get<1>(constructionHandle)});
      userDataObject.insert({std::get<0>(constructionMaterialName), std::get<1>(constructionMaterialName)});
      userDataObject.insert({std::get<0>(surfaceName), std::get<1>(surfaceName)});
      userDataObject.insert({std::get<0>(surfaceHandle), std::get<1>(surfaceHandle)});
      userDataObject.insert({std::get<0>(subSurfaceName), std::get<1>(subSurfaceName)});
      userDataObject.insert({std::get<0>(subSurfaceHandle), std::get<1>(subSurfaceHandle)});
      userDataObject.insert({std::get<0>(spaceName), std::get<1>(spaceName)});
      userDataObject.insert({std::get<0>(spaceHandle), std::get<1>(spaceHandle)});
      userDataObject.insert({std::get<0>(shadingName), std::get<1>(shadingName)});
      userDataObject.insert({std::get<0>(shadingHandle), std::get<1>(shadingHandle)});
      userDataObject.insert({std::get<0>(thermalZoneName), std::get<1>(thermalZoneName)});
      userDataObject.insert({std::get<0>(thermalZoneHandle), std::get<1>(thermalZoneHandle)});
      userDataObject.insert({std::get<0>(thermalZoneMaterialName), std::get<1>(thermalZoneMaterialName)});
      userDataObject.insert({std::get<0>(spaceTypeName), std::get<1>(spaceTypeName)});
      userDataObject.insert({std::get<0>(spaceTypeHandle), std::get<1>(spaceTypeHandle)});
      userDataObject.insert({std::get<0>(spaceTypeMaterialName), std::get<1>(spaceTypeMaterialName)});
      userDataObject.insert({std::get<0>(buildingStoryName), std::get<1>(buildingStoryName)});
      userDataObject.insert({std::get<0>(buildingStoryHandle), std::get<1>(buildingStoryHandle)});
      userDataObject.insert({std::get<0>(buildingStoryMaterialName), std::get<1>(buildingStoryMaterialName)});
      userDataObject.insert({std::get<0>(buildingUnitHandle), std::get<1>(buildingUnitHandle)});
      userDataObject.insert({std::get<0>(buildingUnitName), std::get<1>(buildingUnitName)});
      userDataObject.insert({std::get<0>(buildingUnitMaterialName), std::get<1>(buildingUnitMaterialName)});
      userDataObject.insert({std::get<0>(constructionSetName), std::get<1>(constructionSetName)});
      userDataObject.insert({std::get<0>(constructionSetHandle), std::get<1>(constructionSetHandle)});
      userDataObject.insert({std::get<0>(constructionSetMaterialName), std::get<1>(constructionSetMaterialName)});
      userDataObject.insert({std::get<0>(outsideBoundaryCondition), std::get<1>(outsideBoundaryCondition)});
      userDataObject.insert({std::get<0>(outsideBoundaryConditionObjectName), std::get<1>(outsideBoundaryConditionObjectName)});
      userDataObject.insert({std::get<0>(outsideBoundaryConditionObjectHandle), std::get<1>(outsideBoundaryConditionObjectHandle)});
      userDataObject.insert({std::get<0>(boundaryMaterialName), std::get<1>(boundaryMaterialName)});
      userDataObject.insert({std::get<0>(coincidentWithOutsideObject), std::get<1>(coincidentWithOutsideObject)});
      userDataObject.insert({std::get<0>(sunExposure), std::get<1>(sunExposure)});
      userDataObject.insert({std::get<0>(windExposure), std::get<1>(windExposure)});
      userDataObject.insert({std::get<0>(illuminanceSetpoint), std::get<1>(illuminanceSetpoint)});
      userDataObject.insert({std::get<0>(airWall), std::get<1>(airWall)});

      // Collections
      userDataObject.insert({std::get<0>(airLoopHVACNames), std::get<1>(airLoopHVACNames)});
      userDataObject.insert({std::get<0>(airLoopHVACHandles), std::get<1>(airLoopHVACHandles)});
      userDataObject.insert({std::get<0>(airLoopHVACMaterialNames), std::get<1>(airLoopHVACMaterialNames)});
    }

    void initializeFromNodeExtras(tinygltf::Node node) {
      userDataObject = node.extras.Get<std::map<std::string, tinygltf::Value>>();
      initializeFromExtras();
    }

    std::string getHandle() const {
      tinygltf::Value v(std::get<1>(handle));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getName() const {
      tinygltf::Value v(std::get<1>(name));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getSurfaceType() const {
      tinygltf::Value v(std::get<1>(surfaceType));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getSurfaceTypeMaterailName() const {
      tinygltf::Value v(std::get<1>(surfaceTypeMaterailName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getConstructionName() const {
      tinygltf::Value v(std::get<1>(constructionName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getConstructionHandle() const {
      tinygltf::Value v(std::get<1>(constructionHandle));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getConstructionMaterialName() const {
      tinygltf::Value v(std::get<1>(constructionMaterialName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getSurfaceName() const {
      tinygltf::Value v(std::get<1>(surfaceName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getSurfaceHandle() const {
      tinygltf::Value v(std::get<1>(surfaceHandle));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getSubSurfaceName() const {
      tinygltf::Value v(std::get<1>(subSurfaceName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getSubSurfaceHandle() const {
      tinygltf::Value v(std::get<1>(subSurfaceHandle));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getSpaceName() const {
      tinygltf::Value v(std::get<1>(spaceName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getSpaceHandle() const {
      tinygltf::Value v(std::get<1>(spaceHandle));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getShadingName() const {
      tinygltf::Value v(std::get<1>(shadingName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getShadingHandle() const {
      tinygltf::Value v(std::get<1>(shadingHandle));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getThermalZoneName() const {
      tinygltf::Value v(std::get<1>(thermalZoneName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getThermalZoneHandle() const {
      tinygltf::Value v(std::get<1>(thermalZoneHandle));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getThermalZoneMaterialName() const {
      tinygltf::Value v(std::get<1>(thermalZoneMaterialName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getSpaceTypeName() const {
      tinygltf::Value v(std::get<1>(spaceTypeName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getSpaceTypeHandle() const {
      tinygltf::Value v(std::get<1>(spaceTypeHandle));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getSpaceTypeMaterialName() const {
      tinygltf::Value v(std::get<1>(spaceTypeMaterialName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getBuildingStoryName() const {
      tinygltf::Value v(std::get<1>(buildingStoryName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getBuildingStoryHandle() const {
      tinygltf::Value v(std::get<1>(buildingStoryHandle));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getBuildingStoryMaterialName() const {
      tinygltf::Value v(std::get<1>(buildingStoryMaterialName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getBuildingUnitName() const {
      tinygltf::Value v(std::get<1>(buildingUnitName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getBuildingUnitHandle() const {
      tinygltf::Value v(std::get<1>(buildingUnitHandle));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getBuildingUnitMaterialName() const {
      tinygltf::Value v(std::get<1>(buildingUnitMaterialName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getConstructionSetName() const {
      tinygltf::Value v(std::get<1>(constructionSetName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getConstructionSetHandle() const {
      tinygltf::Value v(std::get<1>(constructionSetHandle));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getConstructionSetMaterialName() const {
      tinygltf::Value v(std::get<1>(constructionSetMaterialName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getOutsideBoundaryCondition() const {
      tinygltf::Value v(std::get<1>(outsideBoundaryCondition));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getOutsideBoundaryConditionObjectName() const {
      tinygltf::Value v(std::get<1>(outsideBoundaryConditionObjectName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getOutsideBoundaryConditionObjectHandle() const {
      tinygltf::Value v(std::get<1>(outsideBoundaryConditionObjectHandle));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getBoundaryMaterialName() const {
      tinygltf::Value v(std::get<1>(boundaryMaterialName));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    bool getCoincidentWithOutsideObject() const {
      tinygltf::Value v(std::get<1>(coincidentWithOutsideObject));
      bool boolVal = v.Get<bool>();
      return boolVal;
    }

    std::string getSunExposure() const {
      tinygltf::Value v(std::get<1>(sunExposure));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    std::string getWindExposure() const {
      tinygltf::Value v(std::get<1>(windExposure));
      std::string stringVal = v.Get<std::string>().c_str();
      return stringVal;
    }

    double getIlluminanceSetpoint() const {
      tinygltf::Value v(std::get<1>(illuminanceSetpoint));
      double doubleVal = v.Get<double>();
      return doubleVal;
    }

    bool getAirWall() const {
      tinygltf::Value v(std::get<1>(airWall));
      bool boolVal = v.Get<bool>();
      return boolVal;
    }

    std::vector<std::string> getAirLoopHVACNames() {
      std::vector<std::string> loopHvacNames;
      tinygltf::Value v(std::get<1>(airLoopHVACNames));
      auto collection = v.Get<std::map<std::string, tinygltf::Value>>();
      for (auto const& x : collection) {
        tinygltf::Value item(x.second);
        std::string loopName = item.Get<std::string>().c_str();
        loopHvacNames.push_back(loopName);
      }
      return loopHvacNames;
    }

    std::map<std::string, std::string> userDataPairs;
    std::map<std::string, std::string> getUserDataPairs() {
      userDataPairs.insert({handlekey, getHandle()});
      return userDataPairs;
    }

   private:
    std::tuple<std::string, tinygltf::Value> handle;
    std::tuple<std::string, tinygltf::Value> name;
    std::tuple<std::string, tinygltf::Value> surfaceType;
    std::tuple<std::string, tinygltf::Value> surfaceTypeMaterailName;
    std::tuple<std::string, tinygltf::Value> constructionName;
    std::tuple<std::string, tinygltf::Value> constructionHandle;
    std::tuple<std::string, tinygltf::Value> constructionMaterialName;
    std::tuple<std::string, tinygltf::Value> surfaceName;
    std::tuple<std::string, tinygltf::Value> surfaceHandle;
    std::tuple<std::string, tinygltf::Value> subSurfaceName;
    std::tuple<std::string, tinygltf::Value> subSurfaceHandle;
    std::tuple<std::string, tinygltf::Value> spaceName;
    std::tuple<std::string, tinygltf::Value> spaceHandle;
    std::tuple<std::string, tinygltf::Value> shadingName;
    std::tuple<std::string, tinygltf::Value> shadingHandle;
    std::tuple<std::string, tinygltf::Value> thermalZoneName;
    std::tuple<std::string, tinygltf::Value> thermalZoneHandle;
    std::tuple<std::string, tinygltf::Value> thermalZoneMaterialName;
    std::tuple<std::string, tinygltf::Value> spaceTypeName;
    std::tuple<std::string, tinygltf::Value> spaceTypeHandle;
    std::tuple<std::string, tinygltf::Value> spaceTypeMaterialName;
    std::tuple<std::string, tinygltf::Value> buildingStoryName;
    std::tuple<std::string, tinygltf::Value> buildingStoryHandle;
    std::tuple<std::string, tinygltf::Value> buildingStoryMaterialName;
    std::tuple<std::string, tinygltf::Value> buildingUnitName;
    std::tuple<std::string, tinygltf::Value> buildingUnitHandle;
    std::tuple<std::string, tinygltf::Value> buildingUnitMaterialName;
    std::tuple<std::string, tinygltf::Value> constructionSetName;
    std::tuple<std::string, tinygltf::Value> constructionSetHandle;
    std::tuple<std::string, tinygltf::Value> constructionSetMaterialName;
    std::tuple<std::string, tinygltf::Value> outsideBoundaryCondition;
    std::tuple<std::string, tinygltf::Value> outsideBoundaryConditionObjectName;
    std::tuple<std::string, tinygltf::Value> outsideBoundaryConditionObjectHandle;
    std::tuple<std::string, tinygltf::Value> boundaryMaterialName;
    std::tuple<std::string, tinygltf::Value> coincidentWithOutsideObject;
    std::tuple<std::string, tinygltf::Value> sunExposure;
    std::tuple<std::string, tinygltf::Value> windExposure;
    std::tuple<std::string, tinygltf::Value> illuminanceSetpoint;
    std::tuple<std::string, tinygltf::Value> airWall;
    std::tuple<std::string, tinygltf::Value> airLoopHVACNames;
    std::tuple<std::string, tinygltf::Value> airLoopHVACHandles;
    std::tuple<std::string, tinygltf::Value> airLoopHVACMaterialNames;

    void initializeAttributesWithDefaults() {
      setHandle("");
      setName("");
      setSurfaceType("");
      setSurfaceTypeMaterailName("");
      setConstructionName("");
      setConstructionHandle("");
      setConstructionMaterialName("");
      setSurfaceName("");
      setSurfaceHandle("");
      setSubSurfaceName("");
      setSubSurfaceHandle("");
      setSpaceName("");
      setSpaceHandle("");
      setShadingName("");
      setShadingHandle("");
      setThermalZoneName("");
      setThermalZoneHandle("");
      setThermalZoneMaterialName("");
      setSpaceTypeName("");
      setSpaceTypeHandle("");
      setSpaceTypeMaterialName("");
      setBuildingStoryName("");
      setBuildingStoryHandle("");
      setBuildingStoryMaterialName("");
      setBuildingUnitName("");
      setBuildingUnitHandle("");
      setBuildingUnitMaterialName("");
      setConstructionSetName("");
      setConstructionSetHandle("");
      setConstructionSetMaterialName("");
      setOutsideBoundaryCondition("");
      setOutsideBoundaryConditionObjectName("");
      setOutsideBoundaryConditionObjectHandle("");
      setBoundaryMaterialName("");
      setCoincidentWithOutsideObject(false);
      setSunExposure("");
      setWindExposure("");
      setIlluminanceSetpoint(0.0);
      setAirWall(false);

      std::map<std::string, tinygltf::Value> airLoopHVACHandlesObject;
      std::map<std::string, tinygltf::Value> airLoopHVACNamesObject;
      std::map<std::string, tinygltf::Value> airLoopHVACMaterialNamesObject;
      setAirLoopHVACHandles(airLoopHVACHandlesObject);
      setAirLoopHVACNames(airLoopHVACNamesObject);
      setAirLoopHVACMaterialNames(airLoopHVACMaterialNamesObject);
    }

    void initializeFromExtras() {
      setHandle(userDataObject[handlekey]);
      setName(userDataObject[namekey]);
      setSurfaceType(userDataObject[surfaceTypekey]);
      setSurfaceTypeMaterailName(userDataObject[surfaceTypeMaterailNamekey]);
      setConstructionName(userDataObject[constructionNamekey]);
      setConstructionHandle(userDataObject[constructionHandlekey]);
      setConstructionMaterialName(userDataObject[constructionMaterialNamekey]);
      setSurfaceName(userDataObject[surfaceNamekey]);
      setSurfaceHandle(userDataObject[surfaceHandlekey]);
      setSubSurfaceName(userDataObject[subSurfaceNamekey]);
      setSubSurfaceHandle(userDataObject[subSurfaceHandlekey]);
      setSpaceName(userDataObject[spaceNamekey]);
      setSpaceHandle(userDataObject[spaceHandlekey]);
      setShadingName(userDataObject[shadingNamekey]);
      setShadingHandle(userDataObject[shadingHandlekey]);
      setThermalZoneName(userDataObject[thermalZoneNamekey]);
      setThermalZoneHandle(userDataObject[thermalZoneHandlekey]);
      setThermalZoneMaterialName(userDataObject[thermalZoneMaterialNamekey]);
      setSpaceTypeName(userDataObject[spaceTypeNamekey]);
      setSpaceTypeHandle(userDataObject[spaceTypeHandlekey]);
      setSpaceTypeMaterialName(userDataObject[spaceTypeMaterialNamekey]);
      setBuildingStoryName(userDataObject[buildingStoryNamekey]);
      setBuildingStoryHandle(userDataObject[buildingStoryHandlekey]);
      setBuildingStoryMaterialName(userDataObject[buildingStoryMaterialNamekey]);
      setBuildingUnitName(userDataObject[buildingUnitNamekey]);
      setBuildingUnitHandle(userDataObject[buildingUnitHandlekey]);
      setBuildingUnitMaterialName(userDataObject[buildingUnitMaterialNamekey]);
      setConstructionSetName(userDataObject[constructionSetNamekey]);
      setConstructionSetHandle(userDataObject[constructionSetHandlekey]);
      setConstructionSetMaterialName(userDataObject[constructionSetMaterialNamekey]);
      setOutsideBoundaryCondition(userDataObject[outsideBoundaryConditionkey]);
      setOutsideBoundaryConditionObjectName(userDataObject[outsideBoundaryConditionObjectNamekey]);
      setOutsideBoundaryConditionObjectHandle(userDataObject[outsideBoundaryConditionObjectHandlekey]);
      setBoundaryMaterialName(userDataObject[boundaryMaterialNamekey]);
      // bool
      setCoincidentWithOutsideObject(userDataObject[coincidentWithOutsideObjectkey]);
      setSunExposure(userDataObject[sunExposurekey]);
      setWindExposure(userDataObject[windExposurekey]);
      setIlluminanceSetpoint(userDataObject[illuminanceSetpointkey]);
      // bool
      setAirWall(userDataObject[airWallkey]);
      // vector string
      setAirLoopHVACNames(userDataObject[airLoopHVACNameskey]);
      setAirLoopHVACHandles(userDataObject[airLoopHVACHandleskey]);
      setAirLoopHVACMaterialNames(userDataObject[airLoopHVACMaterialNameskey]);
    }
  };

  // For Indices of Indices, Coordinates & Normal buffers
  // against each Components
  struct ShapeComponentIds
  {
    int IndicesAccessorId;
    int VerticesAccessorId;
    int NormalsAccessorId;
  };

  // Creates a GLTF material on the basis of raw Material Values
  // i.e, R, G, B, A & isDoubleSided
  // and adds to GLTF model's materials
  // param : materials
  // param : materialData
  // returns : index of the created GLTF Material
  int createMaterial(std::vector<GLTF::Material>& materials, const MaterialData& materialData) {
    GLTF::Material m;
    m.name = materialData.materialName;
    GLTF::PbrMetallicRoughness pbr;
    // this contains the red, green, blue and alpha components of the main color of the material
    pbr.baseColorFactor = {(static_cast<float>(materialData.r) / 255.0f), (static_cast<float>(materialData.g) / 255.0f),
                           (static_cast<float>(materialData.b) / 255.0f), static_cast<float>(materialData.a)};
    // this indicates that the material should have very minimum reflection characteristics
    // between that of a metal and non-metal material.
    pbr.metallicFactor = 0.0;
    // this makes the material to not be perfectly mirror-like, but instead scatter the
    // reflected light a bit.
    pbr.roughnessFactor = 1.0;
    m.pbrMetallicRoughness = pbr;
    // emissive texture describes the parts of the object surface that emit light with a certain color
    m.emissiveFactor = {0.01, 0.01, 0.00};
    // occlusion texture can be used to simulate the effect of objects self-shadowing each other.
    // m.occlusionTexture = "";
    // normal map is a texture applied to modulate the surface normal in a way that makes
    // it possible to simulate finer geometric details without the cost of a higher mesh.
    // m.normalTexture = "";
    m.alphaMode = (materialData.a < 1.0f) ? "BLEND" : "OPAQUE";
    //alphaCutoff should not be 1.0f else MATERIAL_ALPHA_CUTOFF_INVALID_MODE	Alpha cutoff is supported only for 'MASK' alpha mode
    m.alphaCutoff = 0.5f;
    m.doubleSided = materialData.isDoubleSided;

    int ret = materials.size();
    materials.emplace_back(std::move(m));
    return ret;
  }

  // Initializes the SCENE for the GLTF model that will be
  // aware of all other nodes, meshes, materials, BufferViews, Accessors tightly
  // tied up in a Heirarchy using indices pointing one to other.
  // param : scene
  // param : topNode
  // param : nodes
  void initScene(GLTF::Scene& scene, GLTF::Node& topNode, std::vector<GLTF::Node>& nodes) {
    std::vector<int> ns{0};
    scene.nodes = ns;

    topNode.name = "Z_UP";
    std::vector<double> mtrx{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    topNode.matrix = mtrx;

    nodes.push_back(topNode);
  }

  // Initializes our two main Buffer Views
  // one for the indices & other for Coordinates and Normals
  // param : indicesBv
  // param : coordinatesBv
  void initBufferViews(GLTF::BufferView& indicesBv, GLTF::BufferView& coordinatesBv) {
    GLTF::BufferView bvIndices;
    bvIndices.buffer = 0;
    // defining bytestride is not required in this case
    bvIndices.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
    indicesBv = bvIndices;

    // The vertices take up 36 bytes (3 vertices * 3 floating points * 4 bytes)
    // at position 8 in the buffer and are of type ARRAY_BUFFER
    GLTF::BufferView bvCoordinates;
    bvCoordinates.buffer = 0;
    bvCoordinates.target = TINYGLTF_TARGET_ARRAY_BUFFER;
    bvCoordinates.byteStride = 12;
    coordinatesBv = bvCoordinates;
  }

  // Creates a collection of raw (values: R,G,B,A & isDoubleSided)
  // for all pre-defined GLTF material
  // returns : collection of raw material Values
  std::vector<MaterialData> getAllMaterials() {
    std::vector<MaterialData> result{
      {"Undefined", 255, 255, 255, 1, true},

      {"NormalMaterial", 255, 255, 255, 1, true},
      {"NormalMaterial_Ext", 255, 255, 255, 1},
      {"NormalMaterial_Int", 255, 0, 0, 1},

      {"Floor", 140, 140, 140, 1, true},
      {"Floor_Ext", 128, 128, 128, 1},
      {"Floor_Int", 191, 191, 191, 1},

      {"Wall", 150, 131, 75, 1, true},
      {"Wall_Ext", 204, 178, 102, 1},
      {"Wall_Int", 235, 226, 197, 1},

      {"RoofCeiling", 112, 56, 57, 1, true},
      {"RoofCeiling_Ext", 153, 76, 76, 1},
      {"RoofCeiling_Int", 202, 149, 149, 1},

      {"Window", 102, 178, 204, 0.6, true},
      {"Window_Ext", 102, 178, 204, 0.6},
      {"Window_Int", 192, 226, 235, 0.6},

      {"Door", 111, 98, 56, 1, true},
      {"Door_Ext", 153, 133, 76, 1},
      {"Door_Int", 202, 188, 149, 1},

      {"SiteShading", 55, 90, 109, 1, true},
      {"SiteShading_Ext", 75, 124, 149, 1},
      {"SiteShading_Int", 187, 209, 220, 1},

      {"BuildingShading", 83, 56, 111, 1, true},
      {"BuildingShading_Ext", 113, 76, 153, 1},
      {"BuildingShading_Int", 216, 203, 229, 1},

      {"SpaceShading", 55, 81, 130, 1, true},
      {"SpaceShading_Ext", 76, 110, 178, 1},
      {"SpaceShading_Int", 183, 197, 224, 1},

      {"InteriorPartitionSurface", 117, 138, 105, 1, true},
      {"InteriorPartitionSurface_Ext", 158, 188, 143, 1},
      {"InteriorPartitionSurface_Int", 213, 226, 207, 1},

      // start textures for boundary conditions
      {"Boundary_Surface", 0, 153, 0, 1, true},
      {"Boundary_Adiabatic", 255, 0, 0, 1, true},
      {"Boundary_Space", 255, 0, 0, 1, true},
      {"Boundary_Outdoors", 163, 204, 204, 1, true},
      {"Boundary_Outdoors_Sun", 40, 204, 204, 1, true},
      {"Boundary_Outdoors_Wind", 9, 159, 162, 1, true},
      {"Boundary_Outdoors_SunWind", 68, 119, 161, 1, true},
      {"Boundary_Ground", 204, 183, 122, 1, true},
      {"Boundary_Foundation", 117, 30, 122, 1, true},
      {"Boundary_Groundfcfactormethod", 153, 122, 30, 1, true},
      {"Boundary_Groundslabpreprocessoraverage", 255, 191, 0, 1, true},
      {"Boundary_Groundslabpreprocessorcore", 255, 182, 50, 1, true},
      {"Boundary_Groundslabpreprocessorperimeter", 255, 178, 101, 1, true},
      {"Boundary_Groundbasementpreprocessoraveragewall", 204, 51, 0, 1, true},
      {"Boundary_Groundbasementpreprocessoraveragefloor", 204, 81, 40, 1, true},
      {"Boundary_Groundbasementpreprocessorupperwall", 204, 112, 81, 1, true},
      {"Boundary_Groundbasementpreprocessorlowerwall", 204, 173, 163, 1, true},
      {"Boundary_Othersidecoefficients", 63, 63, 63, 1, true},
      {"Boundary_Othersideconditionsmodel", 153, 0, 76, 1, true},

      // special rendering materials
      {"SpaceType_Plenum", 192, 192, 192, 0.1, true},
      {"ThermalZone_Plenum", 192, 192, 192, 0.1, true},

      // special rendering materials, these are components or textures in SketchUp
      {"DaylightingControl", 102, 178, 204, 0.1, true},
      {"AirWall", 102, 178, 204, 0.1, true},
      {"SolarCollector", 255, 255, 255, 1, true},
      {"Photovoltaic", 255, 255, 255, 0.1, true},
    };
    return result;
  }

  // Adds all pre-defined materials to GLTF Model materials
  // param : materialList
  // param : materials
  void addGLTFMaterials(std::map<std::string, int>& materialList, std::vector<GLTF::Material>& materials) {
    std::vector<MaterialData> result = getAllMaterials();
    for (const auto& m : result) {
      if (materialList.find(m.materialName) != materialList.end()) {
        LOG_FREE(Debug, "GltfForwardTranslator", "addGLTFMaterials, Key " << m.materialName << " found in materialList");
      } else {
        materialList[m.materialName] = createMaterial(materials, m);
      }
    }
  }

  // Gets GLTF Material name on the basis of idd Object Type and Name
  // param : iddObjectType
  // param : name
  // returns : name of GLTF Material Name
  std::string getObjectGLTFMaterialName(const std::string& iddObjectType, const std::string& name) {
    std::string result;
    if (istringEqual(iddObjectType, "OS:Construction")) {
      result = "Construction_" + name;
    } else if (istringEqual(iddObjectType, "OS:ThermalZone")) {
      result = "ThermalZone_" + name;
    } else if (istringEqual(iddObjectType, "OS:AirLoopHVAC")) {
      result = "AirLoopHVAC_" + name;
    } else if (istringEqual(iddObjectType, "OS:SpaceType")) {
      result = "SpaceType_" + name;
    } else if (istringEqual(iddObjectType, "OS:BuildingStory")) {
      result = "BuildingStory_" + name;
    } else if (istringEqual(iddObjectType, "OS:BuildingUnit")) {
      result = "BuildingUnit_" + name;
    } else if (istringEqual(iddObjectType, "OS:Construction:AirBoundary")) {
      // This shouldn't happen
      LOG_FREE(Error, "getObjectGLTFMaterialName", "Didn't expect it would be called for '" << iddObjectType << "' (name = '" << name << "')");
      result = "ConstructionAirBoundary_" + name;
    } else {
      LOG_FREE(Error, "getObjectGLTFMaterialName", "Unknown iddObjectType '" << iddObjectType << "'");
    }
    return result;
  }

  // Gets GLTF Material Name on the basis of Model Object
  // param : object
  // returns : name of GLTF material Name
  std::string getObjectGLTFMaterialName(const ModelObject& object) {
    return getObjectGLTFMaterialName(object.iddObjectType().valueDescription(), object.nameString());
  }

  // Adds Model Specific GLTF Material to all Materials Collection
  // param : color
  // param : materialName
  // param : isDoubleSided
  // param : allMaterials
  void addModelSpecificMaterialToCollection(const RenderingColor& color, const std::string& materialName, bool isDoubleSided,
                                            std::vector<MaterialData>& allMaterials) {
    allMaterials.emplace_back(MaterialData{materialName, color.renderingRedValue(), color.renderingGreenValue(), color.renderingBlueValue(),
                                           color.renderingAlphaValue() / 255.0, true});
  }

  // Gets GLTF Material name on the basis of Suface Type
  // param : surfaceType
  // returns : surface Type
  std::string getSurfaceTypeGLTFMaterialName(const std::string& surfaceType) {
    if (istringEqual(surfaceType, "FixedWindow") || istringEqual(surfaceType, "OperableWindow") || istringEqual(surfaceType, "GlassDoor")
        || istringEqual(surfaceType, "Skylight") || istringEqual(surfaceType, "TubularDaylightDome")
        || istringEqual(surfaceType, "TubularDaylightDiffuser")) {
      return "Window";
    } else if (istringEqual(surfaceType, "Door") || istringEqual(surfaceType, "OverheadDoor")) {
      return "Door";
    }
    return surfaceType;
  }

  // Finds / Create GLTF Material depending upon MaterialName
  // param : materialName
  // param : materialList
  // param : allMaterials
  // param : materials
  // returns : index of Material
  int getGLTFMaterialIndex(const std::string& materialName, std::map<std::string, int>& materialList, std::vector<MaterialData>& allMaterials,
                           std::vector<GLTF::Material>& materials) {
    std::map<std::string, int>::const_iterator it = materialList.find(materialName);
    if (it != materialList.end()) {
      return it->second;
    } else {
      for (size_t i = 0; i < allMaterials.size(); i++) {
        if (allMaterials[i].materialName == materialName) {
          // Create Material and Add to _materials
          int newMatIndex = createMaterial(materials, allMaterials[i]);
          // Add to map list
          materialList[allMaterials[i].materialName] = newMatIndex;
          // send back index
          // return newMatIndex;
          break;
        }
      }
      std::map<std::string, int>::const_iterator it = materialList.find(materialName);
      if (it != materialList.end()) {
        return it->second;
      }
      it = materialList.find("Undefined");
      if (it != materialList.end()) {
        return it->second;
      }
    }
    return 0;
  }

  // Now: Instead of creating materials for all the objects in Model
  // we're adding them to the all materials collection and create them only if required
  // while processing the surface i.e, in MAIN LOOP
  void buildMaterials(const Model& model, std::map<std::string, int>& materialList, std::vector<MaterialData>& allMaterials) {
    std::string materialName;

    auto getOrCreateRenderingColor = [&model](auto& object) {
      boost::optional<RenderingColor> color_ = object.renderingColor();
      if (!color_) {
        color_ = RenderingColor(model);
        object.setRenderingColor(color_.get());
      }
      return color_.get();
    };

    auto getOrCreateMaterial = [&getOrCreateRenderingColor, &allMaterials, &materialList](auto& object) {
      std::string materialName = getObjectGLTFMaterialName(object);
      if (materialList.find(materialName) != materialList.end()) {
        LOG_FREE(Debug, "GltfForwardTranslator", "buildMaterials, Key " << materialName << " found in materialList");
      } else {
        // instead of creating the material whose node might get not used inshort to avoid
        // warning "UNUSED_OBJECT". we'll add the material to our allMaterial collection
        // and whie processing the surface if required we'll create and the node.
        // createMaterial(materials, color, materialName, true);
        auto color = getOrCreateRenderingColor(object);
        addModelSpecificMaterialToCollection(color, materialName, true, allMaterials);
      }
    };

    // make construction materials
    for (auto& construction : model.getModelObjects<ConstructionBase>()) {
      // If it's ConstructionAirBoundary, we'll later use the standard material "AirWall"
      if (!construction.optionalCast<ConstructionAirBoundary>()) {
        getOrCreateMaterial(construction);
      }
    }

    // make thermal zone materials
    for (auto& thermalZone : model.getConcreteModelObjects<ThermalZone>()) {
      getOrCreateMaterial(thermalZone);
    }

    // make space type materials
    for (auto& spaceType : model.getConcreteModelObjects<SpaceType>()) {
      getOrCreateMaterial(spaceType);
    }

    // make building story materials
    for (auto& buildingStory : model.getConcreteModelObjects<BuildingStory>()) {
      getOrCreateMaterial(buildingStory);
    }

    // make building unit materials
    for (auto& buildingUnit : model.getConcreteModelObjects<BuildingUnit>()) {
      getOrCreateMaterial(buildingUnit);
    }

    // make air loop HVAC materials
    for (auto& airLoopHVAC : model.getConcreteModelObjects<AirLoopHVAC>()) {
      materialName = getObjectGLTFMaterialName(airLoopHVAC);
      if (materialList.find(materialName) != materialList.end()) {
        LOG_FREE(Debug, "GltfForwardTranslator", "buildMaterials, Key " << materialName << " found in materialList");
      } else {
        RenderingColor color = RenderingColor(model);
        addModelSpecificMaterialToCollection(color, materialName, true, allMaterials);
      }
    }
  }

  // Finds the vertex Index from all Vertices of a planar Surface
  // param : point3d
  // param : allPoints
  // param : tol
  // returns :  index of the Vertex
  size_t getOrCreateVertexIndexT(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol = 0.001) {
    size_t n = allPoints.size();
    for (size_t i = 0; i < n; ++i) {
      if (openstudio::getDistance(point3d, allPoints[i]) < tol) {
        return i;
      }
    }
    allPoints.push_back(point3d);
    return (allPoints.size() - 1);
  }

  // Get Bytes from an integer
  // param : paramInt
  // returns : collection of Bytes
  std::vector<unsigned char> intToBytes(int paramInt) {
    std::vector<unsigned char> arrayOfByte(4);
    for (int i = 0; i < 4; ++i)
      arrayOfByte[3 - i] = (paramInt >> (i * 8));
    return arrayOfByte;
  }

  // Get Bytes from a value
  // typeparam name :"T"
  // param : value
  // returns : collection of bytes
  template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
  std::vector<uint8_t> splitValueToBytes(T const& value) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < sizeof(value); ++i) {
      uint8_t byte = value >> (i * 8);
      bytes.insert(bytes.begin(), byte);
    }
    return bytes;
  }

  // Adds & Creates Face Indices buffers and Accessors
  // This method infers the indicesbuffer and adds them to the glTF model accessor
  // which expects two types of accessor input one from indicesbuffer and the second
  // one form coordinates buffer. So here after appending the buffer content it is sending
  // over the index so the containing node will be aware of which one to refer.
  // A better overview here at https://github.com/KhronosGroup/glTF/blob/main/specification/2.0/figures/gltfOverview-2.0.0b.png
  // param : faceIndices
  // param : _indicesBuffer
  // param : _accessors
  // returns : index of the Face Indices
  int addIndices(std::vector<size_t>& faceIndices, std::vector<unsigned char>& indicesBuffer, std::vector<GLTF::Accessor>& accessors) {
    auto [min, max] = std::minmax_element(std::cbegin(faceIndices), std::cend(faceIndices));

    // This but seems to be just figuring out the best way to back the indices in
    // depenbding on the range of numbers
    int ct = TINYGLTF_COMPONENT_TYPE_BYTE;
    auto size = 0;

    if (*max <= static_cast<size_t>(std::pow(2, 8))) {
      ct = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
      size = sizeof(std::byte);
    } else if (*max <= static_cast<size_t>(std::pow(2, 16))) {
      ct = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
      size = sizeof(short);
    } else {
      ct = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
      size = sizeof(int);
    }
    // the offset position needs to be a multiple of the size
    // (this is from a warning we received in beta testing)
    // so we inject some padding when needed
    auto padding = indicesBuffer.size() % size;
    for (size_t i = 0; i < padding; ++i) {
      indicesBuffer.push_back(0x00);
    }

    // To avoid Accessor offset Validation Issue
    auto _padding = indicesBuffer.size() % 4;
    for (size_t i = 0; i < _padding; ++i) {
      indicesBuffer.push_back(0x00);
    }

    GLTF::Accessor indAccessor;
    indAccessor.bufferView = 0;
    indAccessor.componentType = ct;
    indAccessor.byteOffset = indicesBuffer.size();
    indAccessor.normalized = false;
    indAccessor.type = TINYGLTF_TYPE_SCALAR;
    indAccessor.count = faceIndices.size();
    indAccessor.minValues = {static_cast<double>(*min)};
    indAccessor.maxValues = {static_cast<double>(*max)};

    std::vector<unsigned char> indicesBufferData;
    for (const auto index : faceIndices) {
      std::vector<unsigned char> arrayOfByte;
      if (ct == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
        arrayOfByte.push_back((unsigned char)index);
      } else if (ct == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
        arrayOfByte = splitValueToBytes(index);
      } else {
        arrayOfByte = splitValueToBytes(index);
      }
      indicesBufferData.insert(indicesBufferData.end(), arrayOfByte.begin(), arrayOfByte.end());
    }

    if (indicesBufferData.size() < 4) {
      indicesBufferData.push_back(0x00);
    }
    indicesBuffer.insert(indicesBuffer.end(), indicesBufferData.begin(), indicesBufferData.end());

    const auto thisIndex = accessors.size();
    accessors.push_back(indAccessor);
    return thisIndex;
  }

  // Creates Coordinates / Normal Buffers and Accessors.
  // param : values
  // param : coordinatesBuffer
  // param : accessors
  // returns : index
  int createBuffers(std::vector<float>& values, std::vector<unsigned char>& coordinatesBuffer, std::vector<GLTF::Accessor>& accessors) {
    // Fixes ACCESSOR_TOTAL_OFFSET_ALIGNMENT
    // Accessor's total byteOffset XXXX isn't a multiple of componentType length 4.
    auto _padding = coordinatesBuffer.size() % 4;
    for (size_t i = 0; i < _padding; ++i) {
      coordinatesBuffer.push_back((unsigned)0);
    }
    int startingBufferPosition = coordinatesBuffer.size();
    std::vector<float> min = {FLT_MAX, FLT_MAX, FLT_MAX};
    std::vector<float> max = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
    int i = 0;
    for (const auto& value : values) {
      min[i] = std::min(value, min[i]);
      max[i] = std::max(value, max[i]);
      ++i;
      if (i > 2) i = 0;
      std::vector<unsigned char> v;
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&value);
      for (size_t i = 0; i < sizeof(float); ++i) {
        v.push_back(ptr[i]);
      }
      coordinatesBuffer.insert(coordinatesBuffer.end(), v.begin(), v.end());
    }
    // To Fix : offset 18 is not a multiple of Comonent Type lenght 4
    auto padding = coordinatesBuffer.size() % 4;
    for (size_t i = 0; i < padding; ++i) {
      coordinatesBuffer.push_back((unsigned)0);
    }
    // convert min and max to double
    std::vector<double> min_d;
    std::vector<double> max_d;
    for (const auto mn : min) {
      min_d.push_back((double)mn);
    }
    for (const auto mx : max) {
      max_d.push_back((double)mx);
    }
    GLTF::Accessor coordAccessor;
    coordAccessor.bufferView = 1;
    coordAccessor.byteOffset = startingBufferPosition;
    coordAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    coordAccessor.normalized = false;
    coordAccessor.count = values.size() / 3;
    coordAccessor.type = TINYGLTF_TYPE_VEC3;
    coordAccessor.minValues = min_d;
    coordAccessor.maxValues = max_d;

    auto ret = accessors.size();
    accessors.push_back(coordAccessor);
    return ret;
  }

  // Adds Coordinate Buffers for all vertices of the surface
  // param : allVertices
  // param : coordinatesBuffer
  // param : accessors
  // returns : index for the Coordinates Buffer
  int addCoordinates(const Point3dVector& allVertices, std::vector<unsigned char>& coordinatesBuffer, std::vector<GLTF::Accessor>& accessors) {
    std::vector<float> values(3 * allVertices.size());
    size_t i = 0;
    for (const auto& point : allVertices) {
      values[i++] = static_cast<float>(point.x());
      values[i++] = static_cast<float>(point.y());
      values[i++] = static_cast<float>(point.z());
    }
    return createBuffers(values, coordinatesBuffer, accessors);
  }

  // Adds Normal Buffers for all normal Vectors
  // param : normalVectors
  // param : coordinatesBuffer
  // param : accessors
  // returns : index for the Normals Buffer
  int addNormals(const Vector3dVector& normalVectors, std::vector<unsigned char>& coordinatesBuffer, std::vector<GLTF::Accessor>& accessors) {
    std::vector<float> values(3 * normalVectors.size());
    size_t i = 0;
    for (const auto& vec : normalVectors) {
      values[i++] = static_cast<float>(vec.x());
      values[i++] = static_cast<float>(vec.y());
      values[i++] = static_cast<float>(vec.z());
    }
    return createBuffers(values, coordinatesBuffer, accessors);
  }

  // Returns material index depending upon the surface type.
  // param : planarSurface
  // param : materials
  // param : materialList
  // param : allMaterials
  // returns : index of the Material
  int getMaterialIndex(const PlanarSurface& planarSurface, std::vector<GLTF::Material>& materials, std::map<std::string, int>& materialList,
                       std::vector<MaterialData>& allMaterials) {
    std::string surfaceType;
    std::string surfaceTypeGLTFMaterialName;
    if (auto surface = planarSurface.optionalCast<Surface>()) {
      surfaceType = surface->surfaceType();
    } else if (planarSurface.optionalCast<SubSurface>()) {
      surfaceType = planarSurface.cast<SubSurface>().subSurfaceType();
    } else if (planarSurface.optionalCast<InteriorPartitionSurface>()) {
      surfaceType = "InteriorPartitionSurface";
    } else if (planarSurface.optionalCast<ShadingSurface>()) {
      boost::optional<ShadingSurface> shadingSurface = planarSurface.optionalCast<ShadingSurface>();
      std::string shadingSurfaceType = "Building";
      if (shadingSurface->shadingSurfaceGroup()) {
        shadingSurfaceType = shadingSurface->shadingSurfaceGroup()->shadingSurfaceType();
      }
      surfaceType = shadingSurfaceType + "Shading";
    }
    surfaceTypeGLTFMaterialName = getSurfaceTypeGLTFMaterialName(surfaceType);
    return getGLTFMaterialIndex(surfaceTypeGLTFMaterialName, materialList, allMaterials, materials);
  }

  std::vector<std::byte> to_bytes(std::string const& s) {
    std::vector<std::byte> bytes;
    bytes.reserve(std::size(s));

    std::transform(std::begin(s), std::end(s), std::back_inserter(bytes), [](char c) { return std::byte(c); });

    return bytes;
  }

  std::string toglTFUUID(const std::string& uuid) {
    // uuid.to_s.gsub('{','').gsub('}','')
    if (uuid.size() > 2) {
      if ((uuid[0] == '{') && (uuid[uuid.size() - 1] == '}')) {
        return uuid.substr(1, uuid.size() - 2);
      }
    }
    return uuid;
  }

  std::string fromglTFUUID(const std::string& uuid) {
    if (uuid.size() > 2) {
      if ((uuid[0] != '{') && (uuid[uuid.size() - 1] != '}')) {
        return "{" + uuid + "}";
      }
    }
    return uuid;
  }

  std::string getBoundaryMaterialName(boost::optional<Surface> surface) {
    std::string result;
    if (surface->outsideBoundaryCondition() == "Outdoors") {
      if ((surface->sunExposure() == "SunExposed") && (surface->windExposure() == "WindExposed")) {
        result = "Boundary_Outdoors_SunWind";
      } else if (surface->sunExposure() == "SunExposed") {
        result = "Boundary_Outdoors_Sun";
      } else if (surface->windExposure() == "WindExposed") {
        result = "Boundary_Outdoors_Wind";
      } else {
        result = "Boundary_Outdoors";
      }
    } else {
      result = "Boundary_" + surface->outsideBoundaryCondition();
    }
    return result;
  }

  std::string getBoundaryMaterialName(const GltfUserData& glTFUserData) {
    std::string result;
    if (glTFUserData.getOutsideBoundaryCondition() == "Outdoors") {
      if ((glTFUserData.getSunExposure() == "SunExposed") && (glTFUserData.getWindExposure() == "WindExposed")) {
        result = "Boundary_Outdoors_SunWind";
      } else if (glTFUserData.getSunExposure() == "SunExposed") {
        result = "Boundary_Outdoors_Sun";
      } else if (glTFUserData.getWindExposure() == "WindExposed") {
        result = "Boundary_Outdoors_Wind";
      } else {
        result = "Boundary_Outdoors";
      }
    } else {
      result = "Boundary_" + glTFUserData.getOutsideBoundaryCondition();
    }
    return result;
  }

  //
  // param : planarSurface
  // returns : UserData object
  GltfUserData GetUserData(const PlanarSurface& planarSurface) {
    std::string emptyString = "";
    // Make sure to get the empty entries if one of the elements is not there
    // so the UserData node look consistent across all surface nodes
    GltfUserData glTFUserData;

    std::string name = planarSurface.nameString();
    boost::optional<Surface> surface = planarSurface.optionalCast<Surface>();
    boost::optional<ShadingSurface> shadingSurface = planarSurface.optionalCast<ShadingSurface>();
    boost::optional<InteriorPartitionSurface> interiorPartitionSurface = planarSurface.optionalCast<InteriorPartitionSurface>();
    boost::optional<SubSurface> subSurface = planarSurface.optionalCast<SubSurface>();
    boost::optional<PlanarSurfaceGroup> planarSurfaceGroup = planarSurface.planarSurfaceGroup();
    boost::optional<Space> space = planarSurface.space();
    boost::optional<ConstructionBase> construction = planarSurface.construction();

    glTFUserData.setHandle(toglTFUUID(toString(planarSurface.handle())));
    glTFUserData.setName(name);
    glTFUserData.setCoincidentWithOutsideObject(false);
    if (surface) {
      glTFUserData.setSurfaceName(surface->nameString());
      glTFUserData.setSurfaceHandle(toglTFUUID(toString(surface->handle())));
      std::string surfaceType = surface->surfaceType();
      glTFUserData.setSurfaceType(surfaceType);
      glTFUserData.setSurfaceTypeMaterailName(getSurfaceTypeGLTFMaterialName(surfaceType));
      glTFUserData.setSunExposure(surface->sunExposure());
      glTFUserData.setWindExposure(surface->windExposure());
      glTFUserData.setOutsideBoundaryCondition(surface->outsideBoundaryCondition());

      boost::optional<Surface> adjacentSurface = surface->adjacentSurface();
      if (adjacentSurface) {
        glTFUserData.setOutsideBoundaryConditionObjectName(adjacentSurface->nameString());
        glTFUserData.setOutsideBoundaryConditionObjectHandle(toglTFUUID(toString(adjacentSurface->handle())));
      }
      // set boundary conditions before calling getBoundaryMaterialName
      glTFUserData.setBoundaryMaterialName(getBoundaryMaterialName(surface));
    }

    if (shadingSurface) {
      glTFUserData.setShadingHandle(toglTFUUID(toString(shadingSurface->handle())));
      glTFUserData.setShadingName(shadingSurface->nameString());

      std::string shadingSurfaceType = "Building";
      if (shadingSurface->shadingSurfaceGroup()) {
        shadingSurfaceType = shadingSurface->shadingSurfaceGroup()->shadingSurfaceType();
      }
      std::string surfaceType = shadingSurfaceType + "Shading";
      glTFUserData.setSurfaceType(surfaceType);
      glTFUserData.setSurfaceTypeMaterailName(getSurfaceTypeGLTFMaterialName(surfaceType));
      glTFUserData.setSunExposure("SunExposed");
      glTFUserData.setWindExposure("WindExposed");
      glTFUserData.setOutsideBoundaryCondition(emptyString);
      glTFUserData.setOutsideBoundaryConditionObjectName(emptyString);
      glTFUserData.setOutsideBoundaryConditionObjectHandle(emptyString);
      glTFUserData.setBoundaryMaterialName(emptyString);
    }

    if (interiorPartitionSurface) {
      std::string surfaceType = "InteriorPartitionSurface";
      glTFUserData.setSurfaceType(surfaceType);
      glTFUserData.setSurfaceTypeMaterailName(getSurfaceTypeGLTFMaterialName(surfaceType));
      glTFUserData.setSunExposure("NoSun");
      glTFUserData.setWindExposure("NoWind");
      glTFUserData.setOutsideBoundaryCondition(emptyString);
      glTFUserData.setOutsideBoundaryConditionObjectName(emptyString);
      glTFUserData.setOutsideBoundaryConditionObjectHandle(emptyString);
      glTFUserData.setBoundaryMaterialName(emptyString);
    }

    if (subSurface) {
      glTFUserData.setSubSurfaceHandle(toglTFUUID(toString(subSurface->handle())));
      glTFUserData.setSubSurfaceName(subSurface->nameString());
      std::string subSurfaceType = subSurface->subSurfaceType();
      glTFUserData.setSurfaceType(subSurfaceType);
      glTFUserData.setSurfaceTypeMaterailName(getSurfaceTypeGLTFMaterialName(subSurfaceType));

      boost::optional<Surface> parentSurface = subSurface->surface();
      if (parentSurface) {
        glTFUserData.setOutsideBoundaryCondition(parentSurface->outsideBoundaryCondition());
        glTFUserData.setSunExposure(parentSurface->sunExposure());
        glTFUserData.setWindExposure(parentSurface->windExposure());
        glTFUserData.setSurfaceName(parentSurface->nameString());
        glTFUserData.setSurfaceHandle(toglTFUUID(toString(parentSurface->handle())));
      }

      boost::optional<SubSurface> adjacentSubSurface = subSurface->adjacentSubSurface();
      if (adjacentSubSurface) {
        glTFUserData.setOutsideBoundaryConditionObjectName(adjacentSubSurface->nameString());
        glTFUserData.setOutsideBoundaryConditionObjectHandle(toglTFUUID(toString(adjacentSubSurface->handle())));
        glTFUserData.setBoundaryMaterialName("Boundary_Surface");
      }
      // set boundary conditions before calling getBoundaryMaterialName
      glTFUserData.setBoundaryMaterialName(getBoundaryMaterialName(glTFUserData));
    }

    if (construction) {
      glTFUserData.setConstructionSetHandle(toglTFUUID(toString(construction->handle())));
      glTFUserData.setConstructionName(construction->nameString());
      // If this is a ConstructionAirBoundary, then set to the standard material "AirWall"
      if (construction->optionalCast<ConstructionAirBoundary>()) {
        glTFUserData.setConstructionMaterialName("AirWall");
      } else {
        glTFUserData.setConstructionMaterialName(getObjectGLTFMaterialName(*construction));
      }
    }

    if (space) {
      glTFUserData.setSpaceHandle(toglTFUUID(toString(space->handle())));
      glTFUserData.setSpaceName(space->nameString());
      boost::optional<ThermalZone> thermalZone = space->thermalZone();
      if (thermalZone) {
        glTFUserData.setThermalZoneHandle(toglTFUUID(toString(thermalZone->handle())));
        glTFUserData.setThermalZoneName(thermalZone->nameString());
        glTFUserData.setThermalZoneMaterialName(getObjectGLTFMaterialName(*thermalZone));
        std::vector<AirLoopHVAC> airLoopHVACs = thermalZone->airLoopHVACs();
        int airLoopHVACHandleCountKey = 0;
        int airLoopHVACNameCountKey = 0;
        int airLoopHVACMaterialNameCountKey = 0;
        std::map<std::string, tinygltf::Value> airLoopHVACHandlesObject;
        std::map<std::string, tinygltf::Value> airLoopHVACNamesObject;
        std::map<std::string, tinygltf::Value> airLoopHVACMaterialNamesObject;
        for (const auto& airLoopHVAC : airLoopHVACs) {
          tinygltf::Value airLoopHVACHandleExtraNodeValue(toglTFUUID(toString(airLoopHVAC.handle())));
          airLoopHVACHandlesObject.insert({std::to_string(airLoopHVACHandleCountKey), airLoopHVACHandleExtraNodeValue});
          airLoopHVACHandleCountKey++;

          tinygltf::Value airLoopHVACNameExtraNodeValue(airLoopHVAC.nameString());
          airLoopHVACNamesObject.insert({std::to_string(airLoopHVACNameCountKey), airLoopHVACNameExtraNodeValue});
          airLoopHVACNameCountKey++;

          tinygltf::Value airLoopHVACMaterialNameExtraNodeValue(getObjectGLTFMaterialName(airLoopHVAC));
          airLoopHVACMaterialNamesObject.insert({std::to_string(airLoopHVACMaterialNameCountKey), airLoopHVACMaterialNameExtraNodeValue});
          airLoopHVACMaterialNameCountKey++;
        }
        glTFUserData.setAirLoopHVACNames(airLoopHVACNamesObject);
        glTFUserData.setAirLoopHVACHandles(airLoopHVACHandlesObject);
        glTFUserData.setAirLoopHVACMaterialNames(airLoopHVACMaterialNamesObject);
      }

      boost::optional<SpaceType> spaceType = space->spaceType();
      if (spaceType) {
        glTFUserData.setSpaceTypeHandle(toglTFUUID(toString(spaceType->handle())));
        glTFUserData.setSpaceTypeName(spaceType->nameString());
        glTFUserData.setSpaceTypeMaterialName(getObjectGLTFMaterialName(*spaceType));
      }

      boost::optional<BuildingStory> buildingStory = space->buildingStory();
      if (buildingStory) {
        glTFUserData.setBuildingStoryHandle(toglTFUUID(toString(buildingStory->handle())));
        glTFUserData.setBuildingStoryName(buildingStory->nameString());
        glTFUserData.setBuildingStoryMaterialName(getObjectGLTFMaterialName(*buildingStory));
      }

      boost::optional<BuildingUnit> buildingUnit = space->buildingUnit();
      if (buildingUnit) {
        glTFUserData.setBuildingUnitHandle(toglTFUUID(toString(buildingUnit->handle())));
        glTFUserData.setBuildingUnitName(buildingUnit->nameString());
        glTFUserData.setBuildingUnitMaterialName(getObjectGLTFMaterialName(*buildingUnit));
      }
    }
    return glTFUserData;
  }

  //
  // param : userData
  // param : planarSurface
  // param : node
  // param : buildingTransformation
  // param : vertices
  void tieUpglTFUserDataAsExtraNode(GltfUserData glTFUserData, const PlanarSurface& planarSurface, GLTF::Node& node,
                                    const Transformation& buildingTransformation, const Point3dVector& vertices) {
    // check if the adjacent surface is truly adjacent
    // this controls display only, not energy model
    if (!glTFUserData.getOutsideBoundaryConditionObjectHandle().empty()) {

      UUID adjacentHandle = toUUID(fromglTFUUID(glTFUserData.getOutsideBoundaryConditionObjectHandle()));
      boost::optional<PlanarSurface> adjacentPlanarSurface = planarSurface.model().getModelObject<PlanarSurface>(adjacentHandle);
      OS_ASSERT(adjacentPlanarSurface);

      Transformation otherBuildingTransformation;
      if (adjacentPlanarSurface->planarSurfaceGroup()) {
        otherBuildingTransformation = adjacentPlanarSurface->planarSurfaceGroup()->buildingTransformation();
      }

      Point3dVector otherVertices = otherBuildingTransformation * adjacentPlanarSurface->vertices();
      if (circularEqual(buildingTransformation * vertices, reverse(otherVertices))) {
        glTFUserData.setCoincidentWithOutsideObject(true);
      } else {
        glTFUserData.setCoincidentWithOutsideObject(false);
      }
    }

    // Initialize UserDataObject Collection with userData private members
    glTFUserData.initializeUserDataObject();

    tinygltf::Value udObj(glTFUserData.userDataObject);
    // tie up the final object as extras
    node.extras = udObj;
  }

  std::vector<GltfUserData> glTFUserDataVector;
  GltfMetaData glTFMetaData;
  GltfMetaDataWrapper glTFMetaDataWrapper;
  void getglTFMetaData(const Model& model, GLTF::Scene& scene, std::function<void(double)>& updatePercentage,
                       std::vector<PlanarSurface>::size_type& N, double& n) {

    std::vector<PlanarSurface> planarSurfaces = model.getModelObjects<PlanarSurface>();
    std::vector<PlanarSurfaceGroup> planarSurfaceGroups = model.getModelObjects<PlanarSurfaceGroup>();
    std::vector<BuildingStory> buildingStories = model.getConcreteModelObjects<BuildingStory>();
    std::vector<BuildingUnit> buildingUnits = model.getConcreteModelObjects<BuildingUnit>();
    std::vector<ThermalZone> thermalZones = model.getConcreteModelObjects<ThermalZone>();
    std::vector<AirLoopHVAC> airLoopHVACs = model.getConcreteModelObjects<AirLoopHVAC>();
    std::vector<SpaceType> spaceTypes = model.getConcreteModelObjects<SpaceType>();
    std::vector<DefaultConstructionSet> defaultConstructionSets = model.getConcreteModelObjects<DefaultConstructionSet>();

    glTFMetaData.setBoundingBox(planarSurfaceGroups, updatePercentage, n, N);

    // Building Story Names
    glTFMetaData.setBuildingStoryNames(buildingStories, updatePercentage, n, N);
    // Generator
    glTFMetaData.setGenerator("OpenStudio");
    // type  | ??
    glTFMetaData.setType("Object");
    // Version
    glTFMetaData.setVersion("4.3");
    // North Axis
    glTFMetaData.setNorthAxis(model);

    // modelObjectMetadata Collection
    glTFMetaData.setModelObjectMetaDataForStories(buildingStories, updatePercentage, n, N);

    glTFMetaData.setModelObjectMetaDataForBuildingUnits(buildingUnits, updatePercentage, n, N);

    glTFMetaData.setModelObjectMetaDataForThermalZones(thermalZones, updatePercentage, n, N);

    glTFMetaData.setModelObjectMetaDataForSpaceTypes(spaceTypes, updatePercentage, n, N);

    glTFMetaData.setModelObjectMetaDataFordefaultConstructionSets(defaultConstructionSets, updatePercentage, n, N);

    glTFMetaData.setModelObjectMetaDataForAirLoopHVACs(airLoopHVACs, updatePercentage, n, N);

    glTFMetaData.InitializeModelObject();

    tinygltf::Value sceneExtraNode(glTFMetaData.metaData);
    // final tie up of MetaData structure as extras on Scene Node
    scene.extras = sceneExtraNode;
  }

  // Exports a gltf against a Model
  // param : model
  // param : outputPath
  // returns : exports a GLTF file against a Model
  bool GltfForwardTranslator::modelToGLTF(const Model& model, const path& outputPath) {
    return modelToGLTF(
      model, [](double percentage) {}, outputPath);
  }

  // MAIN PIPELINE TO TRANSLATE OPENSTUDIO MODEL -> GLTF MODEL
  // param : model
  // param : updatePercentage
  // param : outputPath
  // returns : exports GLTF file aginst the Model
  bool GltfForwardTranslator::modelToGLTF(const Model& model, std::function<void(double)> updatePercentage, const path& outputPath) {
    m_logSink.setThreadId(std::this_thread::get_id());
    m_logSink.resetStringStream();

    bool triangulateSurfaces = true;  //we're always triangulating the surfaces to get the best possible output.
    bool BufferInBase64 = false;      //no *.bin file is involed | everything is integrated in the mail output gltf file only.

    GLTF::TinyGLTF loader;
    GLTF::Model gltfModel;
    std::string err;
    std::string warning;
    path p;

    std::vector<GLTF::Accessor> accessors;
    std::vector<GLTF::Material> materials;
    std::vector<GLTF::Mesh> meshes;
    std::vector<GLTF::Node> nodes;
    std::vector<GLTF::BufferView> bufferViews;

    GLTF::Scene scene;
    GLTF::Buffer buffer;
    GLTF::Node topNode;

    GLTF::BufferView indicesBv;
    GLTF::BufferView coordinatesBv;
    std::vector<unsigned char> indicesBuffer;
    std::vector<unsigned char> coordinatesBuffer;

    GLTF::Asset asset;

    // Start Region INIT

    // Start Region SCENE

    initScene(scene, topNode, nodes);

    // End Region SCENE

    // Start Region BUFFERVIEWS

    initBufferViews(indicesBv, coordinatesBv);

    // End Region BUFFERVIEWS

    // End Region INIT

    std::map<std::string, int> materialList;
    std::map<std::string, int> materialIndicesUsed;
    std::vector<MaterialData> allMaterials = getAllMaterials();

    /*
    
    This comprises of the other nodes that GLTF provides 
    for further addition of complex details, textures, lights, etc.
    
    std::vector<GLTF::Scene> scenes;
    std::vector<GLTF::Buffer> buffers;
    std::vector<GLTF::Animation> animations;
    std::vector<GLTF::Texture> textures;
    std::vector<GLTF::Image> images;
    std::vector<GLTF::Skin> skins;
    std::vector<GLTF::Sampler> samplers;
    std::vector<GLTF::Camera> cameras;
    std::vector<GLTF::Light> lights;

    */

    // get number of things to translate
    std::vector<PlanarSurface> planarSurfaces = model.getModelObjects<PlanarSurface>();
    std::vector<PlanarSurfaceGroup> planarSurfaceGroups = model.getModelObjects<PlanarSurfaceGroup>();
    std::vector<BuildingStory> buildingStories = model.getConcreteModelObjects<BuildingStory>();
    std::vector<BuildingUnit> buildingUnits = model.getConcreteModelObjects<BuildingUnit>();
    std::vector<ThermalZone> thermalZones = model.getConcreteModelObjects<ThermalZone>();
    std::vector<AirLoopHVAC> airLoopHVACs = model.getConcreteModelObjects<AirLoopHVAC>();
    std::vector<SpaceType> spaceTypes = model.getConcreteModelObjects<SpaceType>();
    std::vector<DefaultConstructionSet> defaultConstructionSets = model.getConcreteModelObjects<DefaultConstructionSet>();
    double n = 0;

    std::vector<PlanarSurface>::size_type N = planarSurfaces.size() + planarSurfaceGroups.size() + buildingStories.size() + buildingUnits.size()
                                              + thermalZones.size() + airLoopHVACs.size() + spaceTypes.size() + defaultConstructionSets.size() + 1;

    updatePercentage(0.0);

    // Start Region CREATE MATERIALS
    // add model specific materials
    buildMaterials(model, materialList, allMaterials);
    // End Region CREATE MATERIALS

    TransformationVector transformStack{Transformation{}};

    for (const auto& planarSurface : planarSurfaces) {
      // Start Region MAIN LOOP
      int materialIndex = 0;
      std::string elementName = planarSurface.nameString();
      // Construct in place
      GLTF::Node& node = nodes.emplace_back();
      node.name = elementName;

      // Now the geometry
      Transformation buildingTransformation;
      if (boost::optional<PlanarSurfaceGroup> planarSurfaceGroup_ = planarSurface.planarSurfaceGroup()) {
        buildingTransformation = planarSurfaceGroup_->buildingTransformation();
      }
      // get the vertices
      Point3dVector vertices = planarSurface.vertices();
      Transformation& t = transformStack.emplace_back(Transformation::alignFace(vertices));
      Transformation tInv = t.inverse();
      Point3dVector faceVertices = reverse(tInv * vertices);
      std::vector<double> matrix = openstudio::toStandardVector(buildingTransformation.vector());

      // Adding a check to avoid warning "NODE_MATRIX_DEFAULT"	<Do not specify default transform matrix>.
      // This is the identity_matrix<4>
      std::vector<double> matrixDefaultTransformation{1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
      if (matrixDefaultTransformation != matrix) {
        node.matrix = matrix;
      } else {
        node.matrix = {};
      }
      node.mesh = meshes.size();

      // EXTRAS

      // TODO: Based on flag add UserData to nodes..
      // Initializes all userdata attributes as per the planar Surface
      GltfUserData glTFUserData = GetUserData(planarSurface);

      GLTF::Mesh& targetMesh = meshes.emplace_back();
      targetMesh.name = elementName;
      materialIndex = getMaterialIndex(planarSurface, materials, materialList, allMaterials);

      // get vertices of all sub surfaces
      Point3dVectorVector faceSubVertices;
      boost::optional<Surface> surface = planarSurface.optionalCast<Surface>();
      if (surface) {
        for (const auto& subSurface : surface->subSurfaces()) {
          faceSubVertices.push_back(reverse(tInv * subSurface.vertices()));
        }
      }

      Point3dVectorVector finalFaceVertices;
      if (triangulateSurfaces) {
        finalFaceVertices = computeTriangulation(faceVertices, faceSubVertices);
        if (finalFaceVertices.empty()) {
          LOG_FREE(Error, "modelToGLTF", "Failed to triangulate surface " << elementName << " with " << faceSubVertices.size() << " sub surfaces");
        }
      } else {
        finalFaceVertices.push_back(faceVertices);
      }

      Point3dVector allVertices;
      Point3dVector triangleVertices;
      std::vector<size_t> faceIndices;
      for (const auto& finalFaceVerts : finalFaceVertices) {
        Point3dVector finalVerts = t * finalFaceVerts;
        triangleVertices.insert(triangleVertices.end(), finalVerts.begin(), finalVerts.end());
        Point3dVector::reverse_iterator it = finalVerts.rbegin();
        Point3dVector::reverse_iterator itend = finalVerts.rend();
        for (; it != itend; ++it) {
          faceIndices.push_back(getOrCreateVertexIndexT(*it, allVertices));
        }
      }

      Vector3d outwardNormal = planarSurface.outwardNormal();
      Vector3dVector normalVectors(allVertices.size(), outwardNormal);

      ShapeComponentIds shapeComponentIds{
        addIndices(faceIndices, indicesBuffer, accessors),          // IndicesAccessorId
        addCoordinates(allVertices, coordinatesBuffer, accessors),  // VerticesAccessorId
        addNormals(normalVectors, coordinatesBuffer, accessors)     // NormalsAccessorId
      };

      GLTF::Primitive& thisPrimitive = targetMesh.primitives.emplace_back();
      thisPrimitive.attributes["NORMAL"] = shapeComponentIds.NormalsAccessorId;
      thisPrimitive.attributes["POSITION"] = shapeComponentIds.VerticesAccessorId;
      thisPrimitive.indices = shapeComponentIds.IndicesAccessorId;
      thisPrimitive.material = materialIndex;
      thisPrimitive.mode = TINYGLTF_MODE_TRIANGLES;

      // TODO: Based on a flag override UserData attribute
      // Addition of UserData as Extras to the node
      tieUpglTFUserDataAsExtraNode(glTFUserData, planarSurface, node, buildingTransformation, vertices);

      n += 1;
      updatePercentage(100.0 * n / N);
      // End Region MAIN
    }
    // Start Region BUILD SCENE | ELEMENT

    if (coordinatesBuffer.size() == 0) {
      return false;
    }

    if (!BufferInBase64) {
      // BPS:having a separate input file for the GLTF is old now everything resides
      // in the main GLTF file only..as a binary buffer data.
      auto padding = indicesBuffer.size() % 4;
      for (unsigned int i = 0; i < padding; i++) {
        indicesBuffer.push_back(0x00);  // padding bytes
      }

      std::vector<unsigned char> allBuffer = indicesBuffer;  //std::move(_indicesBuffer);
      allBuffer.insert(allBuffer.end(), coordinatesBuffer.begin(), coordinatesBuffer.end());

      buffer.data = allBuffer;
    }

    indicesBv.byteLength = indicesBuffer.size();
    indicesBv.byteOffset = 0;

    coordinatesBv.byteLength = coordinatesBuffer.size();
    coordinatesBv.byteOffset = indicesBuffer.size();

    bufferViews.emplace_back(std::move(indicesBv));
    bufferViews.emplace_back(std::move(coordinatesBv));

    // End Region BUILD SCENE | ELEMENT

    gltfModel.accessors = std::move(accessors);
    gltfModel.bufferViews = std::move(bufferViews);
    gltfModel.buffers = {buffer};

    // Other tie ups
    // Define the asset. The version is required
    asset.version = "2.0";
    asset.generator = "OpenStudio";

    // SCENE EXTRAS | this will be having all the metadata
    getglTFMetaData(model, scene, updatePercentage, N, n);

    // Now all that remains is to tie back all the loose objects into the
    // our single model.
    gltfModel.scenes.push_back(scene);  // Default scene
    gltfModel.meshes = meshes;

    std::vector<GLTF::Node> nodesNew;
    std::vector<int> nodesTemp;
    nodesTemp.resize(nodes.size() - 1);
    for (unsigned int i = 1; i < nodes.size(); i++) {
      int j = i - 1;
      nodesTemp[j] = i;
    }
    topNode.children = nodesTemp;

    nodesNew.push_back(topNode);
    int sk = 0;
    for (const auto& nn : nodes) {
      if (sk != 0) {
        nodesNew.push_back(nn);
      }
      sk++;
    }

    gltfModel.nodes = std::move(nodesNew);
    gltfModel.asset = std::move(asset);

    // Add Materials to Model
    gltfModel.materials = std::move(materials);

    // Save it to a file

    // Uncomment this to use extras_as_string [std::string] instead of extras [tinygltf::Value]
    loader.SetStoreOriginalJSONForExtrasAndExtensions(true);
    bool ret = loader.WriteGltfSceneToFile(&gltfModel, toString(outputPath),
                                           true,    // embedImages
                                           true,    // embedBuffers
                                           true,    // pretty print
                                           false);  // write binary

    updatePercentage(100.0);

    return ret;
  }

  // To export a Minimal GLTF file (Triangle with 3 Points) using raw buffer data.
  // param : outputPath
  // returns : exports triangle.gltf
  bool GltfForwardTranslator::createTriangleGLTF(const path& outputPath) {
    GLTF::Model m;
    GLTF::Scene scene;
    GLTF::Mesh mesh;
    GLTF::Primitive primitive;
    GLTF::Node node;
    GLTF::Buffer buffer;
    GLTF::BufferView bufferView1;
    GLTF::BufferView bufferView2;
    GLTF::Accessor accessor1;
    GLTF::Accessor accessor2;
    GLTF::Asset asset;

    // This is the raw data buffer.
    buffer.data = {// 6 bytes of indices and two bytes of padding
                   // 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00,
                   0x00, 0x01, 0x02, 0x00,
                   // 36 bytes of floating point numbers
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00};

    // Create an array of size equivalent to vector
    // unsigned char arr[42]

    // The indices of the vertices (ELEMENT_ARRAY_BUFFER) take up 6 bytes in the
    // start of the buffer.
    bufferView1.buffer = 0;
    bufferView1.byteOffset = 0;
    bufferView1.byteLength = 3;
    bufferView1.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

    // The vertices take up 36 bytes (3 vertices * 3 floating points * 4 bytes)
    // at position 8 in the buffer and are of type ARRAY_BUFFER
    bufferView2.buffer = 0;
    bufferView2.byteOffset = 4;
    bufferView2.byteLength = 36;
    bufferView2.target = TINYGLTF_TARGET_ARRAY_BUFFER;

    // Describe the layout of bufferView1, the indices of the vertices
    accessor1.bufferView = 0;
    accessor1.byteOffset = 0;
    accessor1.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;  // TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
    accessor1.count = 3;
    accessor1.type = TINYGLTF_TYPE_SCALAR;
    accessor1.maxValues.push_back(2);
    accessor1.minValues.push_back(0);

    // Describe the layout of bufferView2, the vertices themself
    accessor2.bufferView = 1;
    accessor2.byteOffset = 0;
    accessor2.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor2.count = 3;
    accessor2.type = TINYGLTF_TYPE_VEC3;
    accessor2.maxValues = {1.0, 1.0, 0.0};
    accessor2.minValues = {0.0, 0.0, 0.0};

    // Build the mesh primitive and add it to the mesh
    primitive.indices = 0;                 // The index of the accessor for the vertex indices
    primitive.attributes["POSITION"] = 1;  // The index of the accessor for positions
    primitive.material = 0;
    // Other Modes in GLTF TINYGLTF_MODE_POINTS | TINYGLTF_MODE_LINE;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;
    mesh.primitives.push_back(primitive);

    // Other tie ups
    node.mesh = 0;
    scene.nodes.push_back(0);  // Default scene

    // Define the asset. The version is required
    asset.version = "2.0";
    asset.generator = "OpenStudio";

    // Now all that remains is to tie back all the loose objects into the
    // our single model.
    m.scenes.push_back(scene);
    m.meshes.push_back(mesh);
    m.nodes.push_back(node);
    m.buffers.push_back(buffer);
    m.bufferViews.push_back(bufferView1);
    m.bufferViews.push_back(bufferView2);
    m.accessors.push_back(accessor1);
    m.accessors.push_back(accessor2);
    m.asset = asset;

    // Create a simple material
    GLTF::Material& mat = m.materials.emplace_back();
    mat.pbrMetallicRoughness.baseColorFactor = {1.0f, 0.9f, 0.9f, 1.0f};
    mat.pbrMetallicRoughness.metallicFactor = 0.8;
    mat.pbrMetallicRoughness.roughnessFactor = 0.5;
    mat.doubleSided = true;

    // Save it to a file
    GLTF::TinyGLTF gltf;
    bool ret = gltf.WriteGltfSceneToFile(&m, toString(outputPath),
                                         true,    // embedImages
                                         true,    // embedBuffers
                                         true,    // pretty print
                                         false);  // write binary
    return ret;
  }

  // To export a Minimal GLTF file (Triangle with 3 Points) using generated
  // raw data from Point3DVector
  // param : outputPath
  // returns : exports triangle_2.gltf
  bool GltfForwardTranslator::createTriangleGLTFFromPoint3DVector(const path& outputPath) {
    GLTF::Model m;

    //BPS: Buffer Views will not give the desired result if used emplace_back()
    GLTF::BufferView bufferView1;
    GLTF::BufferView bufferView2;

    GLTF::Scene& scene = m.scenes.emplace_back();
    GLTF::Mesh& mesh = m.meshes.emplace_back();
    GLTF::Node& node = m.nodes.emplace_back();
    GLTF::Buffer& buffer = m.buffers.emplace_back();
    GLTF::Asset& asset = m.asset;

    // Create an array of size equivalent to vector
    // unsigned char arr[42];
    // The indices of the vertices (ELEMENT_ARRAY_BUFFER) take up 6 bytes in the
    // start of the buffer.
    bufferView1.buffer = 0;
    bufferView1.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

    // The vertices take up 36 bytes (3 vertices * 3 floating points * 4 bytes)
    // at position 8 in the buffer and are of type ARRAY_BUFFER
    bufferView2.buffer = 0;
    bufferView2.target = TINYGLTF_TARGET_ARRAY_BUFFER;

    std::vector<size_t> faceIndices{0, 1, 2};
    std::vector<unsigned char> _indicesBuffer;
    std::vector<unsigned char> _coordinatesBuffer;
    Point3dVector allVertices{
      {0, 0, 0},
      {1, 0, 0},
      {0, 1, 0},
    };

    ShapeComponentIds shapeComponentIds{
      addIndices(faceIndices, _indicesBuffer, m.accessors),          //IndicesAccessorId
      addCoordinates(allVertices, _coordinatesBuffer, m.accessors),  //VerticesAccessorId
      0                                                              //NormalsAccessorId
    };

    // Build the mesh primitive and add it to the mesh
    // The index of the accessor for the vertex indices
    GLTF::Primitive& primitive = mesh.primitives.emplace_back();
    primitive.indices = 0;
    // The index of the accessor for positions
    primitive.attributes["POSITION"] = shapeComponentIds.VerticesAccessorId;
    primitive.material = 0;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;

    bufferView1.byteOffset = 0;
    bufferView1.byteLength = _indicesBuffer.size();
    bufferView2.byteOffset = _indicesBuffer.size();
    bufferView2.byteLength = _coordinatesBuffer.size();

    std::vector<unsigned char> _allBuffer = _indicesBuffer;  //std::move(_indicesBuffer);
    _allBuffer.insert(_allBuffer.end(), _coordinatesBuffer.begin(), _coordinatesBuffer.end());

    buffer.data = _allBuffer;

    // Other tie ups
    node.mesh = 0;
    scene.nodes.push_back(0);  // Default scene

    // Define the asset. The version is required
    asset.version = "2.0";
    asset.generator = "OpenStudio";

    // Now all that remains is to tie back all the loose objects into the
    // our single model.
    m.bufferViews.emplace_back(std::move(bufferView1));
    m.bufferViews.emplace_back(std::move(bufferView2));

    // Create a simple material
    GLTF::Material mat;
    mat.pbrMetallicRoughness.baseColorFactor = {1.0f, 0.9f, 0.9f, 1.0f};
    mat.pbrMetallicRoughness.metallicFactor = 1.0;
    mat.pbrMetallicRoughness.roughnessFactor = 0.5;
    mat.doubleSided = true;
    m.materials.emplace_back(std::move(mat));

    // Save it to a file
    GLTF::TinyGLTF gltf;
    bool ret = gltf.WriteGltfSceneToFile(&m, toString(outputPath),
                                         true,    // embedImages
                                         true,    // embedBuffers
                                         true,    // pretty print
                                         false);  // write binary
    return ret;
  }

  // To populate a GLTF Model from an existing GLTF file.
  // also exports a gltf file with a .bin file (non embeded version).
  // param : inputPath
  // param : inputNonEmbededPath
  // returns :
  bool GltfForwardTranslator::loadGLTF(const path& inputPath, const path& inputNonEmbededPath) {
    GLTF::TinyGLTF loader;
    GLTF::Model gltf_Model;
    std::string err;
    std::string warning;
    std::string fileName = toString(inputPath);
    bool ret = loader.LoadASCIIFromFile(&gltf_Model, &err, &warning, fileName);
    if (err.size() != 0) {
      LOG(Error, "Error loading GLTF " << err);
      //ret = false;
    }
    if (!warning.empty()) {
      LOG(Warn, "Error loading GLTF " << warning);
    }

    if (!ret) {
      LOG(Error, "Failed to parse glTF");
    } else {
      std::string output_filename = toString(inputNonEmbededPath);
      std::string embedded_filename = output_filename.substr(0, output_filename.size() - 5) + "-Embedded.gltf";
      bool a = loader.WriteGltfSceneToFile(&gltf_Model, output_filename);
      OS_ASSERT(a);
      // Embedd buffers and (images if present)
#ifndef TINYGLTF_NO_STB_IMAGE_WRITE
      bool b = loader.WriteGltfSceneToFile(&gltf_Model, embedded_filename, true, true);
      OS_ASSERT(b);
#endif
    }
    return ret;
  }

  GltfBoundingBoxWrapper glTFBoundingBoxWrapper;
  std::vector<GltfUserDataWrapper> glTFUserDataWrapperVector;

  void initializeBoundingBoxWrapper(tinygltf::Value value) {
    glTFBoundingBoxWrapper = GltfBoundingBoxWrapper();
    for (auto const& x : value.Get<std::map<std::string, tinygltf::Value>>()) {
      std::string key = x.first;
      if (key == "lookAtR") {
        tinygltf::Value v(x.second);
        double vv = v.Get<double>();
        glTFBoundingBoxWrapper.setlookAtR(vv);
      } else if (key == "lookAtX") {
        tinygltf::Value v(x.second);
        double vv = v.Get<double>();
        glTFBoundingBoxWrapper.setlookAtX(vv);
      } else if (key == "lookAtY") {
        tinygltf::Value v(x.second);
        double vv = v.Get<double>();
        glTFBoundingBoxWrapper.setlookAtY(vv);
      } else if (key == "lookAtZ") {
        tinygltf::Value v(x.second);
        double vv = v.Get<double>();
        glTFBoundingBoxWrapper.setlookAtZ(vv);
      } else if (key == "maxX") {
        tinygltf::Value v(x.second);
        double vv = v.Get<double>();
        glTFBoundingBoxWrapper.setmaxX(vv);
      } else if (key == "maxY") {
        tinygltf::Value v(x.second);
        double vv = v.Get<double>();
        glTFBoundingBoxWrapper.setmaxY(vv);
      } else if (key == "maxZ") {
        tinygltf::Value v(x.second);
        double vv = v.Get<double>();
        glTFBoundingBoxWrapper.setmaxZ(vv);
      } else if (key == "minX") {
        tinygltf::Value v(x.second);
        double vv = v.Get<double>();
        glTFBoundingBoxWrapper.setminX(vv);
      } else if (key == "minY") {
        tinygltf::Value v(x.second);
        double vv = v.Get<double>();
        glTFBoundingBoxWrapper.setminY(vv);
      } else if (key == "minZ") {
        tinygltf::Value v(x.second);
        double vv = v.Get<double>();
        glTFBoundingBoxWrapper.setminZ(vv);
      }
    }
  }

  void setModelObjectMetaDataWrapper(tinygltf::Value value) {
    auto modelObjectMetatDataVector = value.Get<tinygltf::Value::Object>();
    int countKey = 0;
    for (auto const& x : modelObjectMetatDataVector) {
      GltfModelObjectMetadataWrapper gltfModelObjectMetadata;
      auto key = x.first;
      tinygltf::Value v(x.second);
      auto modelObjectMetaData1 = v.Get<tinygltf::Value::Object>();
      for (auto const& y : modelObjectMetaData1) {
        auto key1 = y.first;
        tinygltf::Value v1(y.second);
        if (key1 == "color") {
          std::string colorStr = y.second.Get<std::string>().c_str();
          gltfModelObjectMetadata.setColor(colorStr);
        } else if (key1 == "handle") {
          std::string handleStr = y.second.Get<std::string>().c_str();
          gltfModelObjectMetadata.setHandle(handleStr);
        } else if (key1 == "iddObjectType") {
          std::string iddObjectTypeStr = y.second.Get<std::string>().c_str();
          gltfModelObjectMetadata.setIddObjectType(iddObjectTypeStr);
        } else if (key1 == "name") {
          std::string nameStr = y.second.Get<std::string>().c_str();
          gltfModelObjectMetadata.setName(nameStr);
        } else if (key1 == "multiplier") {
          int multiplierVal = y.second.Get<int>();
          gltfModelObjectMetadata.setMultiplier(multiplierVal);
        } else if (key1 == "open_to_below") {
          bool openToBelow = y.second.Get<bool>();
          gltfModelObjectMetadata.setOpen_to_below(openToBelow);
        } else if (key1 == "nominal_z_coordinate") {
          double z = y.second.Get<double>();
          gltfModelObjectMetadata.setNominal_z_coordinate(z);
        } else if (key1 == "nominal_floorCeiling_Height") {
          double h = y.second.Get<double>();
          gltfModelObjectMetadata.setNominal_floorCeiling_Height(h);
        }
      }
      std::string iddObjectTypeStr = gltfModelObjectMetadata.getIddObjectType();
      if (iddObjectTypeStr == "OS:BuildingStory") {
        glTFMetaDataWrapper.storyCountIncrement();
      } else if (iddObjectTypeStr == "OS:Space") {
        glTFMetaDataWrapper.spaceCountIncrement();
      } else if (iddObjectTypeStr == "OS:ThermalZone") {
        glTFMetaDataWrapper.thermalZoneCountIncrement();
      } else if (iddObjectTypeStr == "OS:SpaceType") {
        glTFMetaDataWrapper.spaceTypeCountIncrement();
      } else if (iddObjectTypeStr == "OS:DefaultConstructionSet") {
        glTFMetaDataWrapper.constructionSetCountIncrement();
      } else if (iddObjectTypeStr.compare("OS:AirLoopHVAC") == 0) {
        glTFMetaDataWrapper.airLoopCountIncrement();
      }
      glTFMetaDataWrapper.addModelObjectMetadata(gltfModelObjectMetadata);
      countKey++;
    }
  }

  void initializeFromSceneExtras(const tinygltf::Scene& scene) {
    glTFMetaDataWrapper = GltfMetaDataWrapper();
    for (auto const& x : scene.extras.Get<std::map<std::string, tinygltf::Value>>()) {
      std::string key = x.first;
      if (key == "generator") {
        tinygltf::Value v(x.second);
        auto vv = v.Get<std::string>().c_str();
        glTFMetaDataWrapper.setGenerator(vv);
      } else if (key == "type") {
        tinygltf::Value v(x.second);
        auto vv = v.Get<std::string>().c_str();
        glTFMetaDataWrapper.setType(vv);
      } else if (key == "northAxis") {
        tinygltf::Value v(x.second);
        double vv = v.Get<double>();
        glTFMetaDataWrapper.setNorthAxis(vv);
      } else if (key == "version") {
        tinygltf::Value v(x.second);
        auto vv = v.Get<std::string>().c_str();
        glTFMetaDataWrapper.setVersion(vv);
      } else if (key == "boundingbox") {
        tinygltf::Value v(x.second);
        initializeBoundingBoxWrapper(v);
        glTFMetaDataWrapper.setglTFBoundingBoxWrapper(glTFBoundingBoxWrapper);
      } else if (key == "modelObjectMetaData") {
        tinygltf::Value v(x.second);
        setModelObjectMetaDataWrapper(v);
      } else if (key == "buildingStoryNames") {
        std::vector<std::string> storyNames;
        for (const auto& n : x.second.Get<std::map<std::string, tinygltf::Value>>()) {
          storyNames.push_back(n.second.Get<std::string>().c_str());
        }
        glTFMetaDataWrapper.setBuildingStoryNames(storyNames);
      }
    }
  }

  // <summary>
  // initializes the glTFUserData from the extras attribute attached to the
  // </summary>
  // param : node
  // param : glTFUserDataWrapper
  void initializeFromNodeExtras(const tinygltf::Node& node, GltfUserDataWrapper& glTFUserDataWrapper) {
    std::map<std::string, tinygltf::Value> userDataObject = node.extras.Get<std::map<std::string, tinygltf::Value>>();
    glTFUserDataWrapper.setHandle(userDataObject["handle"].Get<std::string>().c_str());
    glTFUserDataWrapper.setName(userDataObject["name"].Get<std::string>().c_str());
    glTFUserDataWrapper.setSurfaceType(userDataObject["surfaceType"].Get<std::string>().c_str());
    glTFUserDataWrapper.setSurfaceTypeMaterailName(userDataObject["surfaceTypeMaterailName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setConstructionName(userDataObject["constructionName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setConstructionHandle(userDataObject["constructionHandle"].Get<std::string>().c_str());
    glTFUserDataWrapper.setConstructionMaterialName(userDataObject["constructionMaterialName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setSurfaceName(userDataObject["surfaceName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setSurfaceHandle(userDataObject["surfaceHandle"].Get<std::string>().c_str());
    glTFUserDataWrapper.setSubSurfaceName(userDataObject["subSurfaceName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setSubSurfaceHandle(userDataObject["subSurfaceHandle"].Get<std::string>().c_str());
    glTFUserDataWrapper.setSpaceName(userDataObject["spaceName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setSpaceHandle(userDataObject["spaceHandle"].Get<std::string>().c_str());
    glTFUserDataWrapper.setShadingName(userDataObject["shadingName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setShadingHandle(userDataObject["shadingHandle"].Get<std::string>().c_str());
    glTFUserDataWrapper.setThermalZoneName(userDataObject["thermalZoneName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setThermalZoneHandle(userDataObject["thermalZoneHandle"].Get<std::string>().c_str());
    glTFUserDataWrapper.setThermalZoneMaterialName(userDataObject["thermalZoneMaterialName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setSpaceTypeName(userDataObject["spaceTypeName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setSpaceTypeHandle(userDataObject["spaceTypeHandle"].Get<std::string>().c_str());
    glTFUserDataWrapper.setSpaceTypeMaterialName(userDataObject["spaceTypeMaterialName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setBuildingStoryName(userDataObject["buildingStoryName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setBuildingStoryHandle(userDataObject["buildingStoryHandle"].Get<std::string>().c_str());
    glTFUserDataWrapper.setBuildingStoryMaterialName(userDataObject["buildingStoryMaterialName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setBuildingUnitName(userDataObject["buildingUnitName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setBuildingUnitHandle(userDataObject["buildingUnitHandle"].Get<std::string>().c_str());
    glTFUserDataWrapper.setBuildingUnitMaterialName(userDataObject["buildingUnitMaterialName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setConstructionSetName(userDataObject["constructionSetName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setConstructionSetHandle(userDataObject["constructionSetHandle"].Get<std::string>().c_str());
    glTFUserDataWrapper.setConstructionSetMaterialName(userDataObject["constructionSetMaterialName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setOutsideBoundaryCondition(userDataObject["outsideBoundaryCondition"].Get<std::string>().c_str());
    glTFUserDataWrapper.setOutsideBoundaryConditionObjectName(userDataObject["outsideBoundaryConditionObjectName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setOutsideBoundaryConditionObjectHandle(userDataObject["outsideBoundaryConditionObjectHandle"].Get<std::string>().c_str());
    glTFUserDataWrapper.setBoundaryMaterialName(userDataObject["boundaryMaterialName"].Get<std::string>().c_str());
    glTFUserDataWrapper.setCoincidentWithOutsideObject(userDataObject["coincidentWithOutsideObject"].Get<double>());
    glTFUserDataWrapper.setSunExposure(userDataObject["sunExposure"].Get<std::string>().c_str());
    glTFUserDataWrapper.setWindExposure(userDataObject["windExposure"].Get<std::string>().c_str());
    glTFUserDataWrapper.setIlluminanceSetpoint(userDataObject["illuminanceSetpoint"].Get<double>());
    glTFUserDataWrapper.setAirWall(userDataObject["airWall"].Get<bool>());
    //vector string
    std::vector<std::string> names;
    for (const auto& n : userDataObject["airLoopHVACNames"].Get<std::map<std::string, tinygltf::Value>>()) {
      names.push_back(n.second.Get<std::string>().c_str());
    }
    glTFUserDataWrapper.setAirLoopHVACNames(names);

    std::vector<std::string> handles;
    for (const auto& h : userDataObject["airLoopHVACHandles"].Get<std::map<std::string, tinygltf::Value>>()) {
      handles.push_back(h.second.Get<std::string>().c_str());
    }
    glTFUserDataWrapper.setAirLoopHVACHandles(handles);

    std::vector<std::string> materialNames;
    for (const auto& mn : userDataObject["airLoopHVACMaterialNames"].Get<std::map<std::string, tinygltf::Value>>()) {
      materialNames.push_back(mn.second.Get<std::string>().c_str());
    }
    glTFUserDataWrapper.setAirLoopHVACMaterialNames(materialNames);
  }

  bool GltfForwardTranslator::loadGLTF(const path& inputPath) {
    GLTF::TinyGLTF loader;
    GLTF::Model gltf_Model;
    std::string err;
    std::string warning;
    std::string fileName = toString(inputPath);
    bool ret = loader.LoadASCIIFromFile(&gltf_Model, &err, &warning, fileName);
    if (err.size() != 0) {
      LOG(Error, "Error loading GLTF " << err);
      //ret = false;
    }
    if (!warning.empty()) {
      LOG(Warn, "Error loading GLTF " << warning);
    }

    if (!ret) {
      LOG(Error, "Failed to parse glTF");
    } else {
      // USERDATA INITIALIZATION FROM EACH NODE's EXTRAS
      glTFUserDataVector.clear();
      for (unsigned int i = 1; i < gltf_Model.nodes.size(); i++) {
        GltfUserData glTFUserData;  // reinitialize glTFUserData instance
        glTFUserData.initializeFromNodeExtras(gltf_Model.nodes[i]);
        glTFUserDataVector.push_back(glTFUserData);

        GltfUserDataWrapper glTFUserDataWrapper;
        initializeFromNodeExtras(gltf_Model.nodes[i], glTFUserDataWrapper);
        glTFUserDataWrapperVector.push_back(glTFUserDataWrapper);
      }

      // METADATA INITIALIZATION FROM SCENE's EXTRAS
      glTFMetaData = GltfMetaData();
      glTFMetaData.initializeFromSceneExtras(gltf_Model.scenes[0]);
      glTFMetaDataWrapper = GltfMetaDataWrapper();  // reinitialize glTFMetaData instance
      initializeFromSceneExtras(gltf_Model.scenes[0]);
    }
    return ret;
  }

  GltfUserDataWrapperVector GltfForwardTranslator::getUserDataCollection() {
    return glTFUserDataWrapperVector;
  }

  GltfUserDataWrapper GltfForwardTranslator::getUserDataBySurfaceName(const std::string& surfaceName) {
    GltfUserDataWrapper userdata;
    // TOOD: Implement find in userdata collection
    std::vector<GltfUserDataWrapper>::iterator it;
    it = std::find_if(glTFUserDataWrapperVector.begin(), glTFUserDataWrapperVector.end(),
                      [&](GltfUserDataWrapper const& obj) { return obj.getName() == surfaceName; });
    if (it != glTFUserDataWrapperVector.end()) {
      // found it->getName() | it->getHandle()
      userdata = *it;
    } else {
      // "Item not Found"
    }
    return userdata;
  }

  GltfMetaDataWrapper GltfForwardTranslator::getMetaData() {
    return glTFMetaDataWrapper;
  }

  GltfForwardTranslator::GltfForwardTranslator() {
    m_logSink.setLogLevel(Warn);
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  std::vector<LogMessage> GltfForwardTranslator::warnings() const {
    std::vector<LogMessage> result;
    for (LogMessage logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() == Warn) {
        result.push_back(logMessage);
      }
    }
    return result;
  }

  std::vector<LogMessage> GltfForwardTranslator::errors() const {
    std::vector<LogMessage> result;
    for (LogMessage logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() > Warn) {
        result.push_back(logMessage);
      }
    }
    return result;
  }

}  // namespace model
}  // namespace openstudio
