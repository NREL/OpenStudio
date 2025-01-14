/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GltfUserData.hpp"
#include "GltfUtils.hpp"

#include "../model/Model.hpp"

#include "../model/PlanarSurface.hpp"

#include "../model/PlanarSurfaceGroup.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
#include "../model/InteriorPartitionSurface.hpp"
#include "../model/InteriorPartitionSurface_Impl.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/InteriorPartitionSurfaceGroup.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionAirBoundary.hpp"
#include "../model/ConstructionAirBoundary_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/Space.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/AirLoopHVAC.hpp"
#include "../model/SpaceType.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingUnit.hpp"

#include "../utilities/core/UUID.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Geometry.hpp"

#include <tiny_gltf.h>

#include <string>
#include <map>

namespace openstudio {
namespace gltf {

  GltfUserData::GltfUserData() = default;

  std::string GltfUserData::surfaceTypeMaterialName() const {
    {
      if (istringEqual(m_surfaceType, "FixedWindow") || istringEqual(m_surfaceType, "OperableWindow") || istringEqual(m_surfaceType, "GlassDoor")
          || istringEqual(m_surfaceType, "Skylight") || istringEqual(m_surfaceType, "TubularDaylightDome")
          || istringEqual(m_surfaceType, "TubularDaylightDiffuser")) {
        return "Window";
      } else if (istringEqual(m_surfaceType, "Door") || istringEqual(m_surfaceType, "OverheadDoor")) {
        return "Door";
      }
      return m_surfaceType;
    }
  }

  std::string GltfUserData::boundaryMaterialName() const {
    std::string result;
    if (m_outsideBoundaryCondition.empty()) {
      return result;
    }

    if (istringEqual(m_outsideBoundaryCondition, "Outdoors")) {
      if (istringEqual(m_sunExposure, "SunExposed") && istringEqual(m_windExposure, "WindExposed")) {
        result = "Boundary_Outdoors_SunWind";
      } else if (istringEqual(m_sunExposure, "SunExposed")) {
        result = "Boundary_Outdoors_Sun";
      } else if (istringEqual(m_windExposure, "WindExposed")) {
        result = "Boundary_Outdoors_Wind";
      } else {
        result = "Boundary_Outdoors";
      }
    } else {
      result = "Boundary_" + m_outsideBoundaryCondition;
    }
    return result;
  }

  GltfUserData::GltfUserData(const model::PlanarSurface& planarSurface) {
    std::string name = planarSurface.nameString();

    m_handle = openstudio::removeBraces(planarSurface.handle());
    m_name = planarSurface.nameString();

    if (auto surface = planarSurface.optionalCast<model::Surface>()) {
      m_surfaceName = surface->nameString();
      m_surfaceHandle = openstudio::removeBraces(surface->handle());
      m_surfaceType = surface->surfaceType();
      m_sunExposure = surface->sunExposure();
      m_windExposure = surface->windExposure();
      m_outsideBoundaryCondition = surface->outsideBoundaryCondition();

      boost::optional<model::Surface> adjacentSurface = surface->adjacentSurface();
      if (adjacentSurface) {
        m_outsideBoundaryConditionObjectName = adjacentSurface->nameString();
        m_outsideBoundaryConditionObjectHandle = openstudio::removeBraces(adjacentSurface->handle());
      }
    } else if (auto shadingSurface = planarSurface.optionalCast<model::ShadingSurface>()) {

      m_shadingHandle = openstudio::removeBraces(shadingSurface->handle());
      m_shadingName = shadingSurface->nameString();

      std::string shadingSurfaceType = "Building";
      if (shadingSurface->shadingSurfaceGroup()) {
        shadingSurfaceType = shadingSurface->shadingSurfaceGroup()->shadingSurfaceType();
      }
      std::string surfaceType = shadingSurfaceType + "Shading";
      m_surfaceType = surfaceType;
      m_sunExposure = "SunExposed";
      m_windExposure = "WindExposed";
      // m_outsideBoundaryCondition.clear();
      // m_outsideBoundaryConditionObjectName.clear();
      // m_outsideBoundaryConditionObjectHandle.clear();
    } else if (auto interiorPartitionSurface = planarSurface.optionalCast<model::InteriorPartitionSurface>()) {
      m_surfaceType = "InteriorPartitionSurface";
      m_sunExposure = "NoSun";
      m_windExposure = "NoWind";
      // m_outsideBoundaryCondition.clear();
      // m_outsideBoundaryConditionObjectName.clear();
      // m_outsideBoundaryConditionObjectHandle.clear();
    }

    else if (auto subSurface = planarSurface.optionalCast<model::SubSurface>()) {
      m_subSurfaceHandle = openstudio::removeBraces(subSurface->handle());
      m_subSurfaceName = subSurface->nameString();
      m_surfaceType = subSurface->subSurfaceType();

      boost::optional<model::Surface> parentSurface = subSurface->surface();
      if (parentSurface) {
        m_outsideBoundaryCondition = parentSurface->outsideBoundaryCondition();
        m_sunExposure = parentSurface->sunExposure();
        m_windExposure = parentSurface->windExposure();
        m_surfaceName = parentSurface->nameString();
        m_surfaceHandle = openstudio::removeBraces(parentSurface->handle());
      }

      boost::optional<model::SubSurface> adjacentSubSurface = subSurface->adjacentSubSurface();
      if (adjacentSubSurface) {
        m_outsideBoundaryConditionObjectName = adjacentSubSurface->nameString();
        m_outsideBoundaryConditionObjectHandle = openstudio::removeBraces(adjacentSubSurface->handle());
      }
    }

    if (boost::optional<model::ConstructionBase> construction = planarSurface.construction()) {

      m_constructionSetHandle = openstudio::removeBraces(construction->handle());
      m_constructionName = construction->nameString();
      // If this is a ConstructionAirBoundary, then set to the standard material "AirWall"
      if (construction->optionalCast<model::ConstructionAirBoundary>()) {
        m_constructionMaterialName = "AirWall";
      } else if (construction->iddObjectType() == model::Construction::iddObjectType()) {
        m_constructionMaterialName = "Construction_" + construction->nameString();
      } else {
        OS_ASSERT(false);  // Shouldn't happen
        m_constructionMaterialName = getObjectGLTFMaterialName(construction.get());
      }
    }

    if (auto space = planarSurface.space()) {
      m_spaceHandle = openstudio::removeBraces(space->handle());
      m_spaceName = space->nameString();
      if (auto thermalZone = space->thermalZone()) {
        m_thermalZoneHandle = openstudio::removeBraces(thermalZone->handle());
        m_thermalZoneName = thermalZone->nameString();
        m_thermalZoneMaterialName = "ThermalZone_" + thermalZone->nameString();
        std::vector<model::AirLoopHVAC> airLoopHVACs = thermalZone->airLoopHVACs();
        for (const auto& airLoopHVAC : thermalZone->airLoopHVACs()) {
          m_airLoopHVACHandles.emplace_back(openstudio::removeBraces(airLoopHVAC.handle()));

          m_airLoopHVACNames.emplace_back(airLoopHVAC.nameString());

          m_airLoopHVACMaterialNames.emplace_back(getObjectGLTFMaterialName(airLoopHVAC));
        }
      }

      if (auto spaceType = space->spaceType()) {
        m_spaceTypeHandle = openstudio::removeBraces(spaceType->handle());
        m_spaceTypeName = spaceType->nameString();
        m_spaceTypeMaterialName = getObjectGLTFMaterialName(spaceType.get());
      }

      if (auto buildingStory = space->buildingStory()) {
        m_buildingStoryHandle = openstudio::removeBraces(buildingStory->handle());
        m_buildingStoryName = buildingStory->nameString();
        m_buildingStoryMaterialName = getObjectGLTFMaterialName(buildingStory.get());
      }

      if (auto buildingUnit = space->buildingUnit()) {
        m_buildingUnitHandle = openstudio::removeBraces(buildingUnit->handle());
        m_buildingUnitName = buildingUnit->nameString();
        m_buildingUnitMaterialName = getObjectGLTFMaterialName(buildingUnit.get());
      }
    }

    // check if the adjacent surface is truly adjacent
    m_coincidentWithOutsideObject = false;

    if (!m_outsideBoundaryConditionObjectHandle.empty()) {

      Transformation buildingTransformation;
      if (boost::optional<model::PlanarSurfaceGroup> planarSurfaceGroup_ = planarSurface.planarSurfaceGroup()) {
        buildingTransformation = planarSurfaceGroup_->buildingTransformation();
      }
      // get the vertices
      Point3dVector vertices = planarSurface.vertices();

      auto adjacentHandle = toUUID(m_outsideBoundaryConditionObjectHandle);
      auto adjacentPlanarSurface = planarSurface.model().getModelObject<model::PlanarSurface>(adjacentHandle);
      OS_ASSERT(adjacentPlanarSurface);

      Transformation otherBuildingTransformation;
      if (adjacentPlanarSurface->planarSurfaceGroup()) {
        otherBuildingTransformation = adjacentPlanarSurface->planarSurfaceGroup()->buildingTransformation();
      }

      Point3dVector otherVertices = otherBuildingTransformation * adjacentPlanarSurface->vertices();
      m_coincidentWithOutsideObject = circularEqual(buildingTransformation * vertices, reverse(otherVertices));
    }
  }

  std::string GltfUserData::handle() const {
    return m_handle;
  }
  void GltfUserData::setHandle(const std::string& handle) {
    m_handle = handle;
  }

  std::string GltfUserData::name() const {
    return m_name;
  }
  void GltfUserData::setName(const std::string& name) {
    m_name = name;
  }

  std::string GltfUserData::surfaceType() const {
    return m_surfaceType;
  }
  void GltfUserData::setSurfaceType(const std::string& surfaceType) {
    m_surfaceType = surfaceType;
  }

  std::string GltfUserData::constructionName() const {
    return m_constructionName;
  }
  void GltfUserData::setConstructionName(const std::string& constructionName) {
    m_constructionName = constructionName;
  }

  std::string GltfUserData::constructionHandle() const {
    return m_constructionHandle;
  }
  void GltfUserData::setConstructionHandle(const std::string& constructionHandle) {
    m_constructionHandle = constructionHandle;
  }

  std::string GltfUserData::constructionMaterialName() const {
    return m_constructionMaterialName;
  }
  void GltfUserData::setConstructionMaterialName(const std::string& constructionMaterialName) {
    m_constructionMaterialName = constructionMaterialName;
  }

  std::string GltfUserData::surfaceName() const {
    return m_surfaceName;
  }
  void GltfUserData::setSurfaceName(const std::string& surfaceName) {
    m_surfaceName = surfaceName;
  }

  std::string GltfUserData::surfaceHandle() const {
    return m_surfaceHandle;
  }
  void GltfUserData::setSurfaceHandle(const std::string& surfaceHandle) {
    m_surfaceHandle = surfaceHandle;
  }

  std::string GltfUserData::subSurfaceName() const {
    return m_subSurfaceName;
  }
  void GltfUserData::setSubSurfaceName(const std::string& subSurfaceName) {
    m_subSurfaceName = subSurfaceName;
  }

  std::string GltfUserData::subSurfaceHandle() const {
    return m_subSurfaceHandle;
  }
  void GltfUserData::setSubSurfaceHandle(const std::string& subSurfaceHandle) {
    m_subSurfaceHandle = subSurfaceHandle;
  }

  std::string GltfUserData::spaceName() const {
    return m_spaceName;
  }
  void GltfUserData::setSpaceName(const std::string& spaceName) {
    m_spaceName = spaceName;
  }

  std::string GltfUserData::spaceHandle() const {
    return m_spaceHandle;
  }
  void GltfUserData::setSpaceHandle(const std::string& spaceHandle) {
    m_spaceHandle = spaceHandle;
  }

  std::string GltfUserData::shadingName() const {
    return m_shadingName;
  }
  void GltfUserData::setShadingName(const std::string& shadingName) {
    m_shadingName = shadingName;
  }

  std::string GltfUserData::shadingHandle() const {
    return m_shadingHandle;
  }
  void GltfUserData::setShadingHandle(const std::string& shadingHandle) {
    m_shadingHandle = shadingHandle;
  }

  std::string GltfUserData::thermalZoneName() const {
    return m_thermalZoneName;
  }
  void GltfUserData::setThermalZoneName(const std::string& thermalZoneName) {
    m_thermalZoneName = thermalZoneName;
  }

  std::string GltfUserData::thermalZoneHandle() const {
    return m_thermalZoneHandle;
  }
  void GltfUserData::setThermalZoneHandle(const std::string& thermalZoneHandle) {
    m_thermalZoneHandle = thermalZoneHandle;
  }

  std::string GltfUserData::thermalZoneMaterialName() const {
    return m_thermalZoneMaterialName;
  }
  void GltfUserData::setThermalZoneMaterialName(const std::string& thermalZoneMaterialName) {
    m_thermalZoneMaterialName = thermalZoneMaterialName;
  }

  std::string GltfUserData::spaceTypeName() const {
    return m_spaceTypeName;
  }
  void GltfUserData::setSpaceTypeName(const std::string& spaceTypeName) {
    m_spaceTypeName = spaceTypeName;
  }

  std::string GltfUserData::spaceTypeHandle() const {
    return m_spaceTypeHandle;
  }
  void GltfUserData::setSpaceTypeHandle(const std::string& spaceTypeHandle) {
    m_spaceTypeHandle = spaceTypeHandle;
  }

  std::string GltfUserData::spaceTypeMaterialName() const {
    return m_spaceTypeMaterialName;
  }
  void GltfUserData::setSpaceTypeMaterialName(const std::string& spaceTypeMaterialName) {
    m_spaceTypeMaterialName = spaceTypeMaterialName;
  }

  std::string GltfUserData::buildingStoryName() const {
    return m_buildingStoryName;
  }
  void GltfUserData::setBuildingStoryName(const std::string& buildingStoryName) {
    m_buildingStoryName = buildingStoryName;
  }

  std::string GltfUserData::buildingStoryHandle() const {
    return m_buildingStoryHandle;
  }
  void GltfUserData::setBuildingStoryHandle(const std::string& buildingStoryHandle) {
    m_buildingStoryHandle = buildingStoryHandle;
  }

  std::string GltfUserData::buildingStoryMaterialName() const {
    return m_buildingStoryMaterialName;
  }
  void GltfUserData::setBuildingStoryMaterialName(const std::string& buildingStoryMaterialName) {
    m_buildingStoryMaterialName = buildingStoryMaterialName;
  }

  std::string GltfUserData::buildingUnitName() const {
    return m_buildingUnitName;
  }
  void GltfUserData::setBuildingUnitName(const std::string& buildingUnitName) {
    m_buildingUnitName = buildingUnitName;
  }

  std::string GltfUserData::buildingUnitHandle() const {
    return m_buildingUnitHandle;
  }
  void GltfUserData::setBuildingUnitHandle(const std::string& buildingUnitHandle) {
    m_buildingUnitHandle = buildingUnitHandle;
  }

  std::string GltfUserData::buildingUnitMaterialName() const {
    return m_buildingUnitMaterialName;
  }
  void GltfUserData::setBuildingUnitMaterialName(const std::string& buildingUnitMaterialName) {
    m_buildingUnitMaterialName = buildingUnitMaterialName;
  }

  std::string GltfUserData::constructionSetName() const {
    return m_constructionSetName;
  }
  void GltfUserData::setConstructionSetName(const std::string& constructionSetName) {
    m_constructionSetName = constructionSetName;
  }

  std::string GltfUserData::constructionSetHandle() const {
    return m_constructionSetHandle;
  }
  void GltfUserData::setConstructionSetHandle(const std::string& constructionSetHandle) {
    m_constructionSetHandle = constructionSetHandle;
  }

  std::string GltfUserData::constructionSetMaterialName() const {
    return m_constructionSetMaterialName;
  }
  void GltfUserData::setConstructionSetMaterialName(const std::string& constructionSetMaterialName) {
    m_constructionSetMaterialName = constructionSetMaterialName;
  }

  std::string GltfUserData::outsideBoundaryCondition() const {
    return m_outsideBoundaryCondition;
  }
  void GltfUserData::setOutsideBoundaryCondition(const std::string& outsideBoundaryCondition) {
    m_outsideBoundaryCondition = outsideBoundaryCondition;
  }

  std::string GltfUserData::outsideBoundaryConditionObjectName() const {
    return m_outsideBoundaryConditionObjectName;
  }
  void GltfUserData::setOutsideBoundaryConditionObjectName(const std::string& outsideBoundaryConditionObjectName) {
    m_outsideBoundaryConditionObjectName = outsideBoundaryConditionObjectName;
  }

  std::string GltfUserData::outsideBoundaryConditionObjectHandle() const {
    return m_outsideBoundaryConditionObjectHandle;
  }
  void GltfUserData::setOutsideBoundaryConditionObjectHandle(const std::string& outsideBoundaryConditionObjectHandle) {
    m_outsideBoundaryConditionObjectHandle = outsideBoundaryConditionObjectHandle;
  }

  bool GltfUserData::coincidentWithOutsideObject() const {
    return m_coincidentWithOutsideObject;
  }
  void GltfUserData::setCoincidentWithOutsideObject(bool coincidentWithOutsideObject) {
    m_coincidentWithOutsideObject = coincidentWithOutsideObject;
  }

  std::string GltfUserData::sunExposure() const {
    return m_sunExposure;
  }
  void GltfUserData::setSunExposure(const std::string& sunExposure) {
    m_sunExposure = sunExposure;
  }

  std::string GltfUserData::windExposure() const {
    return m_windExposure;
  }
  void GltfUserData::setWindExposure(const std::string& windExposure) {
    m_windExposure = windExposure;
  }

  double GltfUserData::illuminanceSetpoint() const {
    return m_illuminanceSetpoint;
  }
  void GltfUserData::setIlluminanceSetpoint(double illuminanceSetpoint) {
    m_illuminanceSetpoint = illuminanceSetpoint;
  }

  bool GltfUserData::airWall() const {
    return m_airWall;
  }
  void GltfUserData::setAirWall(bool airWall) {
    m_airWall = airWall;
  }

  std::vector<std::string> GltfUserData::airLoopHVACNames() const {
    return m_airLoopHVACNames;
  }

  void GltfUserData::setAirLoopHVACNames(const std::vector<std::string>& airLoopHVACNames) {
    m_airLoopHVACNames = airLoopHVACNames;
  }

  void GltfUserData::addAirLoopHVACName(std::string airLoopHVACName) {
    m_airLoopHVACNames.emplace_back(std::move(airLoopHVACName));
  }

  std::vector<std::string> GltfUserData::airLoopHVACHandles() const {
    return m_airLoopHVACHandles;
  }

  void GltfUserData::setAirLoopHVACHandles(const std::vector<std::string>& airLoopHVACHandles) {
    m_airLoopHVACHandles = airLoopHVACHandles;
  }

  void GltfUserData::addAirLoopHVACHandle(std::string airLoopHVACHandle) {
    m_airLoopHVACHandles.emplace_back(std::move(airLoopHVACHandle));
  }

  std::vector<std::string> GltfUserData::airLoopHVACMaterialNames() const {
    return m_airLoopHVACMaterialNames;
  }

  void GltfUserData::setAirLoopHVACMaterialNames(const std::vector<std::string>& airLoopHVACMaterialNames) {
    m_airLoopHVACMaterialNames = airLoopHVACMaterialNames;
  }

  void GltfUserData::addAirLoopHVACMaterialName(std::string airLoopHVACMaterialName) {
    m_airLoopHVACMaterialNames.emplace_back(std::move(airLoopHVACMaterialName));
  }

  std::map<std::string, tinygltf::Value> GltfUserData::toExtras() const {
    // TODO: probably make the airloopHVAC stuff a single object with multiple properties
    // But in any case, we want Arrays not a useless map<std::string(int), std::string>

    std::map<std::string, tinygltf::Value> airLoopHVACNamesObject;
    std::map<std::string, tinygltf::Value> airLoopHVACHandlesObject;
    std::map<std::string, tinygltf::Value> airLoopHVACMaterialNamesObject;
    for (size_t i = 0; i < m_airLoopHVACNames.size(); ++i) {
      airLoopHVACNamesObject.emplace(std::make_pair(std::to_string(i), tinygltf::Value(m_airLoopHVACNames[i])));
      airLoopHVACHandlesObject.emplace(std::make_pair(std::to_string(i), tinygltf::Value(m_airLoopHVACHandles[i])));
      airLoopHVACMaterialNamesObject.emplace(std::make_pair(std::to_string(i), tinygltf::Value(m_airLoopHVACMaterialNames[i])));
    }

    return {
      {"handle", tinygltf::Value(m_handle)},
      {"name", tinygltf::Value(m_name)},
      {"surfaceType", tinygltf::Value(m_surfaceType)},
      {"surfaceTypeMaterialName", tinygltf::Value(this->surfaceTypeMaterialName())},
      {"constructionName", tinygltf::Value(m_constructionName)},
      {"constructionHandle", tinygltf::Value(m_constructionHandle)},
      {"constructionMaterialName", tinygltf::Value(m_constructionMaterialName)},
      {"surfaceName", tinygltf::Value(m_surfaceName)},
      {"surfaceHandle", tinygltf::Value(m_surfaceHandle)},
      {"subSurfaceName", tinygltf::Value(m_subSurfaceName)},
      {"subSurfaceHandle", tinygltf::Value(m_subSurfaceHandle)},
      {"spaceName", tinygltf::Value(m_spaceName)},
      {"spaceHandle", tinygltf::Value(m_spaceHandle)},
      {"shadingName", tinygltf::Value(m_shadingName)},
      {"shadingHandle", tinygltf::Value(m_shadingHandle)},
      {"thermalZoneName", tinygltf::Value(m_thermalZoneName)},
      {"thermalZoneHandle", tinygltf::Value(m_thermalZoneHandle)},
      {"thermalZoneMaterialName", tinygltf::Value(m_thermalZoneMaterialName)},
      {"spaceTypeName", tinygltf::Value(m_spaceTypeName)},
      {"spaceTypeHandle", tinygltf::Value(m_spaceTypeHandle)},
      {"spaceTypeMaterialName", tinygltf::Value(m_spaceTypeMaterialName)},
      {"buildingStoryName", tinygltf::Value(m_buildingStoryName)},
      {"buildingStoryHandle", tinygltf::Value(m_buildingStoryHandle)},
      {"buildingStoryMaterialName", tinygltf::Value(m_buildingStoryMaterialName)},
      {"buildingUnitName", tinygltf::Value(m_buildingUnitName)},
      {"buildingUnitHandle", tinygltf::Value(m_buildingUnitHandle)},
      {"buildingUnitMaterialName", tinygltf::Value(m_buildingUnitMaterialName)},
      {"constructionSetName", tinygltf::Value(m_constructionSetName)},
      {"constructionSetHandle", tinygltf::Value(m_constructionSetHandle)},
      {"constructionSetMaterialName", tinygltf::Value(m_constructionSetMaterialName)},
      {"outsideBoundaryCondition", tinygltf::Value(m_outsideBoundaryCondition)},
      {"outsideBoundaryConditionObjectName", tinygltf::Value(m_outsideBoundaryConditionObjectName)},
      {"outsideBoundaryConditionObjectHandle", tinygltf::Value(m_outsideBoundaryConditionObjectHandle)},
      {"boundaryMaterialName", tinygltf::Value(this->boundaryMaterialName())},
      {"coincidentWithOutsideObject", tinygltf::Value(m_coincidentWithOutsideObject)},
      {"sunExposure", tinygltf::Value(m_sunExposure)},
      {"windExposure", tinygltf::Value(m_windExposure)},
      {"illuminanceSetpoint", tinygltf::Value(m_illuminanceSetpoint)},
      {"airWall", tinygltf::Value(m_airWall)},
      {"airLoopHVACNames", tinygltf::Value(airLoopHVACNamesObject)},
      {"airLoopHVACHandles", tinygltf::Value(airLoopHVACHandlesObject)},
      {"airLoopHVACMaterialNames", tinygltf::Value(airLoopHVACMaterialNamesObject)},
    };
  }

  GltfUserData::GltfUserData(const tinygltf::Value& userDataObj) {

    if (userDataObj.Has("handle")) {
      auto val = userDataObj.Get("handle");
      if (val.IsString()) {
        m_handle = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'handle' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'handle'");
    }

    if (userDataObj.Has("name")) {
      auto val = userDataObj.Get("name");
      if (val.IsString()) {
        m_name = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'name' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'name'");
    }

    if (userDataObj.Has("surfaceType")) {
      auto val = userDataObj.Get("surfaceType");
      if (val.IsString()) {
        m_surfaceType = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'surfaceType' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'surfaceType'");
    }

    //    if (userDataObj.Has("surfaceTypeMaterialName")) {
    //      auto val = userDataObj.Get("surfaceTypeMaterialName");
    //      if (val.IsString()) {
    //        m_surfaceTypeMaterialName = val.Get<std::string>();
    //      } else {
    //        LOG(Warn, "userDataObj has the key 'surfaceTypeMaterialName' but it is not a std::string");
    //      }
    //    } else {
    //      LOG(Debug, "userDataObj is missing the key 'surfaceTypeMaterialName'");
    //    }

    if (userDataObj.Has("constructionName")) {
      auto val = userDataObj.Get("constructionName");
      if (val.IsString()) {
        m_constructionName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'constructionName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'constructionName'");
    }

    if (userDataObj.Has("constructionHandle")) {
      auto val = userDataObj.Get("constructionHandle");
      if (val.IsString()) {
        m_constructionHandle = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'constructionHandle' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'constructionHandle'");
    }

    if (userDataObj.Has("constructionMaterialName")) {
      auto val = userDataObj.Get("constructionMaterialName");
      if (val.IsString()) {
        m_constructionMaterialName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'constructionMaterialName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'constructionMaterialName'");
    }

    if (userDataObj.Has("surfaceName")) {
      auto val = userDataObj.Get("surfaceName");
      if (val.IsString()) {
        m_surfaceName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'surfaceName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'surfaceName'");
    }

    if (userDataObj.Has("surfaceHandle")) {
      auto val = userDataObj.Get("surfaceHandle");
      if (val.IsString()) {
        m_surfaceHandle = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'surfaceHandle' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'surfaceHandle'");
    }

    if (userDataObj.Has("subSurfaceName")) {
      auto val = userDataObj.Get("subSurfaceName");
      if (val.IsString()) {
        m_subSurfaceName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'subSurfaceName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'subSurfaceName'");
    }

    if (userDataObj.Has("subSurfaceHandle")) {
      auto val = userDataObj.Get("subSurfaceHandle");
      if (val.IsString()) {
        m_subSurfaceHandle = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'subSurfaceHandle' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'subSurfaceHandle'");
    }

    if (userDataObj.Has("spaceName")) {
      auto val = userDataObj.Get("spaceName");
      if (val.IsString()) {
        m_spaceName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'spaceName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'spaceName'");
    }

    if (userDataObj.Has("spaceHandle")) {
      auto val = userDataObj.Get("spaceHandle");
      if (val.IsString()) {
        m_spaceHandle = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'spaceHandle' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'spaceHandle'");
    }

    if (userDataObj.Has("shadingName")) {
      auto val = userDataObj.Get("shadingName");
      if (val.IsString()) {
        m_shadingName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'shadingName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'shadingName'");
    }

    if (userDataObj.Has("shadingHandle")) {
      auto val = userDataObj.Get("shadingHandle");
      if (val.IsString()) {
        m_shadingHandle = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'shadingHandle' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'shadingHandle'");
    }

    if (userDataObj.Has("thermalZoneName")) {
      auto val = userDataObj.Get("thermalZoneName");
      if (val.IsString()) {
        m_thermalZoneName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'thermalZoneName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'thermalZoneName'");
    }

    if (userDataObj.Has("thermalZoneHandle")) {
      auto val = userDataObj.Get("thermalZoneHandle");
      if (val.IsString()) {
        m_thermalZoneHandle = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'thermalZoneHandle' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'thermalZoneHandle'");
    }

    if (userDataObj.Has("thermalZoneMaterialName")) {
      auto val = userDataObj.Get("thermalZoneMaterialName");
      if (val.IsString()) {
        m_thermalZoneMaterialName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'thermalZoneMaterialName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'thermalZoneMaterialName'");
    }

    if (userDataObj.Has("spaceTypeName")) {
      auto val = userDataObj.Get("spaceTypeName");
      if (val.IsString()) {
        m_spaceTypeName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'spaceTypeName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'spaceTypeName'");
    }

    if (userDataObj.Has("spaceTypeHandle")) {
      auto val = userDataObj.Get("spaceTypeHandle");
      if (val.IsString()) {
        m_spaceTypeHandle = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'spaceTypeHandle' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'spaceTypeHandle'");
    }

    if (userDataObj.Has("spaceTypeMaterialName")) {
      auto val = userDataObj.Get("spaceTypeMaterialName");
      if (val.IsString()) {
        m_spaceTypeMaterialName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'spaceTypeMaterialName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'spaceTypeMaterialName'");
    }

    if (userDataObj.Has("buildingStoryName")) {
      auto val = userDataObj.Get("buildingStoryName");
      if (val.IsString()) {
        m_buildingStoryName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'buildingStoryName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'buildingStoryName'");
    }

    if (userDataObj.Has("buildingStoryHandle")) {
      auto val = userDataObj.Get("buildingStoryHandle");
      if (val.IsString()) {
        m_buildingStoryHandle = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'buildingStoryHandle' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'buildingStoryHandle'");
    }

    if (userDataObj.Has("buildingStoryMaterialName")) {
      auto val = userDataObj.Get("buildingStoryMaterialName");
      if (val.IsString()) {
        m_buildingStoryMaterialName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'buildingStoryMaterialName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'buildingStoryMaterialName'");
    }

    if (userDataObj.Has("buildingUnitName")) {
      auto val = userDataObj.Get("buildingUnitName");
      if (val.IsString()) {
        m_buildingUnitName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'buildingUnitName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'buildingUnitName'");
    }

    if (userDataObj.Has("buildingUnitHandle")) {
      auto val = userDataObj.Get("buildingUnitHandle");
      if (val.IsString()) {
        m_buildingUnitHandle = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'buildingUnitHandle' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'buildingUnitHandle'");
    }

    if (userDataObj.Has("buildingUnitMaterialName")) {
      auto val = userDataObj.Get("buildingUnitMaterialName");
      if (val.IsString()) {
        m_buildingUnitMaterialName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'buildingUnitMaterialName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'buildingUnitMaterialName'");
    }

    if (userDataObj.Has("constructionSetName")) {
      auto val = userDataObj.Get("constructionSetName");
      if (val.IsString()) {
        m_constructionSetName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'constructionSetName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'constructionSetName'");
    }

    if (userDataObj.Has("constructionSetHandle")) {
      auto val = userDataObj.Get("constructionSetHandle");
      if (val.IsString()) {
        m_constructionSetHandle = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'constructionSetHandle' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'constructionSetHandle'");
    }

    if (userDataObj.Has("constructionSetMaterialName")) {
      auto val = userDataObj.Get("constructionSetMaterialName");
      if (val.IsString()) {
        m_constructionSetMaterialName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'constructionSetMaterialName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'constructionSetMaterialName'");
    }

    if (userDataObj.Has("outsideBoundaryCondition")) {
      auto val = userDataObj.Get("outsideBoundaryCondition");
      if (val.IsString()) {
        m_outsideBoundaryCondition = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'outsideBoundaryCondition' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'outsideBoundaryCondition'");
    }

    if (userDataObj.Has("outsideBoundaryConditionObjectName")) {
      auto val = userDataObj.Get("outsideBoundaryConditionObjectName");
      if (val.IsString()) {
        m_outsideBoundaryConditionObjectName = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'outsideBoundaryConditionObjectName' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'outsideBoundaryConditionObjectName'");
    }

    if (userDataObj.Has("outsideBoundaryConditionObjectHandle")) {
      auto val = userDataObj.Get("outsideBoundaryConditionObjectHandle");
      if (val.IsString()) {
        m_outsideBoundaryConditionObjectHandle = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'outsideBoundaryConditionObjectHandle' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'outsideBoundaryConditionObjectHandle'");
    }

    //     if (userDataObj.Has("boundaryMaterialName")) {
    //       auto val = userDataObj.Get("boundaryMaterialName");
    //       if (val.IsString()) {
    //         m_boundaryMaterialName = val.Get<std::string>();
    //       } else {
    //         LOG(Warn, "userDataObj has the key 'boundaryMaterialName' but it is not a std::string");
    //       }
    //     } else {
    //       LOG(Debug, "userDataObj is missing the key 'boundaryMaterialName'");
    //     }
    //
    if (userDataObj.Has("coincidentWithOutsideObject")) {
      auto val = userDataObj.Get("coincidentWithOutsideObject");
      if (val.IsBool()) {
        m_coincidentWithOutsideObject = val.Get<bool>();
      } else {
        LOG(Warn, "userDataObj has the key 'coincidentWithOutsideObject' but it is not a bool");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'coincidentWithOutsideObject'");
    }

    if (userDataObj.Has("sunExposure")) {
      auto val = userDataObj.Get("sunExposure");
      if (val.IsString()) {
        m_sunExposure = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'sunExposure' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'sunExposure'");
    }

    if (userDataObj.Has("windExposure")) {
      auto val = userDataObj.Get("windExposure");
      if (val.IsString()) {
        m_windExposure = val.Get<std::string>();
      } else {
        LOG(Warn, "userDataObj has the key 'windExposure' but it is not a std::string");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'windExposure'");
    }

    if (userDataObj.Has("illuminanceSetpoint")) {
      auto val = userDataObj.Get("illuminanceSetpoint");
      if (val.IsReal()) {
        m_illuminanceSetpoint = val.Get<double>();
      } else {
        LOG(Warn, "userDataObj has the key 'illuminanceSetpoint' but it is not a double");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'illuminanceSetpoint'");
    }

    if (userDataObj.Has("airWall")) {
      auto val = userDataObj.Get("airWall");
      if (val.IsBool()) {
        m_airWall = val.Get<bool>();
      } else {
        LOG(Warn, "userDataObj has the key 'airWall' but it is not a bool");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'airWall'");
    }

    if (userDataObj.Has("airLoopHVACNames")) {
      auto val = userDataObj.Get("airLoopHVACNames");
      if (val.IsObject()) {
        std::vector<std::string> names;
        for (const auto& vv : val.Get<std::map<std::string, tinygltf::Value>>()) {
          if (vv.second.IsString()) {
            m_airLoopHVACNames.emplace_back(vv.second.Get<std::string>());
          } else {
            LOG(Warn, "userDataObj has the key 'airLoopHVACNames', it is an Object, but at least one of the subobjects is not a std::string");
          }
        }
      } else {
        LOG(Warn, "userDataObj has the key 'airLoopHVACNames' but it is not an Object");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'airLoopHVACNames'");
    }

    if (userDataObj.Has("airLoopHVACHandles")) {
      auto val = userDataObj.Get("airLoopHVACHandles");
      if (val.IsObject()) {
        std::vector<std::string> names;
        for (const auto& vv : val.Get<std::map<std::string, tinygltf::Value>>()) {
          if (vv.second.IsString()) {
            m_airLoopHVACHandles.emplace_back(vv.second.Get<std::string>());
          } else {
            LOG(Warn, "userDataObj has the key 'airLoopHVACHandles', it is an Object, but at least one of the subobjects is not a std::string");
          }
        }
      } else {
        LOG(Warn, "userDataObj has the key 'airLoopHVACHandles' but it is not an Object");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'airLoopHVACHandles'");
    }

    if (userDataObj.Has("airLoopHVACMaterialNames")) {
      auto val = userDataObj.Get("airLoopHVACMaterialNames");
      if (val.IsObject()) {
        std::vector<std::string> names;
        for (const auto& vv : val.Get<std::map<std::string, tinygltf::Value>>()) {
          if (vv.second.IsString()) {
            m_airLoopHVACMaterialNames.emplace_back(vv.second.Get<std::string>());
          } else {
            LOG(Warn, "userDataObj has the key 'airLoopHVACMaterialNames', it is an Object, but at least one of the subobjects is not a std::string");
          }
        }
      } else {
        LOG(Warn, "userDataObj has the key 'airLoopHVACMaterialNames' but it is not an Object");
      }
    } else {
      LOG(Debug, "userDataObj is missing the key 'airLoopHVACMaterialNames'");
    }
  }

}  // namespace gltf
}  // namespace openstudio
