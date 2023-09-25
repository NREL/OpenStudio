/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
