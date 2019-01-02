/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ThreeJSReverseTranslator.hpp"
#include "ThreeJSForwardTranslator.hpp"

#include "FileOperations.hpp"
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
#include "Construction.hpp"
#include "Construction_Impl.hpp"
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
#include "AirWallMaterial.hpp"

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

    Point3dVectorVector ThreeJSReverseTranslator::getFaces(const ThreeGeometryData& data) const
    {
      Point3dVectorVector result;

      const Point3dVector vertices = fromThreeVector(data.vertices());
      const std::vector<size_t> faces = data.faces();
      const size_t n = faces.size();

      if (n < 1){
        return result;
      }


      if (faces[0] == openstudioFaceFormatId()){
        // openstudio, all vertices belong to one face
        Point3dVector face;

        // faces[0] is format
        for (size_t i = 1; i < n; ++i){
          face.push_back(vertices[faces[i]]);
        }

//        try{
//          Plane p(face);
//        } catch (const std::exception&)
//        {
//          std::cout << "Vertices: " << vertices << std::endl;
//          std::cout << "faces: " << std::endl;
//          for (const auto& f : faces){
//            std::cout << "  " << f << std::endl;
//          }
//          bool t = false;
//        }


        result.push_back(face);
      }

      return result;
    }

    Construction ThreeJSReverseTranslator::getAirWallConstruction(Model& model)
    {
      boost::optional<Construction> result = model.getConcreteModelObjectByName<Construction>("AirWall");
      if (!result){
        AirWallMaterial airWall(model);
        airWall.setName("AirWall");
        result = Construction(airWall);
      }
      OS_ASSERT(result);
      return result.get();
    }

    ThreeJSReverseTranslator::ThreeJSReverseTranslator()
    {
      m_logSink.setLogLevel(Warn);
      //m_logSink.setChannelRegex(boost::regex("openstudio\\.model\\.ThreeJSReverseTranslator"));
      m_logSink.setThreadId(QThread::currentThread());
    }

    std::vector<LogMessage> ThreeJSReverseTranslator::warnings() const
    {
      std::vector<LogMessage> result;

      for (LogMessage logMessage : m_logSink.logMessages()){
        if (logMessage.logLevel() == Warn){
          result.push_back(logMessage);
        }
      }

      return result;
    }

    std::vector<LogMessage> ThreeJSReverseTranslator::errors() const
    {
      std::vector<LogMessage> result;

      for (LogMessage logMessage : m_logSink.logMessages()){
        if (logMessage.logLevel() > Warn){
          result.push_back(logMessage);
        }
      }

      return result;
    }

    std::map<UUID, UUID> ThreeJSReverseTranslator::handleMapping() const
    {
      return m_handleMapping;
    }

    unsigned getIddObjectTypeOrder(const std::string& iddObjectType){
      unsigned typeOrder = 999;
      if (istringEqual(iddObjectType, "OS:Space")){
        typeOrder = 0;
      }else if (istringEqual(iddObjectType, "OS:ThermalZone")){
        typeOrder = 1;
      }else if (istringEqual(iddObjectType, "OS:SpaceType")){
        typeOrder = 2;
      }else if (istringEqual(iddObjectType, "OS:BuildingStory")){
        typeOrder = 3;
      }else if (istringEqual(iddObjectType, "OS:BuildingUnit")){
        typeOrder = 4;
      } else if (istringEqual(iddObjectType, "OS:DefaultConstructionSet")){
        typeOrder = 5;
      }
      return typeOrder;
    }

    bool sortModelObjectMetadata(const ThreeModelObjectMetadata &lhs, const ThreeModelObjectMetadata &rhs) {
      unsigned leftTypeOrder = getIddObjectTypeOrder(lhs.iddObjectType());
      unsigned rightTypeOrder = getIddObjectTypeOrder(rhs.iddObjectType());

      if (leftTypeOrder == rightTypeOrder){
        return lhs.name() < rhs.name();
      }
      return leftTypeOrder < rightTypeOrder;
    }


    unsigned getUserDataSurfaceTypeOrder(const std::string& userDataSurfaceType){

    /// surfaceType is overloaded as a more general type:
    /// Surfaces {"Wall", "Floor", "RoofCeiling"}
    /// SubSurfaces {"FixedWindow", "OperableWindow", "GlassDoor", "Skylight", "TubularDaylightDome", "TubularDaylightDiffuser", "Door", "OverheadDoor"}
    /// ShadingSurfaces {"SiteShading", "BuildingShading", "SpaceShading"}
    /// InteriorPartitionSurfaces {"InteriorPartitionSurface"}
    /// DaylightingControl {"DaylightingControl"}

      unsigned typeOrder = 999;
      if (istringEqual(userDataSurfaceType, "Wall") || istringEqual(userDataSurfaceType, "Floor") || istringEqual(userDataSurfaceType, "RoofCeiling")){
        typeOrder = 0;
      }else if (istringEqual(userDataSurfaceType, "FixedWindow") || istringEqual(userDataSurfaceType, "OperableWindow") || istringEqual(userDataSurfaceType, "GlassDoor") ||
                istringEqual(userDataSurfaceType, "Skylight") || istringEqual(userDataSurfaceType, "TubularDaylightDome") || istringEqual(userDataSurfaceType, "TubularDaylightDiffuser") ||
                istringEqual(userDataSurfaceType, "Door") || istringEqual(userDataSurfaceType, "OverheadDoor")){
        typeOrder = 1;
      }else if (istringEqual(userDataSurfaceType, "SiteShading") || istringEqual(userDataSurfaceType, "BuildingShading") || istringEqual(userDataSurfaceType, "SpaceShading")){
        typeOrder = 2;
      }else if (istringEqual(userDataSurfaceType, "InteriorPartitionSurface")){
        typeOrder = 3;
      }else if (istringEqual(userDataSurfaceType, "DaylightingControl")){
        typeOrder = 4;
      }
      return typeOrder;
    }

    bool sortSceneChildren(const ThreeSceneChild &lhs, const ThreeSceneChild &rhs) {
      unsigned leftTypeOrder = getIddObjectTypeOrder(lhs.userData().surfaceType());
      unsigned rightTypeOrder = getIddObjectTypeOrder(rhs.userData().surfaceType());

      if (leftTypeOrder == rightTypeOrder){
        return lhs.userData().name() < rhs.userData().name();
      }
      return leftTypeOrder < rightTypeOrder;
    }

    boost::optional<RenderingColor> makeRenderingColor(const std::string& color, Model& model)
    {
      return RenderingColor::fromColorString(color, model);
    }

    boost::optional<Model> ThreeJSReverseTranslator::modelFromThreeJS(const ThreeScene& scene)
    {
      m_logSink.setThreadId(QThread::currentThread());
      m_logSink.resetStringStream();

      /// Mapping between handles referenced in ThreeScene (keys) and handles of objects in returned model (values) for last translation
      m_handleMapping.clear();

      Model model;
      initializeModelObjects(model);

      std::map<std::string, Space> originalNameToSpaceMap;
      std::map<std::string, ShadingSurfaceGroup> originalNameToShadingSurfaceGroupMap;
      std::map<std::string, ThermalZone> originalNameToThermalZoneMap;
      std::map<std::string, SpaceType> originalNameToSpaceTypeMap;
      std::map<std::string, BuildingStory> originalNameToBuildingStoryMap;
      std::map<std::string, BuildingUnit> originalNameToBuildingUnitMap;
      std::map<std::string, DefaultConstructionSet> originalNameToDefaultConstructionSetMap;
      std::map<std::string, Surface> originalNameToSurfaceMap;
      std::map<std::string, SubSurface> originalNameToSubSurfaceMap;
      std::map<std::string, unsigned> originalNameToSpaceMultiplierMap;
      std::map<std::string, unsigned> originalNameToThermalZoneMultiplierMap;

      // create all the objects we will need
      ThreeSceneMetadata metadata = scene.metadata();
      std::vector<ThreeModelObjectMetadata> modelObjectMetadata = metadata.modelObjectMetadata();

      // sort object types so we make spaces first, etc
      std::sort(modelObjectMetadata.begin(), modelObjectMetadata.end(), sortModelObjectMetadata);

      for (const auto& m : modelObjectMetadata){
        std::string iddObjectType = m.iddObjectType();
        UUID handle = toUUID(m.handle());
        std::string name = m.name();
        std::string color = m.color();

        boost::optional<ModelObject> modelObject;

        if (istringEqual(iddObjectType, "OS:Space")){
          Space space(model);
          modelObject = space;

          unsigned multiplier = 1;
          if (m.multiplier()){
            multiplier = m.multiplier().get();
          }

          // DLM: temp code
          if (multiplier == 0){
            LOG(Warn, "Muliplier for Space '" << name << "' is 0, setting to 1");
            multiplier = 1;
          } else if (multiplier > 1){
            LOG(Warn, "Multiplier translation not yet implemented for Space '" << name << "', setting to 1");
            multiplier = 1;
          }

          originalNameToSpaceMultiplierMap.insert(std::make_pair(name, multiplier));
          originalNameToSpaceMap.insert(std::make_pair(name, space));
        }else if (istringEqual(iddObjectType, "OS:ShadingSurfaceGroup")){
          ShadingSurfaceGroup shadingSurfaceGroup(model);
          modelObject = shadingSurfaceGroup;
          originalNameToShadingSurfaceGroupMap.insert(std::make_pair(name, shadingSurfaceGroup));
        }else if (istringEqual(iddObjectType, "OS:ThermalZone")){
          ThermalZone thermalZone(model);
          boost::optional<RenderingColor> renderingColor = makeRenderingColor(color, model);
          if (renderingColor){
            thermalZone.setRenderingColor(*renderingColor);
          }
          modelObject = thermalZone;
          originalNameToThermalZoneMap.insert(std::make_pair(name, thermalZone));
        }else if (istringEqual(iddObjectType, "OS:SpaceType")){
          SpaceType spaceType(model);
          boost::optional<RenderingColor> renderingColor = makeRenderingColor(color, model);
          if (renderingColor){
            spaceType.setRenderingColor(*renderingColor);
          }
          modelObject = spaceType;
          originalNameToSpaceTypeMap.insert(std::make_pair(name, spaceType));
        }else if (istringEqual(iddObjectType, "OS:BuildingStory")){
          BuildingStory buildingStory(model);

          boost::optional<unsigned> multiplier = m.multiplier(); // DLM: should we apply multipliers only to Spaces in ThreeJS?
          boost::optional<double> nominalZCoordinate = m.nominalZCoordinate();
          boost::optional<double> belowFloorPlenumHeight = m.belowFloorPlenumHeight();
          boost::optional<double> floorToCeilingHeight = m.floorToCeilingHeight();
          boost::optional<double> aboveCeilingPlenumHeight = m.aboveCeilingPlenumHeight();

          if (nominalZCoordinate){
            buildingStory.setNominalZCoordinate(*nominalZCoordinate);
          }

          if (floorToCeilingHeight){
            buildingStory.setNominalFloortoCeilingHeight(*floorToCeilingHeight);
            if (belowFloorPlenumHeight && aboveCeilingPlenumHeight){
              buildingStory.setNominalFloortoFloorHeight(*floorToCeilingHeight + *belowFloorPlenumHeight + *aboveCeilingPlenumHeight);
            }
          }

          boost::optional<RenderingColor> renderingColor = makeRenderingColor(color, model);
          if (renderingColor){
            buildingStory.setRenderingColor(*renderingColor);
          }

          modelObject = buildingStory;
          originalNameToBuildingStoryMap.insert(std::make_pair(name, buildingStory));
        }else if (istringEqual(iddObjectType, "OS:BuildingUnit")){
          BuildingUnit buildingUnit(model);
          boost::optional<RenderingColor> renderingColor = makeRenderingColor(color, model);
          if (renderingColor){
            buildingUnit.setRenderingColor(*renderingColor);
          }
          modelObject = buildingUnit;
          originalNameToBuildingUnitMap.insert(std::make_pair(name, buildingUnit));
        } else if (istringEqual(iddObjectType, "OS:DefaultConstructionSet")){
          DefaultConstructionSet defaultConstructionSet(model);
          modelObject = defaultConstructionSet;
          originalNameToDefaultConstructionSetMap.insert(std::make_pair(name, defaultConstructionSet));
        }else{
          LOG(Error, "Unknown IddObjectType '" << iddObjectType << "'");
        }

        if (modelObject){
          modelObject->setName(name);

          if (!handle.isNull()){
            m_handleMapping[handle] = modelObject->handle();
          }
        }

      }

      // now translate all the surfaces
      ThreeSceneObject sceneObject = scene.object();
      std::vector<ThreeSceneChild> children = sceneObject.children();

      // sort the children to create all surfaces before sub surfaces
      std::sort(children.begin(), children.end(), sortSceneChildren);

      for (const auto& child : sceneObject.children()){
        boost::optional<ThreeGeometry> geometry = scene.getGeometry(child.geometry());
        if (!geometry){
          continue;
        }

        Point3dVectorVector faces = getFaces(geometry->data());

        boost::optional<ThreeMaterial> material = scene.getMaterial(child.material());

        ThreeUserData userData = child.userData();

        std::string handle = userData.handle();
        std::string name = userData.name();
        std::string surfaceType = userData.surfaceType();
        std::string constructionName = userData.constructionName();
        UUID constructionHandle = toUUID(userData.constructionHandle());
        std::string surfaceName = userData.surfaceName();
        UUID surfaceHandle = toUUID(userData.surfaceHandle());
        std::string subSurfaceName = userData.subSurfaceName();
        UUID subSurfaceHandle = toUUID(userData.subSurfaceHandle());
        std::string spaceName = userData.spaceName();
        UUID spaceHandle = toUUID(userData.spaceHandle());
        std::string shadingName = userData.shadingName();
        UUID shadingHandle = toUUID(userData.shadingHandle());
        std::string thermalZoneName = userData.thermalZoneName();
        UUID thermalZoneHandle = toUUID(userData.thermalZoneHandle());
        std::string spaceTypeName = userData.spaceTypeName();
        UUID spaceTypeHandle = toUUID(userData.spaceTypeHandle());
        std::string buildingStoryName = userData.buildingStoryName();
        UUID buildingStoryHandle = toUUID(userData.buildingStoryHandle());
        std::string buildingUnitName = userData.buildingUnitName();
        UUID buildingUnitHandle = toUUID(userData.buildingUnitHandle());
        std::string constructionSetName = userData.constructionSetName();
        UUID constructionSetHandle = toUUID(userData.constructionSetHandle());
        std::string outsideBoundaryCondition = userData.outsideBoundaryCondition();
        std::string outsideBoundaryConditionObjectName = userData.outsideBoundaryConditionObjectName();
        std::string outsideBoundaryConditionObjectHandle = userData.outsideBoundaryConditionObjectHandle();
        double illuminanceSetpoint = userData.illuminanceSetpoint();
        bool airWall = userData.airWall();
        //bool plenum = userData.plenum();
        //bool belowFloorPlenum = userData.belowFloorPlenum();
        //bool aboveFloorPlenum = userData.aboveCeilingPlenum();

        // DLM: these objects may have been renamed on import (e.g. if space and space type have the same name)
        // need to keep a map of original name to object for each type rather than relying on model name

        boost::optional<ThermalZone> thermalZone;
        if (!thermalZoneName.empty()){
          const auto it = originalNameToThermalZoneMap.find(thermalZoneName);
          if (it != originalNameToThermalZoneMap.end()){
            thermalZone = it->second;
          }else{
            LOG(Error, "Could not find ThermalZone '" << thermalZoneName << "'");
          }
        }

        boost::optional<SpaceType> spaceType;
        if (!spaceTypeName.empty()){
          const auto it = originalNameToSpaceTypeMap.find(spaceTypeName);
          if (it != originalNameToSpaceTypeMap.end()){
            spaceType = it->second;
          }else{
            LOG(Error, "Could not find SpaceType '" << spaceTypeName << "'");
          }
        }

        boost::optional<BuildingStory> buildingStory;
        if (!buildingStoryName.empty()){
          const auto it = originalNameToBuildingStoryMap.find(buildingStoryName);
          if (it != originalNameToBuildingStoryMap.end()){
            buildingStory = it->second;
          }else{
            LOG(Error, "Could not find BuildingStory '" << buildingStoryName << "'");
          }
        }

        boost::optional<BuildingUnit> buildingUnit;
        if (!buildingUnitName.empty()){
          const auto it = originalNameToBuildingUnitMap.find(buildingUnitName);
          if (it != originalNameToBuildingUnitMap.end()){
            buildingUnit = it->second;
          }else{
            LOG(Error, "Could not find BuildingUnit '" << buildingUnitName << "'");
          }
        }

        boost::optional<DefaultConstructionSet> constructionSet;
        if (!constructionSetName.empty()){
          const auto it = originalNameToDefaultConstructionSetMap.find(constructionSetName);
          if (it != originalNameToDefaultConstructionSetMap.end()){
            constructionSet = it->second;
          }else{
            LOG(Error, "Could not find DefaultConstructionSet '" << constructionSetName << "'");
          }
        }

        // Check if creating a surface
        if (istringEqual(surfaceType, "Wall") || istringEqual(surfaceType, "Floor") || istringEqual(surfaceType, "RoofCeiling")){

          if (spaceName.empty()){
            spaceName = "Default Space";
          }

          boost::optional<Space> space;
          if (!spaceName.empty()){
            const auto it = originalNameToSpaceMap.find(spaceName);
            if (it != originalNameToSpaceMap.end()){
              space = it->second;
            }else{
              LOG(Error, "Could not find Space '" << spaceName << "'");
              continue;
            }
          }else{
            LOG(Error, "Surface missing required Space Name");
            continue;
          }
          OS_ASSERT(space);

          unsigned multiplier = 1;
          const auto it = originalNameToSpaceMultiplierMap.find(spaceName);
          if (it != originalNameToSpaceMultiplierMap.end()){
            multiplier = it->second;
          }else{
            LOG(Warn, "Could not find muliplier for Space '" << spaceName << "'");
          }

          if (thermalZone){
            if (!space->thermalZone()){

              const auto it = originalNameToThermalZoneMultiplierMap.find(thermalZoneName);
              if (it != originalNameToThermalZoneMultiplierMap.end()){
                if (it->second != multiplier){
                  LOG(Warn, "Varying mulipliers detected for ThermalZone '" << thermalZoneName << "', setting multiplier to 1");
                  thermalZone->setMultiplier(1);
                }
              } else{
                originalNameToThermalZoneMultiplierMap[thermalZoneName] = multiplier;
                thermalZone->setMultiplier(multiplier);
              }

              space->setThermalZone(*thermalZone);
            }
          } else if (multiplier > 1){
            LOG(Warn, "Space '" << spaceName << "' has muliplier of " << multiplier << " but no ThermalZone, multiplier ignored");
          }

          if (spaceType){
            if (!space->spaceType()){
              space->setSpaceType(*spaceType);
            }
          }

          if (buildingStory){
            if (!space->buildingStory()){
              space->setBuildingStory(*buildingStory);
            }
          }

          if (buildingUnit){
            if (!space->buildingUnit()){
              space->setBuildingUnit(*buildingUnit);
            }
          }

          if (constructionSet){
            if (!space->defaultConstructionSet()){
              space->setDefaultConstructionSet(*constructionSet);
            }
          }

          for (const auto& face : faces){
            try{
              // ensure we can create a plane before calling Surface ctor that might mess up the model
              Plane plane(face);

              bool test;

              Surface surface(face, model);
              test = surface.setName(name);
              if (!test){
                LOG(Warn, "Could not set Surface name '" << name << "' for Surface '" << surface.nameString() << "'");
              }
              surface.setSpace(*space);
              if (!test){
                LOG(Warn, "Could not set Space '" << space->nameString() << "' for Surface '" << surface.nameString() << "'");
              }
              surface.setSurfaceType(surfaceType);
              if (!test){
                LOG(Warn, "Could not set SurfaceType '" << surfaceType << "' for Surface '" << surface.nameString() << "'");
              }

              if (airWall){
                surface.setConstruction(getAirWallConstruction(model));
              }

              originalNameToSurfaceMap.insert(std::make_pair(name, surface));

              // DLM: can we use these to set adjacencies?
              //std::string outsideBoundaryCondition = userData.outsideBoundaryCondition();
              //std::string outsideBoundaryConditionObjectName = userData.outsideBoundaryConditionObjectName();
              //std::string outsideBoundaryConditionObjectHandle = userData.outsideBoundaryConditionObjectHandle();

            } catch (const std::exception&){
              LOG(Warn, "Could not create surface for vertices " << face);
            }
          }

        } else if (istringEqual(surfaceType, "FixedWindow") || istringEqual(surfaceType, "OperableWindow") || istringEqual(surfaceType, "GlassDoor") ||
          istringEqual(surfaceType, "Skylight") || istringEqual(surfaceType, "TubularDaylightDome") || istringEqual(surfaceType, "TubularDaylightDiffuser") ||
          istringEqual(surfaceType, "Door") || istringEqual(surfaceType, "OverheadDoor")){

          boost::optional<Surface> surface;
          if (!surfaceName.empty()){
            const auto it = originalNameToSurfaceMap.find(surfaceName);
            if (it != originalNameToSurfaceMap.end()){
              surface = it->second;
            }else{
              LOG(Error, "Could not find Surface '" << surfaceName << "'");
              continue;
            }
          }else{
            LOG(Error, "SubSurface missing required Surface Name");
            continue;
          }
          OS_ASSERT(surface);

          for (const auto& face : faces){
            try{
              // ensure we can create a plane before calling Surface ctor that might mess up the model
              Plane plane(face);

              bool test;

              SubSurface subSurface(face, model);
              test = subSurface.setName(name);
              if (!test){
                LOG(Warn, "Could not set SubSurface name '" << name << "' for SubSurface '" << subSurface.nameString() << "'");
              }
              test = subSurface.setSurface(*surface);
              if (!test){
                LOG(Warn, "Could not set SubSurface to '" << surface->nameString() << "' for SubSurface '" << subSurface.nameString() << "'");
              }
              test = subSurface.setSubSurfaceType(surfaceType);
              if (!test){
                LOG(Warn, "Could not set SubSurfaceType to '" << surfaceType << "' for SubSurface '" << subSurface.nameString() << "'");
              }

              if (airWall){
                subSurface.setConstruction(getAirWallConstruction(model));
              }

              originalNameToSubSurfaceMap.insert(std::make_pair(name, subSurface));

            } catch (const std::exception&){
              LOG(Warn, "Could not create sub surface for vertices " << face);
            }
          }

        } else if (istringEqual(surfaceType, "SiteShading") || istringEqual(surfaceType, "BuildingShading") || istringEqual(surfaceType, "SpaceShading")){

          if (shadingName.empty()){
            shadingName = "Default " + surfaceType ;
          }

          boost::optional<ShadingSurfaceGroup> shadingSurfaceGroup;
          if (istringEqual(surfaceType, "SpaceShading")){

            boost::optional<SubSurface> subSurface;
            if (!subSurfaceName.empty()){
              const auto it = originalNameToSubSurfaceMap.find(subSurfaceName);
              if (it != originalNameToSubSurfaceMap.end()){
                subSurface = it->second;
              }else{
                LOG(Error, "Could not find SubSurface '" << subSurfaceName << "'");
                continue;
              }
            }else{
              LOG(Error, "SpaceShading missing required SubSurface Name");
              continue;
            }
            OS_ASSERT(subSurface);

            boost::optional<Space> space = subSurface->space();
            if (!space){
              LOG(Error, "Cannot find Space for SubSurface '" << subSurfaceName << "'");
              continue;
            }
            OS_ASSERT(space);

            std::vector<ShadingSurfaceGroup> groups = subSurface->shadingSurfaceGroups();
            if (groups.empty()){
              std::string shadingGroupName = subSurfaceName + " Shading Group";
              shadingSurfaceGroup = ShadingSurfaceGroup(model);
              shadingSurfaceGroup->setName(shadingGroupName);
              shadingSurfaceGroup->setSpace(*space);
              shadingSurfaceGroup->setShadedSubSurface(*subSurface);
            } else{
              shadingSurfaceGroup = groups[0];
            }
          } else{
            const auto it = originalNameToShadingSurfaceGroupMap.find(shadingName);
            if (it != originalNameToShadingSurfaceGroupMap.end()){
              shadingSurfaceGroup = it->second;
            }

            if (!shadingSurfaceGroup){
              shadingSurfaceGroup = ShadingSurfaceGroup(model);
              if (istringEqual(surfaceType, "SiteShading")){
                shadingSurfaceGroup->setShadingSurfaceType("Site");
              }else if (istringEqual(surfaceType, "BuildingShading")){
                shadingSurfaceGroup->setShadingSurfaceType("Building");
              }
              shadingSurfaceGroup->setName(shadingName);
            }
          }

          OS_ASSERT(shadingSurfaceGroup);

          for (const auto& face : faces){
            try{
              // ensure we can create a plane before calling Surface ctor that might mess up the model
              Plane plane(face);

              bool test;

              ShadingSurface shadingSurface(face, model);
              test = shadingSurface.setName(name);
              if (!test){
                LOG(Warn, "Could not set ShadingSurface name '" << name << "' for ShadingSurface '" << shadingSurface.nameString() << "'");
              }
              test = shadingSurface.setShadingSurfaceGroup(*shadingSurfaceGroup);
              if (!test){
                LOG(Warn, "Could not set ShadingSurfaceGroup '" << shadingSurfaceGroup->nameString() << "' for ShadingSurface '" << shadingSurface.nameString() << "'");
              }

            } catch (const std::exception&){
              LOG(Warn, "Could not create shading surface for vertices " << face);
            }
          }

        } else if (istringEqual(surfaceType, "DaylightingControl")){

          if (spaceName.empty()){
            spaceName = "Default Space";
          }

          boost::optional<Space> space;
          if (!spaceName.empty()){
            const auto it = originalNameToSpaceMap.find(spaceName);
            if (it != originalNameToSpaceMap.end()){
              space = it->second;
            }else{
              LOG(Error, "Could not find Space '" << spaceName << "'");
              continue;
            }
          }
          OS_ASSERT(space);

          for (const auto& face : faces){
            boost::optional<Point3d> centroid = getCentroid(face);

            if (centroid){
              DaylightingControl dc(model);
              dc.setName(name);
              dc.setPositionXCoordinate(centroid->x());
              dc.setPositionYCoordinate(centroid->y());
              dc.setPositionZCoordinate(centroid->z());
              dc.setSpace(*space);
              dc.setIlluminanceSetpoint(illuminanceSetpoint);

              boost::optional<ThermalZone> zone = space->thermalZone();
              if (thermalZone){
                if (!thermalZone->primaryDaylightingControl()){
                  thermalZone->setPrimaryDaylightingControl(dc);
                } else if (!thermalZone->secondaryDaylightingControl()){
                  thermalZone->setSecondaryDaylightingControl(dc);
                }
              }

            } else{
              LOG(Warn, "Could not create daylighting control for vertices " << face);
            }
          }

        }
      }

      // do intersections and matching between stories
      std::vector<BuildingStory> stories = model.getConcreteModelObjects<BuildingStory>();
      unsigned storiesN = stories.size();
      for (unsigned i = 0; i < storiesN; ++i){
        for (unsigned j = i + 1; j < storiesN; ++j){

          // DLM: TODO add a bounding box check to story

          for (auto& spaceI : stories[i].spaces()){
            for (auto& spaceJ : stories[j].spaces()){
              if (spaceI.boundingBox().intersects(spaceJ.boundingBox())){
                spaceI.intersectSurfaces(spaceJ);
                spaceI.matchSurfaces(spaceJ);
              }
            }
          }
        }
      }

      // do surface matching for spaces on same story
      for (const auto& story: stories){
        std::vector<Space> spaces = story.spaces();
        unsigned spacesN = spaces.size();
        for (unsigned i = 0; i < spacesN; ++i){
          for (unsigned j = i + 1; j < spacesN; ++j){
            if (spaces[i].boundingBox().intersects(spaces[j].boundingBox())){
              // DLM: should not need to intersect on same floor?
              spaces[i].intersectSurfaces(spaces[j]);
              spaces[i].matchSurfaces(spaces[j]);
            }
          }
        }
      }

      return model;
    }


  }//model
}//openstudio
