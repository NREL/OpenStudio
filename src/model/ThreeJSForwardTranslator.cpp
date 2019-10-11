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

#include "ThreeJSForwardTranslator.hpp"

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
#include "../utilities/geometry/ThreeJS.hpp"

#include <thread>

#include <cmath>

namespace openstudio
{
  namespace model
  {

    std::string getObjectThreeMaterialName(const ModelObject& object){
      return openstudio::getObjectThreeMaterialName(object.iddObjectType().valueDescription(), object.nameString());
    }

    void buildMaterials(Model model, std::vector<ThreeMaterial>& materials, std::map<std::string, std::string>& materialMap)
    {
      // make construction materials
      for (auto& construction : model.getModelObjects<ConstructionBase>()){
        boost::optional<RenderingColor> color = construction.renderingColor();
        if (!color){
          color = RenderingColor(model);
          construction.setRenderingColor(*color);
        }
        std::string name = getObjectThreeMaterialName(construction);
        addThreeMaterial(materials, materialMap, makeThreeMaterial(name, toThreeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }

      // make thermal zone materials
      for (auto& thermalZone : model.getConcreteModelObjects<ThermalZone>()){
        boost::optional<RenderingColor> color = thermalZone.renderingColor();
        if (!color){
          color = RenderingColor(model);
          thermalZone.setRenderingColor(*color);
        }
        std::string name = getObjectThreeMaterialName(thermalZone);
        addThreeMaterial(materials, materialMap, makeThreeMaterial(name, toThreeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }

      // make space type materials
      for (auto& spaceType : model.getConcreteModelObjects<SpaceType>()){
        boost::optional<RenderingColor> color = spaceType.renderingColor();
        if (!color){
          color = RenderingColor(model);
          spaceType.setRenderingColor(*color);
        }
        std::string name = getObjectThreeMaterialName(spaceType);
        addThreeMaterial(materials, materialMap, makeThreeMaterial(name, toThreeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }

      // make building story materials
      for (auto& buildingStory : model.getConcreteModelObjects<BuildingStory>()){
        boost::optional<RenderingColor> color = buildingStory.renderingColor();
        if (!color){
          color = RenderingColor(model);
          buildingStory.setRenderingColor(*color);
        }
        std::string name = getObjectThreeMaterialName(buildingStory);
        addThreeMaterial(materials, materialMap, makeThreeMaterial(name, toThreeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }

      // make building unit materials
      for (auto& buildingUnit : model.getConcreteModelObjects<BuildingUnit>()){
        boost::optional<RenderingColor> color = buildingUnit.renderingColor();
        if (!color){
          color = RenderingColor(model);
          buildingUnit.setRenderingColor(*color);
        }
        std::string name = getObjectThreeMaterialName(buildingUnit);
        addThreeMaterial(materials, materialMap, makeThreeMaterial(name, toThreeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }

    }

    size_t getVertexIndex(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol = 0.001)
    {
      size_t n = allPoints.size();
      for (size_t i = 0; i < n; ++i){
        if (std::sqrt(std::pow(point3d.x()-allPoints[i].x(), 2) + std::pow(point3d.y()-allPoints[i].y(), 2) + std::pow(point3d.z()-allPoints[i].z(), 2)) < tol){
          return i;
        }
      }
      allPoints.push_back(point3d);
      return (allPoints.size() - 1);
    }

    void updateUserData(ThreeUserData& userData, const PlanarSurface& planarSurface)
    {
      std::string name = planarSurface.nameString();
      boost::optional<Surface> surface = planarSurface.optionalCast<Surface>();
      boost::optional<ShadingSurface> shadingSurface = planarSurface.optionalCast<ShadingSurface>();
      boost::optional<InteriorPartitionSurface> interiorPartitionSurface = planarSurface.optionalCast<InteriorPartitionSurface>();
      boost::optional<SubSurface> subSurface = planarSurface.optionalCast<SubSurface>();
      boost::optional<PlanarSurfaceGroup> planarSurfaceGroup = planarSurface.planarSurfaceGroup();
      boost::optional<Space> space = planarSurface.space();
      boost::optional<ConstructionBase> construction = planarSurface.construction();

      userData.setHandle(toThreeUUID(toString(planarSurface.handle())));
      userData.setName(name);
      userData.setCoincidentWithOutsideObject(false);

      if (surface)
      {
        std::string surfaceType = surface->surfaceType();
        userData.setSurfaceType(surfaceType);
        userData.setSurfaceTypeMaterialName(getSurfaceTypeThreeMaterialName(surfaceType));
        userData.setSunExposure(surface->sunExposure());
        userData.setWindExposure(surface->windExposure());
        userData.setOutsideBoundaryCondition(surface->outsideBoundaryCondition());

        boost::optional<Surface> adjacentSurface = surface->adjacentSurface();
        if (adjacentSurface){
          userData.setOutsideBoundaryConditionObjectName(adjacentSurface->nameString());
          userData.setOutsideBoundaryConditionObjectHandle(toThreeUUID(toString(adjacentSurface->handle())));
        }

        if (userData.outsideBoundaryCondition() == "Outdoors"){
          if ((userData.sunExposure() == "SunExposed") && (userData.windExposure() == "WindExposed")){
            userData.setBoundaryMaterialName("Boundary_Outdoors_SunWind");
          } else if (userData.sunExposure() == "SunExposed") {
            userData.setBoundaryMaterialName("Boundary_Outdoors_Sun");
          } else if (userData.windExposure() == "WindExposed") {
            userData.setBoundaryMaterialName("Boundary_Outdoors_Wind");
          } else{
            userData.setBoundaryMaterialName("Boundary_Outdoors");
          }
        } else{
          userData.setBoundaryMaterialName("Boundary_" + userData.outsideBoundaryCondition());
        }
      }

      if (shadingSurface)
      {
        std::string shadingSurfaceType = "Building";
        if (shadingSurface->shadingSurfaceGroup()){
          shadingSurfaceType = shadingSurface->shadingSurfaceGroup()->shadingSurfaceType();
        }
        std::string surfaceType = shadingSurfaceType + "Shading";
        userData.setSurfaceType(surfaceType);
        userData.setSurfaceTypeMaterialName(getSurfaceTypeThreeMaterialName(surfaceType));
        userData.setSunExposure("SunExposed");
        userData.setWindExposure("WindExposed");
        userData.setOutsideBoundaryCondition("");
        userData.setOutsideBoundaryConditionObjectName("");
        userData.setOutsideBoundaryConditionObjectHandle("");
        userData.setBoundaryMaterialName("");
      }

      if (interiorPartitionSurface)
      {
        std::string surfaceType = "InteriorPartitionSurface";
        userData.setSurfaceType(surfaceType);
        userData.setSurfaceTypeMaterialName(getSurfaceTypeThreeMaterialName(surfaceType));
        userData.setSunExposure("NoSun");
        userData.setWindExposure("NoWind");
        userData.setOutsideBoundaryCondition("");
        userData.setOutsideBoundaryConditionObjectName("");
        userData.setOutsideBoundaryConditionObjectHandle("");
        userData.setBoundaryMaterialName("");
      }

      if (subSurface)
      {
        std::string subSurfaceType = subSurface->subSurfaceType();
        userData.setSurfaceType(subSurfaceType);
        userData.setSurfaceTypeMaterialName(getSurfaceTypeThreeMaterialName(subSurfaceType));

        boost::optional<Surface> parentSurface = subSurface->surface();
        std::string boundaryMaterialName;
        if (surface){
          boundaryMaterialName = "Boundary_" + surface->surfaceType();
          userData.setOutsideBoundaryCondition(surface->outsideBoundaryCondition());
          userData.setSunExposure(parentSurface->sunExposure());
          userData.setWindExposure(parentSurface->windExposure());
        }

        boost::optional<SubSurface> adjacentSubSurface = subSurface->adjacentSubSurface();
        if (adjacentSubSurface){
          userData.setOutsideBoundaryConditionObjectName(adjacentSubSurface->nameString());
          userData.setOutsideBoundaryConditionObjectHandle(toThreeUUID(toString(adjacentSubSurface->handle())));
          userData.setBoundaryMaterialName("Boundary_Surface");
        } else{
          if (boundaryMaterialName == "Boundary_Surface"){
            userData.setBoundaryMaterialName("");
          } else{
            userData.setBoundaryMaterialName(boundaryMaterialName);
          }
        }
      }

      if (construction)
      {
        userData.setConstructionName(construction->nameString());
        userData.setConstructionMaterialName(getObjectThreeMaterialName(*construction));
      }

      if (space)
      {
        userData.setSpaceName(space->nameString());

        boost::optional<ThermalZone> thermalZone = space->thermalZone();
        if (thermalZone){
          userData.setThermalZoneName(thermalZone->nameString());
          userData.setThermalZoneMaterialName(getObjectThreeMaterialName(*thermalZone));
        }

        boost::optional<SpaceType> spaceType = space->spaceType();
        if (spaceType){
          userData.setSpaceTypeName(spaceType->nameString());
          userData.setSpaceTypeMaterialName(getObjectThreeMaterialName(*spaceType));
        }

        boost::optional<BuildingStory> buildingStory = space->buildingStory();
        if (buildingStory){
          userData.setBuildingStoryName(buildingStory->nameString());
          userData.setBuildingStoryMaterialName(getObjectThreeMaterialName(*buildingStory));
        }

        boost::optional<BuildingUnit> buildingUnit = space->buildingUnit();
        if (buildingUnit){
          userData.setBuildingUnitName(buildingUnit->nameString());
          userData.setBuildingUnitMaterialName(getObjectThreeMaterialName(*buildingUnit));
        }
      }
    }

    void makeGeometries(const PlanarSurface& planarSurface, std::vector<ThreeGeometry>& geometries, std::vector<ThreeUserData>& userDatas, bool triangulateSurfaces)
    {
      std::string name = planarSurface.nameString();
      boost::optional<Surface> surface = planarSurface.optionalCast<Surface>();
      boost::optional<PlanarSurfaceGroup> planarSurfaceGroup = planarSurface.planarSurfaceGroup();

      // get the transformation to site coordinates
      Transformation siteTransformation;
      if (planarSurfaceGroup){
        siteTransformation = planarSurfaceGroup->siteTransformation();
      }

      // get the vertices
      Point3dVector vertices = planarSurface.vertices();
      Transformation t = Transformation::alignFace(vertices);
      //Transformation r = t.rotationMatrix();
      Transformation tInv = t.inverse();
      Point3dVector faceVertices = reverse(tInv*vertices);

      // get vertices of all sub surfaces
      Point3dVectorVector faceSubVertices;
      if (surface){
        for (const auto& subSurface : surface->subSurfaces()){
          faceSubVertices.push_back(reverse(tInv*subSurface.vertices()));
        }
      }

      Point3dVectorVector finalFaceVertices;
      if (triangulateSurfaces){
        finalFaceVertices = computeTriangulation(faceVertices, faceSubVertices);
        if (finalFaceVertices.empty()){
          LOG_FREE(Error, "modelToThreeJS", "Failed to triangulate surface " << name << " with " << faceSubVertices.size() << " sub surfaces");
          return;
        }
      } else{
        finalFaceVertices.push_back(faceVertices);
      }

      Point3dVector allVertices;
      std::vector<size_t> faceIndices;
      for (const auto& finalFaceVerts : finalFaceVertices) {
        Point3dVector finalVerts = siteTransformation*t*finalFaceVerts;
        //normal = siteTransformation.rotationMatrix*r*z

        // https://github.com/mrdoob/three.js/wiki/JSON-Model-format-3
        // 0 indicates triangle
        // 1 indicates quad
        // 2 indicates triangle with material
        // 3 indicates quad with material
        // ....
        // 255 quad with everything
        // 1024 - OpenStudio format, all vertices belong to single face

        if (triangulateSurfaces){
          faceIndices.push_back(0);
        } else{
          faceIndices.push_back(openstudioFaceFormatId());
        }

        Point3dVector::reverse_iterator it = finalVerts.rbegin();
        Point3dVector::reverse_iterator itend = finalVerts.rend();
        for (; it != itend; ++it){
          faceIndices.push_back(getVertexIndex(*it, allVertices));
        }

        // convert to 1 based indices
        //face_indices.each_index {|i| face_indices[i] = face_indices[i] + 1}
      }

      ThreeGeometryData geometryData(toThreeVector(allVertices), faceIndices);

      ThreeGeometry geometry(toThreeUUID(toString(planarSurface.handle())), "Geometry", geometryData);
      geometries.push_back(geometry);

      ThreeUserData userData;
      updateUserData(userData, planarSurface);

      // check if the adjacent surface is truly adjacent
      // this controls display only, not energy model
      if (!userData.outsideBoundaryConditionObjectHandle().empty()){

        UUID adjacentHandle = toUUID(fromThreeUUID(userData.outsideBoundaryConditionObjectHandle()));
        boost::optional<PlanarSurface> adjacentPlanarSurface = planarSurface.model().getModelObject<PlanarSurface>(adjacentHandle);
        OS_ASSERT(adjacentPlanarSurface);

        Transformation otherSiteTransformation;
        if (adjacentPlanarSurface->planarSurfaceGroup()){
          otherSiteTransformation = adjacentPlanarSurface->planarSurfaceGroup()->siteTransformation();
        }

        Point3dVector otherVertices = otherSiteTransformation*adjacentPlanarSurface->vertices();
        if (circularEqual(siteTransformation*vertices, reverse(otherVertices))){
          userData.setCoincidentWithOutsideObject(true);
        } else{
          userData.setCoincidentWithOutsideObject(false);
        }
      }

      userDatas.push_back(userData);
    }


    ThreeJSForwardTranslator::ThreeJSForwardTranslator()
    {
      m_logSink.setLogLevel(Warn);
      //m_logSink.setChannelRegex(boost::regex("openstudio\\.model\\.ThreeJSForwardTranslator"));
      m_logSink.setThreadId(std::this_thread::get_id());
    }

    std::vector<LogMessage> ThreeJSForwardTranslator::warnings() const
    {
      std::vector<LogMessage> result;

      for (LogMessage logMessage : m_logSink.logMessages()){
        if (logMessage.logLevel() == Warn){
          result.push_back(logMessage);
        }
      }

      return result;
    }

    std::vector<LogMessage> ThreeJSForwardTranslator::errors() const
    {
      std::vector<LogMessage> result;

      for (LogMessage logMessage : m_logSink.logMessages()){
        if (logMessage.logLevel() > Warn){
          result.push_back(logMessage);
        }
      }

      return result;
    }

    ThreeScene ThreeJSForwardTranslator::modelToThreeJS(const Model& model, bool triangulateSurfaces)
    {
      return modelToThreeJS(model, triangulateSurfaces, [](double percentage) {});
    }

    ThreeScene ThreeJSForwardTranslator::modelToThreeJS(const Model& model, bool triangulateSurfaces, std::function<void(double)> updatePercentage)
    {
      m_logSink.setThreadId(std::this_thread::get_id());
      m_logSink.resetStringStream();

      updatePercentage(0.0);

      std::vector<ThreeMaterial> materials;
      std::map<std::string, std::string> materialMap;

      // DLM: always add the standard materials
      //if (triangulateSurfaces){
        // add the standard materials for rendering
        for (const auto& material : makeStandardThreeMaterials()){
          addThreeMaterial(materials, materialMap, material);
        }
      //}

      // add model specific materials
      buildMaterials(model, materials, materialMap);

      std::vector<ThreeSceneChild> sceneChildren;
      std::vector<ThreeGeometry> allGeometries;
      std::vector<ThreeModelObjectMetadata> modelObjectMetadata;

      // get number of things to translate
      std::vector<PlanarSurface> planarSurfaces = model.getModelObjects<PlanarSurface>();
      std::vector<PlanarSurfaceGroup> planarSurfaceGroups = model.getModelObjects<PlanarSurfaceGroup>();
      std::vector<BuildingStory> buildingStories = model.getConcreteModelObjects<BuildingStory>();
      std::vector<BuildingUnit> buildingUnits = model.getConcreteModelObjects<BuildingUnit>();
      std::vector<ThermalZone> thermalZones = model.getConcreteModelObjects<ThermalZone>();
      std::vector<SpaceType> spaceTypes = model.getConcreteModelObjects<SpaceType>();
      std::vector<DefaultConstructionSet> defaultConstructionSets = model.getConcreteModelObjects<DefaultConstructionSet>();
      double n = 0;
      std::vector<PlanarSurface>::size_type N = planarSurfaces.size() + planarSurfaceGroups.size() + buildingStories.size() + buildingUnits.size() + thermalZones.size() + spaceTypes.size() + defaultConstructionSets.size() + 1;

      // loop over all surfaces
      for (const auto& planarSurface : planarSurfaces)
      {
        std::vector<ThreeGeometry> geometries;
        std::vector<ThreeUserData> userDatas;
        makeGeometries(planarSurface, geometries, userDatas, triangulateSurfaces);
        OS_ASSERT(geometries.size() == userDatas.size());

        size_t n = geometries.size();
        for (size_t i = 0; i < n; ++i){

         allGeometries.push_back(geometries[i]);

         std::string thisUUID(toThreeUUID(toString(createUUID())));
         std::string thisName(userDatas[i].name());
         std::string thisMaterialId = getThreeMaterialId(userDatas[i].surfaceTypeMaterialName(), materialMap);

         ThreeSceneChild sceneChild(thisUUID, thisName, "Mesh", geometries[i].uuid(), thisMaterialId, userDatas[i]);
           sceneChildren.push_back(sceneChild);
        }

        n += 1;
        updatePercentage(100.0*n / N);
      }

      ThreeSceneObject sceneObject(toThreeUUID(toString(openstudio::createUUID())), sceneChildren);

      BoundingBox boundingBox;
      boundingBox.addPoint(Point3d(0, 0, 0));
      boundingBox.addPoint(Point3d(1, 1, 1));
      for (const auto& group : planarSurfaceGroups){
        boundingBox.add(group.transformation()*group.boundingBox());

        n += 1;
        updatePercentage(100.0*n / N);
      }

      double lookAtX = 0; // (boundingBox.minX().get() + boundingBox.maxX().get()) / 2.0
      double lookAtY = 0; // (boundingBox.minY().get() + boundingBox.maxY().get()) / 2.0
      double lookAtZ = 0; // (boundingBox.minZ().get() + boundingBox.maxZ().get()) / 2.0
      double lookAtR =            sqrt(std::pow(boundingBox.maxX().get() / 2.0, 2) + std::pow(boundingBox.maxY().get() / 2.0, 2) + std::pow(boundingBox.maxZ().get() / 2.0, 2));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.minX().get() / 2.0, 2) + std::pow(boundingBox.maxY().get() / 2.0, 2) + std::pow(boundingBox.maxZ().get() / 2.0, 2)));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.maxX().get() / 2.0, 2) + std::pow(boundingBox.minY().get() / 2.0, 2) + std::pow(boundingBox.maxZ().get() / 2.0, 2)));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.maxX().get() / 2.0, 2) + std::pow(boundingBox.maxY().get() / 2.0, 2) + std::pow(boundingBox.minZ().get() / 2.0, 2)));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.minX().get() / 2.0, 2) + std::pow(boundingBox.minY().get() / 2.0, 2) + std::pow(boundingBox.maxZ().get() / 2.0, 2)));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.minX().get() / 2.0, 2) + std::pow(boundingBox.maxY().get() / 2.0, 2) + std::pow(boundingBox.minZ().get() / 2.0, 2)));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.maxX().get() / 2.0, 2) + std::pow(boundingBox.minY().get() / 2.0, 2) + std::pow(boundingBox.minZ().get() / 2.0, 2)));
      lookAtR = std::max(lookAtR, sqrt(std::pow(boundingBox.minX().get() / 2.0, 2) + std::pow(boundingBox.minY().get() / 2.0, 2) + std::pow(boundingBox.minZ().get() / 2.0, 2)));

      ThreeBoundingBox threeBoundingBox(boundingBox.minX().get(), boundingBox.minY().get(), boundingBox.minZ().get(),
                                        boundingBox.maxX().get(), boundingBox.maxY().get(), boundingBox.maxZ().get(),
                                        lookAtX, lookAtY, lookAtZ, lookAtR);

      std::vector<std::string> buildingStoryNames;
      for (const auto& buildingStory : buildingStories){
        buildingStoryNames.push_back(buildingStory.nameString());

        ThreeModelObjectMetadata storyMetaData(buildingStory.iddObjectType().valueDescription(), toString(buildingStory.handle()), buildingStory.nameString());
        if (buildingStory.nominalZCoordinate()){
          storyMetaData.setNominalZCoordinate(buildingStory.nominalZCoordinate().get());
        }
        if (buildingStory.nominalFloortoCeilingHeight()){
          storyMetaData.setFloorToCeilingHeight(buildingStory.nominalFloortoCeilingHeight().get());
        }
        if (buildingStory.nominalFloortoFloorHeight()){
          // DLM: how to translate this?
        }
        if (buildingStory.renderingColor()){
          storyMetaData.setColor(buildingStory.renderingColor()->colorString());
        }
        modelObjectMetadata.push_back(storyMetaData);

        for (const auto& space : buildingStory.spaces()){
          ThreeModelObjectMetadata spaceMetaData(space.iddObjectType().valueDescription(), toString(space.handle()), space.nameString());
          // multiplier?
          // open to below?
          modelObjectMetadata.push_back(spaceMetaData);
        }

        n += 1;
        updatePercentage(100.0*n / N);
      }
      std::sort(buildingStoryNames.begin(), buildingStoryNames.end(), IstringCompare());

      for (const auto& buildingUnit : buildingUnits){

        ThreeModelObjectMetadata unitMetaData(buildingUnit.iddObjectType().valueDescription(), toString(buildingUnit.handle()), buildingUnit.nameString());
        if (buildingUnit.renderingColor()){
          unitMetaData.setColor(buildingUnit.renderingColor()->colorString());
        }
        modelObjectMetadata.push_back(unitMetaData);

        n += 1;
        updatePercentage(100.0*n / N);
      }

      for (const auto& thermalZone : thermalZones){
        ThreeModelObjectMetadata zoneMetaData(thermalZone.iddObjectType().valueDescription(), toString(thermalZone.handle()), thermalZone.nameString());
        if (thermalZone.renderingColor()){
          zoneMetaData.setColor(thermalZone.renderingColor()->colorString());
        }
        modelObjectMetadata.push_back(zoneMetaData);

        n += 1;
        updatePercentage(100.0*n / N);
      }

      for (const auto& spaceType : spaceTypes){
        ThreeModelObjectMetadata spaceTypeMetaData(spaceType.iddObjectType().valueDescription(), toString(spaceType.handle()), spaceType.nameString());
        if (spaceType.renderingColor()){
          spaceTypeMetaData.setColor(spaceType.renderingColor()->colorString());
        }
        modelObjectMetadata.push_back(spaceTypeMetaData);

        n += 1;
        updatePercentage(100.0*n / N);
      }

      for (const auto& defaultConstructionSet : defaultConstructionSets){
        ThreeModelObjectMetadata setMetaData(defaultConstructionSet.iddObjectType().valueDescription(), toString(defaultConstructionSet.handle()), defaultConstructionSet.nameString());
        modelObjectMetadata.push_back(setMetaData);

        n += 1;
        updatePercentage(100.0*n / N);
      }

      double northAxis = 0.0;
      boost::optional<Building> building = model.getOptionalUniqueModelObject<Building>();
      if (building) {
        northAxis = -building->northAxis();
      }

      ThreeSceneMetadata metadata(buildingStoryNames, threeBoundingBox, northAxis, modelObjectMetadata);

      ThreeScene scene(metadata, allGeometries, materials, sceneObject);

      updatePercentage(100.0);

      return scene;
    }

  }//model
}//openstudio
