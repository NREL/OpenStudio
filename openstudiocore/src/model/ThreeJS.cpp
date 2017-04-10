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

#include "ThreeJS.hpp"

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

#include "../utilities/core/Assert.hpp"

namespace openstudio
{
  namespace model
  {
    
    ThreeMaterial makeMaterial(const std::string& name, unsigned color, double opacity, unsigned side, unsigned shininess = 50, const std::string type = "MeshPhongMaterial")
    {
      bool transparent = false;
      if (opacity < 1){
        transparent = true;
      }

      ThreeMaterial result(threeUUID(toString(openstudio::createUUID())), name, type,
        color, color, threeColor(0, 0, 0), color, shininess, opacity, transparent, false, side);

      return result;
    }

    void addMaterial(std::vector<ThreeMaterial>& materials, std::map<std::string, std::size_t>& materialMap, const ThreeMaterial& material)
    {
      materialMap[material.name()] = materials.size();
      materials.push_back(material);
    }

    void buildMaterials(Model model, std::vector<ThreeMaterial>& materials, std::map<std::string, std::size_t>& materialMap)
    {
      // materials from 'openstudio\openstudiocore\ruby\openstudio\sketchup_plugin\lib\interfaces\MaterialsInterface.rb' 

      addMaterial(materials, materialMap, makeMaterial("Undefined", threeColor(255, 255, 255), 1, ThreeSide::DoubleSide, 50, "MeshBasicMaterial"));

      addMaterial(materials, materialMap, makeMaterial("NormalMaterial", threeColor(255, 255, 255), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("NormalMaterial_Ext", threeColor(255, 255, 255), 1, ThreeSide::FrontSide, 50, "MeshBasicMaterial"));
      addMaterial(materials, materialMap, makeMaterial("NormalMaterial_Int", threeColor(255, 0, 0), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("Floor", threeColor(128, 128, 128), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Floor_Ext", threeColor(128, 128, 128), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("Floor_Int", threeColor(191, 191, 191), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("Wall", threeColor(204, 178, 102), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Wall_Ext", threeColor(204, 178, 102), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("Wall_Int", threeColor(235, 226, 197), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("RoofCeiling", threeColor(153, 76, 76), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("RoofCeiling_Ext", threeColor(153, 76, 76), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("RoofCeiling_Int", threeColor(202, 149, 149), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("Window", threeColor(102, 178, 204), 0.6, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Window_Ext", threeColor(102, 178, 204), 0.6, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("Window_Int", threeColor(192, 226, 235), 0.6, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("Door", threeColor(153, 133, 76), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Door_Ext", threeColor(153, 133, 76), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("Door_Int", threeColor(202, 188, 149), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("SiteShading", threeColor(75, 124, 149), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("SiteShading_Ext", threeColor(75, 124, 149), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("SiteShading_Int", threeColor(187, 209, 220), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("BuildingShading", threeColor(113, 76, 153), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("BuildingShading_Ext", threeColor(113, 76, 153), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("BuildingShading_Int", threeColor(216, 203, 229), 1, ThreeSide::BackSide));

      addMaterial(materials, materialMap, makeMaterial("SpaceShading", threeColor(76, 110, 178), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("SpaceShading_Ext", threeColor(76, 110, 178), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("SpaceShading_Int", threeColor(183, 197, 224), 1, ThreeSide::BackSide));
      
      addMaterial(materials, materialMap, makeMaterial("InteriorPartitionSurface", threeColor(158, 188, 143), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("InteriorPartitionSurface_Ext", threeColor(158, 188, 143), 1, ThreeSide::FrontSide));
      addMaterial(materials, materialMap, makeMaterial("InteriorPartitionSurface_Int", threeColor(213, 226, 207), 1, ThreeSide::BackSide));

      // start textures for boundary conditions
      addMaterial(materials, materialMap, makeMaterial("Boundary_Surface", threeColor(0, 153, 0), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Adiabatic", threeColor(255, 0, 0), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Space", threeColor(255, 0, 0), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Outdoors", threeColor(163, 204, 204), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Outdoors_Sun", threeColor(40, 204, 204), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Outdoors_Wind", threeColor(9, 159, 162), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Outdoors_SunWind", threeColor(68, 119, 161), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Ground", threeColor(204, 183, 122), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundfcfactormethod", threeColor(153, 122, 30), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundslabpreprocessoraverage", threeColor(255, 191, 0), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundslabpreprocessorcore", threeColor(255, 182, 50), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundslabpreprocessorperimeter", threeColor(255, 178, 101), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundbasementpreprocessoraveragewall", threeColor(204, 51, 0), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundbasementpreprocessoraveragefloor", threeColor(204, 81, 40), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundbasementpreprocessorupperwall", threeColor(204, 112, 81), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Groundbasementpreprocessorlowerwall", threeColor(204, 173, 163), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Othersidecoefficients", threeColor(63, 63, 63), 1, ThreeSide::DoubleSide));
      addMaterial(materials, materialMap, makeMaterial("Boundary_Othersideconditionsmodel", threeColor(153, 0, 76), 1, ThreeSide::DoubleSide));
      
      // make construction materials
      for (auto& construction : model.getModelObjects<ConstructionBase>()){
        boost::optional<RenderingColor> color = construction.renderingColor();
        if (!color){
          color = RenderingColor(model);
          construction.setRenderingColor(*color);
        }
        std::string name = "Construction_" + construction.nameString();
        addMaterial(materials, materialMap, makeMaterial(name, threeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }
      
      // make thermal zone materials
      for (auto& thermalZone : model.getConcreteModelObjects<ThermalZone>()){
        boost::optional<RenderingColor> color = thermalZone.renderingColor();
        if (!color){
          color = RenderingColor(model);
          thermalZone.setRenderingColor(*color);
        }
        std::string name = "ThermalZone_" + thermalZone.nameString();
        addMaterial(materials, materialMap, makeMaterial(name, threeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }

      // make space type materials
      for (auto& spaceType : model.getConcreteModelObjects<SpaceType>()){
        boost::optional<RenderingColor> color = spaceType.renderingColor();
        if (!color){
          color = RenderingColor(model);
          spaceType.setRenderingColor(*color);
        }
        std::string name = "SpaceType_" + spaceType.nameString();
        addMaterial(materials, materialMap, makeMaterial(name, threeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }
      
      // make building story materials
      for (auto& buildingStory : model.getConcreteModelObjects<BuildingStory>()){
        boost::optional<RenderingColor> color = buildingStory.renderingColor();
        if (!color){
          color = RenderingColor(model);
          buildingStory.setRenderingColor(*color);
        }
        std::string name = "BuildingStory_" + buildingStory.nameString();
        addMaterial(materials, materialMap, makeMaterial(name, threeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }

      // make building unit materials
      for (auto& buildingUnit : model.getConcreteModelObjects<BuildingUnit>()){
        boost::optional<RenderingColor> color = buildingUnit.renderingColor();
        if (!color){
          color = RenderingColor(model);
          buildingUnit.setRenderingColor(*color);
        }
        std::string name = "BuildingUnit_" + buildingUnit.nameString();
        addMaterial(materials, materialMap, makeMaterial(name, threeColor(color->renderingRedValue(), color->renderingBlueValue(), color->renderingGreenValue()), 1, ThreeSide::DoubleSide));
      }
    
    }

    /*
    std::pair<std::vector<ThreeGeometry>, std::vector<ThreeUserData> > makeGeometries(const PlanarSurface& surface)
    {
      std::pair<std::vector<ThreeGeometry>, std::vector<ThreeUserData> > result;

      return result;
    }
    */

    ThreeScene modelToThreeJS(Model model, bool triangulateSurfaces)
    {
      std::vector<ThreeMaterial> materials;
      std::map<std::string, std::size_t> materialMap;
      buildMaterials(model, materials, materialMap);

     /*
    object = Hash.new
    object[:uuid] = format_uuid(OpenStudio::createUUID)
    object[:type] = 'Scene'
    object[:matrix] = identity_matrix
    object[:children] = []
    
    floor_material = materials.find {|m| m[:name] == 'Floor'}
    wall_material = materials.find {|m| m[:name] == 'Wall'}
    roof_material = materials.find {|m| m[:name] == 'RoofCeiling'}
    window_material = materials.find {|m| m[:name] == 'Window'}
    door_material = materials.find {|m| m[:name] == 'Door'}
    site_shading_material = materials.find {|m| m[:name] == 'SiteShading'}
    building_shading_material = materials.find {|m| m[:name] == 'BuildingShading'}
    space_shading_material = materials.find {|m| m[:name] == 'SpaceShading'}
    interior_partition_surface_material = materials.find {|m| m[:name] == 'InteriorPartitionSurface'}
    
    # loop over all surfaces
    all_geometries = []
    model.getSurfaces.each do |surface|

      material = nil
      surfaceType = surface.surfaceType.upcase
      if surfaceType == 'FLOOR'
        material = floor_material
      elsif surfaceType == 'WALL'
        material = wall_material
      elsif surfaceType == 'ROOFCEILING'
        material = roof_material  
      end
  
      geometries, user_datas = make_geometries(surface)
      if geometries
        geometries.each_index do |i| 
          geometry = geometries[i]
          user_data = user_datas[i]
          
          all_geometries << geometry

          scene_child = SceneChild.new
          scene_child.uuid = format_uuid(OpenStudio::createUUID) 
          scene_child.name = user_data[:name]
          scene_child.type = "Mesh"
          scene_child.geometry = geometry[:uuid]

          if i == 0
            # first geometry is base surface
            scene_child.material = material[:uuid]
          else
            # sub surface
            if /Window/.match(user_data[:surfaceType]) || /Glass/.match(user_data[:surfaceType]) 
              scene_child.material =  window_material[:uuid]
            else
              scene_child.material =  door_material[:uuid]
            end
          end
          
          scene_child.matrix = identity_matrix
          scene_child.userData = user_data
          object[:children] << scene_child.to_h
        end
      end
    end
    
    # loop over all shading surfaces
    model.getShadingSurfaces.each do |surface|
  
      geometries, user_datas = make_shade_geometries(surface)
      if geometries
        geometries.each_index do |i| 
          geometry = geometries[i]
          user_data = user_datas[i]
          
          material = nil
          if /Site/.match(user_data[:surfaceType])
            material = site_shading_material
          elsif /Building/.match(user_data[:surfaceType]) 
            material = building_shading_material
          elsif /Space/.match(user_data[:surfaceType]) 
            material = space_shading_material
          end
          
          all_geometries << geometry

          scene_child = SceneChild.new
          scene_child.uuid = format_uuid(OpenStudio::createUUID) 
          scene_child.name = user_data[:name]
          scene_child.type = 'Mesh'
          scene_child.geometry = geometry[:uuid]
          scene_child.material = material[:uuid]
          scene_child.matrix = identity_matrix
          scene_child.userData = user_data
          object[:children] << scene_child.to_h
        end
      end
    end    
    
    # loop over all interior partition surfaces
    model.getInteriorPartitionSurfaces.each do |surface|
  
      geometries, user_datas = make_interior_partition_geometries(surface)
      geometries.each_index do |i| 
        geometry = geometries[i]
        user_data = user_datas[i]
        
        material = interior_partition_surface_material

        all_geometries << geometry

        scene_child = SceneChild.new
        scene_child.uuid = format_uuid(OpenStudio::createUUID) 
        scene_child.name = user_data[:name]
        scene_child.type = 'Mesh'
        scene_child.geometry = geometry[:uuid]
        scene_child.material = material[:uuid]
        scene_child.matrix = identity_matrix
        scene_child.userData = user_data
        object[:children] << scene_child.to_h
      end
      
    end    
    
    #light = AmbientLight.new
    #light.uuid = "#{format_uuid(OpenStudio::createUUID)}"
    #light.type = "AmbientLight"
    #light.color = "0xFFFFFF".hex
    #light.matrix = identity_matrix
    #object[:children] << light.to_h
      
    scene = Scene.new
    scene.geometries = all_geometries
    scene.materials = materials
    scene.object = object

    return scene
    */

      std::vector<ThreeGeometry> geometries;
      ThreeSceneObject sceneObject("");
      return ThreeScene(geometries, materials, sceneObject);
    }
    
    boost::optional<Model> modelFromThreeJS(const ThreeScene& scene)
    {
      return boost::none;
    }
    
  }//model
}//openstudio
