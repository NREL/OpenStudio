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

    unsigned ThreeJSForwardTranslator::openstudioFaceFormatId()
    {
      return 1024;
    }
    
    ThreeMaterial makeMaterial(const std::string& name, unsigned color, double opacity, unsigned side, unsigned shininess = 50, const std::string type = "MeshPhongMaterial")
    {
      bool transparent = false;
      if (opacity < 1){
        transparent = true;
      }

      ThreeMaterial result(toThreeUUID(toString(openstudio::createUUID())), name, type,
        color, color, toThreeColor(0, 0, 0), color, shininess, opacity, transparent, false, side);

      return result;
    }

    void addMaterial(std::vector<ThreeMaterial>& materials, std::map<std::string, std::string>& materialMap, const ThreeMaterial& material)
    {
      materialMap[material.name()] = material.uuid();
      materials.push_back(material);
    }

    std::string getMaterialId(const std::string& materialName, std::map<std::string, std::string>& materialMap)
    {
      std::map<std::string, std::string>::const_iterator it = materialMap.find(materialName);
      if (it != materialMap.end()){
        return it->second;
      }

      it = materialMap.find("Undefined");
      if (it != materialMap.end()){
        return it->second;
      }
      OS_ASSERT(false);
      return "";
    }

    void buildMaterials(Model model, std::vector<ThreeMaterial>& materials, std::map<std::string, std::string>& materialMap)
    {
      // materials from 'openstudio\openstudiocore\ruby\openstudio\sketchup_plugin\lib\interfaces\MaterialsInterface.rb' 

      //addMaterial(materials, materialMap, makeMaterial("Undefined", toThreeColor(255, 255, 255), 1, ThreeSide::DoubleSide, 50, "MeshBasicMaterial"));
      addMaterial(materials, materialMap, makeMaterial("Undefined", toThreeColor(255, 255, 255), 1, ThreeSide::DoubleSide));

      addMaterial(materials, materialMap, makeMaterial("NormalMaterial", toThreeColor(255, 255, 255), 1, ThreeSide::DoubleSide));
      //addMaterial(materials, materialMap, makeMaterial("NormalMaterial_Ext", toThreeColor(255, 255, 255), 1, ThreeSide::FrontSide, 50, "MeshBasicMaterial"));
      addMaterial(materials, materialMap, makeMaterial("NormalMaterial_Ext", toThreeColor(255, 255, 255), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("NormalMaterial_Int", toThreeColor(255, 0, 0), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("Floor", toThreeColor(128, 128, 128), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Floor_Ext", toThreeColor(128, 128, 128), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("Floor_Int", toThreeColor(191, 191, 191), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("Wall", toThreeColor(204, 178, 102), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Wall_Ext", toThreeColor(204, 178, 102), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("Wall_Int", toThreeColor(235, 226, 197), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("RoofCeiling", toThreeColor(153, 76, 76), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("RoofCeiling_Ext", toThreeColor(153, 76, 76), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("RoofCeiling_Int", toThreeColor(202, 149, 149), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("Window", toThreeColor(102, 178, 204), 0.6, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Window_Ext", toThreeColor(102, 178, 204), 0.6, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("Window_Int", toThreeColor(192, 226, 235), 0.6, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("Door", toThreeColor(153, 133, 76), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Door_Ext", toThreeColor(153, 133, 76), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("Door_Int", toThreeColor(202, 188, 149), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("SiteShading", toThreeColor(75, 124, 149), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("SiteShading_Ext", toThreeColor(75, 124, 149), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("SiteShading_Int", toThreeColor(187, 209, 220), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("BuildingShading", toThreeColor(113, 76, 153), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("BuildingShading_Ext", toThreeColor(113, 76, 153), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("BuildingShading_Int", toThreeColor(216, 203, 229), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("SpaceShading", toThreeColor(76, 110, 178), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("SpaceShading_Ext", toThreeColor(76, 110, 178), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("SpaceShading_Int", toThreeColor(183, 197, 224), 1, ThreeSide::BackSide));
      
      addMaterial(materials, materialMap, makeMaterial("InteriorPartitionSurface", toThreeColor(158, 188, 143), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("InteriorPartitionSurface_Ext", toThreeColor(158, 188, 143), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("InteriorPartitionSurface_Int", toThreeColor(213, 226, 207), 1, ThreeSide::BackSide));

      // start textures for boundary conditions
      addMaterial(materials, materialMap, makeMaterial("Boundary_Surface", toThreeColor(0, 153, 0), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Adiabatic", toThreeColor(255, 0, 0), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Space", toThreeColor(255, 0, 0), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Outdoors", toThreeColor(163, 204, 204), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Outdoors_Sun", toThreeColor(40, 204, 204), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Outdoors_Wind", toThreeColor(9, 159, 162), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Outdoors_SunWind", toThreeColor(68, 119, 161), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Ground", toThreeColor(204, 183, 122), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundfcfactormethod", toThreeColor(153, 122, 30), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundslabpreprocessoraverage", toThreeColor(255, 191, 0), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundslabpreprocessorcore", toThreeColor(255, 182, 50), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundslabpreprocessorperimeter", toThreeColor(255, 178, 101), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundbasementpreprocessoraveragewall", toThreeColor(204, 51, 0), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundbasementpreprocessoraveragefloor", toThreeColor(204, 81, 40), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundbasementpreprocessorupperwall", toThreeColor(204, 112, 81), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundbasementpreprocessorlowerwall", toThreeColor(204, 173, 163), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Othersidecoefficients", toThreeColor(63, 63, 63), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Othersideconditionsmodel", toThreeColor(153, 0, 76), 1, ThreeSide::DoubleSide));
      
      // make construction materials
      for (auto& construction : model.getModelObjects<ConstructionBase>()){
        boost::optional<RenderingColor> color = construction.renderingColor();
        if (!color){
          color = RenderingColor(model);
          construction.setRenderingColor(*color);
        }
        std::string name = "Construction_" + construction.nameString();
        addMaterial(materials, materialMap, makeMaterial(name, toThreeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }
      
      // make thermal zone materials
      for (auto& thermalZone : model.getConcreteModelObjects<ThermalZone>()){
        boost::optional<RenderingColor> color = thermalZone.renderingColor();
        if (!color){
          color = RenderingColor(model);
          thermalZone.setRenderingColor(*color);
        }
        std::string name = "ThermalZone_" + thermalZone.nameString();
        addMaterial(materials, materialMap, makeMaterial(name, toThreeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }

      // make space type materials
      for (auto& spaceType : model.getConcreteModelObjects<SpaceType>()){
        boost::optional<RenderingColor> color = spaceType.renderingColor();
        if (!color){
          color = RenderingColor(model);
          spaceType.setRenderingColor(*color);
        }
        std::string name = "SpaceType_" + spaceType.nameString();
        addMaterial(materials, materialMap, makeMaterial(name, toThreeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }
      
      // make building story materials
      for (auto& buildingStory : model.getConcreteModelObjects<BuildingStory>()){
        boost::optional<RenderingColor> color = buildingStory.renderingColor();
        if (!color){
          color = RenderingColor(model);
          buildingStory.setRenderingColor(*color);
        }
        std::string name = "BuildingStory_" + buildingStory.nameString();
        addMaterial(materials, materialMap, makeMaterial(name, toThreeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }

      // make building unit materials
      for (auto& buildingUnit : model.getConcreteModelObjects<BuildingUnit>()){
        boost::optional<RenderingColor> color = buildingUnit.renderingColor();
        if (!color){
          color = RenderingColor(model);
          buildingUnit.setRenderingColor(*color);
        }
        std::string name = "BuildingUnit_" + buildingUnit.nameString();
        addMaterial(materials, materialMap, makeMaterial(name, toThreeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
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
        userData.setSurfaceType(surface->surfaceType());
        userData.setSurfaceTypeMaterialName(surface->surfaceType());
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
        userData.setSurfaceType(shadingSurfaceType + "Shading");
        userData.setSurfaceTypeMaterialName(shadingSurfaceType + "Shading");
        userData.setSunExposure("SunExposed");
        userData.setWindExposure("WindExposed");
        userData.setOutsideBoundaryCondition("");
        userData.setOutsideBoundaryConditionObjectName("");
        userData.setOutsideBoundaryConditionObjectHandle("");
        userData.setBoundaryMaterialName("");
      }

      if (interiorPartitionSurface)
      {
        userData.setSurfaceType("InteriorPartitionSurface");
        userData.setSurfaceTypeMaterialName("InteriorPartitionSurface");
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
        std::string subSurfaceTypeMaterialName;
        if (istringEqual(subSurfaceType, "FixedWindow") ||
            istringEqual(subSurfaceType, "OperableWindow") ||
            istringEqual(subSurfaceType, "GlassDoor") || 
            istringEqual(subSurfaceType, "Skylight") ||
            istringEqual(subSurfaceType, "TubularDaylightDome") ||
            istringEqual(subSurfaceType, "TubularDaylightDiffuser"))
        {
          subSurfaceTypeMaterialName = "Window";
        } else if (istringEqual(subSurfaceType, "Door") || 
                  istringEqual(subSurfaceType, "OverheadDoor"))
        {
          subSurfaceTypeMaterialName = "Door";
        }

        userData.setSurfaceType(subSurfaceType);
        userData.setSurfaceTypeMaterialName(subSurfaceTypeMaterialName);

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
        userData.setConstructionMaterialName("Construction_" + construction->nameString());
      }
    
      if (space)
      {
        userData.setSpaceName(space->nameString());

        boost::optional<ThermalZone> thermalZone = space->thermalZone();
        if (thermalZone){
          userData.setThermalZoneName(thermalZone->nameString());
          userData.setThermalZoneMaterialName("ThermalZone_" + thermalZone->nameString());
        }

        boost::optional<SpaceType> spaceType = space->spaceType();
        if (spaceType){
          userData.setSpaceTypeName(spaceType->nameString());
          userData.setSpaceTypeMaterialName("SpaceType_" + spaceType->nameString());
        }

        boost::optional<BuildingStory> buildingStory = space->buildingStory();
        if (buildingStory){
          userData.setBuildingStoryName(buildingStory->nameString());
          userData.setBuildingStoryMaterialName("BuildingStory_" + buildingStory->nameString());
        }

        boost::optional<BuildingUnit> buildingUnit = space->buildingUnit();
        if (buildingUnit){
          userData.setBuildingUnitName(buildingUnit->nameString());
          userData.setBuildingUnitMaterialName("BuildingUnit_" + buildingUnit->nameString());
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
          faceIndices.push_back(ThreeJSForwardTranslator::openstudioFaceFormatId());
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
      m_logSink.setThreadId(QThread::currentThread());  
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
      m_logSink.setThreadId(QThread::currentThread());
      m_logSink.resetStringStream();

      updatePercentage(0.0);

      std::vector<ThreeMaterial> materials;
      std::map<std::string, std::string> materialMap;
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
      double N = planarSurfaces.size() + planarSurfaceGroups.size() + buildingStories.size() + buildingUnits.size() + thermalZones.size() + spaceTypes.size() + defaultConstructionSets.size() + 1;

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
         std::string thisMaterialId = getMaterialId(userDatas[i].surfaceTypeMaterialName(), materialMap);

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

        modelObjectMetadata.push_back(ThreeModelObjectMetadata("OS:BuildingStory", toString(buildingStory.handle()), buildingStory.nameString()));

        for (const auto& space : buildingStory.spaces()){
          modelObjectMetadata.push_back(ThreeModelObjectMetadata("OS:Space", toString(space.handle()), space.nameString()));
        }

        n += 1;
        updatePercentage(100.0*n / N);
      }
      std::sort(buildingStoryNames.begin(), buildingStoryNames.end(), IstringCompare());

      for (const auto& buildingUnit : buildingUnits){
        modelObjectMetadata.push_back(ThreeModelObjectMetadata("OS:BuildingUnit", toString(buildingUnit.handle()), buildingUnit.nameString()));
        
        n += 1;
        updatePercentage(100.0*n / N);
      }

      for (const auto& thermalZone : thermalZones){
        modelObjectMetadata.push_back(ThreeModelObjectMetadata("OS:ThermalZone", toString(thermalZone.handle()), thermalZone.nameString()));
        
        n += 1;
        updatePercentage(100.0*n / N);
      }

      for (const auto& spaceType : spaceTypes){
        modelObjectMetadata.push_back(ThreeModelObjectMetadata("OS:SpaceType", toString(spaceType.handle()), spaceType.nameString()));
        
        n += 1;
        updatePercentage(100.0*n / N);
      }

      for (const auto& defaultConstructionSet : defaultConstructionSets){
        modelObjectMetadata.push_back(ThreeModelObjectMetadata("OS:DefaultConstructionSet", toString(defaultConstructionSet.handle()), defaultConstructionSet.nameString()));
        
        n += 1;
        updatePercentage(100.0*n / N);
      }
  
      ThreeSceneMetadata metadata(buildingStoryNames, threeBoundingBox, modelObjectMetadata);
  
      ThreeScene scene(metadata, allGeometries, materials, sceneObject);

      updatePercentage(100.0);

      return scene;
    }
    
  }//model
}//openstudio
