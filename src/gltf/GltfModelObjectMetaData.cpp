/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GltfModelObjectMetaData.hpp"

#include "../model/AirLoopHVAC.hpp"
#include "../model/BuildingUnit.hpp"
#include "../model/SpaceType.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/Space.hpp"
#include "../model/RenderingColor.hpp"

#include "../utilities/core/UUID.hpp"

#include <tiny_gltf.h>

#include <string>
#include <map>

namespace openstudio {
namespace gltf {

  GltfModelObjectMetaData::GltfModelObjectMetaData() = default;

  GltfModelObjectMetaData::GltfModelObjectMetaData(const model::AirLoopHVAC& airLoopHVAC)
    : m_handle(openstudio::removeBraces(airLoopHVAC.handle())),
      m_iddObjectType(model::AirLoopHVAC::iddObjectType().valueDescription()),
      m_name(airLoopHVAC.nameString()) {}

  GltfModelObjectMetaData::GltfModelObjectMetaData(const model::BuildingUnit& buildingUnit)
    : m_handle(openstudio::removeBraces(buildingUnit.handle())),
      m_iddObjectType(model::BuildingUnit::iddObjectType().valueDescription()),
      m_name(buildingUnit.nameString()) {

    if (buildingUnit.renderingColor()) {
      m_color = buildingUnit.renderingColor()->colorString();
    }
  }

  GltfModelObjectMetaData::GltfModelObjectMetaData(const model::SpaceType& spaceType)
    : m_handle(openstudio::removeBraces(spaceType.handle())),
      m_iddObjectType(model::SpaceType::iddObjectType().valueDescription()),
      m_name(spaceType.nameString()) {

    if (spaceType.renderingColor()) {
      m_color = spaceType.renderingColor()->colorString();
    }
  }

  GltfModelObjectMetaData::GltfModelObjectMetaData(const model::BuildingStory& buildingStory)
    : m_handle(openstudio::removeBraces(buildingStory.handle())),
      m_iddObjectType(model::BuildingStory::iddObjectType().valueDescription()),
      m_name(buildingStory.nameString()) {

    if (buildingStory.renderingColor()) {
      m_color = buildingStory.renderingColor()->colorString();
    }
    if (buildingStory.nominalZCoordinate()) {
      m_nominalZCoordinate = buildingStory.nominalZCoordinate().get();
    }
    // TBD: Do we have to include this ?
    if (buildingStory.nominalFloortoCeilingHeight()) {
      m_nominalFloortoCeilingHeight = buildingStory.nominalFloortoCeilingHeight().get();
    }

    // TODO: How to calculate this ?
    m_opentoBelow = false;

    if (buildingStory.nominalFloortoFloorHeight()) {
      // TODO: DLM how to translate this?
    }
  }

  GltfModelObjectMetaData::GltfModelObjectMetaData(const model::ThermalZone& thermalZone)
    : m_handle(openstudio::removeBraces(thermalZone.handle())),
      m_iddObjectType(model::ThermalZone::iddObjectType().valueDescription()),
      m_name(thermalZone.nameString()) {

    if (thermalZone.renderingColor()) {
      m_color = thermalZone.renderingColor()->colorString();
    }

    // TODO: How to calculate this ?
    m_opentoBelow = false;

    m_multiplier = thermalZone.multiplier();
  }
  GltfModelObjectMetaData::GltfModelObjectMetaData(const model::DefaultConstructionSet& defaultConstructionSet)
    : m_handle(openstudio::removeBraces(defaultConstructionSet.handle())),
      m_iddObjectType(model::DefaultConstructionSet::iddObjectType().valueDescription()),
      m_name(defaultConstructionSet.nameString()) {}

  GltfModelObjectMetaData::GltfModelObjectMetaData(const model::Space& space)
    : m_handle(openstudio::removeBraces(space.handle())),
      m_iddObjectType(model::Space::iddObjectType().valueDescription()),
      m_name(space.nameString()) {

    // TODO: the original implementation doesn't use the space multiplier (well: it does come from the attached thermal Zone so...)
  }

  std::string GltfModelObjectMetaData::color() const {
    return m_color;
  }
  void GltfModelObjectMetaData::setColor(const std::string& color) {
    m_color = color;
  };

  std::string GltfModelObjectMetaData::handle() const {
    return m_handle;
  }
  void GltfModelObjectMetaData::setHandle(const std::string& handle) {
    m_handle = handle;
  };

  std::string GltfModelObjectMetaData::iddObjectType() const {
    return m_iddObjectType;
  }
  void GltfModelObjectMetaData::setIddObjectType(const std::string& iddObjectType) {
    m_iddObjectType = iddObjectType;
  };

  std::string GltfModelObjectMetaData::name() const {
    return m_name;
  }
  void GltfModelObjectMetaData::setName(const std::string& name) {
    m_name = name;
  };

  bool GltfModelObjectMetaData::isOpentoBelow() const {
    return m_opentoBelow;
  }
  void GltfModelObjectMetaData::setOpentoBelow(bool opentoBelow) {
    m_opentoBelow = opentoBelow;
  };

  double GltfModelObjectMetaData::nominalZCoordinate() const {
    return m_nominalZCoordinate;
  }
  void GltfModelObjectMetaData::setNominalZCoordinate(double nominalZCoordinate) {
    m_nominalZCoordinate = nominalZCoordinate;
  };

  double GltfModelObjectMetaData::nominalFloortoCeilingHeight() const {
    return m_nominalFloortoCeilingHeight;
  }
  void GltfModelObjectMetaData::setNominalFloortoCeilingHeight(double nominalFloortoCeilingHeight) {
    m_nominalFloortoCeilingHeight = nominalFloortoCeilingHeight;
  };

  int GltfModelObjectMetaData::multiplier() const {
    return m_multiplier;
  }
  void GltfModelObjectMetaData::setMultiplier(int multiplier) {
    m_multiplier = multiplier;
  };

  std::map<std::string, tinygltf::Value> GltfModelObjectMetaData::toExtras() const {
    return {
      {"color", tinygltf::Value(m_color)},
      {"handle", tinygltf::Value(m_handle)},
      {"iddObjectType", tinygltf::Value(m_iddObjectType)},
      {"name", tinygltf::Value(m_name)},
      {"open_to_below", tinygltf::Value(m_opentoBelow)},
      {"nominal_z_coordinate", tinygltf::Value(m_nominalZCoordinate)},
      {"nominal_floorCeiling_Height", tinygltf::Value(m_nominalFloortoCeilingHeight)},
      {"multiplier", tinygltf::Value(m_multiplier)},
    };
  }

  GltfModelObjectMetaData::GltfModelObjectMetaData(const tinygltf::Value& modelObjectMetaDataObj) {

    if (modelObjectMetaDataObj.Has("color")) {
      auto val = modelObjectMetaDataObj.Get("color");
      if (val.IsString()) {
        m_color = val.Get<std::string>();
      } else {
        LOG(Warn, "modelObjectMetaDataObj has the key 'color' but it is not a std::string");
      }
    } else {
      LOG(Debug, "modelObjectMetaDataObj is missing the key 'color'");
    }

    if (modelObjectMetaDataObj.Has("handle")) {
      auto val = modelObjectMetaDataObj.Get("handle");
      if (val.IsString()) {
        m_handle = val.Get<std::string>();
      } else {
        LOG(Warn, "modelObjectMetaDataObj has the key 'handle' but it is not a std::string");
      }
    } else {
      LOG(Debug, "modelObjectMetaDataObj is missing the key 'handle'");
    }

    if (modelObjectMetaDataObj.Has("iddObjectType")) {
      auto val = modelObjectMetaDataObj.Get("iddObjectType");
      if (val.IsString()) {
        m_iddObjectType = val.Get<std::string>();
      } else {
        LOG(Warn, "modelObjectMetaDataObj has the key 'iddObjectType' but it is not a std::string");
      }
    } else {
      LOG(Debug, "modelObjectMetaDataObj is missing the key 'iddObjectType'");
    }

    if (modelObjectMetaDataObj.Has("name")) {
      auto val = modelObjectMetaDataObj.Get("name");
      if (val.IsString()) {
        m_name = val.Get<std::string>();
      } else {
        LOG(Warn, "modelObjectMetaDataObj has the key 'name' but it is not a std::string");
      }
    } else {
      LOG(Debug, "modelObjectMetaDataObj is missing the key 'name'");
    }

    if (modelObjectMetaDataObj.Has("opentoBelow")) {
      auto val = modelObjectMetaDataObj.Get("opentoBelow");
      if (val.IsBool()) {
        m_opentoBelow = val.Get<bool>();
      } else {
        LOG(Warn, "modelObjectMetaDataObj has the key 'opentoBelow' but it is not a bool");
      }
    } else {
      LOG(Debug, "modelObjectMetaDataObj is missing the key 'opentoBelow'");
    }

    if (modelObjectMetaDataObj.Has("nominalZCoordinate")) {
      auto val = modelObjectMetaDataObj.Get("nominalZCoordinate");
      if (val.IsReal()) {
        m_nominalZCoordinate = val.Get<double>();
      } else {
        LOG(Warn, "modelObjectMetaDataObj has the key 'nominalZCoordinate' but it is not a double");
      }
    } else {
      LOG(Debug, "modelObjectMetaDataObj is missing the key 'nominalZCoordinate'");
    }

    if (modelObjectMetaDataObj.Has("nominalFloortoCeilingHeight")) {
      auto val = modelObjectMetaDataObj.Get("nominalFloortoCeilingHeight");
      if (val.IsReal()) {
        m_nominalFloortoCeilingHeight = val.Get<double>();
      } else {
        LOG(Warn, "modelObjectMetaDataObj has the key 'nominalFloortoCeilingHeight' but it is not a double");
      }
    } else {
      LOG(Debug, "modelObjectMetaDataObj is missing the key 'nominalFloortoCeilingHeight'");
    }

    if (modelObjectMetaDataObj.Has("multiplier")) {
      auto val = modelObjectMetaDataObj.Get("multiplier");
      if (val.IsInt()) {
        m_multiplier = val.Get<int>();
      } else {
        LOG(Warn, "modelObjectMetaDataObj has the key 'multiplier' but it is not a int");
      }
    } else {
      LOG(Debug, "modelObjectMetaDataObj is missing the key 'multiplier'");
    }
  }

}  // namespace gltf
}  // namespace openstudio
