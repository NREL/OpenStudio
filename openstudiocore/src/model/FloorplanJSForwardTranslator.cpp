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

#include <QThread>

#include <cmath>

namespace openstudio
{
  namespace model
  {

    FloorplanJSForwardTranslator::FloorplanJSForwardTranslator()
    {
      m_logSink.setLogLevel(Warn);
      //m_logSink.setChannelRegex(boost::regex("openstudio\\.model\\.FloorplanJSForwardTranslator"));
      m_logSink.setThreadId(QThread::currentThread());    
    }

    std::vector<LogMessage> FloorplanJSForwardTranslator::warnings() const
    {
      std::vector<LogMessage> result;

      for (LogMessage logMessage : m_logSink.logMessages()){
        if (logMessage.logLevel() == Warn){
          result.push_back(logMessage);
        }
      }

      return result;
    }

    std::vector<LogMessage> FloorplanJSForwardTranslator::errors() const
    {
      std::vector<LogMessage> result;

      for (LogMessage logMessage : m_logSink.logMessages()){
        if (logMessage.logLevel() > Warn){
          result.push_back(logMessage);
        }
      }

      return result;
    }

    FloorplanJS FloorplanJSForwardTranslator::updateFloorplanJS(const FloorplanJS& floorplan, const Model& model, bool removeMissingObjects)
    {
      m_logSink.setThreadId(QThread::currentThread());
      m_logSink.resetStringStream();

      FloorplanJS result(floorplan.toJSON());

      // first have to update all the names

      // update resources first since these will have references to them
      std::vector<FloorplanObject> unitObjects;
      for (const auto& unit : model.getConcreteModelObjects<BuildingUnit>())
      {
        FloorplanObject unitObject("", unit.nameString(), unit.handle());
        unitObjects.push_back(unitObject);
      }
      result.updateBuildingUnits(unitObjects, removeMissingObjects);

      std::vector<FloorplanObject> zoneObjects;
      for (const auto& zone : model.getConcreteModelObjects<ThermalZone>())
      {
        FloorplanObject zoneObject("", zone.nameString(), zone.handle());

        boost::optional<RenderingColor> color = zone.renderingColor();
        if (color){
          zoneObject.setDataString("color", color->colorString());
        }

        zoneObjects.push_back(zoneObject);
      }
      result.updateThermalZones(zoneObjects, removeMissingObjects);

      std::vector<FloorplanObject> spaceTypeObjects;
      for (const auto& spaceType : model.getConcreteModelObjects<SpaceType>())
      {
        FloorplanObject spaceTypeObject("", spaceType.nameString(), spaceType.handle());

        boost::optional<RenderingColor> color = spaceType.renderingColor();
        if (color){
          spaceTypeObject.setDataString("color", color->colorString());
        }

        spaceTypeObjects.push_back(spaceTypeObject);
      }
      result.updateSpaceTypes(spaceTypeObjects, removeMissingObjects);

      std::vector<FloorplanObject> setObjects;
      for (const auto& set : model.getConcreteModelObjects<DefaultConstructionSet>())
      {
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
      for (const auto& story : model.getConcreteModelObjects<BuildingStory>())
      {
        FloorplanObject storyObject("", story.nameString(), story.handle());
        
        boost::optional<RenderingColor> color = story.renderingColor();
        if (color){
          storyObject.setDataString("color", color->colorString());
        }

        // DLM: TODO "below_floor_plenum_height","floor_to_ceiling_height","above_ceiling_plenum_height","multiplier"

        storyObjects.push_back(storyObject);
      }
      result.updateStories(storyObjects, removeMissingObjects);

      std::vector<FloorplanObject> spaceObjects;
      for (const auto& space : model.getConcreteModelObjects<Space>())
      {
        FloorplanObject spaceObject("", space.nameString(), space.handle());

        // DLM: Should we even continue if the story is empty?
        boost::optional<BuildingStory> story = space.buildingStory();
        if (story){
          spaceObject.setParentHandleString(toString(story->handle()));
        }

        boost::optional<BuildingUnit> unit = space.buildingUnit();
        if (unit){
          spaceObject.setDataReference("building_unit_id", FloorplanObject("", unit->nameString(), unit->handle()));
        }

        boost::optional<ThermalZone> zone = space.thermalZone();
        if (zone){
          spaceObject.setDataReference("thermal_zone_id", FloorplanObject("", zone->nameString(), zone->handle()));
        }

        boost::optional<SpaceType> spaceType = space.spaceType();
        if (spaceType){
          spaceObject.setDataReference("space_type_id", FloorplanObject("", spaceType->nameString(), spaceType->handle()));
        }

        boost::optional<DefaultConstructionSet> set = space.defaultConstructionSet();
        if (set){
          spaceObject.setDataReference("construction_set_id", FloorplanObject("", set->nameString(), set->handle()));
        }

        spaceObjects.push_back(spaceObject);
      }
      result.updateSpaces(spaceObjects, removeMissingObjects);

      return result;
    }

  }//model
}//openstudio
