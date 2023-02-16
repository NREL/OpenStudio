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

#include "FloorspaceReverseTranslator.hpp"
#include "FloorspaceReverseTranslator_Impl.hpp"

#include "FileOperations.hpp"
#include "RenderingColor.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
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
#include "Construction.hpp"
#include "Construction_Impl.hpp"
#include "ConstructionAirBoundary.hpp"
#include "ConstructionAirBoundary_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "ShadingSurfaceGroup.hpp"
#include "ShadingSurfaceGroup_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"
#include "InteriorPartitionSurfaceGroup.hpp"
#include "InteriorPartitionSurfaceGroup_Impl.hpp"
#include "PlanarSurfaceGroup.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "DefaultConstructionSet.hpp"
#include "DefaultConstructionSet_Impl.hpp"
#include "DaylightingControl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Plane.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Intersection.hpp"
#include "../utilities/floorspace/FSVisitor.hpp"

#include <thread>

#include <cmath>

namespace openstudio {
namespace model {
  static constexpr auto BELOWFLOORPLENUMPOSTFIX(" Floor Plenum");
  static constexpr auto ABOVECEILINGPLENUMPOSTFIX(" Plenum");
  static constexpr auto PLENUMSPACETYPENAME("Plenum Space Type");  // DLM: needs to be coordinated with name in Model_Impl::plenumSpaceType()

  // TODO: Creating surfaces for spaces and creatig shading surfaces for building shading have
  // a lot of code in common but there are significant differences also. At some point we
  // shoud look into creating some common functions that calculate the vertices

  // A Visitor design pattern class. Each Dispatch method handles creation of OSM
  // concepts from the FloorspaceJS concepts
  namespace detail {
    FloorspaceReverseTranslator_Impl::FloorspaceReverseTranslator_Impl(Model& model) : m_model(model) {}

    /// Mapping between handles referenced in Floorspace (keys) and handles of objects in returned model (values) for last translation
    /// This handle mapping can be used by the ModelMerger when merging returned model (new handles) with an existing model (existing handles)
    /// Note that this mapping may not include all objects such as Site, Building, or other objects not specified in the ThreeScene
    std::map<UUID, UUID> FloorspaceReverseTranslator_Impl::handleMapping() const {
      return m_handleMapping;
    }

    // Creates a BuildingUnit
    void FloorspaceReverseTranslator_Impl::Dispatch(const FSBuildingUnit& fsEntity) {
      BuildingUnit buildingUnit(m_model);
      buildingUnit.setName(fsEntity.name());
      boost::optional<RenderingColor> renderingColor = RenderingColor::fromColorString(fsEntity.color(), m_model);
      if (renderingColor.has_value()) {
        buildingUnit.setRenderingColor(*renderingColor);
      }
      MapHandles(fsEntity, buildingUnit);
    }

    void FloorspaceReverseTranslator_Impl::Dispatch(const FSBuildingType& entity) {
      // TODO: building_type does not appear to go anywhere
    }

    // Config provides units but usints already factored in so no need to do anything here
    void FloorspaceReverseTranslator_Impl::Dispatch(const FSConfig& entity) {}

    // Creates a DefaultConstructionSet
    void FloorspaceReverseTranslator_Impl::Dispatch(const FSConstructionSet& fsEntity) {
      DefaultConstructionSet constructionSet(m_model);
      constructionSet.setName(fsEntity.name());
      MapHandles(fsEntity, constructionSet);
    }

    // Creates a daylighting control
    void FloorspaceReverseTranslator_Impl::Dispatch(const FSDaylightingControl& fsEntity) {
      auto definition = fsEntity.definition();
      if (definition.has_value()) {
        DaylightingControl osDaylightingControl(m_model);
        osDaylightingControl.setName(fsEntity.name());
        osDaylightingControl.setIlluminanceSetpoint(definition->illuminanceSetpoint());
        osDaylightingControl.setPosition(fsEntity.point());
        osDaylightingControl.setSpace(*m_currentSpace);

        if (m_currentSpace->thermalZone().has_value()) {
          if (!m_currentSpace->thermalZone()->primaryDaylightingControl().has_value()) {
            m_currentSpace->thermalZone()->setPrimaryDaylightingControl(osDaylightingControl);
          } else if (!m_currentSpace->thermalZone()->secondaryDaylightingControl().has_value()) {
            m_currentSpace->thermalZone()->setSecondaryDaylightingControl(osDaylightingControl);
          }
        }

        MapHandles(fsEntity, osDaylightingControl);
      }
    }

    void FloorspaceReverseTranslator_Impl::Dispatch(const FSDaylightingControlDefinition& entity) {}
    // Door is processed when FSSpace is processed
    void FloorspaceReverseTranslator_Impl::Dispatch(const FSDoor& entity) {}
    void FloorspaceReverseTranslator_Impl::Dispatch(const FSDoorDefinition& entity) {}
    void FloorspaceReverseTranslator_Impl::Dispatch(const FSGround& entity) {}

    void FloorspaceReverseTranslator_Impl::Dispatch(const FSModel& fsModel) {
      initializeModelObjects(m_model);
      Building building = m_model.getUniqueModelObject<Building>();
      // set the model's north axis, opposite of the floorspace and threejs convention
      double northAxis = fsModel.project()->northAxis();
      building.setNorthAxis(-northAxis);

      for (auto& zone : fsModel.thermalZones()) {
        zone.Accept(*this);
      }

      for (auto& unit : fsModel.buildingUnits()) {
        unit.Accept(*this);
      }

      for (auto& type : fsModel.spaceTypes()) {
        type.Accept(*this);
      }

      for (auto& constructionSet : fsModel.constructionSets()) {
        constructionSet.Accept(*this);
      }

      // This is the starting elevation for the stories
      m_currentStoryZ = fsModel.project()->ground().floorOffset();

      for (auto& story : fsModel.stories()) {
        story.Accept(*this);
      }
    }

    // There's no OS equivalent
    void FloorspaceReverseTranslator_Impl::Dispatch(const FSProject& entity) {}

    // Creates a building shading group and shading surfaces
    void FloorspaceReverseTranslator_Impl::Dispatch(const FSShading& entity) {
      if (m_currentFSStory.has_value()) {

        double minZ = m_currentStoryZ;
        double maxZ = minZ + entity.belowFloorPlenumHeight();
        if (entity.belowFloorPlenumHeight() > 0) {
          createShadingSurfaceGroup(entity, minZ, maxZ, SpaceTypeEnum::BELOWFLOOR);
        }

        minZ = maxZ;
        maxZ = minZ + entity.floorToCeilingHeight();
        if (entity.floorToCeilingHeight() > 0) {
          createShadingSurfaceGroup(entity, minZ, maxZ, SpaceTypeEnum::ABOVEFLOOR);
        }

        minZ = maxZ;
        maxZ = minZ + entity.aboveCeilingPlenumHeight();
        if (entity.aboveCeilingPlenumHeight() > 0) {
          createShadingSurfaceGroup(entity, minZ, maxZ, SpaceTypeEnum::ABOVECEILING);
        }
      }
    }

    void FloorspaceReverseTranslator_Impl::Dispatch(const FSSpace& space) {
      // Minz and Maxz will define the height and elevation of the surfaces
      double minZ = m_currentStoryZ + space.offset();
      double maxZ = minZ + space.belowFloorPlenumHeight();

      // The lower of the three spaces gets to be open to below
      bool openToBelow = space.openToBelow();
      // Create up to three spaces depending on the plenum heights
      // If the Floorspace space is in a thermal zone then thermal zones are created for all three spaces
      if (space.belowFloorPlenumHeight() > 0) {
        createSpace(space, minZ, maxZ, SpaceTypeEnum::BELOWFLOOR, openToBelow);
        openToBelow = false;
      }

      minZ = maxZ;
      maxZ = minZ + space.floorToCeilingHeight();
      if (space.floorToCeilingHeight() > 0) {
        createSpace(space, minZ, maxZ, SpaceTypeEnum::ABOVEFLOOR, openToBelow);
        openToBelow = false;
      }

      minZ = maxZ;
      maxZ = minZ + space.aboveCeilingHeight();
      if (space.aboveCeilingHeight() > 0) {
        createSpace(space, minZ, maxZ, SpaceTypeEnum::ABOVECEILING, openToBelow);
        openToBelow = false;
      }
    }

    void FloorspaceReverseTranslator_Impl::Dispatch(const FSSpaceType& fsEntity) {
      SpaceType spaceType(m_model);
      spaceType.setName(fsEntity.name());
      boost::optional<RenderingColor> renderingColor = RenderingColor::fromColorString(fsEntity.color(), m_model);
      if (renderingColor.has_value()) {
        spaceType.setRenderingColor(*renderingColor);
      }

      MapHandles(fsEntity, spaceType);
    }

    // Creates a BuildingStory and its contents
    void FloorspaceReverseTranslator_Impl::Dispatch(const FSStory& fsEntity) {
      BuildingStory buildingStory(m_model);
      buildingStory.setName(fsEntity.name());
      buildingStory.setNominalZCoordinate(m_currentStoryZ);
      buildingStory.setNominalFloortoCeilingHeight(fsEntity.floorToCeilingHeight());
      buildingStory.setNominalFloortoFloorHeight(fsEntity.floorToFloorHeight());
      boost::optional<RenderingColor> renderingColor = RenderingColor::fromColorString(fsEntity.color(), m_model);
      if (renderingColor.has_value()) {
        buildingStory.setRenderingColor(*renderingColor);
      }
      m_currentStory = buildingStory;
      m_currentFSStory = fsEntity;

      // Create space and surfaces
      for (const auto& space : fsEntity.spaces()) {
        space.Accept(*this);
      }

      // Create shading
      for (const auto& shading : fsEntity.shadings()) {
        shading.Accept(*this);
      }

      // Increment story elevation for the next story
      m_currentStoryZ += fsEntity.floorToFloorHeight();

      MapHandles(fsEntity, buildingStory);

      m_currentStory = boost::none;
    }

    void FloorspaceReverseTranslator_Impl::Dispatch(const FSThermalZone& fsEntity) {
      ThermalZone thermalZone(m_model);
      thermalZone.setName(fsEntity.name());
      boost::optional<RenderingColor> renderingColor = RenderingColor::fromColorString(fsEntity.color(), m_model);
      if (renderingColor) {
        thermalZone.setRenderingColor(*renderingColor);
      }

      MapHandles(fsEntity, thermalZone);
    }

    void FloorspaceReverseTranslator_Impl::Dispatch(const FSWindow& entity) {}
    void FloorspaceReverseTranslator_Impl::Dispatch(const FSWindowDefinition& entity) {}

    // Maps the handle in the floorspace model (if defined) to the handle of the object created in the open studio model
    void FloorspaceReverseTranslator_Impl::MapHandles(const FSBase& fsEntity, const ModelObject& modelObject) {
      UUID handle = toUUID(fsEntity.handle());
      if (!handle.isNull()) {
        m_handleMapping[handle] = modelObject.handle();
      }
    }

    std::string FloorspaceReverseTranslator_Impl::getPlenumPostfix(SpaceTypeEnum spaceType) {
      switch (spaceType) {
        case SpaceTypeEnum::ABOVECEILING:
          return ABOVECEILINGPLENUMPOSTFIX;
        case SpaceTypeEnum::BELOWFLOOR:
          return BELOWFLOORPLENUMPOSTFIX;
        case SpaceTypeEnum::ABOVEFLOOR:
          return "";
      }

      return "";
    }

    void FloorspaceReverseTranslator_Impl::createShadingSurfaceGroup(const FSShading& fsEntity, double minZ, double maxZ, SpaceTypeEnum spaceType) {
      auto face = fsEntity.face();
      if (face.has_value()) {
        ShadingSurfaceGroup osShadingGroup(m_model);
        osShadingGroup.setName(fsEntity.name() + getPlenumPostfix(spaceType));
        osShadingGroup.setShadingSurfaceType("Building");

        Point3dVector faceVertices;
        for (const auto& edgeRef : face->edgeRefs()) {
          const FSVertex& vertex = edgeRef.getNextVertex();
          faceVertices.emplace_back(Point3d(vertex.x(), vertex.y(), minZ));
        }

        // Get the original orientation pf the face vertices
        boost::optional<Vector3d> outwardNormal = getOutwardNormal(faceVertices);
        if (!outwardNormal) {
          //LOG(Error, "Cannot compute outwardNormal for floorPrint.");
          return;
        }

        // We need the face to be oriented anti-clockwise to guarantee that vertical
        // surfaces are generated correctly. If the orientation is reversed then
        // we will reverse the sense of the edge when extracting vertices
        bool reversed = outwardNormal->z() < 0;

        createFloorShading(osShadingGroup, faceVertices, minZ);
        createRoofShading(osShadingGroup, faceVertices, maxZ);

        // Create wall surfaces
        for (const auto& edgeRef : face->edgeRefs()) {
          createWallShading(osShadingGroup, fsEntity, edgeRef, minZ, maxZ, reversed);
        }

        MapHandles(fsEntity, osShadingGroup);
      }
    }

    void FloorspaceReverseTranslator_Impl::createFloorShading(ShadingSurfaceGroup& osGroup, Point3dVector& faceVertices, double z) {
      if (orientVerticesForFloor(faceVertices)) {
        createShadingSurface(osGroup, faceVertices, z);
      }
    }

    void FloorspaceReverseTranslator_Impl::createRoofShading(ShadingSurfaceGroup& osGroup, Point3dVector& faceVertices, double z) {
      if (orientVerticesForRoof(faceVertices)) {
        createShadingSurface(osGroup, faceVertices, z);
      }
    }

    // TODO: fsShading unused
    void FloorspaceReverseTranslator_Impl::createWallShading(ShadingSurfaceGroup& osGroup, const FSShading& fsShading, const FSEdgeReference& edgeRef,
                                                             double minZ, double maxZ, bool reversed) {

      Point3dVector wallVertices;
      const FSVertex& v1 = edgeRef.edge().firstVertex();
      const FSVertex& v2 = edgeRef.edge().secondVertex();

      // Get the edge order (v1..v2 or v2..v1) and reverse if necessary to ensure
      // the wall surface is oriented correctly
      int edgeOrder = edgeRef.edgeOrder();
      if (edgeOrder == 1 && reversed) {
        edgeOrder = 0;
      } else if (edgeOrder == 0 && reversed) {
        edgeOrder = 1;
      }

      if (edgeOrder == 1) {
        wallVertices.push_back(Point3d(v2.x(), v2.y(), maxZ));
        wallVertices.push_back(Point3d(v1.x(), v1.y(), maxZ));
        wallVertices.push_back(Point3d(v1.x(), v1.y(), minZ));
        wallVertices.push_back(Point3d(v2.x(), v2.y(), minZ));
      } else {
        wallVertices.push_back(Point3d(v1.x(), v1.y(), maxZ));
        wallVertices.push_back(Point3d(v2.x(), v2.y(), maxZ));
        wallVertices.push_back(Point3d(v2.x(), v2.y(), minZ));
        wallVertices.push_back(Point3d(v1.x(), v1.y(), minZ));
      }

      ShadingSurface surface(wallVertices, m_model);
      surface.setName("Face " + std::to_string(m_nSurfaces++));
      surface.setShadingSurfaceGroup(osGroup);
    }

    void FloorspaceReverseTranslator_Impl::createShadingSurface(ShadingSurfaceGroup& osGroup, Point3dVector& faceVertices, double z) {
      Point3dVector vertices;
      for (const auto& vert : faceVertices) {
        vertices.emplace_back(Point3d(vert.x(), vert.y(), z));
      }

      ShadingSurface surface(vertices, m_model);
      surface.setName("Face " + std::to_string(m_nSurfaces++));
      surface.setShadingSurfaceGroup(osGroup);
    }

    // Creates a Space from an FSSpace
    void FloorspaceReverseTranslator_Impl::createSpace(const FSSpace& fsEntity, double minZ, double maxZ, SpaceTypeEnum typeOfSpace,
                                                       bool openToBelow) {
      Space osSpace(m_model);
      osSpace.setBuildingStory(*m_currentStory);
      osSpace.setName(fsEntity.name() + getPlenumPostfix(typeOfSpace));
      m_currentSpace = osSpace;

      // Make the surfaces/sub-surfaces
      createSurfaces(osSpace, fsEntity, minZ, maxZ, typeOfSpace, openToBelow);

      // Do the assignments
      // Thermal zones for above floor spaces are already created, here plenum thermal zones will be created
      // to associated them with the plenum spaces
      if (fsEntity.thermalZone().has_value()) {
        std::string zoneName = fsEntity.thermalZone()->name() + getPlenumPostfix(typeOfSpace);
        // Space is assigned to a thermal zone
        auto zone = m_model.getConcreteModelObjectByName<ThermalZone>(zoneName);
        if (!zone) {
          // Zone does not yet exist so create it
          zone = ThermalZone(m_model);
          zone->setName(zoneName);
        }
        if (zone) {
          int multiplier = fsEntity.multiplier();
          // Assign the space to the zone and set the multiplier
          if (multiplier == 0) {
            LOG(Warn, "Muliplier for Space '" << fsEntity.name() << "' is 0, setting to 1");
            multiplier = 1;
          } else if (multiplier > 1) {
            LOG(Warn, "Multiplier translation not yet implemented for Space '" << fsEntity.name() << "', setting to 1");
            multiplier = 1;
          }

          osSpace.setThermalZone(*zone);
          if (zone->spaces().size() == 1) {
            zone->setMultiplier(multiplier);
          }
        }
      }

      // For space type plenum spaces are always added to a space type called 'Plenum Space Type'
      // regardless of whether the space is associated with a space type.
      if (typeOfSpace == SpaceTypeEnum::ABOVEFLOOR) {
        if (fsEntity.spaceType().has_value()) {
          // The Space type for the main space will already have been created
          auto spaceType = m_model.getConcreteModelObjectByName<SpaceType>(fsEntity.spaceType()->name());
          if (spaceType.has_value()) {
            osSpace.setSpaceType(*spaceType);
          }
        }
      } else {
        auto spaceType = m_model.getConcreteModelObjectByName<SpaceType>(PLENUMSPACETYPENAME);
        // The plenum space type might not exist yet as it is created on demand
        if (!spaceType.has_value()) {
          spaceType = SpaceType(m_model);
          spaceType->setName(PLENUMSPACETYPENAME);
        }
        if (spaceType) {
          osSpace.setSpaceType(*spaceType);
        }
      }

      // Plena get assigned to the same construction set
      if (fsEntity.constructionSet().has_value()) {
        auto constructionSet = m_model.getConcreteModelObjectByName<DefaultConstructionSet>(fsEntity.constructionSet()->name());
        if (constructionSet) {
          osSpace.setDefaultConstructionSet(*constructionSet);
        }
      }

      // Plenum spaces get assigned to the same buiding unit that the main space is assigned to
      if (fsEntity.buildingUnit().has_value()) {
        auto unit = m_model.getConcreteModelObjectByName<BuildingUnit>(fsEntity.buildingUnit()->name());
        if (unit) {
          osSpace.setBuildingUnit(*unit);
        }
      }

      // Daylighting control and ditto daylighting
      for (const auto& fsDaylightingControl : fsEntity.daylightingControls()) {
        fsDaylightingControl.Accept(*this);
      }

      MapHandles(fsEntity, osSpace);
    }

    void FloorspaceReverseTranslator_Impl::createSurfaces(Space& osSpace, const FSSpace& fsSpace, double minZ, double maxZ, SpaceTypeEnum typeOfSpace,
                                                          bool openToBelow) {
      auto face = fsSpace.face();
      if (face.has_value()) {
        // Iterate over face edges to create the vector of Point3d
        // make sure points are oriented so the normal is -1
        // create the "floor" surface
        // Reverse the points and add maxz to the points
        // create the"RoofCeiling" surface
        // Iterate over edges to create walls
        // find window aligned to the edge, make sub-surface
        // find door aligned to the edge, make sub-surface
        Point3dVector faceVertices;
        for (const auto& edgeRef : face->edgeRefs()) {
          const FSVertex& vertex = edgeRef.getNextVertex();
          faceVertices.emplace_back(Point3d(vertex.x(), vertex.y(), 0));
        }

        // simplify the vertices to remove potential duplicate, colinear points
        double tol = 0.001;
        //std::vector<Point3d> testFaceVertices(faceVertices);
        faceVertices = simplify(faceVertices, false, tol);

        // Get the original orientation pf the face vertices
        boost::optional<Vector3d> outwardNormal = getOutwardNormal(faceVertices);
        if (!outwardNormal) {
          //LOG(Error, "Cannot compute outwardNormal for floorPrint.");
          return;
        }

        // Construct the vertices for the lower face
        createFloorSurface(osSpace, faceVertices, minZ, openToBelow);
        createRoofSurface(osSpace, faceVertices, maxZ);

        for (unsigned int i = 0; i < faceVertices.size(); i++) {
          const Point3d& p1 = faceVertices[i];
          const Point3d& p2 = faceVertices[(i + 1) % faceVertices.size()];
          createWallSurface(osSpace, fsSpace, p1, p2, minZ, maxZ, typeOfSpace == SpaceTypeEnum::ABOVEFLOOR);
        }
      }
    }

    // Creates "Floor" surface
    void FloorspaceReverseTranslator_Impl::createFloorSurface(Space& osSpace, Point3dVector& faceVertices, double minZ, bool openToBelow) {
      if (orientVerticesForFloor(faceVertices)) {
        createSurface(osSpace, faceVertices, minZ, "Floor", openToBelow);
      }
    }

    // Creates a "RoofCeiling" surface
    void FloorspaceReverseTranslator_Impl::createRoofSurface(Space& osSpace, Point3dVector& faceVertices, double maxZ) {
      if (orientVerticesForRoof(faceVertices)) {
        createSurface(osSpace, faceVertices, maxZ, "RoofCeiling", false);
      }
    }

    void FloorspaceReverseTranslator_Impl::createWallSurface(Space& osSpace, const FSSpace& fsSpace, const Point3d& p1, const Point3d& p2,
                                                             double minZ, double maxZ, bool createSubsurfaces) {
      constexpr double tol = 0.001;

      Point3dVector wallVertices;
      wallVertices.reserve(4);
      wallVertices.emplace_back(p2.x(), p2.y(), maxZ);
      wallVertices.emplace_back(p1.x(), p1.y(), maxZ);
      wallVertices.emplace_back(p1.x(), p1.y(), minZ);
      wallVertices.emplace_back(p2.x(), p2.y(), minZ);
      const Point3dVector wallSegment{p1, p2};

      Surface surface(wallVertices, m_model);
      surface.setName("Face " + std::to_string(m_nSurfaces++));
      surface.setSpace(osSpace);
      surface.setSurfaceType("Wall");

      if (createSubsurfaces) {
        if (m_currentFSStory.has_value()) {

          // Create a window subsurface for every window that is on this edge
          for (const auto& window : m_currentFSStory->windows()) {
            // Get window's x/y position which is the alpha based on its edge
            // windows can have multiple alphas but we only need one
            if (window.edge().has_value()) {
              auto edge = *window.edge();
              const double alpha = window.alphas()[0];
              const Point3d pp = window.centerVertex(alpha);

              if (getDistancePointToLineSegment(pp, wallSegment) < tol) {
                createWindowSubsurface(window, surface, edge, minZ, maxZ);
              }
            }
          }

          // Create a door subsurface for every window that is on this edge
          for (const auto& door : m_currentFSStory->doors()) {
            if (door.edge().has_value()) {
              auto edge = *door.edge();
              const double alpha = door.alphas()[0];
              const Point3d pp = door.centerVertex(alpha);

              if (getDistancePointToLineSegment(pp, wallSegment) < tol) {
                createDoorSubsurface(door, surface, edge, minZ);
              }
            }
          }
        }
      }
    }

    // Creates one or more window subsurfaces along an edge
    void FloorspaceReverseTranslator_Impl::createWindowSubsurface(const FSWindow& window, Surface& osSurface, const FSEdge& edge, double minZ,
                                                                  double maxZ) {

      auto windowDefinition = window.windowDefinition();
      if (!windowDefinition.has_value()) {
        return;
      }

      const auto& vertex1 = edge.firstVertex();
      const auto& vertex2 = edge.secondVertex();
      Vector3d edgeVector = edge.edgeVector();

      edgeVector.setLength(1.0);
      Vector3d upVector(0, 0, 1);
      Vector3d crossVector = upVector.cross(edgeVector);

      double sillHeight = windowDefinition->sillHeight();
      double height = 0.0;
      double width = 0.0;
      if ((windowDefinition->windowDefinitionMode() == "Single Window") || (windowDefinition->windowDefinitionMode() == "Repeating Windows")) {
        height = windowDefinition->height();
        width = windowDefinition->width();
      } else if (windowDefinition->windowDefinitionMode() == "Window to Wall Ratio") {
        double length = edge.edgeVector().length();
        width = length - 0.0508;  // Allow for 1" either end of the window
        // Area of the wall * wwr gives area of the window divided by width of the window gives height of the window
        height = (maxZ - minZ) * length * windowDefinition->wwr() / width;
      }

      for (double alpha : window.alphas()) {
        // Calculate the window center from the alpha and sill height
        double x = (1.0 - alpha) * vertex1.x() + alpha * vertex2.x();
        double y = (1.0 - alpha) * vertex1.y() + alpha * vertex2.y();
        Point3d windowCenter(x, y, 0);
        edgeVector.setLength(0.5 * width);
        Point3d window1 = windowCenter + edgeVector;
        edgeVector.setLength(-0.5 * width);
        Point3d window2 = windowCenter + edgeVector;

        Point3dVector windowVertices;
        windowVertices.push_back(Point3d(window1.x(), window1.y(), minZ + sillHeight + height));
        windowVertices.push_back(Point3d(window1.x(), window1.y(), minZ + sillHeight));
        windowVertices.push_back(Point3d(window2.x(), window2.y(), minZ + sillHeight));
        windowVertices.push_back(Point3d(window2.x(), window2.y(), minZ + sillHeight + height));

        // Make sure the sub-surface orientation matches the surface
        Plane plane1(osSurface.vertices());
        Plane plane2(windowVertices);
        if (plane1.reverseEqual(plane2)) {
          std::reverse(windowVertices.begin(), windowVertices.end());
          Point3d temp = window1;
          window1 = window2;
          window2 = temp;
          edgeVector = edgeVector.reverseVector();
          crossVector = crossVector.reverseVector();
        }

        SubSurface subSurface(windowVertices, m_model);
        subSurface.setName("Face " + std::to_string(m_nSurfaces++));
        subSurface.setSurface(osSurface);
        subSurface.setSubSurfaceType(windowDefinition->windowType());

        if (windowDefinition->overhangProjectionFactor() > 0 || windowDefinition->finProjectionFactor() > 0) {
          ShadingSurfaceGroup shadingGroup(m_model);
          shadingGroup.setName(*subSurface.name() + " Shading Group");
          shadingGroup.setSpace(subSurface.surface()->space().value());
          shadingGroup.setShadedSubSurface(subSurface);
          shadingGroup.setShadingSurfaceType("Space");

          if (double factor = windowDefinition->overhangProjectionFactor(); factor > 0) {
            Vector3d outVector = crossVector;
            outVector.setLength(factor * height);
            Point3d window3 = window1 + outVector;
            Point3d window4 = window2 + outVector;

            Point3dVector shadingVertices;
            shadingVertices.push_back(Point3d(window1.x(), window1.y(), minZ + sillHeight + height));
            shadingVertices.push_back(Point3d(window3.x(), window3.y(), minZ + sillHeight + height));
            shadingVertices.push_back(Point3d(window4.x(), window4.y(), minZ + sillHeight + height));
            shadingVertices.push_back(Point3d(window2.x(), window2.y(), minZ + sillHeight + height));

            ShadingSurface shadingSurface(shadingVertices, m_model);
            shadingSurface.setName("Face " + std::to_string(m_nSurfaces++));
            shadingSurface.setShadingSurfaceGroup(shadingGroup);
          }

          if (double factor = windowDefinition->finProjectionFactor(); factor > 0) {
            Vector3d outVector = crossVector;
            outVector.setLength(factor * height);
            Point3d window3 = window1 + outVector;
            Point3d window4 = window2 + outVector;

            // Right
            Point3dVector rightShadingVertices;
            rightShadingVertices.push_back(Point3d(window1.x(), window1.y(), minZ + sillHeight + height));
            rightShadingVertices.push_back(Point3d(window1.x(), window1.y(), minZ + sillHeight));
            rightShadingVertices.push_back(Point3d(window3.x(), window3.y(), minZ + sillHeight));
            rightShadingVertices.push_back(Point3d(window3.x(), window3.y(), minZ + sillHeight + height));

            ShadingSurface shadingSurfaceLeft(rightShadingVertices, m_model);
            shadingSurfaceLeft.setName("Face " + std::to_string(m_nSurfaces++));
            shadingSurfaceLeft.setShadingSurfaceGroup(shadingGroup);

            // Right
            Point3dVector leftShadingVertices;
            leftShadingVertices.push_back(Point3d(window2.x(), window2.y(), minZ + sillHeight + height));
            leftShadingVertices.push_back(Point3d(window2.x(), window2.y(), minZ + sillHeight));
            leftShadingVertices.push_back(Point3d(window4.x(), window4.y(), minZ + sillHeight));
            leftShadingVertices.push_back(Point3d(window4.x(), window4.y(), minZ + sillHeight + height));

            ShadingSurface shadingSurfaceRight(leftShadingVertices, m_model);
            shadingSurfaceRight.setName("Face " + std::to_string(m_nSurfaces++));
            shadingSurfaceRight.setShadingSurfaceGroup(shadingGroup);
          }
        }
      }
    }

    // Creates a door subsurface along an edge
    void FloorspaceReverseTranslator_Impl::createDoorSubsurface(const FSDoor& door, Surface& osSurface, const FSEdge& edge, double minZ) {

      const auto& vertex1 = edge.firstVertex();
      const auto& vertex2 = edge.secondVertex();
      Vector3d edgeVector = edge.edgeVector();

      edgeVector.setLength(1.0);
      Vector3d upVector(0, 0, 1);
      Vector3d crossVector = upVector.cross(edgeVector);

      auto doorDefinition = door.doorDefinition();
      if (!doorDefinition.has_value()) {
        return;
      }

      double height = doorDefinition->height();
      double width = doorDefinition->width();

      for (double alpha : door.alphas()) {
        // Calculate the window center from the alpha and sill height
        double x = (1.0 - alpha) * vertex1.x() + alpha * vertex2.x();
        double y = (1.0 - alpha) * vertex1.y() + alpha * vertex2.y();
        Point3d windowCenter(x, y, 0);
        edgeVector.setLength(0.5 * width);
        Point3d window1 = windowCenter + edgeVector;
        edgeVector.setLength(-0.5 * width);
        Point3d window2 = windowCenter + edgeVector;

        Point3dVector windowVertices;
        windowVertices.push_back(Point3d(window1.x(), window1.y(), minZ + height));
        windowVertices.push_back(Point3d(window1.x(), window1.y(), minZ));
        windowVertices.push_back(Point3d(window2.x(), window2.y(), minZ));
        windowVertices.push_back(Point3d(window2.x(), window2.y(), minZ + height));

        // Make sure the sub-surface orientation matches the surface
        Plane plane1(osSurface.vertices());
        Plane plane2(windowVertices);
        if (plane1.reverseEqual(plane2)) {
          std::reverse(windowVertices.begin(), windowVertices.end());
        }

        SubSurface subSurface(windowVertices, m_model);
        subSurface.setName("Face " + std::to_string(m_nSurfaces++));
        subSurface.setSurface(osSurface);
        std::string doorType = doorDefinition->doorType();
        if (doorType == "Glass Door") {
          doorType = "GlassDoor";
        } else if (doorType == "Overhead Door") {
          doorType = "OverheadDoor";
        }
        subSurface.setSubSurfaceType(doorType);
      }
    }

    void FloorspaceReverseTranslator_Impl::createSurface(Space& osSpace, Point3dVector& faceVertices, double z, const std::string& surfaceType,
                                                         bool openToBelow) {
      Point3dVector vertices;
      for (const auto& vert : faceVertices) {
        vertices.emplace_back(Point3d(vert.x(), vert.y(), z));
      }

      Surface surface(vertices, m_model);
      surface.setName("Face " + std::to_string(m_nSurfaces++));
      surface.setSpace(osSpace);
      surface.setSurfaceType(surfaceType);
      if (openToBelow) {
        surface.setConstruction(getAirWallConstruction(m_model));
      }
    }

    // Ensures the points are oriented correctly for a floor surface, reurns false if the normal cant be calculated
    bool FloorspaceReverseTranslator_Impl::orientVerticesForFloor(Point3dVector& points) {
      boost::optional<Vector3d> outwardNormal = getOutwardNormal(points);
      if (!outwardNormal) {
        //LOG(Error, "Cannot compute outwardNormal for floorPrint.");
        return false;
      }
      if (outwardNormal->z() > 0) {
        std::reverse(points.begin(), points.end());
      }
      return true;
    }

    bool FloorspaceReverseTranslator_Impl::orientVerticesForRoof(Point3dVector& points) {
      boost::optional<Vector3d> outwardNormal = getOutwardNormal(points);
      if (!outwardNormal) {
        //LOG(Error, "Cannot compute outwardNormal for floorPrint.");
        return false;
      }
      if (outwardNormal->z() < 0) {
        std::reverse(points.begin(), points.end());
      }
      return true;
    }

    ConstructionAirBoundary FloorspaceReverseTranslator_Impl::getAirWallConstruction(Model& model) {
      boost::optional<ConstructionAirBoundary> result = model.getConcreteModelObjectByName<ConstructionAirBoundary>("AirWall");
      if (!result) {
        ConstructionAirBoundary airWall(model);
        airWall.setName("AirWall");
        return airWall;
      }
      OS_ASSERT(result);
      return result.get();
    }
  }  // namespace detail

  ////////////////////////////////////////////////////////////////////////////////////////////

  FloorspaceReverseTranslator::FloorspaceReverseTranslator() {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.model\\.FloorspaceReverseTranslator"));
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  boost::optional<Model> FloorspaceReverseTranslator::modelFromFloorspace(const std::string& jsonOrPath) {
    FSModel fsModel;
    fsModel.load(jsonOrPath);

    Model model;
    detail::FloorspaceReverseTranslator_Impl visitor(model);
    fsModel.Accept(visitor);

    // Do some intersections - the logic is lifted form the ThreeJS reverse translator
    DoSurfaceMatchingAndIntersection(model);

    // Get the handle map
    m_handleMapping = visitor.handleMapping();

    return model;
  }

  //boost::optional<Model> FloorspaceReverseTranslator::modelFromFloorspace(FSModel const& fsModel) {

  //  // Reset the log sink before translation
  //  m_logSink.setThreadId(std::this_thread::get_id());
  //  m_logSink.resetStringStream();ll takes a

  //  Model model;
  //  detail::FloorspaceReverseTranslator_Impl visitor(model);
  //  fsModel.Accept(visitor);

  //  // Do some intersections - the logic is lifted form the ThreeJS reverse translator
  //  DoSurfaceMatchingAndIntersection(model);

  //  // Get the handle map
  //  m_handleMapping = visitor.handleMapping();

  //  return model;
  //}

  std::map<UUID, UUID> FloorspaceReverseTranslator::handleMapping() const {
    return m_handleMapping;
  }

  void FloorspaceReverseTranslator::DoSurfaceMatchingAndIntersection(Model& model) {
    // Get a list of stories sotted by elevation and name
    std::vector<BuildingStory> stories = SortStoriesByElevationAndName(model);

    // map of story index to sorted spaces
    std::vector<std::vector<Space>> sortedSpaces;

    for (unsigned i = 0; i < stories.size(); ++i) {
      std::vector<Space> spaces = SortSpacesByAreaAndName(stories[i]);
      sortedSpaces.push_back(spaces);
    }

    // Intersect between stories
    for (unsigned i = 0; i < stories.size(); ++i) {
      // bounding box for story i in building coordinates
      BoundingBox storyI_BB = stories[i].boundingBoxBuildingCoordinates();
      std::string namei = *stories[i].name();

      for (unsigned j = i + 1; j < stories.size(); ++j) {
        std::string namej = *stories[j].name();

        // check if storyI_BB intersects bounding box for story j in building coordinates
        if (!storyI_BB.intersects(stories[j].boundingBoxBuildingCoordinates())) {
          continue;
        }

        // loop over sorted spaces on story i
        for (auto& spaceI : sortedSpaces[i]) {
          // bounding box for space on story i in building coordinates
          BoundingBox spaceI_BB = spaceI.boundingBoxBuildingCoordinates();
          // loop over sorted spaces on story j
          for (auto& spaceJ : sortedSpaces[j]) {
            // check if intersects bounding box for space on story j in building coordinates
            if (spaceI_BB.intersects(spaceJ.boundingBoxBuildingCoordinates())) {
              spaceI.intersectSurfaces(spaceJ);
              spaceI.matchSurfaces(spaceJ);
            }
          }
        }
      }
    }

    // Intersect within the same story
    for (unsigned i = 0; i < stories.size(); ++i) {
      std::vector<Space>& spaces = sortedSpaces[i];

      for (unsigned j = 0; j < spaces.size(); ++j) {
        // bounding box for space j in building coordinates
        BoundingBox spaceJ_BB = spaces[j].boundingBoxBuildingCoordinates();

        for (unsigned k = j + 1; k < spaces.size(); ++k) {
          // check if space bounding boxes intersect
          if (spaceJ_BB.intersects(spaces[k].boundingBoxBuildingCoordinates())) {
            // DLM: should not need to intersect on same floor?
            spaces[j].intersectSurfaces(spaces[k]);
            spaces[j].matchSurfaces(spaces[k]);
          }
        }
      }
    }
  }

  std::vector<BuildingStory> FloorspaceReverseTranslator::SortStoriesByElevationAndName(Model& model) {
    std::vector<BuildingStory> stories = model.getConcreteModelObjects<BuildingStory>();
    std::sort(stories.begin(), stories.end(), [](const BuildingStory& a, const BuildingStory& b) -> bool {
      boost::optional<double> aZ = a.nominalZCoordinate();
      boost::optional<double> bZ = b.nominalZCoordinate();
      if (aZ && bZ) {
        if (aZ.get() == bZ.get()) {
          return a.nameString() < b.nameString();
        } else {
          return aZ.get() < bZ.get();
        }
      } else if (aZ) {
        return true;
      } else if (bZ) {
        return false;
      }
      return a.nameString() < b.nameString();
    });

    return stories;
  }

  std::vector<Space> FloorspaceReverseTranslator::SortSpacesByAreaAndName(BuildingStory& story) {
    std::vector<Space> spaces = story.spaces();

    // sort spaces by floor area then name
    std::sort(spaces.begin(), spaces.end(), [](const Space& a, const Space& b) -> bool {
      double areaA = a.floorArea();
      double areaB = b.floorArea();
      if (areaA == areaB) {
        return a.nameString() < b.nameString();
      }
      return areaA < areaB;
    });

    return spaces;
  }

  std::vector<LogMessage> FloorspaceReverseTranslator::warnings() const {
    std::vector<LogMessage> result;

    for (const LogMessage& logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() == Warn) {
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::vector<LogMessage> FloorspaceReverseTranslator::errors() const {
    std::vector<LogMessage> result;

    for (const LogMessage& logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() > Warn) {
        result.push_back(logMessage);
      }
    }

    return result;
  }

}  // namespace model
}  // namespace openstudio
