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

#include <cmath>

namespace openstudio
{
  namespace model
  {

    FloorplanJSForwardTranslator::FloorplanJSForwardTranslator()
    {}

    FloorplanJS FloorplanJSForwardTranslator::updateFloorplanJS(const FloorplanJS& floorplan, const Model& model)
    {
      FloorplanJS result(floorplan.toJSON());

      // first have to update all the names

      std::vector<FloorplanObjectId> storyObjectIds;
      for (const auto& story : model.getConcreteModelObjects<BuildingStory>())
      {
        storyObjectIds.push_back(FloorplanObjectId("", story.nameString(), story.handle()));
      }
      result.updateStories(storyObjectIds, true);

      std::vector<FloorplanObjectId> spaceObjectIds;
      for (const auto& space : model.getConcreteModelObjects<Space>())
      {
        FloorplanObjectId spaceObjectId("", space.nameString(), space.handle());
        if (boost::optional<BuildingStory> story = space.buildingStory()){
          spaceObjectId.setParentHandleString(toString(story->handle()));
        }
        spaceObjectIds.push_back(spaceObjectId);
      }
      result.updateStories(spaceObjectIds, true);

      std::vector<FloorplanObjectId> unitObjectIds;
      for (const auto& unit : model.getConcreteModelObjects<BuildingUnit>())
      {
        unitObjectIds.push_back(FloorplanObjectId("", unit.nameString(), unit.handle()));
      }
      result.updateBuildingUnits(unitObjectIds, true);

      std::vector<FloorplanObjectId> zoneObjectIds;
      for (const auto& zone : model.getConcreteModelObjects<ThermalZone>())
      {
        zoneObjectIds.push_back(FloorplanObjectId("", zone.nameString(), zone.handle()));
      }
      result.updateThermalZones(zoneObjectIds, true);

      std::vector<FloorplanObjectId> spaceTypeObjectIds;
      for (const auto& spaceType : model.getConcreteModelObjects<SpaceType>())
      {
        spaceTypeObjectIds.push_back(FloorplanObjectId("", spaceType.nameString(), spaceType.handle()));
      }
      result.updateSpaceTypes(spaceTypeObjectIds, true);

      std::vector<FloorplanObjectId> setObjectIds;
      for (const auto& set : model.getConcreteModelObjects<DefaultConstructionSet>())
      {
        setObjectIds.push_back(FloorplanObjectId("", set.nameString(), set.handle()));
      }
      result.updateConstructionSets(setObjectIds, true);

      // second update all the properties

      return result;
    }

    FloorplanJS FloorplanJSForwardTranslator::updateFloorplanJSResources(const FloorplanJS& floorplan, const Model& model)
    {
      FloorplanJS result(floorplan.toJSON());

      // first have to update all the names
      std::vector<FloorplanObjectId> unitObjectIds;
      for (const auto& unit : model.getConcreteModelObjects<BuildingUnit>())
      {
        unitObjectIds.push_back(FloorplanObjectId("", unit.nameString(), unit.handle()));
      }
      result.updateBuildingUnits(unitObjectIds, false);

      std::vector<FloorplanObjectId> zoneObjectIds;
      for (const auto& zone : model.getConcreteModelObjects<ThermalZone>())
      {
        zoneObjectIds.push_back(FloorplanObjectId("", zone.nameString(), zone.handle()));
      }
      result.updateThermalZones(zoneObjectIds, false);

      std::vector<FloorplanObjectId> spaceTypeObjectIds;
      for (const auto& spaceType : model.getConcreteModelObjects<SpaceType>())
      {
        spaceTypeObjectIds.push_back(FloorplanObjectId("", spaceType.nameString(), spaceType.handle()));
      }
      result.updateSpaceTypes(spaceTypeObjectIds, false);

      std::vector<FloorplanObjectId> setObjectIds;
      for (const auto& set : model.getConcreteModelObjects<DefaultConstructionSet>())
      {
        setObjectIds.push_back(FloorplanObjectId("", set.nameString(), set.handle()));
      }
      result.updateConstructionSets(setObjectIds, false);

      // second update all the properties

      return result;
    }
    
  }//model
}//openstudio
