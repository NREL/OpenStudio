/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelMerger.hpp"

#include "RenderingColor.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"
#include "Facility.hpp"
#include "Facility_Impl.hpp"
#include "Building.hpp"
#include "Building_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
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
#include "ShadingSurfaceGroup.hpp"
#include "ShadingSurfaceGroup_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "DefaultConstructionSet.hpp"
#include "DefaultConstructionSet_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "InteriorPartitionSurfaceGroup.hpp"
#include "DaylightingControl.hpp"
#include "DaylightingControl_Impl.hpp"
#include "AdditionalProperties.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Plane.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Geometry.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <thread>

#include <cmath>
#include <tuple>

namespace openstudio {
namespace model {

  ModelMerger::ModelMerger() {
    // DLM: TODO expose this to user to give more control over merging?

    // Unique Objects
    m_iddObjectTypesToMerge.push_back(IddObjectType::OS_Site);
    m_iddObjectTypesToMerge.push_back(IddObjectType::OS_Facility);
    m_iddObjectTypesToMerge.push_back(IddObjectType::OS_Building);

    // Non unique Objects
    m_iddObjectTypesToMerge.push_back(IddObjectType::OS_Space);
    m_iddObjectTypesToMerge.push_back(IddObjectType::OS_ShadingSurfaceGroup);
    m_iddObjectTypesToMerge.push_back(IddObjectType::OS_ThermalZone);
    m_iddObjectTypesToMerge.push_back(IddObjectType::OS_SpaceType);
    m_iddObjectTypesToMerge.push_back(IddObjectType::OS_BuildingStory);
    m_iddObjectTypesToMerge.push_back(IddObjectType::OS_BuildingUnit);
    m_iddObjectTypesToMerge.push_back(IddObjectType::OS_DefaultConstructionSet);

    m_logSink.setLogLevel(Warn);
    //m_logSink.setChannelRegex(boost::regex("openstudio\\.model\\.ThreeJSReverseTranslator"));
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  std::map<UUID, UUID> ModelMerger::suggestHandleMapping(const Model& currentModel, const Model& newModel) const {
    std::map<UUID, UUID> result;

    using HandleSet = std::set<UUID>;
    using StringHandleMap = std::map<std::string, UUID>;
    using ObjectLookup = std::tuple<HandleSet, StringHandleMap, StringHandleMap>;  // 0 - handle, 1 - CADObjectId, 2 - Name
    using IddToObjectLookupMap = std::map<IddObjectType, ObjectLookup>;

    IddToObjectLookupMap currentIddToObjectLookupMap;
    for (const auto& iddObjectType : m_iddObjectTypesToMerge) {
      ObjectLookup currentLookup;
      for (const auto& object : currentModel.getObjectsByType(iddObjectType)) {
        Handle handle = object.handle();
        std::get<0>(currentLookup).insert(handle);

        auto modelObject = object.cast<ModelObject>();
        if (modelObject.hasAdditionalProperties()) {
          model::AdditionalProperties additionalProperties = modelObject.additionalProperties();
          if (additionalProperties.hasFeature("CADObjectId")) {
            boost::optional<std::string> cadObjectId = additionalProperties.getFeatureAsString("CADObjectId");
            if (cadObjectId) {
              std::get<1>(currentLookup).insert(std::make_pair(*cadObjectId, handle));
            }
          }
        }

        std::get<2>(currentLookup).insert(std::make_pair(object.nameString(), handle));
      }
      currentIddToObjectLookupMap.insert(std::make_pair(iddObjectType, currentLookup));
    }

    for (const auto& iddObjectType : m_iddObjectTypesToMerge) {
      ObjectLookup currentLookup = currentIddToObjectLookupMap[iddObjectType];
      for (const auto& object : newModel.getObjectsByType(iddObjectType)) {
        Handle handle = object.handle();
        if (std::get<0>(currentLookup).count(handle) > 0) {
          // handle is in both models
          result[handle] = handle;
          continue;
        }

        if ((iddObjectType == IddObjectType::OS_Site) || (iddObjectType == IddObjectType::OS_Facility)
            || (iddObjectType == IddObjectType::OS_Building)) {
          // this is a unique object
          if (std::get<0>(currentLookup).size() == 1) {
            Handle currentHandle = *(std::get<0>(currentLookup).begin());
            result[currentHandle] = handle;
            continue;
          }
        }

        auto modelObject = object.cast<ModelObject>();
        if (modelObject.hasAdditionalProperties()) {
          model::AdditionalProperties additionalProperties = modelObject.additionalProperties();
          if (additionalProperties.hasFeature("CADObjectId")) {
            boost::optional<std::string> cadObjectId = additionalProperties.getFeatureAsString("CADObjectId");
            if (cadObjectId) {
              if (std::get<1>(currentLookup).count(*cadObjectId) > 0) {
                // cadObjectId is in both models
                Handle currentHandle = std::get<1>(currentLookup)[*cadObjectId];
                result[currentHandle] = handle;
                continue;
              }
            }
          }
        }

        if (std::get<2>(currentLookup).count(object.nameString()) > 0) {
          // name is in both models
          Handle currentHandle = std::get<2>(currentLookup)[object.nameString()];
          result[currentHandle] = handle;
          continue;
        }
      }
    }

    return result;
  }

  std::vector<LogMessage> ModelMerger::warnings() const {
    std::vector<LogMessage> result = m_logSink.logMessages();
    result.erase(std::remove_if(result.begin(), result.end(), [](const auto& logMessage) { return logMessage.logLevel() != Warn; }), result.end());
    return result;
  }

  std::vector<LogMessage> ModelMerger::errors() const {
    std::vector<LogMessage> result = m_logSink.logMessages();
    result.erase(std::remove_if(result.begin(), result.end(), [](const auto& logMessage) { return logMessage.logLevel() <= Warn; }), result.end());
    return result;
  }

  boost::optional<UUID> ModelMerger::getNewModelHandle(const UUID& currentHandle) {
    auto it = m_currentToNewHandleMapping.find(currentHandle);
    if (it != m_currentToNewHandleMapping.end()) {
      return it->second;
    }
    return boost::none;
  }

  boost::optional<UUID> ModelMerger::getCurrentModelHandle(const UUID& newHandle) {
    auto it = m_newToCurrentHandleMapping.find(newHandle);
    if (it != m_newToCurrentHandleMapping.end()) {
      return it->second;
    }
    return boost::none;
  }

  void ModelMerger::mergeSite(Site& currentSite, const Site& newSite) {
    if (m_newMergedHandles.find(newSite.handle()) != m_newMergedHandles.end()) {
      // already merged
      return;
    }
    m_newMergedHandles.insert(newSite.handle());

    currentSite.setName(newSite.nameString());

    if (!newSite.isLatitudeDefaulted()) {
      currentSite.setLatitude(newSite.latitude());
    }

    if (!newSite.isLongitudeDefaulted()) {
      currentSite.setLongitude(newSite.longitude());
    }

    if (!newSite.isTimeZoneDefaulted()) {
      currentSite.setTimeZone(newSite.timeZone());
    }

    if (!newSite.isElevationDefaulted()) {
      currentSite.setElevation(newSite.elevation());
    }

    if (!newSite.isTerrainDefaulted()) {
      currentSite.setTerrain(newSite.terrain());
    }
  }

  void ModelMerger::mergeFacility(Facility& currentFacility, const Facility& newFacility) {
    if (m_newMergedHandles.find(newFacility.handle()) != m_newMergedHandles.end()) {
      // already merged
      return;
    }
    m_newMergedHandles.insert(newFacility.handle());

    currentFacility.setName(newFacility.nameString());
  }

  void ModelMerger::mergeBuilding(Building& currentBuilding, const Building& newBuilding) {
    if (m_newMergedHandles.find(newBuilding.handle()) != m_newMergedHandles.end()) {
      // already merged
      return;
    }
    m_newMergedHandles.insert(newBuilding.handle());

    currentBuilding.setName(newBuilding.nameString());

    if (!newBuilding.isNorthAxisDefaulted()) {
      currentBuilding.setNorthAxis(newBuilding.northAxis());
    }

    if (newBuilding.nominalFloortoFloorHeight()) {
      currentBuilding.setNominalFloortoFloorHeight(newBuilding.nominalFloortoFloorHeight().get());
    }

    if (newBuilding.nominalFloortoCeilingHeight()) {
      currentBuilding.setNominalFloortoCeilingHeight(newBuilding.nominalFloortoCeilingHeight().get());
    }

    if (newBuilding.standardsNumberOfStories()) {
      currentBuilding.setStandardsNumberOfStories(newBuilding.standardsNumberOfStories().get());
    }

    if (newBuilding.standardsNumberOfAboveGroundStories()) {
      currentBuilding.setStandardsNumberOfAboveGroundStories(newBuilding.standardsNumberOfAboveGroundStories().get());
    }

    if (newBuilding.standardsNumberOfLivingUnits()) {
      currentBuilding.setStandardsNumberOfLivingUnits(newBuilding.standardsNumberOfLivingUnits().get());
    }

    if (newBuilding.standardsTemplate()) {
      currentBuilding.setStandardsTemplate(newBuilding.standardsTemplate().get());
    }

    if (newBuilding.standardsBuildingType()) {
      currentBuilding.setStandardsBuildingType(newBuilding.standardsBuildingType().get());
    }

    if (!newBuilding.isRelocatableDefaulted()) {
      currentBuilding.setRelocatable(newBuilding.relocatable());
    }

    // default construction set
    if (boost::optional<DefaultConstructionSet> newDefaultConstructionSet = newBuilding.defaultConstructionSet()) {
      boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newDefaultConstructionSet);
      if (currentObject) {
        auto currentDefaultConstructionSet = currentObject->cast<DefaultConstructionSet>();
        currentBuilding.setDefaultConstructionSet(currentDefaultConstructionSet);
      } else {
        currentBuilding.resetDefaultConstructionSet();
      }
    } else {
      currentBuilding.resetDefaultConstructionSet();
    }

    // TODO: default schedule sets
  }

  void ModelMerger::mergeSpace(Space& currentSpace, const Space& newSpace) {
    if (m_newMergedHandles.find(newSpace.handle()) != m_newMergedHandles.end()) {
      // already merged
      return;
    }
    m_newMergedHandles.insert(newSpace.handle());

    currentSpace.setName(newSpace.nameString());

    if (newSpace.isDirectionofRelativeNorthDefaulted()) {
      currentSpace.resetDirectionofRelativeNorth();
    } else {
      currentSpace.setDirectionofRelativeNorth(newSpace.directionofRelativeNorth());
    }

    if (newSpace.isXOriginDefaulted()) {
      currentSpace.resetXOrigin();
    } else {
      currentSpace.setXOrigin(newSpace.xOrigin());
    }

    if (newSpace.isYOriginDefaulted()) {
      currentSpace.resetYOrigin();
    } else {
      currentSpace.setYOrigin(newSpace.yOrigin());
    }

    if (newSpace.isZOriginDefaulted()) {
      currentSpace.resetZOrigin();
    } else {
      currentSpace.setZOrigin(newSpace.zOrigin());
    }

    // remove current surfaces
    for (auto& currentSurface : currentSpace.surfaces()) {
      currentSurface.remove();
    }

    // remove current shadingSurfaceGroups
    for (auto& shadingSurfaceGroup : currentSpace.shadingSurfaceGroups()) {
      shadingSurfaceGroup.remove();
    }

    // add new surfaces
    for (const auto& newSurface : newSpace.surfaces()) {
      // DLM: this should probably be moved to a mergeSurface method
      auto clone = newSurface.clone(m_currentModel).cast<Surface>();
      clone.setSpace(currentSpace);

      m_newMergedHandles.insert(newSurface.handle());
      m_currentToNewHandleMapping[clone.handle()] = newSurface.handle();
      m_newToCurrentHandleMapping[newSurface.handle()] = clone.handle();

      boost::optional<Surface> newAdjacentSurface = newSurface.adjacentSurface();
      if (newAdjacentSurface) {
        boost::optional<UUID> currentAdjacentSurfaceHandle = getCurrentModelHandle(newAdjacentSurface->handle());
        if (currentAdjacentSurfaceHandle) {
          boost::optional<Surface> currentAdjacentSurface = m_currentModel.getModelObject<Surface>(*currentAdjacentSurfaceHandle);
          if (currentAdjacentSurface) {
            clone.setAdjacentSurface(*currentAdjacentSurface);
          }
        }
      }

      // setAdjacentSurface resets the AdjacentSubSurface on all child subsurfaces
      for (const auto& newSubSurface : newSurface.subSurfaces()) {
        // for performance reasons, only find matching subsurfaces if there is an AdjacentSubSurface
        boost::optional<SubSurface> newAdjacentSubSurface = newSubSurface.adjacentSubSurface();
        if (newAdjacentSubSurface) {
          for (auto& cloneSubSurface : clone.subSurfaces()) {
            if (circularEqual(newSubSurface.vertices(), cloneSubSurface.vertices(), 0.01)) {
              // only subsurfaces with an AdjacentSubSurface will be added to the handle mapping
              m_currentToNewHandleMapping[cloneSubSurface.handle()] = newSubSurface.handle();
              m_newToCurrentHandleMapping[newSubSurface.handle()] = cloneSubSurface.handle();
              boost::optional<UUID> currentAdjacentSubSurfaceHandle = getCurrentModelHandle(newAdjacentSubSurface->handle());
              if (currentAdjacentSubSurfaceHandle) {
                boost::optional<SubSurface> currentAdjacentSubSurface = m_currentModel.getModelObject<SubSurface>(*currentAdjacentSubSurfaceHandle);
                if (currentAdjacentSubSurface) {
                  cloneSubSurface.setAdjacentSubSurface(*currentAdjacentSubSurface);
                }
              }
              break;
            }
          }
        }
      }
    }

    // add new shadingSurfaceGroups
    for (const auto& newShadingSurfaceGroup : newSpace.shadingSurfaceGroups()) {

      // check if this already merged via a window clone
      if (m_newMergedHandles.find(newShadingSurfaceGroup.handle()) != m_newMergedHandles.end()) {
        continue;
      }

      auto clone = newShadingSurfaceGroup.clone(m_currentModel).cast<ShadingSurfaceGroup>();
      clone.setSpace(currentSpace);

      m_newMergedHandles.insert(newShadingSurfaceGroup.handle());
      m_currentToNewHandleMapping[clone.handle()] = newShadingSurfaceGroup.handle();
      m_newToCurrentHandleMapping[newShadingSurfaceGroup.handle()] = clone.handle();

      boost::optional<SubSurface> newShadedSubSurface = newShadingSurfaceGroup.shadedSubSurface();
      if (newShadedSubSurface) {
        boost::optional<UUID> currentSubSurfaceHandle = getCurrentModelHandle(newShadedSubSurface->handle());
        if (currentSubSurfaceHandle) {
          boost::optional<SubSurface> currentSubSurface = m_currentModel.getModelObject<SubSurface>(*currentSubSurfaceHandle);
          if (currentSubSurface) {
            clone.setShadedSubSurface(*currentSubSurface);
          }
        }
      }

      boost::optional<Surface> newShadedSurface = newShadingSurfaceGroup.shadedSurface();
      if (newShadedSurface) {
        boost::optional<UUID> currentSurfaceHandle = getCurrentModelHandle(newShadedSurface->handle());
        if (currentSurfaceHandle) {
          boost::optional<Surface> currentSurface = m_currentModel.getModelObject<Surface>(*currentSurfaceHandle);
          if (currentSurface) {
            clone.setShadedSurface(*currentSurface);
          }
        }
      }
    }

    // DLM: TODO remove current and bring over new hard assigned loads, optional

    // DLM: TODO interiorPartitionSurfaceGroups

    // thermal zone
    if (boost::optional<ThermalZone> newThermalZone = newSpace.thermalZone()) {
      boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newThermalZone);
      if (currentObject) {
        auto currentThermalZone = currentObject->cast<ThermalZone>();
        currentSpace.setThermalZone(currentThermalZone);
      } else {
        currentSpace.resetThermalZone();
      }
    } else {
      currentSpace.resetThermalZone();
    }

    // space type
    if (boost::optional<SpaceType> newSpaceType = newSpace.spaceType()) {
      boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newSpaceType);
      if (currentObject) {
        auto currentSpaceType = currentObject->cast<SpaceType>();
        currentSpace.setSpaceType(currentSpaceType);
      } else {
        currentSpace.resetSpaceType();
      }
    } else {
      currentSpace.resetSpaceType();
    }

    // building story
    if (boost::optional<BuildingStory> newBuildingStory = newSpace.buildingStory()) {
      boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newBuildingStory);
      if (currentObject) {
        auto currentBuildingStory = currentObject->cast<BuildingStory>();
        currentSpace.setBuildingStory(currentBuildingStory);
      } else {
        currentSpace.resetBuildingStory();
      }
    } else {
      currentSpace.resetBuildingStory();
    }

    // building unit
    if (boost::optional<BuildingUnit> newBuildingUnit = newSpace.buildingUnit()) {
      boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newBuildingUnit);
      if (currentObject) {
        auto currentBuildingUnit = currentObject->cast<BuildingUnit>();
        currentSpace.setBuildingUnit(currentBuildingUnit);
      } else {
        currentSpace.resetBuildingUnit();
      }
    } else {
      currentSpace.resetBuildingUnit();
    }

    // default construction set
    if (boost::optional<DefaultConstructionSet> newDefaultConstructionSet = newSpace.defaultConstructionSet()) {
      boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newDefaultConstructionSet);
      if (currentObject) {
        auto currentDefaultConstructionSet = currentObject->cast<DefaultConstructionSet>();
        currentSpace.setDefaultConstructionSet(currentDefaultConstructionSet);
      } else {
        currentSpace.resetDefaultConstructionSet();
      }
    } else {
      currentSpace.resetDefaultConstructionSet();
    }

    // remove current daylightingControls
    for (auto& daylightingControl : currentSpace.daylightingControls()) {
      daylightingControl.remove();
    }

    // add new daylightingControls
    for (const auto& newDaylightingControl : newSpace.daylightingControls()) {

      auto clone = newDaylightingControl.clone(m_currentModel).cast<DaylightingControl>();
      clone.setSpace(currentSpace);

      m_newMergedHandles.insert(newDaylightingControl.handle());
      m_currentToNewHandleMapping[clone.handle()] = newDaylightingControl.handle();
      m_newToCurrentHandleMapping[newDaylightingControl.handle()] = clone.handle();

      // hook up daylighting control to thermal zone
      for (const auto& newThermalZone : newDaylightingControl.getModelObjectSources<ThermalZone>()) {
        boost::optional<DaylightingControl> primaryDaylightingControl = newThermalZone.primaryDaylightingControl();
        boost::optional<DaylightingControl> secondaryDaylightingControl = newThermalZone.secondaryDaylightingControl();

        if (primaryDaylightingControl && (primaryDaylightingControl->handle() == newDaylightingControl.handle())) {
          boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(newThermalZone);
          if (currentObject) {
            auto currentThermalZone = currentObject->cast<ThermalZone>();
            currentThermalZone.setPrimaryDaylightingControl(clone);
            currentThermalZone.setFractionofZoneControlledbyPrimaryDaylightingControl(
              newThermalZone.fractionofZoneControlledbyPrimaryDaylightingControl());
          }

        } else if (secondaryDaylightingControl && (secondaryDaylightingControl->handle() == newDaylightingControl.handle())) {
          boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(newThermalZone);
          if (currentObject) {
            auto currentThermalZone = currentObject->cast<ThermalZone>();
            currentThermalZone.setSecondaryDaylightingControl(clone);
            currentThermalZone.setFractionofZoneControlledbySecondaryDaylightingControl(
              newThermalZone.fractionofZoneControlledbySecondaryDaylightingControl());
          }
        }
      }
    }
  }

  void ModelMerger::mergeShadingSurfaceGroup(ShadingSurfaceGroup& currentGroup, const ShadingSurfaceGroup& newGroup) {
    if (m_newMergedHandles.find(newGroup.handle()) != m_newMergedHandles.end()) {
      // already merged
      return;
    }
    m_newMergedHandles.insert(newGroup.handle());

    currentGroup.setName(newGroup.nameString());

    if (newGroup.isDirectionofRelativeNorthDefaulted()) {
      currentGroup.resetDirectionofRelativeNorth();
    } else {
      currentGroup.setDirectionofRelativeNorth(newGroup.directionofRelativeNorth());
    }

    if (newGroup.isXOriginDefaulted()) {
      currentGroup.resetXOrigin();
    } else {
      currentGroup.setXOrigin(newGroup.xOrigin());
    }

    if (newGroup.isYOriginDefaulted()) {
      currentGroup.resetYOrigin();
    } else {
      currentGroup.setYOrigin(newGroup.yOrigin());
    }

    if (newGroup.isZOriginDefaulted()) {
      currentGroup.resetZOrigin();
    } else {
      currentGroup.setZOrigin(newGroup.zOrigin());
    }

    // remove current shading surfaces
    for (auto& currentSurface : currentGroup.shadingSurfaces()) {
      currentSurface.remove();
    }

    // change shading surface group type
    currentGroup.setShadingSurfaceType(newGroup.shadingSurfaceType());

    // add new shading surfaces
    for (const auto& newSurface : newGroup.shadingSurfaces()) {
      // DLM: this should probably be moved to a mergeSurface method
      auto clone = newSurface.clone(m_currentModel).cast<ShadingSurface>();
      clone.setShadingSurfaceGroup(currentGroup);

      m_newMergedHandles.insert(newSurface.handle());
      m_currentToNewHandleMapping[clone.handle()] = newSurface.handle();
      m_newToCurrentHandleMapping[newSurface.handle()] = clone.handle();
    }
  }

  void ModelMerger::mergeThermalZone(ThermalZone& currentThermalZone, const ThermalZone& newThermalZone) {
    if (m_newMergedHandles.find(newThermalZone.handle()) != m_newMergedHandles.end()) {
      // already merged
      return;
    }
    m_newMergedHandles.insert(newThermalZone.handle());

    currentThermalZone.setName(newThermalZone.nameString());

    // rendering color
    boost::optional<RenderingColor> newColor = newThermalZone.renderingColor();
    if (newColor) {
      boost::optional<RenderingColor> currentColor = currentThermalZone.renderingColor();
      if (currentColor) {
        currentColor->setRenderingRedValue(newColor->renderingRedValue());
        currentColor->setRenderingGreenValue(newColor->renderingGreenValue());
        currentColor->setRenderingBlueValue(newColor->renderingBlueValue());
        currentColor->setRenderingAlphaValue(newColor->renderingAlphaValue());
      } else {
        currentColor = RenderingColor::fromColorString(newColor->colorString(), currentThermalZone.model());
        OS_ASSERT(currentColor);
        currentThermalZone.setRenderingColor(*currentColor);
      }
    }

    // multiplier
    if (newThermalZone.isMultiplierDefaulted()) {
      // DLM: TODO: add option as this might have been intentionally reset on other model
      //currentThermalZone.resetMultiplier();
    } else {
      currentThermalZone.setMultiplier(newThermalZone.multiplier());
    }

    // ceilingHeight
    if (newThermalZone.isCeilingHeightDefaulted() || newThermalZone.isCeilingHeightAutocalculated()) {
      // DLM: TODO: add option as this might have been intentionally reset on other model
      //currentThermalZone.resetCeilingHeight();
    } else {
      currentThermalZone.setCeilingHeight(newThermalZone.ceilingHeight());
    }

    // volume
    if (newThermalZone.isVolumeDefaulted() || newThermalZone.isVolumeAutocalculated()) {
      // DLM: TODO: add option as this might have been intentionally reset on other model
      //currentThermalZone.resetVolume();
    } else {
      currentThermalZone.setVolume(newThermalZone.volume());
    }

    // DLM: TODO zoneInsideConvectionAlgorithm
    // DLM: TODO zoneOutsideConvectionAlgorithm
    // DLM: TODO zoneConditioningEquipmentListName ?
    // DLM: TODO thermostat
    // DLM: TODO zoneControlHumidistat
    // DLM: TODO zoneControlContaminantController
    // DLM: TODO sizingZone
  }

  void ModelMerger::mergeSpaceType(SpaceType& currentSpaceType, const SpaceType& newSpaceType) {
    if (m_newMergedHandles.find(newSpaceType.handle()) != m_newMergedHandles.end()) {
      // already merged
      return;
    }
    m_newMergedHandles.insert(newSpaceType.handle());

    currentSpaceType.setName(newSpaceType.nameString());

    //default construction set.
    if (boost::optional<DefaultConstructionSet> newDefaultConstructionSet = newSpaceType.defaultConstructionSet()) {
      boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newDefaultConstructionSet);
      if (currentObject) {
        auto currentDefaultConstructionSet = currentObject->cast<DefaultConstructionSet>();
        currentSpaceType.setDefaultConstructionSet(currentDefaultConstructionSet);
      } else {
        // DLM: this is an error
        //currentSpaceType.resetDefaultConstructionSet();
      }
    } else {
      // DLM: TODO: add option as this might have been intentionally reset on other model
      //currentSpaceType.resetDefaultConstructionSet();
    }

    // default schedule set
    if (boost::optional<DefaultScheduleSet> newDefaultScheduleSet = newSpaceType.defaultScheduleSet()) {
      boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newDefaultScheduleSet);
      if (currentObject) {
        auto currentDefaultScheduleSet = currentObject->cast<DefaultScheduleSet>();
        currentSpaceType.setDefaultScheduleSet(currentDefaultScheduleSet);
      } else {
        // DLM: this is an error
        //currentSpaceType.resetDefaultScheduleSet();
      }
    } else {
      // DLM: TODO: add option as this might have been intentionally reset on other model
      //currentSpaceType.resetDefaultScheduleSet();
    }

    // rendering color
    boost::optional<RenderingColor> newColor = newSpaceType.renderingColor();
    if (newColor) {
      boost::optional<RenderingColor> currentColor = currentSpaceType.renderingColor();
      if (currentColor) {
        currentColor->setRenderingRedValue(newColor->renderingRedValue());
        currentColor->setRenderingGreenValue(newColor->renderingGreenValue());
        currentColor->setRenderingBlueValue(newColor->renderingBlueValue());
        currentColor->setRenderingAlphaValue(newColor->renderingAlphaValue());
      } else {
        currentColor = RenderingColor::fromColorString(newColor->colorString(), currentSpaceType.model());
        OS_ASSERT(currentColor);
        currentSpaceType.setRenderingColor(*currentColor);
      }
    }

    // standardsBuildingType
    boost::optional<std::string> newStandardsBuildingType = newSpaceType.standardsBuildingType();
    if (newStandardsBuildingType) {
      currentSpaceType.setStandardsBuildingType(*newStandardsBuildingType);
    } else {
      // DLM: TODO: add option as this might have been intentionally reset on other model
      //currentSpaceType.resetStandardsBuildingType();
    }

    // standardsSpaceType
    boost::optional<std::string> newStandardsSpaceType = newSpaceType.standardsSpaceType();
    if (newStandardsSpaceType) {
      currentSpaceType.setStandardsSpaceType(*newStandardsSpaceType);
    } else {
      // DLM: TODO: add option as this might have been intentionally reset on other model
      //currentSpaceType.resetStandardsSpaceType();
    }

    // bring over child loads
    // DLM: should only do this if new model can have loads, if doing this should also remove current loads
    //for (const auto& newChild : newSpaceType.children()){
    //  ModelObject currentChild = newChild.clone(m_currentModel).cast<ModelObject>();
    //  currentChild.setParent(currentSpaceType);
    //}
  }

  void ModelMerger::mergeBuildingStory(BuildingStory& currentBuildingStory, const BuildingStory& newBuildingStory) {
    if (m_newMergedHandles.find(newBuildingStory.handle()) != m_newMergedHandles.end()) {
      // already merged
      return;
    }
    m_newMergedHandles.insert(newBuildingStory.handle());

    currentBuildingStory.setName(newBuildingStory.nameString());

    // rendering color
    boost::optional<RenderingColor> newColor = newBuildingStory.renderingColor();
    if (newColor) {
      boost::optional<RenderingColor> currentColor = currentBuildingStory.renderingColor();
      if (currentColor) {
        currentColor->setRenderingRedValue(newColor->renderingRedValue());
        currentColor->setRenderingGreenValue(newColor->renderingGreenValue());
        currentColor->setRenderingBlueValue(newColor->renderingBlueValue());
        currentColor->setRenderingAlphaValue(newColor->renderingAlphaValue());
      } else {
        currentColor = RenderingColor::fromColorString(newColor->colorString(), currentBuildingStory.model());
        OS_ASSERT(currentColor);
        currentBuildingStory.setRenderingColor(*currentColor);
      }
    }

    // nominalZCoordinate
    boost::optional<double> newNominalZCoordinate = newBuildingStory.nominalZCoordinate();
    if (newNominalZCoordinate) {
      currentBuildingStory.setNominalZCoordinate(*newNominalZCoordinate);
    } else {
      currentBuildingStory.resetNominalZCoordinate();
    }

    // nominalFloortoFloorHeight
    boost::optional<double> newNominalFloortoFloorHeight = newBuildingStory.nominalFloortoFloorHeight();
    if (newNominalFloortoFloorHeight) {
      currentBuildingStory.setNominalFloortoFloorHeight(*newNominalFloortoFloorHeight);
    } else {
      currentBuildingStory.resetNominalFloortoFloorHeight();
    }

    // nominalFloortoCeilingHeight
    boost::optional<double> newNominalFloortoCeilingHeight = newBuildingStory.nominalFloortoCeilingHeight();
    if (newNominalFloortoCeilingHeight) {
      currentBuildingStory.setNominalFloortoCeilingHeight(*newNominalFloortoCeilingHeight);
    } else {
      currentBuildingStory.resetNominalFloortoCeilingHeight();
    }

    //default construction set.
    if (boost::optional<DefaultConstructionSet> newDefaultConstructionSet = newBuildingStory.defaultConstructionSet()) {
      boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newDefaultConstructionSet);
      if (currentObject) {
        auto currentDefaultConstructionSet = currentObject->cast<DefaultConstructionSet>();
        currentBuildingStory.setDefaultConstructionSet(currentDefaultConstructionSet);
      } else {
        // DLM: this is an error
        //currentBuildingStory.resetDefaultConstructionSet();
      }
    } else {
      // DLM: TODO: add option as this might have been intentionally reset on other model
      //currentBuildingStory.resetDefaultConstructionSet();
    }

    // default schedule set
    if (boost::optional<DefaultScheduleSet> newDefaultScheduleSet = newBuildingStory.defaultScheduleSet()) {
      boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newDefaultScheduleSet);
      if (currentObject) {
        auto currentDefaultScheduleSet = currentObject->cast<DefaultScheduleSet>();
        currentBuildingStory.setDefaultScheduleSet(currentDefaultScheduleSet);
      } else {
        // DLM: this is an error
        //currentBuildingStory.resetDefaultScheduleSet();
      }
    } else {
      // DLM: TODO: add option as this might have been intentionally reset on other model
      //currentBuildingStory.resetDefaultScheduleSet();
    }
  }

  void ModelMerger::mergeBuildingUnit(BuildingUnit& currentBuildingUnit, const BuildingUnit& newBuildingUnit) {
    if (m_newMergedHandles.find(newBuildingUnit.handle()) != m_newMergedHandles.end()) {
      // already merged
      return;
    }
    m_newMergedHandles.insert(newBuildingUnit.handle());

    currentBuildingUnit.setName(newBuildingUnit.nameString());

    // rendering color
    boost::optional<RenderingColor> newColor = newBuildingUnit.renderingColor();
    if (newColor) {
      boost::optional<RenderingColor> currentColor = currentBuildingUnit.renderingColor();
      if (currentColor) {
        currentColor->setRenderingRedValue(newColor->renderingRedValue());
        currentColor->setRenderingGreenValue(newColor->renderingGreenValue());
        currentColor->setRenderingBlueValue(newColor->renderingBlueValue());
        currentColor->setRenderingAlphaValue(newColor->renderingAlphaValue());
      } else {
        currentColor = RenderingColor::fromColorString(newColor->colorString(), currentBuildingUnit.model());
        OS_ASSERT(currentColor);
        currentBuildingUnit.setRenderingColor(*currentColor);
      }
    }

    // buildingUnitType
    // DLM: TODO need to check if other buildingUnitType is defaulted and optionally set
    //currentBuildingUnit.setBuildingUnitType(newBuildingUnit.buildingUnitType());

    // DLM: TODO featureNames() const;
  }

  void ModelMerger::mergeDefaultConstructionSet(DefaultConstructionSet& currentDefaultConstructionSet,
                                                const DefaultConstructionSet& newDefaultConstructionSet) {
    if (m_newMergedHandles.find(newDefaultConstructionSet.handle()) != m_newMergedHandles.end()) {
      // already merged
      return;
    }
    m_newMergedHandles.insert(newDefaultConstructionSet.handle());

    currentDefaultConstructionSet.setName(newDefaultConstructionSet.nameString());

    // DLM: TODO defaultExteriorSurfaceConstructions() const;

    // DLM: TODO defaultInteriorSurfaceConstructions() const;

    // DLM: TODO defaultGroundContactSurfaceConstructions() const;

    // DLM: TODO defaultExteriorSubSurfaceConstructions() const;

    // DLM: TODO defaultInteriorSubSurfaceConstructions() const;

    // DLM: TODO interiorPartitionConstruction() const;

    // DLM: TODO spaceShadingConstruction() const;

    // DLM: TODO buildingShadingConstruction() const;

    // DLM: TODO siteShadingConstruction() const;
  }

  boost::optional<WorkspaceObject> ModelMerger::getCurrentModelObject(const WorkspaceObject& newObject) {
    // find object in current model
    IddObjectType iddObjectType = newObject.iddObject().type();
    boost::optional<UUID> currentHandle = getCurrentModelHandle(newObject.handle());
    boost::optional<WorkspaceObject> currentObject;
    if (currentHandle) {
      currentObject = m_currentModel.getObject(*currentHandle);
      if (!currentObject) {
        LOG(Error, "Could not find object in current model for handle " << *currentHandle << " of type " << iddObjectType.valueName());
      }
    }

    // create object in current model if needed
    if (!currentObject) {
      switch (iddObjectType.value()) {
        case IddObjectType::OS_Site:
          currentObject = m_currentModel.getUniqueModelObject<Site>();
          break;
        case IddObjectType::OS_Facility:
          currentObject = m_currentModel.getUniqueModelObject<Facility>();
          break;
        case IddObjectType::OS_Building:
          currentObject = m_currentModel.getUniqueModelObject<Building>();
          break;
        case IddObjectType::OS_Space:
          currentObject = model::Space(m_currentModel);
          break;
        case IddObjectType::OS_ShadingSurfaceGroup:
          currentObject = model::ShadingSurfaceGroup(m_currentModel);
          break;
        case IddObjectType::OS_ThermalZone:
          currentObject = model::ThermalZone(m_currentModel);
          break;
        case IddObjectType::OS_SpaceType:
          currentObject = model::SpaceType(m_currentModel);
          break;
        case IddObjectType::OS_BuildingStory:
          currentObject = model::BuildingStory(m_currentModel);
          break;
        case IddObjectType::OS_BuildingUnit:
          currentObject = model::BuildingUnit(m_currentModel);
          break;
        case IddObjectType::OS_DefaultConstructionSet:
          currentObject = model::DefaultConstructionSet(m_currentModel);
          break;
        default:
          LOG(Error, "No constructor registered for IddObjectType " << iddObjectType.valueName());
      }

      OS_ASSERT(currentObject);
      m_currentToNewHandleMapping[currentObject->handle()] = newObject.handle();
      m_newToCurrentHandleMapping[newObject.handle()] = currentObject->handle();
    }

    // merge objects
    switch (iddObjectType.value()) {
      case IddObjectType::OS_Site: {
        Site currentSite = currentObject->cast<Site>();
        Site newSite = newObject.cast<Site>();
        mergeSite(currentSite, newSite);
      } break;
      case IddObjectType::OS_Facility: {
        auto currentFacility = currentObject->cast<Facility>();
        auto newFacility = newObject.cast<Facility>();
        mergeFacility(currentFacility, newFacility);
      } break;
      case IddObjectType::OS_Building: {
        auto currentBuilding = currentObject->cast<Building>();
        auto newBuilding = newObject.cast<Building>();
        mergeBuilding(currentBuilding, newBuilding);
      } break;
      case IddObjectType::OS_Space: {
        auto currentSpace = currentObject->cast<Space>();
        auto newSpace = newObject.cast<Space>();
        mergeSpace(currentSpace, newSpace);
      } break;
      case IddObjectType::OS_ShadingSurfaceGroup: {
        auto currentGroup = currentObject->cast<ShadingSurfaceGroup>();
        auto newGroup = newObject.cast<ShadingSurfaceGroup>();

        // if the new group has a space it will be added under that space
        if (!newGroup.space()) {
          mergeShadingSurfaceGroup(currentGroup, newGroup);
        }
      } break;
      case IddObjectType::OS_ThermalZone: {
        auto currentThermalZone = currentObject->cast<ThermalZone>();
        auto newThermalZone = newObject.cast<ThermalZone>();
        mergeThermalZone(currentThermalZone, newThermalZone);
      } break;
      case IddObjectType::OS_SpaceType: {
        auto currentSpaceType = currentObject->cast<SpaceType>();
        auto newSpaceType = newObject.cast<SpaceType>();
        mergeSpaceType(currentSpaceType, newSpaceType);
      } break;
      case IddObjectType::OS_BuildingStory: {
        auto currentBuildingStory = currentObject->cast<BuildingStory>();
        auto newBuildingStory = newObject.cast<BuildingStory>();
        mergeBuildingStory(currentBuildingStory, newBuildingStory);
      } break;
      case IddObjectType::OS_BuildingUnit: {
        auto currentBuildingUnit = currentObject->cast<BuildingUnit>();
        auto newBuildingUnit = newObject.cast<BuildingUnit>();
        mergeBuildingUnit(currentBuildingUnit, newBuildingUnit);
      } break;
      case IddObjectType::OS_DefaultConstructionSet: {
        auto currentDefaultConstructionSet = currentObject->cast<DefaultConstructionSet>();
        auto newDefaultConstructionSet = newObject.cast<DefaultConstructionSet>();
        mergeDefaultConstructionSet(currentDefaultConstructionSet, newDefaultConstructionSet);
      } break;
      default:
        LOG(Error, "No merge function registered for IddObjectType " << iddObjectType.valueName());
    }

    OS_ASSERT(currentObject);
    OS_ASSERT(!currentObject->handle().isNull());
    return *currentObject;
  }

  void ModelMerger::mergeModels(Model& currentModel, const Model& newModel, const std::map<UUID, UUID>& handleMapping) {
    m_logSink.setThreadId(std::this_thread::get_id());
    m_logSink.resetStringStream();

    m_currentModel = currentModel;
    m_newModel = newModel;

    m_newMergedHandles.clear();
    m_currentToNewHandleMapping = handleMapping;
    m_newToCurrentHandleMapping.clear();
    for (const auto& it : handleMapping) {
      if (m_newToCurrentHandleMapping.find(it.second) != m_newToCurrentHandleMapping.end()) {
        LOG(Error, "Multiple entries in current model refer to handle '" << toString(it.second) << "' in new model");
      }
      m_newToCurrentHandleMapping[it.second] = it.first;
    }

    //** Remove objects from current model that are not in new model **//
    for (const auto& iddObjectType : iddObjectTypesToMerge()) {
      for (auto& currenObject : currentModel.getObjectsByType(iddObjectType)) {
        if (m_currentToNewHandleMapping.find(currenObject.handle()) == m_currentToNewHandleMapping.end()) {
          if ((iddObjectType == IddObjectType::OS_Site) || (iddObjectType == IddObjectType::OS_Facility)
              || (iddObjectType == IddObjectType::OS_Building)) {
            // These are unique objects, so no need to delete them
            continue;
          } else {
            currenObject.remove();
          }
        }
      }
    }

    //** Merge objects from new model into current model **//
    for (const auto& iddObjectType : iddObjectTypesToMerge()) {
      for (auto& newObject : newModel.getObjectsByType(iddObjectType)) {
        getCurrentModelObject(newObject);
      }
    }
  }

  std::vector<IddObjectType> ModelMerger::iddObjectTypesToMerge() const {
    return m_iddObjectTypesToMerge;
  }

  bool ModelMerger::setIddObjectTypesToMerge(const std::vector<IddObjectType>& /*iddObjectTypesToMerge*/) {
    LOG(Error, "setIddObjectTypesToMerge is not yet implemented");
    return false;
  }

}  // namespace model
}  // namespace openstudio
