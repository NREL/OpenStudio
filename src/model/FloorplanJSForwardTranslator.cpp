/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "FloorplanJSForwardTranslator.hpp"

#include "RenderingColor.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
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

#include <thread>

#include <cmath>

namespace openstudio {
namespace model {

  FloorplanJSForwardTranslator::FloorplanJSForwardTranslator() {
    m_logSink.setLogLevel(Warn);
    //m_logSink.setChannelRegex(boost::regex("openstudio\\.model\\.FloorplanJSForwardTranslator"));
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  std::vector<LogMessage> FloorplanJSForwardTranslator::warnings() const {
    std::vector<LogMessage> result = m_logSink.logMessages();
    result.erase(std::remove_if(result.begin(), result.end(), [](const auto& logMessage) { return logMessage.logLevel() != Warn; }), result.end());
    return result;
  }

  std::vector<LogMessage> FloorplanJSForwardTranslator::errors() const {
    std::vector<LogMessage> result = m_logSink.logMessages();
    result.erase(std::remove_if(result.begin(), result.end(), [](const auto& logMessage) { return logMessage.logLevel() <= Warn; }), result.end());
    return result;
  }

  FloorplanJS FloorplanJSForwardTranslator::updateFloorplanJS(const FloorplanJS& floorplan, const Model& model, bool removeMissingObjects) {
    m_logSink.setThreadId(std::this_thread::get_id());
    m_logSink.resetStringStream();

    FloorplanJS result(floorplan.toJSON());

    // first have to update all the names

    // update resources first since these will have references to them
    std::vector<FloorplanObject> unitObjects;
    for (const auto& unit : model.getConcreteModelObjects<BuildingUnit>()) {
      FloorplanObject unitObject("", unit.nameString(), unit.handle());
      unitObjects.push_back(unitObject);
    }
    result.updateBuildingUnits(unitObjects, removeMissingObjects);

    std::vector<FloorplanObject> zoneObjects;
    for (const auto& zone : model.getConcreteModelObjects<ThermalZone>()) {
      FloorplanObject zoneObject("", zone.nameString(), zone.handle());

      boost::optional<RenderingColor> color = zone.renderingColor();
      if (color) {
        zoneObject.setDataString("color", color->colorString());
      }

      zoneObjects.push_back(zoneObject);
    }
    result.updateThermalZones(zoneObjects, removeMissingObjects);

    std::vector<FloorplanObject> spaceTypeObjects;
    for (const auto& spaceType : model.getConcreteModelObjects<SpaceType>()) {
      FloorplanObject spaceTypeObject("", spaceType.nameString(), spaceType.handle());

      boost::optional<RenderingColor> color = spaceType.renderingColor();
      if (color) {
        spaceTypeObject.setDataString("color", color->colorString());
      }

      spaceTypeObjects.push_back(spaceTypeObject);
    }
    result.updateSpaceTypes(spaceTypeObjects, removeMissingObjects);

    std::vector<FloorplanObject> setObjects;
    for (const auto& set : model.getConcreteModelObjects<DefaultConstructionSet>()) {
      FloorplanObject setObject("", set.nameString(), set.handle());

      //boost::optional<RenderingColor> color = set.renderingColor();
      //if (color){
      // setObject.setDataString("color", color->colorString());
      //}

      setObjects.push_back(setObject);
    }
    result.updateConstructionSets(setObjects, removeMissingObjects);

    // do stories and spaces after resources
    std::vector<FloorplanObject> storyObjects;
    for (const auto& story : model.getConcreteModelObjects<BuildingStory>()) {
      FloorplanObject storyObject("", story.nameString(), story.handle());

      boost::optional<RenderingColor> color = story.renderingColor();
      if (color) {
        storyObject.setDataString("color", color->colorString());
      }

      // DLM: TODO "below_floor_plenum_height","floor_to_ceiling_height","above_ceiling_plenum_height","multiplier"

      storyObjects.push_back(storyObject);
    }
    result.updateStories(storyObjects, removeMissingObjects);

    std::vector<FloorplanObject> spaceObjects;
    for (const auto& space : model.getConcreteModelObjects<Space>()) {
      FloorplanObject spaceObject("", space.nameString(), space.handle());

      // DLM: Should we even continue if the story is empty?
      boost::optional<BuildingStory> story = space.buildingStory();
      if (story) {
        spaceObject.setParentHandleString(toString(story->handle()));
      }

      boost::optional<BuildingUnit> unit = space.buildingUnit();
      if (unit) {
        spaceObject.setDataReference("building_unit_id", FloorplanObject("", unit->nameString(), unit->handle()));
      }

      boost::optional<ThermalZone> zone = space.thermalZone();
      if (zone) {
        spaceObject.setDataReference("thermal_zone_id", FloorplanObject("", zone->nameString(), zone->handle()));
      }

      boost::optional<SpaceType> spaceType = space.spaceType();
      if (spaceType) {
        spaceObject.setDataReference("space_type_id", FloorplanObject("", spaceType->nameString(), spaceType->handle()));
      }

      boost::optional<DefaultConstructionSet> set = space.defaultConstructionSet();
      if (set) {
        spaceObject.setDataReference("construction_set_id", FloorplanObject("", set->nameString(), set->handle()));
      }

      spaceObjects.push_back(spaceObject);
    }
    result.updateSpaces(spaceObjects, removeMissingObjects);

    return result;
  }

}  // namespace model
}  // namespace openstudio
