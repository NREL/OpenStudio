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

#ifndef MODEL_FLOORSPACEREVERSETRANSLATOR_IMPL_HPP
#define MODEL_FLOORSPACEREVERSETRANSLATOR_IMPL_HPP

#include "Model.hpp"
#include "BuildingStory.hpp"
#include "Space.hpp"

#include "../utilities/floorspace/FSVisitor.hpp"
#include "../utilities/floorspace/FSModel.hpp"

namespace openstudio {

// Because of member boost::optional<FSStory> m_currentFSStory; we must include FSModel.hpp which includes everything,
// so forward declaration is pointless
//class FSBase;
//class FSEdgeReference;
//class FSBuildingUnit;
//class FSBuildingType;
//class FSConfig;
//class FSConstructionSet;
//class FSDaylightingControl;
//class FSDaylightingControlDefinition;
//class FSDoor;
//class FSDoorDefinition;
//class FSGround;
//class FSModel;
//class FSProject;
//class FSShading;
//class FSSpace;
//class FSSpaceType;
//class FSStory;
//class FSThermalZone;
//class FSWindow;
//class FSWindowDefinition;

class Point3d;

namespace model {

  class ConstructionAirBoundary;
  class ShadingSurfaceGroup;
  class Surface;

  namespace detail {

    // Defines the type of space
    enum class SpaceTypeEnum
    {
      BELOWFLOOR,
      ABOVEFLOOR,
      ABOVECEILING
    };

    // TODO: Creating surfaces for spaces and creatig shading surfaces for building shading have
    // a lot of code in common but there are significant differences also. At some point we
    // shoud look into creating some common functions that calculate the vertices

    // A Visitor design pattern class. Each Dispatch method handles creation of OSM
    // concepts from the FloorspaceJS concepts

    class FloorspaceReverseTranslator_Impl final : public FSVisitor
    {
     public:
      FloorspaceReverseTranslator_Impl(Model& model);

      /// Mapping between handles referenced in Floorspace (keys) and handles of objects in returned model (values) for last translation
      /// This handle mapping can be used by the ModelMerger when merging returned model (new handles) with an existing model (existing handles)
      /// Note that this mapping may not include all objects such as Site, Building, or other objects not specified in the ThreeScene
      std::map<UUID, UUID> handleMapping() const;

      // Creates a BuildingUnit
      void Dispatch(const FSBuildingUnit& fsEntity);

      // TODO: building_type does not appear to go anywhere
      void Dispatch(const FSBuildingType& entity);

      // Config provides units but usints already factored in so no need to do anything here
      void Dispatch(const FSConfig& entity);

      // Creates a DefaultConstructionSet
      void Dispatch(const FSConstructionSet& fsEntity);

      // Creates a daylighting control
      void Dispatch(const FSDaylightingControl& fsEntity);

      void Dispatch(const FSDaylightingControlDefinition& entity);
      // Door is processed when FSSpace is processed
      void Dispatch(const FSDoor& entity);
      void Dispatch(const FSDoorDefinition& entity);
      void Dispatch(const FSGround& entity);

      void Dispatch(const FSModel& fsModel);

      // There's no OS equivalent
      void Dispatch(const FSProject& entity);

      // Creates a building shading group and shading surfaces
      void Dispatch(const FSShading& entity);

      void Dispatch(const FSSpace& space);

      void Dispatch(const FSSpaceType& fsEntity);

      // Creates a BuildingStory and its contents
      void Dispatch(const FSStory& fsEntity);

      void Dispatch(const FSThermalZone& fsEntity);

      void Dispatch(const FSWindow& entity);
      void Dispatch(const FSWindowDefinition& entity);

     private:
      REGISTER_LOGGER("openstudio.model.FloorspaceReverseTranslator");

      Model& m_model;
      boost::optional<BuildingStory> m_currentStory;
      boost::optional<FSStory> m_currentFSStory;
      boost::optional<Space> m_currentSpace;
      double m_currentStoryZ = 0.0;
      int m_nSurfaces = 0;
      std::map<UUID, UUID> m_handleMapping;

      // Maps the handle in the floorspace model (if defined) to the handle of the object created in the open studio model
      void MapHandles(const FSBase& fsEntity, const ModelObject& modelObject);

      void createShadingSurfaceGroup(const FSShading& fsEntity, double minZ, double maxZ, SpaceTypeEnum spaceType);

      void createFloorShading(ShadingSurfaceGroup& osGroup, std::vector<Point3d>& faceVertices, double z);

      void createRoofShading(ShadingSurfaceGroup& osGroup, std::vector<Point3d>& faceVertices, double z);

      void createWallShading(ShadingSurfaceGroup& osGroup, const FSShading& fsShading, const FSEdgeReference& edgeRef, double minZ, double maxZ,
                             bool reversed);

      void createShadingSurface(ShadingSurfaceGroup& osGroup, std::vector<Point3d>& faceVertices, double z);

      // Creates a Space from an FSSpace
      void createSpace(const FSSpace& fsEntity, double minZ, double maxZ, SpaceTypeEnum typeOfSpace, bool openToBelow);

      void createSurfaces(Space& osSpace, const FSSpace& fsSpace, double minZ, double maxZ, SpaceTypeEnum typeOfSpace, bool openToBelow);

      // Creates "Floor" surface
      void createFloorSurface(Space& osSpace, std::vector<Point3d>& faceVertices, double minZ, bool openToBelow);

      // Creates a "RoofCeiling" surface
      void createRoofSurface(Space& osSpace, std::vector<Point3d>& faceVertices, double maxZ);

      // Creates a "Wall" surface and sub-surfaces
      void createWallSurface(Space& osSpace, const FSSpace& fsSpace, const Point3d& p1, const Point3d& p2, double minX, double maxZ,
                             bool createSubsurfaces);
      // Creates one or more window subsurfaces along an edge
      void createWindowSubsurface(const FSWindow& window, Surface& osSurface, const FSEdge& edge, double minZ, double maxZ);

      // Creates a door subsurface along an edge
      void createDoorSubsurface(const FSDoor& door, Surface& osSurface, const FSEdge& edge, double minZ);

      void createSurface(Space& osSpace, std::vector<Point3d>& faceVertices, double z, const std::string& surfaceType, bool openToBelow);

      static bool orientVerticesForFloor(std::vector<Point3d>& points);
      static bool orientVerticesForRoof(std::vector<Point3d>& points);
      static ConstructionAirBoundary getAirWallConstruction(Model& model);
      static std::string getPlenumPostfix(SpaceTypeEnum spaceType);
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FLOORSPACEREVERSETRANSLATOR_IMPL_HPP
