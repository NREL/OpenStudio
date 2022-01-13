/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "GltfForwardTranslator.hpp"

#include "RenderingColor.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "ConstructionAirBoundary.hpp"
#include "ConstructionAirBoundary_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
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
#include "../utilities/geometry/Polygon3d.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

#include <thread>

#include <cmath>
#include <stack>

namespace openstudio {
namespace model {

  namespace GLTF = tinygltf;
  using namespace GLTF;

  /// <summary>
  /// For raw values for GLTF Materials
  /// </summary>
  struct mat_Data
  {
    std::string matName;
    int r;
    int g;
    int b;
    double a;
    bool isDoubleSided = false;
  };

  /// <summary>
  /// For Indices of Indices, Coordinates & Normal buffers
  /// against each Components
  /// </summary>
  struct ShapeComponentIds
  {
    int IndicesAccessorId;
    int VerticesAccessorId;
    int NormalsAccessorId;
  };

  /// <summary>
  /// Creates and returns a GLTF material
  /// </summary>
  /// <param name="name"></param>
  /// <param name="red"></param>
  /// <param name="green"></param>
  /// <param name="blue"></param>
  /// <param name="alpha"></param>
  /// <param name="isDoubleSided"></param>
  /// <returns>GLTF Material</returns>
  GLTF::Material CreateMaterial(const std::string& name, float red, float green, float blue, float alpha, bool isDoubleSided) {

    GLTF::Material m;
    m.name = name;
    PbrMetallicRoughness pbr;
    pbr.baseColorFactor = {red, green, blue, alpha};
    pbr.metallicFactor = 0.1;
    pbr.roughnessFactor = 0.5;
    m.pbrMetallicRoughness = pbr;
    std::vector<double> eF{0, 0, 0};
    m.emissiveFactor = eF;
    m.alphaMode = (alpha < 1.0f) ? "BLEND" : "OPAQUE";
    m.alphaCutoff = 0.5f;
    m.doubleSided = isDoubleSided;

    return m;
  }

  /// <summary>
  /// Initializes the Default material for the GLTF Model materials
  /// </summary>
  /// <param name="_materials"></param>
  void initMaterials(std::vector<GLTF::Material>& _materials) {
    // default material is always set at index 0;
    const float grey = 0.8f;
    const float alpha = 1.0f;
    GLTF::Material m = CreateMaterial("Default material", grey, grey, grey, alpha, true);
    _materials.push_back(m);
  }

  /// <summary>
  /// Initializes the SCENE for the GLTF model that will be
  /// aware of all other nodes, meshes, materials, BufferViews, Accessors tightly
  /// tied up in a Heirarchy using indices pointing one to other.
  /// </summary>
  /// <param name="_scene"></param>
  /// <param name="_topNode"></param>
  /// <param name="_nodes"></param>
  void initScene(GLTF::Scene& _scene, GLTF::Node& _topNode, std::vector<GLTF::Node>& _nodes) {
    std::vector<int> ns{0};
    _scene.nodes = ns;

    _topNode.name = "Z_UP";
    std::vector<double> mtrx{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    _topNode.matrix = mtrx;

    _nodes.push_back(_topNode);
  }

  /// <summary>
  /// Initializes our two main Buffer Views
  /// one for the indices & other for Coordinates and Normals
  /// </summary>
  /// <param name="_indicesBv"></param>
  /// <param name="_coordinatesBv"></param>
  /// <param name="_bufferViews"></param>
  void initBufferViews(GLTF::BufferView& _indicesBv, GLTF::BufferView& _coordinatesBv, std::vector<GLTF::BufferView>& _bufferViews) {
    GLTF::BufferView bv_indices;
    bv_indices.buffer = 0;
    //bv_indices.byteStride = 4; //This is not required
    bv_indices.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
    _indicesBv = bv_indices;
    //_bufferViews.push_back(_indicesBv);

    // The vertices take up 36 bytes (3 vertices * 3 floating points * 4 bytes)
    // at position 8 in the buffer and are of type ARRAY_BUFFER
    GLTF::BufferView bv_coordinates;
    bv_coordinates.buffer = 0;
    bv_coordinates.target = TINYGLTF_TARGET_ARRAY_BUFFER;

    bv_coordinates.byteStride = 12;
    //bv_coordinates.byteLength = 36;
    _coordinatesBv = bv_coordinates;
    //_bufferViews.push_back(_coordinatesBv);
  }

  /// <summary>
  /// Creates a GLTF material on the basis of raw Material Values
  /// i.e, R, G, B, A & isDoubleSided
  /// and adds to GLTF model's materials
  /// </summary>
  /// <param name="_materials"></param>
  /// <param name="mat_Data"></param>
  /// <returns>index of the created GLTF Material</returns>
  int CreateMaterial(std::vector<GLTF::Material>& _materials, mat_Data mat_Data) {
    GLTF::Material m;
    m.name = mat_Data.matName;
    GLTF::PbrMetallicRoughness pbr;
    //this contains the red, green, blue and alpha components of the main color of the material
    pbr.baseColorFactor = {(float)(mat_Data.r / 255.0), (float)(mat_Data.g / 255.0), (float)(mat_Data.b / 255.0), (float)(mat_Data.a)};
    //this indicates that the material should have very minimum reflection characteristics
    //between that of a metal and non-metal material.
    pbr.metallicFactor = 0.1;
    //this makes the material to not be perfectly mirror-like, but instead scatter the
    //reflected light a bit.
    pbr.roughnessFactor = 0.5;
    m.pbrMetallicRoughness = pbr;

    //emissive texture describes the parts of the object surface that emit light with a
    //a certain color
    m.emissiveFactor = {0.01, 0.01, 0.01};
    //occlusion texture can be used to simulate the effect of objects self-shadowing
    //each other.
    //m.occlusionTexture = "";
    //normal map is a texture applied to modulate the surface normal in a way that makes
    // it possible to simulate finer geometric details without the cost of a higher mesh.
    //m.normalTexture = "";
    m.alphaMode = (mat_Data.a < 1.0f) ? "BLEND" : "OPAQUE";
    //alphaCutoff should not be 1.0f else MATERIAL_ALPHA_CUTOFF_INVALID_MODE	Alpha cutoff is supported only for 'MASK' alpha mode
    m.alphaCutoff = 0.5f;
    m.doubleSided = mat_Data.isDoubleSided;

    int ret = _materials.size();
    _materials.push_back(m);
    return ret;
  }

  /// <summary>
  /// Creates a collection of raw (values: R,G,B,A & isDoubleSided)
  /// for all pre-defined GLTF material
  /// </summary>
  /// <returns>collection of raw material Values </returns>
  std::vector<mat_Data> GetAllMaterials() {
    std::vector<mat_Data> result;

    result.push_back({"Undefined", 255, 255, 255, 1, true});

    result.push_back({"NormalMaterial", 255, 255, 255, 1, true});
    result.push_back({"NormalMaterial_Ext", 255, 255, 255, 1});
    result.push_back({"NormalMaterial_Int", 255, 0, 0, 1});

    //result.push_back({"Floor", 128, 128, 128, 1, true});
    result.push_back({"Floor", 140, 140, 140, 1, true});
    result.push_back({"Floor_Ext", 128, 128, 128, 1});
    result.push_back({"Floor_Int", 191, 191, 191, 1});

    //result.push_back({"Wall", 204, 178, 102, 1, true});
    result.push_back({"Wall", 150, 131, 75, 1, true});
    result.push_back({"Wall_Ext", 204, 178, 102, 1});
    result.push_back({"Wall_Int", 235, 226, 197, 1});

    //result.push_back({"RoofCeiling", 153, 76, 76, 1, true});
    result.push_back({"RoofCeiling", 112, 56, 57, 1, true});
    result.push_back({"RoofCeiling_Ext", 153, 76, 76, 1});
    result.push_back({"RoofCeiling_Int", 202, 149, 149, 1});

    result.push_back({"Window", 102, 178, 204, 0.6, true});
    result.push_back({"Window_Ext", 102, 178, 204, 0.6});
    result.push_back({"Window_Int", 192, 226, 235, 0.6});

    //result.push_back({"Door", 153, 133, 76, 1, true});
    result.push_back({"Door", 111, 98, 56, 1, true});
    result.push_back({"Door_Ext", 153, 133, 76, 1});
    result.push_back({"Door_Int", 202, 188, 149, 1});

    //result.push_back({"SiteShading", 75, 124, 149, 1, true});
    result.push_back({"SiteShading", 55, 90, 109, 1, true});
    result.push_back({"SiteShading_Ext", 75, 124, 149, 1});
    result.push_back({"SiteShading_Int", 187, 209, 220, 1});

    //result.push_back({"BuildingShading", 113, 76, 153, 1, true});
    result.push_back({"BuildingShading", 83, 56, 111, 1, true});
    result.push_back({"BuildingShading_Ext", 113, 76, 153, 1});
    result.push_back({"BuildingShading_Int", 216, 203, 229, 1});

    //result.push_back({"SpaceShading", 76, 110, 178, 1, true});
    result.push_back({"SpaceShading", 55, 81, 130, 1, true});
    result.push_back({"SpaceShading_Ext", 76, 110, 178, 1});
    result.push_back({"SpaceShading_Int", 183, 197, 224, 1});

    //result.push_back({"InteriorPartitionSurface", 158, 188, 143, 1, true});
    result.push_back({"InteriorPartitionSurface", 117, 138, 105, 1, true});
    result.push_back({"InteriorPartitionSurface_Ext", 158, 188, 143, 1});
    result.push_back({"InteriorPartitionSurface_Int", 213, 226, 207, 1});

    // start textures for boundary conditions
    result.push_back({"Boundary_Surface", 0, 153, 0, 1, true});
    result.push_back({"Boundary_Adiabatic", 255, 0, 0, 1, true});
    result.push_back({"Boundary_Space", 255, 0, 0, 1, true});
    result.push_back({"Boundary_Outdoors", 163, 204, 204, 1, true});
    result.push_back({"Boundary_Outdoors_Sun", 40, 204, 204, 1, true});
    result.push_back({"Boundary_Outdoors_Wind", 9, 159, 162, 1, true});
    result.push_back({"Boundary_Outdoors_SunWind", 68, 119, 161, 1, true});
    result.push_back({"Boundary_Ground", 204, 183, 122, 1, true});
    result.push_back({"Boundary_Groundfcfactormethod", 153, 122, 30, 1, true});
    result.push_back({"Boundary_Groundslabpreprocessoraverage", 255, 191, 0, 1, true});
    result.push_back({"Boundary_Groundslabpreprocessorcore", 255, 182, 50, 1, true});
    result.push_back({"Boundary_Groundslabpreprocessorperimeter", 255, 178, 101, 1, true});
    result.push_back({"Boundary_Groundbasementpreprocessoraveragewall", 204, 51, 0, 1, true});
    result.push_back({"Boundary_Groundbasementpreprocessoraveragefloor", 204, 81, 40, 1, true});
    result.push_back({"Boundary_Groundbasementpreprocessorupperwall", 204, 112, 81, 1, true});
    result.push_back({"Boundary_Groundbasementpreprocessorlowerwall", 204, 173, 163, 1, true});
    result.push_back({"Boundary_Othersidecoefficients", 63, 63, 63, 1, true});
    result.push_back({"Boundary_Othersideconditionsmodel", 153, 0, 76, 1, true});

    // special rendering materials
    result.push_back({"SpaceType_Plenum", 192, 192, 192, 0.1, true});
    result.push_back({"ThermalZone_Plenum", 192, 192, 192, 0.1, true});

    // special rendering materials, these are components or textures in SketchUp
    result.push_back({"DaylightingControl", 102, 178, 204, 0.1, true});
    result.push_back({"AirWall", 102, 178, 204, 0.1, true});
    result.push_back({"SolarCollector", 255, 255, 255, 1, true});
    result.push_back({"Photovoltaic", 255, 255, 255, 0.1, true});

    return result;
  }

  /// <summary>
  /// Adds all pre-defined materials to GLTF Model materials
  /// </summary>
  /// <param name="materialList"></param>
  /// <param name="_materials"></param>
  void AddGLTFMaterials(std::map<std::string, int>& materialList, std::vector<GLTF::Material>& _materials) {
    std::vector<mat_Data> result = GetAllMaterials();
    for (auto& m : result) {
      if (materialList.find(m.matName) != materialList.end()) {
        std::cout << "Key found";
      } else {
        materialList[m.matName] = CreateMaterial(_materials, m);
      }
    }
    //int index = CreateMaterial(_materials, 255, 255, 255, 1, "Undefined");
  }

  /// <summary>
  /// Gets GLTF Material name on the basis of idd Object Type and Name
  /// </summary>
  /// <param name="iddObjectType"></param>
  /// <param name="name"></param>
  /// <returns>name of GLTF Material Name</returns>
  std::string getObjectGLTFMaterialName(const std::string& iddObjectType, const std::string& name) {
    std::string result;
    if (istringEqual(iddObjectType, "OS:Construction")) {
      result = "Construction_" + name;
    } else if (istringEqual(iddObjectType, "OS:ThermalZone")) {
      result = "ThermalZone_" + name;
    } else if (istringEqual(iddObjectType, "OS:AirLoopHVAC")) {
      result = "AirLoopHVAC_" + name;
    } else if (istringEqual(iddObjectType, "OS:SpaceType")) {
      result = "SpaceType_" + name;
    } else if (istringEqual(iddObjectType, "OS:BuildingStory")) {
      result = "BuildingStory_" + name;
    } else if (istringEqual(iddObjectType, "OS:BuildingUnit")) {
      result = "BuildingUnit_" + name;
    } else if (istringEqual(iddObjectType, "OS:Construction:AirBoundary")) {
      // This shouldn't happen
      LOG_FREE(Error, "getObjectGLTFMaterialName", "Didn't expect it would be called for '" << iddObjectType << "' (name = '" << name << "')");
      result = "ConstructionAirBoundary_" + name;
    } else {
      LOG_FREE(Error, "getObjectGLTFMaterialName", "Unknown iddObjectType '" << iddObjectType << "'");
    }
    return result;
  }

  /// <summary>
  /// Gets GLTF Material Name on the basis of Model Object
  /// </summary>
  /// <param name="object"></param>
  /// <returns>name of GLTF material Name</returns>
  std::string getObjectGLTFMaterialName(const ModelObject& object) {
    return getObjectGLTFMaterialName(object.iddObjectType().valueDescription(), object.nameString());
  }

  /// <summary>
  /// Adds Model Specific GLTF Material to all Materials Collection
  /// </summary>
  /// <param name="_materials"></param>
  /// <param name="color"></param>
  /// <param name="materialName"></param>
  /// <param name="isDoubleSided"></param>
  /// <param name="allMaterials"></param>
  void AddModelSpecificMaterialToCollection(std::vector<GLTF::Material>& _materials, boost::optional<RenderingColor>& color,
                                            std::string& materialName, bool isDoubleSided, std::vector<mat_Data>& allMaterials) {

    mat_Data material{
      materialName, color->renderingRedValue(), color->renderingGreenValue(), color->renderingBlueValue(), (double)color->renderingAlphaValue(),
      true};

    allMaterials.push_back(material);
  }

  /// <summary>
  /// Gets GLTF Material name on the basis of Suface Type
  /// </summary>
  /// <param name="surfaceType"></param>
  /// <returns>surface Type</returns>
  std::string getSurfaceTypeGLTFMaterialName(const std::string& surfaceType) {
    if (istringEqual(surfaceType, "FixedWindow") || istringEqual(surfaceType, "OperableWindow") || istringEqual(surfaceType, "GlassDoor")
        || istringEqual(surfaceType, "Skylight") || istringEqual(surfaceType, "TubularDaylightDome")
        || istringEqual(surfaceType, "TubularDaylightDiffuser")) {
      return "Window";
    } else if (istringEqual(surfaceType, "Door") || istringEqual(surfaceType, "OverheadDoor")) {
      return "Door";
    }
    return surfaceType;
  }

  /// <summary>
  /// Finds / Create GLTF Material depending upon MaterialName
  /// </summary>
  /// <param name="materialName"></param>
  /// <param name="materialList"></param>
  /// <param name="allMaterials"></param>
  /// <param name="_materials"></param>
  /// <returns>index of Material</returns>
  int getGLTFMaterialIndex(const std::string& materialName, std::map<std::string, int>& materialList, std::vector<mat_Data>& allMaterials,
                           std::vector<GLTF::Material>& _materials) {
    std::map<std::string, int>::const_iterator it = materialList.find(materialName);
    if (it != materialList.end()) {
      return it->second;
    } else {
      for (int i = 0; i < allMaterials.size(); i++) {
        if (allMaterials[i].matName == materialName) {
          //Create Material and Add to _materials
          int newMatIndex = CreateMaterial(_materials, allMaterials[i]);
          //Add to map list
          materialList[allMaterials[i].matName] = newMatIndex;
          //send back index
          //return newMatIndex;
          break;
        }
      }
      std::map<std::string, int>::const_iterator it = materialList.find(materialName);
      if (it != materialList.end()) {
        return it->second;
      }
      // ..
      it = materialList.find("Undefined");
      if (it != materialList.end()) {
        return it->second;
      }
    }
    return 0;
  }

  /// <summary>
  /// Now: Instead of creating materials for all the objects in Model
  /// we're adding them to the all materials collection and create them only if required
  /// while processing the surface i.e, in MAIN LOOP
  /// </summary>
  void buildMaterials(Model model, std::vector<GLTF::Material>& _materials, std::map<std::string, int>& materialList,
                      std::vector<mat_Data>& allMaterials) {
    std::string materialName;
    // make construction materials
    for (auto& construction : model.getModelObjects<ConstructionBase>()) {
      // If it's ConstructionAirBoundary, we'll later use the standard material "AirWall"
      if (!construction.optionalCast<ConstructionAirBoundary>()) {
        boost::optional<RenderingColor> color = construction.renderingColor();
        if (!color) {
          color = RenderingColor(model);
          construction.setRenderingColor(*color);
        }
        materialName = getObjectGLTFMaterialName(construction);
        if (materialList.find(materialName) != materialList.end()) {
          std::cout << "Key found";
        } else {
          // instead of creating the material whose node might get not used inshort to avoid
          // warning "UNUSED_OBJECT". we'll add the material to our allMaterial collection
          // and whie processing the surface if required we'll create and the node.
          //CreateMaterial(_materials, color, materialName, true);

          AddModelSpecificMaterialToCollection(_materials, color, materialName, true, allMaterials);
        }
      }
    }

    // make thermal zone materials
    for (auto& thermalZone : model.getConcreteModelObjects<ThermalZone>()) {
      boost::optional<RenderingColor> color = thermalZone.renderingColor();
      if (!color) {
        color = RenderingColor(model);
        thermalZone.setRenderingColor(*color);
      }
      materialName = getObjectGLTFMaterialName(thermalZone);
      if (materialList.find(materialName) != materialList.end()) {
        std::cout << "Key found";
      } else {
        AddModelSpecificMaterialToCollection(_materials, color, materialName, true, allMaterials);
      }
    }

    // make space type materials
    for (auto& spaceType : model.getConcreteModelObjects<SpaceType>()) {
      boost::optional<RenderingColor> color = spaceType.renderingColor();
      if (!color) {
        color = RenderingColor(model);
        spaceType.setRenderingColor(*color);
      }
      materialName = getObjectGLTFMaterialName(spaceType);
      if (materialList.find(materialName) != materialList.end()) {
        std::cout << "Key found";
      } else {
        AddModelSpecificMaterialToCollection(_materials, color, materialName, true, allMaterials);
      }
    }

    // make building story materials
    for (auto& buildingStory : model.getConcreteModelObjects<BuildingStory>()) {
      boost::optional<RenderingColor> color = buildingStory.renderingColor();
      if (!color) {
        color = RenderingColor(model);
        buildingStory.setRenderingColor(*color);
      }
      materialName = getObjectGLTFMaterialName(buildingStory);
      if (materialList.find(materialName) != materialList.end()) {
        std::cout << "Key found";
      } else {
        AddModelSpecificMaterialToCollection(_materials, color, materialName, true, allMaterials);
      }
    }

    // make building unit materials
    for (auto& buildingUnit : model.getConcreteModelObjects<BuildingUnit>()) {
      boost::optional<RenderingColor> color = buildingUnit.renderingColor();
      if (!color) {
        color = RenderingColor(model);
        buildingUnit.setRenderingColor(*color);
      }
      materialName = getObjectGLTFMaterialName(buildingUnit);
      if (materialList.find(materialName) != materialList.end()) {
        std::cout << "Key found";
      } else {
        AddModelSpecificMaterialToCollection(_materials, color, materialName, true, allMaterials);
      }
    }

    // make air loop HVAC materials
    for (auto& airLoopHVAC : model.getConcreteModelObjects<AirLoopHVAC>()) {
      materialName = getObjectGLTFMaterialName(airLoopHVAC);
      boost::optional<RenderingColor> color = RenderingColor(model);
      if (materialList.find(materialName) != materialList.end()) {
        std::cout << "Key found";
      } else {
        AddModelSpecificMaterialToCollection(_materials, color, materialName, true, allMaterials);
      }
    }
  }

  /// <summary>
  /// Finds the vertex Index from all Vertices of a planar Surface
  /// </summary>
  /// <param name="point3d"></param>
  /// <param name="allPoints"></param>
  /// <param name="tol"></param>
  /// <returns> index of the Vertex </returns>
  size_t getVertexIndexT(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol = 0.001) {
    size_t n = allPoints.size();
    for (size_t i = 0; i < n; ++i) {
      if (std::sqrt(std::pow(point3d.x() - allPoints[i].x(), 2) + std::pow(point3d.y() - allPoints[i].y(), 2)
                    + std::pow(point3d.z() - allPoints[i].z(), 2))
          < tol) {
        return i;
      }
    }
    allPoints.push_back(point3d);
    return (allPoints.size() - 1);
  }

  /// <summary>
  /// Get BYtes from an integer
  /// </summary>
  /// <param name="paramInt"></param>
  /// <returns>collection of Bytes</returns>
  std::vector<unsigned char> intToBytes(int paramInt) {
    std::vector<unsigned char> arrayOfByte(4);
    for (int i = 0; i < 4; i++)
      arrayOfByte[3 - i] = (paramInt >> (i * 8));
    return arrayOfByte;
  }

  /// <summary>
  /// Get Bytes from a value
  /// </summary>
  /// <typeparam name="T"></typeparam>
  /// <param name="value"></param>
  /// <returns>collection of bytes</returns>
  template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
  std::vector<uint8_t> splitValueToBytes(T const& value) {
    std::vector<uint8_t> bytes;

    for (size_t i = 0; i < sizeof(value); i++) {
      uint8_t byte = value >> (i * 8);
      bytes.insert(bytes.begin(), byte);
    }
    return bytes;
  }

  /// <summary>
  /// Adds & Creates Face Indices buffers and Accessors
  /// </summary>
  /// <param name="faceIndices"></param>
  /// <param name="_indicesBuffer"></param>
  /// <param name="_accessors"></param>
  /// <returns>index of the Face Indices</returns>
  int AddIndices(std::vector<size_t>& faceIndices, std::vector<unsigned char>& _indicesBuffer, std::vector<GLTF::Accessor>& _accessors) {
    //Value of INT_MAX is + 2147483647.
    //Value of INT_MIN is - 2147483648.
    int min = INT_MAX;
    int max = INT_MIN;
    int j = 0;
    for (auto& item : faceIndices) {
      int i = (int)item;
      min = std::min(i, min);
      max = std::max(i, max);
    }
    // TODO: This but seems to be just figuring out the best way to back the indices in
    // depenbding on the range of numbers
    int ct = TINYGLTF_COMPONENT_TYPE_BYTE;
    auto size = 0;

    if (max <= std::pow(2, 8)) {
      ct = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
      //size = sizeof(BYTE);
      size = sizeof(std::byte);
    } else if (max <= std::pow(2, 16)) {
      ct = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
      size = sizeof(short);
    } else {
      ct = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
      size = sizeof(int);
    }
    // the offset position needs to be a multiple of the size
    // (this is from a warning we received in beta testing)
    // so we inject some padding when needed
    auto padding = _indicesBuffer.size() % size;
    for (int i = 0; i < padding; i++) {
      _indicesBuffer.push_back(0x00);
    }

    GLTF::Accessor indAccessor;
    indAccessor.bufferView = 0;
    indAccessor.componentType = ct;
    indAccessor.byteOffset = _indicesBuffer.size();
    indAccessor.normalized = false;
    indAccessor.type = TINYGLTF_TYPE_SCALAR;
    indAccessor.count = faceIndices.size();
    indAccessor.minValues = {(double)min};
    indAccessor.maxValues = {(double)max};

    std::vector<unsigned char> indicesBufferData;
    for (auto index : faceIndices) {
      std::vector<unsigned char> arrayOfByte;
      if (ct == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
        arrayOfByte.push_back((unsigned char)index);
      } else if (ct == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
        arrayOfByte = splitValueToBytes(index);
      } else {
        arrayOfByte = splitValueToBytes(index);
      }
      indicesBufferData.insert(indicesBufferData.end(), arrayOfByte.begin(), arrayOfByte.end());
    }
    //May be add empty like we did in Triangle test
    /*if (indicesBufferData.size() % 8 != 0) {
      std::cout << "needs padding";
      auto padding = indicesBufferData.size() % size;
      for (int i = 0; i < padding; i++) {
        indicesBufferData.push_back((std::byte)0);
      }
    }*/
    if (indicesBufferData.size() < 4) {
      indicesBufferData.push_back(0x00);
    } else {
      //TODO: larger models are required to test & add padding in case of unsigned short,int,etc
    }
    _indicesBuffer.insert(_indicesBuffer.end(), indicesBufferData.begin(), indicesBufferData.end());

    const auto thisIndex = _accessors.size();
    _accessors.push_back(indAccessor);
    return thisIndex;
  }

  /// <summary>
  /// Creates Coordinates / Normal Buffers and Accessors.
  /// </summary>
  /// <param name="values"></param>
  /// <param name="_coordinatesBuffer"></param>
  /// <param name="_accessors"></param>
  /// <returns></returns>
  int CreateBuffers(std::vector<float>& values, std::vector<unsigned char>& _coordinatesBuffer, std::vector<GLTF::Accessor>& _accessors) {
    // Fixes ACCESSOR_TOTAL_OFFSET_ALIGNMENT	
    // Accessor's total byteOffset XXXX isn't a multiple of componentType length 4.
    auto _padding = _coordinatesBuffer.size() % 4;
    for (int i = 0; i < _padding; i++) {
      _coordinatesBuffer.push_back((unsigned)0);
    }
    int startingBufferPosition = _coordinatesBuffer.size();
    std::vector<float> min = {FLT_MAX, FLT_MAX, FLT_MAX};
    std::vector<float> max = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
    int i = 0;
    for (auto& value : values) {
      min[i] = std::min(value, min[i]);
      max[i] = std::max(value, max[i]);
      i++;
      if (i > 2) i = 0;
      std::vector<unsigned char> v;
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&value);
      for (size_t i = 0; i < sizeof(float); ++i)
        v.push_back(ptr[i]);
      _coordinatesBuffer.insert(_coordinatesBuffer.end(), v.begin(), v.end());
    }
    //to fix : offset 18 is not a multiple of Comonent Type lenght 4
    auto padding = _coordinatesBuffer.size() % 4;
    for (int i = 0; i < padding; i++) {
      _coordinatesBuffer.push_back((unsigned)0);
    }
    //convert min and max to double
    std::vector<double> min_d;
    std::vector<double> max_d;
    for (auto mn : min) {
      min_d.push_back((double)mn);
    }
    for (auto mx : max) {
      max_d.push_back((double)mx);
    }
    GLTF::Accessor coordAccessor;
    coordAccessor.bufferView = 1;
    coordAccessor.byteOffset = startingBufferPosition;
    coordAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    coordAccessor.normalized = false;
    coordAccessor.count = values.size() / 3;
    coordAccessor.type = TINYGLTF_TYPE_VEC3;
    coordAccessor.minValues = min_d;
    coordAccessor.maxValues = max_d;

    auto ret = _accessors.size();
    _accessors.push_back(coordAccessor);
    return ret;
  }

  /// <summary>
  /// Adds Coordinate Buffers for all vertices of the surface
  /// </summary>
  /// <param name="allVertices"></param>
  /// <param name="_coordinatesBuffer"></param>
  /// <param name="_accessors"></param>
  /// <returns>index for the Coordinates Buffer</returns>
  int AddCoordinates(Point3dVector& allVertices, std::vector<unsigned char>& _coordinatesBuffer, std::vector<GLTF::Accessor>& _accessors) {
    std::vector<float> values;
    float divider = 1;
    for (auto& loc : allVertices) {
      values.push_back((float)(loc.x() / divider));
      values.push_back((float)(loc.y() / divider));
      values.push_back((float)(loc.z() / divider));
    }
    return CreateBuffers(values, _coordinatesBuffer, _accessors);
  }

  /// <summary>
  /// Adds Normal Buffers for all normal Vectors
  /// </summary>
  /// <param name="normalVectors"></param>
  /// <param name="_coordinatesBuffer"></param>
  /// <param name="_accessors"></param>
  /// <returns>index for the Normals Buffer</returns>
  int AddNormals(Vector3dVector& normalVectors, std::vector<unsigned char>& _coordinatesBuffer, std::vector<GLTF::Accessor>& _accessors) {
    std::vector<float> values;
    float divider = 1;
    for (auto& vec : normalVectors) {
      values.push_back((float)(vec.x() / divider));
      values.push_back((float)(vec.y() / divider));
      values.push_back((float)(vec.z() / divider));
    }
    return CreateBuffers(values, _coordinatesBuffer, _accessors);
  }

  /// <summary>
  /// Returns material index depending upon the surface type.
  /// </summary>
  /// <param name="planarSurface"></param>
  /// <param name="_materials"></param>
  /// <param name="materialList"></param>
  /// <param name="allMaterials"></param>
  /// <returns>index of the Material</returns>
  int GetMaterialIndex(const PlanarSurface& planarSurface, std::vector<GLTF::Material>& _materials, std::map<std::string, int>& materialList,
                       std::vector<mat_Data>& allMaterials) {
    std::string surfaceType;
    std::string surfaceTypeGLTFMaterialName;
    if (auto surface = planarSurface.optionalCast<Surface>()) {
      surfaceType = surface->surfaceType();
    } else if (planarSurface.optionalCast<SubSurface>()) {
      surfaceType = planarSurface.cast<SubSurface>().subSurfaceType();
    } else if (planarSurface.optionalCast<InteriorPartitionSurface>()) {
      surfaceType = "InteriorPartitionSurface";
    } else if (planarSurface.optionalCast<ShadingSurface>()) {
      boost::optional<ShadingSurface> shadingSurface = planarSurface.optionalCast<ShadingSurface>();
      std::string shadingSurfaceType = "Building";
      if (shadingSurface->shadingSurfaceGroup()) {
        shadingSurfaceType = shadingSurface->shadingSurfaceGroup()->shadingSurfaceType();
      }
      surfaceType = shadingSurfaceType + "Shading";
    }
    surfaceTypeGLTFMaterialName = getSurfaceTypeGLTFMaterialName(surfaceType);
    return getGLTFMaterialIndex(surfaceTypeGLTFMaterialName, materialList, allMaterials, _materials);
  }

  /// <summary>
  /// Exports a gltf against a Model
  /// </summary>
  /// <param name="model"></param>
  /// <param name="outputPath"></param>
  /// <returns>exports a GLTF file against a Model</returns>
  bool GltfForwardTranslator::modelToGLTF(const Model& model, const path& outputPath) {
    return modelToGLTF(
      model, [](double percentage) {}, outputPath);
  }

  /// <summary>
  /// MAIN PIPELINE TO TRANSLATE OPENSTUDIO MODEL -> GLTF MODEL
  /// </summary>
  /// <param name="model"></param>
  /// <param name="updatePercentage"></param>
  /// <param name="outputPath"></param>
  /// <returns>exports GLTF file aginst the Model</returns>
  bool GltfForwardTranslator::modelToGLTF(const Model& model, std::function<void(double)> updatePercentage,
                                          const path& outputPath) {
    m_logSink.setThreadId(std::this_thread::get_id());
    m_logSink.resetStringStream();

    bool triangulateSurfaces = true; //we're always triangulating the surfaces to get the best possible output.
    bool BufferInBase64 = false;  //no *.bin file is involed | everything is integrated in the mail output gltf file only.

    GLTF::TinyGLTF loader;
    GLTF::Model gltf_Model;
    std::string err;
    std::string warning;
    path p;

    std::vector<GLTF::Accessor> _accessors;
    std::vector<GLTF::Material> _materials;
    std::vector<GLTF::Mesh> _meshes;
    std::vector<GLTF::Node> _nodes;
    std::vector<GLTF::BufferView> _bufferViews;

    GLTF::Scene _scene;
    GLTF::Buffer _buffer;
    GLTF::Node _topNode;

    GLTF::BufferView _indicesBv;
    GLTF::BufferView _coordinatesBv;
    std::vector<unsigned char> _indicesBuffer;
    std::vector<unsigned char> _coordinatesBuffer;

    GLTF::Asset _asset;

#pragma region INIT

#pragma region MATERIALS

    //initMaterials(_materials);

#pragma endregion MATERIALS

#pragma region SCENE

    initScene(_scene, _topNode, _nodes);

#pragma endregion SCENE

#pragma region BUFFERVIEWS

    initBufferViews(_indicesBv, _coordinatesBv, _bufferViews);

#pragma endregion BUFFERVIEWS

#pragma endregion INIT

    std::map<std::string, int> materialList;
    std::map<std::string, int> materialIndicesUsed;
    //AddGLTFMaterials(materialList, _materials);
    std::vector<mat_Data> allMaterials = GetAllMaterials();

    /*std::vector<GLTF::Scene> scenes;
    std::vector<GLTF::Buffer> buffers;
    std::vector<GLTF::Animation> animations;
    std::vector<GLTF::Texture> textures;
    std::vector<GLTF::Image> images;
    std::vector<GLTF::Skin> skins;
    std::vector<GLTF::Sampler> samplers;
    std::vector<GLTF::Camera> cameras;
    std::vector<GLTF::Light> lights;*/

    // get number of things to translate
    std::vector<PlanarSurface> planarSurfaces = model.getModelObjects<PlanarSurface>();
    std::vector<PlanarSurfaceGroup> planarSurfaceGroups = model.getModelObjects<PlanarSurfaceGroup>();
    std::vector<BuildingStory> buildingStories = model.getConcreteModelObjects<BuildingStory>();
    std::vector<BuildingUnit> buildingUnits = model.getConcreteModelObjects<BuildingUnit>();
    std::vector<ThermalZone> thermalZones = model.getConcreteModelObjects<ThermalZone>();
    std::vector<AirLoopHVAC> airLoopHVACs = model.getConcreteModelObjects<AirLoopHVAC>();
    std::vector<SpaceType> spaceTypes = model.getConcreteModelObjects<SpaceType>();
    std::vector<DefaultConstructionSet> defaultConstructionSets = model.getConcreteModelObjects<DefaultConstructionSet>();
    double n = 0;
    std::vector<PlanarSurface>::size_type _N = planarSurfaces.size() + planarSurfaceGroups.size() + buildingStories.size() + buildingUnits.size()
                                               + thermalZones.size() + spaceTypes.size() + defaultConstructionSets.size() + airLoopHVACs.size() + 1;

    std::vector<PlanarSurface>::size_type N = planarSurfaces.size() + 1;

    updatePercentage(0.0);

#pragma region CREATE MATERIALS
    // add model specific materials
    buildMaterials(model, _materials, materialList, allMaterials);

#pragma endregion CREATE MATERIALS

    TransformationVector transformStack;
    Transformation transformation;
    transformStack.push_back(transformation);

    for (const auto& planarSurface : planarSurfaces) {
#pragma region MAIN LOOP
      int materialIndex = 0;
      std::string elementName = planarSurface.nameString();
      GLTF::Node node;
      node.name = elementName;

      // Now the geometry
      boost::optional<PlanarSurfaceGroup> planarSurfaceGroup = planarSurface.planarSurfaceGroup();
      // get the transformation to site coordinates
      Transformation buildingTransformation;
      if (planarSurfaceGroup) {
        buildingTransformation = planarSurfaceGroup->buildingTransformation();
      }
      // get the vertices
      Point3dVector vertices = planarSurface.vertices();
      Transformation t = transformStack.back();
      t = Transformation::alignFace(vertices);
      transformStack.push_back(t);
      Transformation tInv = t.inverse();
      Point3dVector faceVertices = reverse(tInv * vertices);
      std::vector<double> matrix;
      for (auto& p : buildingTransformation.vector()) {
        matrix.push_back(p);
      }

      //Adding a check to avoid warning "NODE_MATRIX_DEFAULT"	<Do not specify default transform matrix>.
      std::vector<double> matrixDefaultTransformation{1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
      if (matrixDefaultTransformation != matrix) {
        node.matrix = matrix;
      } else {
        node.matrix = {};
      }
      node.mesh = _meshes.size();

      GLTF::Mesh targetMesh;
      targetMesh.name = elementName;

      materialIndex = GetMaterialIndex(planarSurface, _materials, materialList, allMaterials);

      // get vertices of all sub surfaces
      Point3dVectorVector faceSubVertices;
      boost::optional<Surface> surface = planarSurface.optionalCast<Surface>();
      if (surface) {
        for (const auto& subSurface : surface->subSurfaces()) {
          faceSubVertices.push_back(reverse(tInv * subSurface.vertices()));
        }
      }

      Point3dVectorVector finalFaceVertices;
      if (triangulateSurfaces) {
        finalFaceVertices = computeTriangulation(faceVertices, faceSubVertices);
        if (finalFaceVertices.empty()) {
          LOG_FREE(Error, "modelToGLTF", "Failed to triangulate surface " << elementName << " with " << faceSubVertices.size() << " sub surfaces");
          //return;
        }
      } else {
        finalFaceVertices.push_back(faceVertices);
      }

      Point3dVector allVertices;
      Point3dVector triangleVertices;
      std::vector<size_t> faceIndices;
      int oe = 1;
      for (const auto& finalFaceVerts : finalFaceVertices) {
        Point3dVector finalVerts = t * finalFaceVerts;
        //Debug only to check for orientation of the faces
        /*Polygon3d trianglePoly = Polygon3d(finalVerts);
          bool isClockwise = trianglePoly.isClockwise();       
          if (isClockwise && oe % 2 == 0) {
            reverse(finalVerts.begin(), finalVerts.end());
          }*/
        triangleVertices.insert(triangleVertices.end(), finalVerts.begin(), finalVerts.end());
        //normal = buildingTransformation.rotationMatrix * r * z;
        if (triangulateSurfaces) {
          //faceIndices.push_back(0);
        }
        Point3dVector::reverse_iterator it = finalVerts.rbegin();
        Point3dVector::reverse_iterator itend = finalVerts.rend();
        for (; it != itend; ++it) {
          faceIndices.push_back(getVertexIndexT(*it, allVertices));
        }
        // convert to 1 based indices
        //face_indices.each_index {|i| face_indices[i] = face_indices[i] + 1}
      }

      Vector3d outwardNormal = planarSurface.outwardNormal();
      double x = outwardNormal.x();
      double y = outwardNormal.y();
      double z = outwardNormal.z();
      Vector3dVector normalVectors;
      for (n = 0; n < allVertices.size(); n++) {
        normalVectors.push_back(outwardNormal);
      }

      ShapeComponentIds shapeCompoentIds{
        AddIndices(faceIndices, _indicesBuffer, _accessors),          //IndicesAccessorId
        AddCoordinates(allVertices, _coordinatesBuffer, _accessors),  //VerticesAccessorId
        AddNormals(normalVectors, _coordinatesBuffer, _accessors)     //NormalsAccessorId
      };
      GLTF::Primitive thisPrimitive;
      std::map<std::string, int> attrib;
      attrib["NORMAL"] = shapeCompoentIds.NormalsAccessorId;
      attrib["POSITION"] = shapeCompoentIds.VerticesAccessorId;

      thisPrimitive.attributes = attrib;
      thisPrimitive.indices = shapeCompoentIds.IndicesAccessorId;
      thisPrimitive.material = materialIndex;
      //thisPrimitive.mode = TINYGLTF_MODE_LINE;
      thisPrimitive.mode = TINYGLTF_MODE_TRIANGLES;
      targetMesh.primitives.push_back(thisPrimitive);
      _nodes.push_back(node);
      _meshes.push_back(targetMesh);

      n += 1;
      updatePercentage(100.0 * n / N);
#pragma endregion MAIN
    }
#pragma region BUILD SCENE | ELEMENT

    if (_coordinatesBuffer.size() == 0) return false;

    if (!BufferInBase64) {
      //BPS:having a separate input file for the GLTF is old now everything resides 
      //in the main GLTF file only as a binary buffer data.
      auto padding = _indicesBuffer.size() % 4;
      for (int i = 0; i < padding; i++) {
        _indicesBuffer.push_back(0x00);  //padding bytes
      }
      std::vector<unsigned char> _allBuffer;
      _allBuffer.insert(_allBuffer.end(), _indicesBuffer.begin(), _indicesBuffer.end());
      _allBuffer.insert(_allBuffer.end(), _coordinatesBuffer.begin(), _coordinatesBuffer.end());
      std::vector<unsigned char> _data;
      for (unsigned char b : _allBuffer) {
        _data.push_back(b);
      }
      _buffer.data = _data;
    }

    _indicesBv.byteLength = _indicesBuffer.size();
    _indicesBv.byteOffset = 0;

    _coordinatesBv.byteLength = _coordinatesBuffer.size();
    _coordinatesBv.byteOffset = _indicesBuffer.size();

    _bufferViews.push_back(_indicesBv);
    _bufferViews.push_back(_coordinatesBv);
    // TODO:BPS _buffer.byteLength = _indicesBuffer.size() + _coordinatesBuffer.size();

#pragma endregion BUILD SCENE | ELEMENT

    gltf_Model.accessors = _accessors;
    gltf_Model.bufferViews = _bufferViews;
    gltf_Model.buffers = {_buffer};

    //Other tie ups
    //node.mesh = 0;
    //scene.nodes.push_back(0);  // Default scene

    // Define the asset. The version is required
    _asset.version = "2.0";
    _asset.generator = "OpenStudio";

    // Now all that remains is to tie back all the loose objects into the
    // our single model.
    gltf_Model.scenes.push_back(_scene);
    gltf_Model.meshes = _meshes;

    std::vector<GLTF::Node> _nodesNew;
    std::vector<int> nodes;
    nodes.resize(_nodes.size() - 1);
    for (int i = 1; i < _nodes.size(); i++) {
      int j = i - 1;
      nodes[j] = i;
    }
    _topNode.children = nodes;

    _nodesNew.push_back(_topNode);
    int sk = 0;
    for (auto& nn : _nodes) {
      if (sk != 0) {
        _nodesNew.push_back(nn);
      }
      sk++;
    }

    //gltf_Model.nodes = _nodes;
    gltf_Model.nodes = _nodesNew;
    gltf_Model.asset = _asset;
    // Add Materials to Model
    gltf_Model.materials = _materials;

    // Save it to a file
    bool ret = loader.WriteGltfSceneToFile(&gltf_Model, toString(outputPath),
                                           true,    // embedImages
                                           true,    // embedBuffers
                                           true,    // pretty print
                                           false);  // write binary

    //may be some validatoin of the gltf generated
    updatePercentage(100.0);

    return ret;
  }

  /// <summary>
  /// TODO: exports JSON explicitly if we
  /// segreagate creation of GLTF MODEL from implicitly exporting
  /// the GLTF json file.
  /// </summary>
  /// <returns></returns>
  bool GltfForwardTranslator::toJSON() {
    return false;
  }

  /// <summary>
  /// To export a Minimal GLTF file (Triangle with 3 Points) using raw
  /// buffer data.
  /// </summary>
  /// <param name="outputPath"></param>
  /// <returns>exports triangle.gltf</returns>
  bool GltfForwardTranslator::CreateTriangleGLTF(const path& outputPath) {
    GLTF::Model m;
    GLTF::Scene scene;
    GLTF::Mesh mesh;
    GLTF::Primitive primitive;
    GLTF::Node node;
    GLTF::Buffer buffer;
    GLTF::BufferView bufferView1;
    GLTF::BufferView bufferView2;
    GLTF::Accessor accessor1;
    GLTF::Accessor accessor2;
    GLTF::Asset asset;

    // This is the raw data buffer.
    buffer.data = {// 6 bytes of indices and two bytes of padding
                   //0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00,
                   0x00, 0x01, 0x02, 0x00,
                   // 36 bytes of floating point numbers
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00};

    //// Create an array of size equivalent to vector
    //unsigned char arr[42]

    // "The indices of the vertices (ELEMENT_ARRAY_BUFFER) take up 6 bytes in the
    // start of the buffer.
    bufferView1.buffer = 0;
    bufferView1.byteOffset = 0;
    //bufferView1.byteLength = 6;
    bufferView1.byteLength = 3;
    bufferView1.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

    // The vertices take up 36 bytes (3 vertices * 3 floating points * 4 bytes)
    // at position 8 in the buffer and are of type ARRAY_BUFFER
    bufferView2.buffer = 0;
    //bufferView2.byteOffset = 8;
    ////bufferView2.byteStride = 12;
    //bufferView2.byteLength = 36;
    bufferView2.byteOffset = 4;
    //bufferView2.byteStride = 12;
    bufferView2.byteLength = 36;
    bufferView2.target = TINYGLTF_TARGET_ARRAY_BUFFER;

    // Describe the layout of bufferView1, the indices of the vertices
    accessor1.bufferView = 0;
    accessor1.byteOffset = 0;
    accessor1.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;  //TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
    accessor1.count = 3;
    accessor1.type = TINYGLTF_TYPE_SCALAR;
    accessor1.maxValues.push_back(2);
    accessor1.minValues.push_back(0);

    // Describe the layout of bufferView2, the vertices themself
    accessor2.bufferView = 1;
    accessor2.byteOffset = 0;
    accessor2.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor2.count = 3;
    accessor2.type = TINYGLTF_TYPE_VEC3;
    accessor2.maxValues = {1.0, 1.0, 0.0};
    accessor2.minValues = {0.0, 0.0, 0.0};

    // Build the mesh primitive and add it to the mesh
    primitive.indices = 0;                 // The index of the accessor for the vertex indices
    primitive.attributes["POSITION"] = 1;  // The index of the accessor for positions
    primitive.material = 0;
    //primitive.mode = TINYGLTF_MODE_POINTS;
    //primitive.mode = TINYGLTF_MODE_LINE;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;
    mesh.primitives.push_back(primitive);

    // Other tie ups
    node.mesh = 0;
    scene.nodes.push_back(0);  // Default scene

    // Define the asset. The version is required
    asset.version = "2.0";
    asset.generator = "OpenStudio";

    // Now all that remains is to tie back all the loose objects into the
    // our single model.
    m.scenes.push_back(scene);
    m.meshes.push_back(mesh);
    m.nodes.push_back(node);
    m.buffers.push_back(buffer);
    m.bufferViews.push_back(bufferView1);
    m.bufferViews.push_back(bufferView2);
    m.accessors.push_back(accessor1);
    m.accessors.push_back(accessor2);
    m.asset = asset;

    // Create a simple material
    GLTF::Material mat;
    mat.pbrMetallicRoughness.baseColorFactor = {1.0f, 0.9f, 0.9f, 1.0f};
    mat.pbrMetallicRoughness.metallicFactor = 0.8;
    mat.pbrMetallicRoughness.roughnessFactor = 0.5;
    mat.doubleSided = true;
    m.materials.push_back(mat);

    // Save it to a file
    GLTF::TinyGLTF gltf;
    bool ret = gltf.WriteGltfSceneToFile(&m, toString(outputPath),
                                         true,    // embedImages
                                         true,    // embedBuffers
                                         true,    // pretty print
                                         false);  // write binary
    return ret;
  }

  /// <summary>
  /// To export a Minimal GLTF file (Triangle with 3 Points) using generated
  /// raw data from Point3DVector
  /// </summary>
  /// <param name="outputPath"></param>
  /// <returns>exports triangle_2.gltf</returns>
  bool GltfForwardTranslator::CreateTriangleGLTF_2(const path& outputPath) {
    GLTF::Model m;
    GLTF::Scene scene;
    GLTF::Mesh mesh;
    GLTF::Primitive primitive;
    GLTF::Node node;
    GLTF::Buffer buffer;
    GLTF::BufferView bufferView1;
    GLTF::BufferView bufferView2;
    GLTF::Accessor accessor1;
    GLTF::Accessor accessor2;
    GLTF::Asset asset;

    //// Create an array of size equivalent to vector
    //unsigned char arr[42];
    //
    // "The indices of the vertices (ELEMENT_ARRAY_BUFFER) take up 6 bytes in the
    // start of the buffer.
    bufferView1.buffer = 0;
    bufferView1.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

    // The vertices take up 36 bytes (3 vertices * 3 floating points * 4 bytes)
    // at position 8 in the buffer and are of type ARRAY_BUFFER
    bufferView2.buffer = 0;
    bufferView2.target = TINYGLTF_TARGET_ARRAY_BUFFER;

    std::vector<size_t> faceIndices{0, 1, 2};
    std::vector<unsigned char> _indicesBuffer;
    std::vector<unsigned char> _coordinatesBuffer;
    std::vector<GLTF::Accessor> _accessors;
    Point3dVector allVertices;
    Point3d p1 = Point3d(0, 0, 0);
    Point3d p2 = Point3d(1, 0, 0);
    Point3d p3 = Point3d(0, 1, 0);
    std::vector<Point3d> pvec{p1, p2, p3};
    allVertices = pvec;

    ShapeComponentIds shapeCompoentIds{
      AddIndices(faceIndices, _indicesBuffer, _accessors),          //IndicesAccessorId
      AddCoordinates(allVertices, _coordinatesBuffer, _accessors),  //VerticesAccessorId
      0                                                             //NormalsAccessorId
    };

    // Build the mesh primitive and add it to the mesh
    // The index of the accessor for the vertex indices
    primitive.indices = 0;
    // The index of the accessor for positions
    primitive.attributes["POSITION"] = shapeCompoentIds.VerticesAccessorId;
    primitive.material = 0;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;
    mesh.primitives.push_back(primitive);

    bufferView1.byteOffset = 0;
    bufferView1.byteLength = _indicesBuffer.size();
    bufferView2.byteOffset = _indicesBuffer.size();
    bufferView2.byteLength = _coordinatesBuffer.size();

    std::vector<unsigned char> _allBuffer;
    _allBuffer.insert(_allBuffer.end(), _indicesBuffer.begin(), _indicesBuffer.end());
    _allBuffer.insert(_allBuffer.end(), _coordinatesBuffer.begin(), _coordinatesBuffer.end());

    std::vector<unsigned char> _data;
    for (unsigned char b : _allBuffer) {
      _data.push_back(b);
    }
    // This is the raw data buffer.
    buffer.data = _data;

    // Other tie ups
    node.mesh = 0;
    scene.nodes.push_back(0);  // Default scene

    // Define the asset. The version is required
    asset.version = "2.0";
    asset.generator = "OpenStudio";

    // Now all that remains is to tie back all the loose objects into the
    // our single model.
    m.scenes.push_back(scene);
    m.meshes.push_back(mesh);
    m.nodes.push_back(node);
    m.buffers.push_back(buffer);
    m.bufferViews.push_back(bufferView1);
    m.bufferViews.push_back(bufferView2);
    m.accessors = _accessors;
    m.asset = asset;

    // Create a simple material
    GLTF::Material mat;
    mat.pbrMetallicRoughness.baseColorFactor = {1.0f, 0.9f, 0.9f, 1.0f};
    mat.pbrMetallicRoughness.metallicFactor = 1.0;
    mat.pbrMetallicRoughness.roughnessFactor = 0.5;
    mat.doubleSided = true;
    m.materials.push_back(mat);

    // Save it to a file
    GLTF::TinyGLTF gltf;
    bool ret = gltf.WriteGltfSceneToFile(&m, toString(outputPath),
                                         true,    // embedImages
                                         true,    // embedBuffers
                                         true,    // pretty print
                                         false);  // write binary
    return ret;
  }

  /// <summary>
  /// To populate a GLTF Model from an existing GLTF file.
  /// also exports a gltf file with a .bin file (non embeded version).
  /// </summary>
  /// <param name="inputPath"></param>
  /// <param name="inputNonEmbededPath"></param>
  /// <returns></returns>
  bool GltfForwardTranslator::loadGLTF(const path& inputPath, const path& inputNonEmbededPath) {
    GLTF::TinyGLTF loader;
    GLTF::Model gltf_Model;
    std::string err;
    std::string warning;
    std::string fileName = toString(inputPath);
    bool ret = loader.LoadASCIIFromFile(&gltf_Model, &err, &warning, fileName);
    if (err.size() != 0) {
      fprintf(stderr, "Error loading GLTF %s", err.c_str());
      //ret = false;
    }
    if (!warning.empty()) {
      printf("Warn: %s\n", warning.c_str());
    }

    if (!err.empty()) {
      printf("Err: %s\n", err.c_str());
    }

    if (!ret) {
      printf("Failed to parse glTF\n");
    } else {
      std::string output_filename = toString(inputNonEmbededPath);
      std::string embedded_filename = output_filename.substr(0, output_filename.size() - 5) + "-Embedded.gltf";
      bool a = loader.WriteGltfSceneToFile(&gltf_Model, output_filename);

      // Embedd buffers and (images if present)
#ifndef TINYGLTF_NO_STB_IMAGE_WRITE
      bool b = loader.WriteGltfSceneToFile(&gltf_Model, embedded_filename, true, true);
#endif
    }
    return ret;
  }

  GltfForwardTranslator::GltfForwardTranslator() {
    m_logSink.setLogLevel(Warn);
    //m_logSink.setChannelRegex(boost::regex("openstudio\\.model\\.GltfForwardTranslator"));
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  std::vector<LogMessage> GltfForwardTranslator::warnings() const {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() == Warn) {
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::vector<LogMessage> GltfForwardTranslator::errors() const {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() > Warn) {
        result.push_back(logMessage);
      }
    }

    return result;
  }

}  // namespace model
}  // namespace openstudio