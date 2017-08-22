
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

#include "ThreeJSReverseTranslator.hpp"
#include "ThreeJSForwardTranslator.hpp"

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

    Point3dVectorVector ThreeJSReverseTranslator::getFaces(const ThreeGeometryData& data) const
    {
      Point3dVectorVector result;

      const Point3dVector vertices = fromThreeVector(data.vertices());
      const std::vector<size_t> faces = data.faces();
      const size_t n = faces.size();

      if (n < 1){
        return result;
      }


      if (faces[0] == ThreeJSForwardTranslator::openstudioFaceFormatId()){
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
    
    boost::optional<Model> ThreeJSReverseTranslator::modelFromThreeJS(const ThreeScene& scene)
    {
      m_logSink.setThreadId(QThread::currentThread());
      m_logSink.resetStringStream();

      /// Mapping between handles referenced in ThreeScene (keys) and handles of objects in returned model (values) for last translation
      m_handleMapping.clear();

      Model model;

      // create all the objects we will need
      ThreeSceneMetadata metadata = scene.metadata();
      for (const auto& m : metadata.modelObjectMetadata()){
        std::string iddObjectType = m.iddObjectType();
        UUID handle = toUUID(m.handle());
        std::string name = m.name();
        
        boost::optional<ModelObject> modelObject;

        if (istringEqual(iddObjectType, "OS:ThermalZone")){
          modelObject = ThermalZone(model);
        }else if (istringEqual(iddObjectType, "OS:SpaceType")){
          modelObject = SpaceType(model);
        }else if (istringEqual(iddObjectType, "OS:BuildingStory")){
          modelObject = BuildingStory(model);
        } else if (istringEqual(iddObjectType, "OS:Space")){
          modelObject = Space(model);
        }else if (istringEqual(iddObjectType, "OS:BuildingUnit")){
          modelObject = BuildingUnit(model);
        } else if (istringEqual(iddObjectType, "OS:DefaultConstructionSet")){
          modelObject = DefaultConstructionSet(model);
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
        std::string spaceName = userData.spaceName();
        UUID spaceHandle = toUUID(userData.spaceHandle());
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
        //bool plenum = userData.plenum();
        //bool belowFloorPlenum = userData.belowFloorPlenum();
        //bool aboveFloorPlenum = userData.aboveCeilingPlenum();

        boost::optional<ThermalZone> thermalZone = model.getConcreteModelObjectByName<ThermalZone>(thermalZoneName);
        if (!thermalZone && !thermalZoneName.empty()){
          LOG(Error, "Could not find ThermalZone '" << thermalZoneName << "'");
        }

        boost::optional<SpaceType> spaceType = model.getConcreteModelObjectByName<SpaceType>(spaceTypeName);
        if (!spaceType && !spaceTypeName.empty()){
          LOG(Error, "Could not find SpaceType '" << spaceTypeName << "'");
        }

        boost::optional<BuildingStory> buildingStory = model.getConcreteModelObjectByName<BuildingStory>(buildingStoryName);
        if (!buildingStory && !buildingStoryName.empty()){
          LOG(Error, "Could not find BuildingStory '" << buildingStoryName << "'");
        }

        boost::optional<BuildingUnit> buildingUnit = model.getConcreteModelObjectByName<BuildingUnit>(buildingUnitName);
        if (!buildingUnit && !buildingUnitName.empty()){
          LOG(Error, "Could not find BuildingUnit '" << buildingUnitName << "'");
        }

        boost::optional<DefaultConstructionSet> constructionSet = model.getConcreteModelObjectByName<DefaultConstructionSet>(constructionSetName);
        if (!constructionSet && !constructionSetName.empty()){
          LOG(Error, "Could not find DefaultConstructionSet '" << constructionSetName << "'");
        }

        // Check if creating a surface
        if (istringEqual(surfaceType, "Wall") || istringEqual(surfaceType, "Floor") || istringEqual(surfaceType, "RoofCeiling")){

          if (spaceName.empty()){
            spaceName = "Default Space";
          }

          boost::optional<Space> space = model.getConcreteModelObjectByName<Space>(spaceName);
          if (!space){
            LOG(Error, "Could not find Space '" << spaceName << "'");
            continue;
          }

          OS_ASSERT(space);

          if (thermalZone){
            if (!space->thermalZone()){
              space->setThermalZone(*thermalZone);
            }
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

              Surface surface(face, model);
              surface.setName(name);
              surface.setSpace(*space);

              // DLM: can we use these to set adjacencies?
        //std::string outsideBoundaryCondition = userData.outsideBoundaryCondition();
        //std::string outsideBoundaryConditionObjectName = userData.outsideBoundaryConditionObjectName();
        //std::string outsideBoundaryConditionObjectHandle = userData.outsideBoundaryConditionObjectHandle();

            } catch (const std::exception& ){
              LOG_FREE(Warn, "modelFromThreeJS", "Could not create surface for vertices " << face);
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
