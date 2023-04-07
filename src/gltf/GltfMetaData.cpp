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

#include "GltfMetaData.hpp"

#include "../model/Model.hpp"

#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"

#include "../model/PlanarSurface.hpp"
#include "../model/PlanarSurfaceGroup.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/BuildingUnit.hpp"
#include "../model/BuildingUnit_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/AirLoopHVAC.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"

#include <OpenStudio.hxx>

#include <tiny_gltf.h>

#include <map>
#include <string>

namespace openstudio {
namespace gltf {

  GltfMetaData::GltfMetaData() = default;

  GltfMetaData::GltfMetaData(const model::Model& model) {

    // Generator
    setGenerator("OpenStudio");
    // type  | ??
    setType("Object");
    // Version
    setVersion(openStudioVersion());
    // North Axis
    setNorthAxis(model);

    m_glTFBoundingBox = GltfBoundingBox(model.getModelObjects<model::PlanarSurfaceGroup>());

    // Building Story Names
    auto buildingStories = model.getConcreteModelObjects<model::BuildingStory>();
    for (const auto& buildingStory : buildingStories) {
      m_glTFModelObjectMetaDataVector.emplace_back(buildingStory);
      m_buildingStoryNames.emplace_back(buildingStory.nameString());
      ++m_buildingStoryCount;
      // TODO: I am matching what the initial implementation was doing, I guess the idea is to have spaces follow the story they belong to
      for (const auto& space : buildingStory.spaces()) {
        m_glTFModelObjectMetaDataVector.emplace_back(space);
        ++m_spaceCount;
      }
    }

    for (const auto& buildingUnit : model.getConcreteModelObjects<model::BuildingUnit>()) {
      m_glTFModelObjectMetaDataVector.emplace_back(buildingUnit);
      ++m_buildingUnitCount;
    }

    for (const auto& thermalZone : model.getConcreteModelObjects<model::ThermalZone>()) {
      m_glTFModelObjectMetaDataVector.emplace_back(thermalZone);
      ++m_thermalZoneCount;
    }

    for (const auto& buildingUnit : model.getConcreteModelObjects<model::SpaceType>()) {
      m_glTFModelObjectMetaDataVector.emplace_back(buildingUnit);
      ++m_spaceTypeCount;
    }

    for (const auto& buildingUnit : model.getConcreteModelObjects<model::DefaultConstructionSet>()) {
      m_glTFModelObjectMetaDataVector.emplace_back(buildingUnit);
      ++m_constructionSetCount;
    }

    for (const auto& buildingUnit : model.getConcreteModelObjects<model::AirLoopHVAC>()) {
      m_glTFModelObjectMetaDataVector.emplace_back(buildingUnit);
      ++m_airLoopCount;
    }
  }

  std::vector<GltfModelObjectMetaData> GltfMetaData::glTFModelObjectMetaDataVector() const {
    return m_glTFModelObjectMetaDataVector;
  }
  void GltfMetaData::setGlTFModelObjectMetaDataVector(const std::vector<GltfModelObjectMetaData>& glTFModelObjectMetaDataVector) {
    m_glTFModelObjectMetaDataVector = glTFModelObjectMetaDataVector;
  };

  GltfBoundingBox GltfMetaData::glTFBoundingBox() const {
    return m_glTFBoundingBox;
  }
  void GltfMetaData::setGlTFBoundingBox(const GltfBoundingBox& glTFBoundingBox) {
    m_glTFBoundingBox = glTFBoundingBox;
  };

  std::vector<std::string> GltfMetaData::buildingStoryNames() const {
    return m_buildingStoryNames;
  }
  void GltfMetaData::setBuildingStoryNames(const std::vector<std::string>& buildingStoryNames) {
    m_buildingStoryNames = buildingStoryNames;
  };

  void GltfMetaData::setBuildingStoryNames(const std::vector<model::BuildingStory>& buildingStories) {
    m_buildingStoryNames.clear();
    m_buildingStoryCount = 0;
    for (const auto& buildingStory : buildingStories) {
      m_buildingStoryNames.emplace_back(buildingStory.nameString());
      ++m_buildingStoryCount;
    }
  }

  std::string GltfMetaData::generator() const {
    return m_generator;
  }
  void GltfMetaData::setGenerator(const std::string& generator) {
    m_generator = generator;
  };

  std::string GltfMetaData::type() const {
    return m_type;
  }
  void GltfMetaData::setType(const std::string& type) {
    m_type = type;
  };

  std::string GltfMetaData::version() const {
    return m_version;
  }
  void GltfMetaData::setVersion(const std::string& version) {
    m_version = version;
  };

  double GltfMetaData::northAxis() const {
    return m_northAxis;
  }
  void GltfMetaData::setNorthAxis(double northAxis) {
    m_northAxis = northAxis;
  };

  void GltfMetaData::setNorthAxis(const model::Model& model) {
    m_northAxis = 0.0;
    if (auto building_ = model.getOptionalUniqueModelObject<model::Building>()) {
      m_northAxis = -building_->northAxis();
    }
  }

  int GltfMetaData::thermalZoneCount() const {
    return m_thermalZoneCount;
  }
  void GltfMetaData::setThermalZoneCount(int thermalZoneCount) {
    m_thermalZoneCount = thermalZoneCount;
  };

  int GltfMetaData::spaceCount() const {
    return m_spaceCount;
  }
  void GltfMetaData::setSpaceCount(int spaceCount) {
    m_spaceCount = spaceCount;
  };

  int GltfMetaData::spaceTypeCount() const {
    return m_spaceTypeCount;
  }
  void GltfMetaData::setSpaceTypeCount(int spaceTypeCount) {
    m_spaceTypeCount = spaceTypeCount;
  };

  int GltfMetaData::constructionSetCount() const {
    return m_constructionSetCount;
  }
  void GltfMetaData::setConstructionSetCount(int constructionSetCount) {
    m_constructionSetCount = constructionSetCount;
  };

  int GltfMetaData::airLoopCount() const {
    return m_airLoopCount;
  }
  void GltfMetaData::setAirLoopCount(int airLoopCount) {
    m_airLoopCount = airLoopCount;
  };

  int GltfMetaData::buildingStoryCount() const {
    return m_buildingStoryCount;
  }
  void GltfMetaData::setBuildingStoryCount(int buildingStoryCount) {
    m_buildingStoryCount = buildingStoryCount;
  };

  std::map<std::string, tinygltf::Value> GltfMetaData::toExtras() const {

    // TODO: use an array instead of a map<std::string(int), std::string>
    std::map<std::string, tinygltf::Value> modelObjectMetaDataVector;
    for (size_t i = 0; i < m_glTFModelObjectMetaDataVector.size(); ++i) {
      modelObjectMetaDataVector.emplace(std::make_pair(std::to_string(i), tinygltf::Value(m_glTFModelObjectMetaDataVector[i].toExtras())));
    }

    std::map<std::string, tinygltf::Value> buildingStoryNamesObject;
    for (size_t i = 0; i < m_buildingStoryNames.size(); ++i) {
      buildingStoryNamesObject.emplace(std::make_pair(std::to_string(i), tinygltf::Value(m_buildingStoryNames[i])));
    }

    return {
      {"modelObjectMetaData", tinygltf::Value(modelObjectMetaDataVector)},
      {"boundingbox", tinygltf::Value(m_glTFBoundingBox.toExtras())},
      {"buildingStoryNames", tinygltf::Value(buildingStoryNamesObject)},
      {"generator", tinygltf::Value(m_generator)},
      {"type", tinygltf::Value(m_type)},
      {"version", tinygltf::Value(m_version)},
      {"northAxis", tinygltf::Value(m_northAxis)},
      // {"thermalZoneCount", tinygltf::Value(m_thermalZoneCount)},
      // {"spaceCount", tinygltf::Value(m_spaceCount)},
      // {"spaceTypeCount", tinygltf::Value(m_spaceTypeCount)},
      // {"constructionSetCount", tinygltf::Value(m_constructionSetCount)},
      // {"airLoopCount", tinygltf::Value(m_airLoopCount)},
      // {"buildingStoryCount", tinygltf::Value(m_buildingStoryCount)},
    };
  }

  GltfMetaData::GltfMetaData(const tinygltf::Value& metaDataObj) {

    if (metaDataObj.Has("modelObjectMetaData")) {
      auto val = metaDataObj.Get("modelObjectMetaData");
      if (val.IsObject()) {
        for (const auto& vv : val.Get<std::map<std::string, tinygltf::Value>>()) {
          if (vv.second.IsObject()) {
            // Constructs in place via GltfModelObjectMetaData(const tinygltf::Value&) ctor
            m_glTFModelObjectMetaDataVector.emplace_back(GltfModelObjectMetaData(vv.second));

            std::string iddObjectTypeStr = m_glTFModelObjectMetaDataVector.back().iddObjectType();
            if (iddObjectTypeStr == "OS:BuildingStory") {
              m_buildingStoryCount++;
            } else if (iddObjectTypeStr == "OS:Space") {
              m_spaceCount++;
            } else if (iddObjectTypeStr == "OS:ThermalZone") {
              m_thermalZoneCount++;
            } else if (iddObjectTypeStr == "OS:SpaceType") {
              m_spaceTypeCount++;
            } else if (iddObjectTypeStr == "OS:DefaultConstructionSet") {
              m_constructionSetCount++;
            } else if (iddObjectTypeStr == "OS:AirLoopHVAC") {
              m_airLoopCount++;
            } else if (iddObjectTypeStr == "OS:BuildingUnit") {
              m_buildingUnitCount++;
            }

          } else {
            LOG(Warn, "metaDataObj has the key 'modelObjectMetaData', it is an Object, but at least one of the subobjects is not an Object");
          }
        }
      } else {
        LOG(Warn, "metaDataObj has the key 'modelObjectMetaData' but it is not an Object");
      }
    } else {
      LOG(Debug, "metaDataObj is missing the key 'airWall'");
    }

    if (metaDataObj.Has("boundingbox")) {
      auto val = metaDataObj.Get("boundingbox");
      // TODO: do I check here, or do I actually let the GltfBoundingBox ctor throw?
      if (val.IsObject()) {
        m_glTFBoundingBox = GltfBoundingBox(val);
      } else {
        LOG(Debug, "metaDataObj has the key 'boundingbox' but it is not a an Object");
      }
    } else {
      LOG(Debug, "metaDataObj is missing the key 'boundingbox'");
    }

    if (metaDataObj.Has("buildingStoryNames")) {
      auto val = metaDataObj.Get("buildingStoryNames");
      if (val.IsObject()) {
        for (const auto& vv : val.Get<std::map<std::string, tinygltf::Value>>()) {
          if (vv.second.IsString()) {
            m_buildingStoryNames.emplace_back(vv.second.Get<std::string>());
          } else {
            LOG(Warn, "metaDataObj has the key 'buildingStoryNames', it is an Object, but at least one of the subobjects is not a std::string");
          }
        }
      } else {
        LOG(Warn, "metaDataObj has the key 'buildingStoryNames' but it is not an Object");
      }
    } else {
      LOG(Debug, "metaDataObj is missing the key 'airWall'");
    }

    if (metaDataObj.Has("generator")) {
      auto val = metaDataObj.Get("generator");
      if (val.IsString()) {
        m_generator = val.Get<std::string>();
      } else {
        LOG(Debug, "metaDataObj has the key 'generator' but it is not a std::string");
      }
    } else {
      LOG(Debug, "metaDataObj is missing the key 'generator'");
    }

    if (metaDataObj.Has("type")) {
      auto val = metaDataObj.Get("type");
      if (val.IsString()) {
        m_type = val.Get<std::string>();
      } else {
        LOG(Debug, "metaDataObj has the key 'type' but it is not a std::string");
      }
    } else {
      LOG(Debug, "metaDataObj is missing the key 'type'");
    }

    if (metaDataObj.Has("version")) {
      auto val = metaDataObj.Get("version");
      if (val.IsString()) {
        m_version = val.Get<std::string>();
      } else {
        LOG(Debug, "metaDataObj has the key 'version' but it is not a std::string");
      }
    } else {
      LOG(Debug, "metaDataObj is missing the key 'version'");
    }

    if (metaDataObj.Has("northAxis")) {
      auto val = metaDataObj.Get("northAxis");
      if (val.IsReal()) {
        m_northAxis = val.Get<double>();
      } else {
        LOG(Debug, "metaDataObj has the key 'northAxis' but it is not a double");
      }
    } else {
      LOG(Debug, "metaDataObj is missing the key 'northAxis'");
    }

    if (metaDataObj.Has("storyCount")) {
      auto val = metaDataObj.Get("storyCount");
      if (val.IsInt()) {
        m_buildingStoryCount = val.Get<int>();
      } else {
        LOG(Debug, "metaDataObj has the key 'storyCount' but it is not a int");
      }
    } else {
      LOG(Debug, "metaDataObj is missing the key 'storyCount'");
    }

    // auto tryGetIntFromkey = [&metaDataObj](const std::string& key) -> boost::optional<double> {
    //   if (metaDataObj.Has(key)) {
    //     auto val = metaDataObj.Get(key);
    //     if (val.IsReal()) {
    //       return val.Get<int>();
    //     } else {
    //       LOG(Warn, "metaDataObj has the key '" << key << "' but it is not an integer");
    //     }
    //   } else {
    //     LOG(Debug, "metaDataObj is missing the key '" << key << "'");
    //   }
    //   return boost::none;
    // };

    // if (auto i_ = tryGetIntFromkey("buildingStoryCount")) {
    //   m_buildingStoryCount = i_.get();
    // }

    // if (auto i_ = tryGetIntFromkey("thermalZoneCount")) {
    //   m_thermalZoneCount = i_.get();
    // }

    // if (auto i_ = tryGetIntFromkey("spaceCount")) {
    //   m_spaceCount = i_.get();
    // }

    // if (auto i_ = tryGetIntFromkey("spaceTypeCount")) {
    //   m_spaceTypeCount = i_.get();
    // }

    // if (auto i_ = tryGetIntFromkey("constructionSetCount")) {
    //   m_constructionSetCount = i_.get();
    // }

    // if (auto i_ = tryGetIntFromkey("airLoopCount")) {
    //   m_airLoopCount = i_.get();
    // }

    // if (auto i_ = tryGetIntFromkey("buildingStoryCount")) {
    //   m_buildingStoryCount = i_.get();
    // }
  }

}  // namespace gltf
}  // namespace openstudio
