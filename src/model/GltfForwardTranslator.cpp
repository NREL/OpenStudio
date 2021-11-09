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

  struct mat_Data
  {
    std::string matName;
    int r;
    int g;
    int b;
    double a;
    //add something here for double sided flag if required
  };

  struct ShapeComponentIds
  {
    int IndicesAccessorId;
    int VerticesAccessorId;
    int NormalsAccessorId;
  };

  class Mesher
  {
   public:
    std::vector<Point3d>& Locations;
    std::vector<Point3d>& Normals;
    std::vector<int> Indices;

    void AddMesh(std::vector<Point3d>& polydata, Transformation t) {
      Locations = polydata;
    }

   private:
    REGISTER_LOGGER("GLTF_MESHER");
  };

  GLTF::Material CreateMaterial(const std::string& name, float red, float green, float blue, float alpha) {

    GLTF::Material m;
    m.name = name;
    PbrMetallicRoughness pbr;
    pbr.baseColorFactor = {red, green, blue, alpha};
    pbr.metallicFactor = 0;
    pbr.roughnessFactor = 1;
    m.pbrMetallicRoughness = pbr;
    std::vector<double> eF{0, 0, 0};
    m.emissiveFactor = eF;
    m.alphaMode = (alpha < 1.0f) ? "BLEND" : "OPAQUE";
    m.alphaCutoff = 0.5f;
    m.doubleSided = false;

    return m;
  }

  void initMaterials(std::vector<GLTF::Material>& _materials) {
    // default material is always set at index 0;
    const float grey = 0.8f;
    const float alpha = 1.0f;
    GLTF::Material m = CreateMaterial("Default material", grey, grey, grey, alpha);
    _materials.push_back(m);
  }

  void initScene(GLTF::Scene& _scene, GLTF::Node& _topNode, std::vector<GLTF::Node>& _nodes) {
    std::vector<int> ns{0};
    _scene.nodes = ns;

    _topNode.name = "Z_UP";
    std::vector<double> mtrx{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    _topNode.matrix = mtrx;

    _nodes.push_back(_topNode);
  }

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

    //bv_coordinates.byteStride = 4;
    //bv_coordinates.byteLength = 36;
    //bv_coordinates.byteStride = 16;
    //bv_coordinates.byteLength = 48;
    _coordinatesBv = bv_coordinates;
    //_bufferViews.push_back(_coordinatesBv);
  }

  int CreateMaterial(std::vector<GLTF::Material>& _materials, int r, int g, int b, double a, std::string materialName) {
    GLTF::Material m;
    m.name = materialName;
    GLTF::PbrMetallicRoughness pbr;
    pbr.baseColorFactor = {(float)(r / 255.0), (float)(g / 255.0), (float)(b / 255.0), (float)(a / 255.0)};
    pbr.metallicFactor = 0;
    pbr.roughnessFactor = 1;
    m.pbrMetallicRoughness = pbr;
    m.emissiveFactor = {0, 0, 0};
    m.alphaMode = (a < 1.0f) ? "BLEND" : "OPAQUE";
    m.alphaCutoff = 0.5f;
    m.doubleSided = false;

    int ret = _materials.size();
    _materials.push_back(m);
    return ret;
  }

  std::vector<mat_Data> GetAllMaterials() {
    std::vector<mat_Data> result;

    result.push_back({"Undefined", 255, 255, 255, 1});
    result.push_back({"NormalMaterial", 255, 255, 255, 1});
    result.push_back({"NormalMaterial_Ext", 255, 255, 255, 1});
    result.push_back({"NormalMaterial_Int", 255, 0, 0, 1});

    result.push_back({"Floor", 128, 128, 128, 1});
    result.push_back({"Floor_Ext", 128, 128, 128, 1});
    result.push_back({"Floor_Int", 191, 191, 191, 1});

    result.push_back({"Wall", 204, 178, 102, 1});
    result.push_back({"Wall_Ext", 204, 178, 102, 1});
    result.push_back({"Wall_Int", 235, 226, 197, 1});

    result.push_back({"RoofCeiling", 153, 76, 76, 1});
    result.push_back({"RoofCeiling_Ext", 153, 76, 76, 1});
    result.push_back({"RoofCeiling_Int", 202, 149, 149, 1});

    result.push_back({"Window", 102, 178, 204, 0.6});
    result.push_back({"Window_Ext", 102, 178, 204, 0.6});
    result.push_back({"Window_Int", 192, 226, 235, 0.6});

    result.push_back({"Door", 153, 133, 76, 1});
    result.push_back({"Door_Ext", 153, 133, 76, 1});
    result.push_back({"Door_Int", 202, 188, 149, 1});

    result.push_back({"SiteShading", 75, 124, 149, 1});
    result.push_back({"SiteShading_Ext", 75, 124, 149, 1});
    result.push_back({"SiteShading_Int", 187, 209, 220, 1});

    result.push_back({"BuildingShading", 113, 76, 153, 1});
    result.push_back({"BuildingShading_Ext", 113, 76, 153, 1});
    result.push_back({"BuildingShading_Int", 216, 203, 229, 1});

    result.push_back({"SpaceShading", 76, 110, 178, 1});
    result.push_back({"SpaceShading_Ext", 76, 110, 178, 1});
    result.push_back({"SpaceShading_Int", 183, 197, 224, 1});

    result.push_back({"InteriorPartitionSurface", 158, 188, 143, 1});
    result.push_back({"InteriorPartitionSurface_Ext", 158, 188, 143, 1});
    result.push_back({"InteriorPartitionSurface_Int", 213, 226, 207, 1});

    // start textures for boundary conditions
    result.push_back({"Boundary_Surface", 0, 153, 0, 1});
    result.push_back({"Boundary_Adiabatic", 255, 0, 0, 1});
    result.push_back({"Boundary_Space", 255, 0, 0, 1});
    result.push_back({"Boundary_Outdoors", 163, 204, 204, 1});
    result.push_back({"Boundary_Outdoors_Sun", 40, 204, 204, 1});
    result.push_back({"Boundary_Outdoors_Wind", 9, 159, 162, 1});
    result.push_back({"Boundary_Outdoors_SunWind", 68, 119, 161, 1});
    result.push_back({"Boundary_Ground", 204, 183, 122, 1});
    result.push_back({"Boundary_Groundfcfactormethod", 153, 122, 30, 1});
    result.push_back({"Boundary_Groundslabpreprocessoraverage", 255, 191, 0, 1});
    result.push_back({"Boundary_Groundslabpreprocessorcore", 255, 182, 50, 1});
    result.push_back({"Boundary_Groundslabpreprocessorperimeter", 255, 178, 101, 1});
    result.push_back({"Boundary_Groundbasementpreprocessoraveragewall", 204, 51, 0, 1});
    result.push_back({"Boundary_Groundbasementpreprocessoraveragefloor", 204, 81, 40, 1});
    result.push_back({"Boundary_Groundbasementpreprocessorupperwall", 204, 112, 81, 1});
    result.push_back({"Boundary_Groundbasementpreprocessorlowerwall", 204, 173, 163, 1});
    result.push_back({"Boundary_Othersidecoefficients", 63, 63, 63, 1});
    result.push_back({"Boundary_Othersideconditionsmodel", 153, 0, 76, 1});

    // special rendering materials
    result.push_back({"SpaceType_Plenum", 192, 192, 192, 0.1});
    result.push_back({"ThermalZone_Plenum", 192, 192, 192, 0.1});

    // special rendering materials, these are components or textures in SketchUp
    result.push_back({"DaylightingControl", 102, 178, 204, 0.1});
    result.push_back({"AirWall", 102, 178, 204, 0.1});
    result.push_back({"SolarCollector", 255, 255, 255, 1});
    result.push_back({"Photovoltaic", 255, 255, 255, 0.1});

    return result;
  }

  void AddGLTFMaterials(std::map<std::string, int>& materialList, std::vector<GLTF::Material>& _materials) {
    std::vector<mat_Data> result = GetAllMaterials();

    for (auto& m : result) {
      if (materialList.find(m.matName) != materialList.end()) {
        std::cout << "Key found";
      } else {
        materialList[m.matName] = CreateMaterial(_materials, m.r, m.g, m.b, m.a, m.matName);
      }
    }
    //int index = CreateMaterial(_materials, 255, 255, 255, 1, "Undefined");
  }

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

  std::string getObjectGLTFMaterialName(const ModelObject& object) {
    return getObjectGLTFMaterialName(object.iddObjectType().valueDescription(), object.nameString());
  }

  int CreateMaterial(std::vector<GLTF::Material>& _materials, boost::optional<RenderingColor>& color, std::string& materialName) {
    GLTF::Material m;
    m.name = materialName;
    GLTF::PbrMetallicRoughness pbr;
    pbr.baseColorFactor = {(float)(color->renderingRedValue() / 255.0), (float)(color->renderingGreenValue() / 255.0),
                           (float)(color->renderingBlueValue() / 255.0), (float)(color->renderingAlphaValue() / 255.0)};
    pbr.metallicFactor = 0;
    pbr.roughnessFactor = 1;
    m.pbrMetallicRoughness = pbr;
    m.emissiveFactor = {0, 0, 0};
    m.alphaMode = (color->renderingAlphaValue() < 1.0f) ? "BLEND" : "OPAQUE";
    m.alphaCutoff = 0.5f;
    m.doubleSided = false;

    int ret = _materials.size();
    _materials.push_back(m);
    return ret;
  }

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

  int getGLTFMaterialIndex(const std::string& materialName, std::map<std::string, int>& materialList, std::vector<mat_Data>& allMaterials,
                           std::vector<GLTF::Material>& _materials) {
    std::map<std::string, int>::const_iterator it = materialList.find(materialName);
    if (it != materialList.end()) {
      return it->second;
    } else {
      for (int i = 0; i < allMaterials.size(); i++) {
        if (allMaterials[i].matName == materialName) {
          //Create Material and Add to  _materials
          int newMatIndex =
            CreateMaterial(_materials, allMaterials[i].r, allMaterials[i].g, allMaterials[i].b, allMaterials[i].a, allMaterials[i].matName);
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

      it = materialList.find("Undefined");
      if (it != materialList.end()) {
        return it->second;
      }
    }

    /*it = materialList.find("Undefined");
    if (it != materialList.end()) {
      return it->second;
    }*/
    //OS_ASSERT(false);
    return 0;
  }

  void buildMaterials(Model model, std::vector<GLTF::Material>& _materials, std::map<std::string, int>& materialList) {
    // make construction materials
    for (auto& construction : model.getModelObjects<ConstructionBase>()) {
      // If it's ConstructionAirBoundary, we'll later use the standard material "AirWall"
      if (!construction.optionalCast<ConstructionAirBoundary>()) {
        boost::optional<RenderingColor> color = construction.renderingColor();
        if (!color) {
          color = RenderingColor(model);
          construction.setRenderingColor(*color);
        }
        std::string materialName = getObjectGLTFMaterialName(construction);
        if (materialList.find(materialName) != materialList.end()) {
          std::cout << "Key found";
        } else {
          materialList[materialName] = CreateMaterial(_materials, color, materialName);
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
      std::string materialName = getObjectGLTFMaterialName(thermalZone);
      if (materialList.find(materialName) != materialList.end()) {
        std::cout << "Key found";
      } else {
        materialList[materialName] = CreateMaterial(_materials, color, materialName);
      }
    }

    // make space type materials
    for (auto& spaceType : model.getConcreteModelObjects<SpaceType>()) {
      boost::optional<RenderingColor> color = spaceType.renderingColor();
      if (!color) {
        color = RenderingColor(model);
        spaceType.setRenderingColor(*color);
      }
      std::string materialName = getObjectGLTFMaterialName(spaceType);
      if (materialList.find(materialName) != materialList.end()) {
        std::cout << "Key found";
      } else {
        materialList[materialName] = CreateMaterial(_materials, color, materialName);
      }
    }

    // make building story materials
    for (auto& buildingStory : model.getConcreteModelObjects<BuildingStory>()) {
      boost::optional<RenderingColor> color = buildingStory.renderingColor();
      if (!color) {
        color = RenderingColor(model);
        buildingStory.setRenderingColor(*color);
      }
      std::string materialName = getObjectGLTFMaterialName(buildingStory);
      if (materialList.find(materialName) != materialList.end()) {
        std::cout << "Key found";
      } else {
        materialList[materialName] = CreateMaterial(_materials, color, materialName);
      }
    }

    // make building unit materials
    for (auto& buildingUnit : model.getConcreteModelObjects<BuildingUnit>()) {
      boost::optional<RenderingColor> color = buildingUnit.renderingColor();
      if (!color) {
        color = RenderingColor(model);
        buildingUnit.setRenderingColor(*color);
      }
      std::string materialName = getObjectGLTFMaterialName(buildingUnit);
      if (materialList.find(materialName) != materialList.end()) {
        std::cout << "Key found";
      } else {
        materialList[materialName] = CreateMaterial(_materials, color, materialName);
      }
    }

    // make air loop HVAC materials
    for (auto& airLoopHVAC : model.getConcreteModelObjects<AirLoopHVAC>()) {
      std::string materialName = getObjectGLTFMaterialName(airLoopHVAC);
      boost::optional<RenderingColor> color = RenderingColor(model);
      if (materialList.find(materialName) != materialList.end()) {
        std::cout << "Key found";
      } else {
        materialList[materialName] = CreateMaterial(_materials, color, materialName);
      }
    }
  }

  unsigned openstudioFaceFormatId() {
    return 1024;
  }

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

  std::vector<unsigned char> intToBytes(int paramInt) {
    std::vector<unsigned char> arrayOfByte(4);
    for (int i = 0; i < 4; i++)
      arrayOfByte[3 - i] = (paramInt >> (i * 8));
    return arrayOfByte;
  }

  template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
  std::vector<uint8_t> splitValueToBytes(T const& value) {
    std::vector<uint8_t> bytes;

    for (size_t i = 0; i < sizeof(value); i++) {
      uint8_t byte = value >> (i * 8);
      bytes.insert(bytes.begin(), byte);
    }

    return bytes;
  }

  int AddIndices(std::vector<size_t>& faceIndices, std::vector<unsigned char>& _indicesBuffer, std::vector<GLTF::Accessor>& _accessors) {
    //Value of INT_MAX is + 2147483647.
    //Value of INT_MIN is - 2147483648.
    int min = INT_MAX;
    int max = INT_MIN;
    int j = 0;
    for (auto& item : faceIndices) {
      //if (j != 0 && j!=4) {
      //if (j != 0) {
      int i = (int)item;
      min = std::min(i, min);
      max = std::max(i, max);
      //}
      //j++;
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
    //indAccessor.count = faceIndices.size() - 1;  //to omit 0th position 1024
    indAccessor.count = faceIndices.size();
    indAccessor.minValues = {(double)min};
    indAccessor.maxValues = {(double)max};

    //const auto IndexSize = faceIndices.size() * size; //?
    //std::vector<std::byte> indicesBufferData;
    std::vector<unsigned char> indicesBufferData;
    //indicesBufferData.resize(IndexSize); //?
    int k = 0;
    for (auto index : faceIndices) {
      //if (k != 0) {

      //std::vector<std::byte> first;
      std::vector<unsigned char> arrayOfByte;
      if (ct == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
        //first = {(std::byte)index};
        //arrayOfByte = splitValueToBytes(index);
        arrayOfByte.push_back((unsigned char)index);
      } else if (ct == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
        //first = std::bitset
        //first = BitConverter::GetBytes((short)index); //TODO
        //first = {(std::byte)index};
        arrayOfByte = splitValueToBytes(index);
      } else {
        //first = BitConverter::GetBytes((int)index); //TODO
        //first = {(std::byte)index};
        //memcpy(arrayOfByte, &index, sizeof index);
        /*arrayOfByte = intToBytes(index);*/
        arrayOfByte = splitValueToBytes(index);
      }
      /*for (auto v : arrayOfByte) {
        first.push_back((std::byte)v);
      }*/
      indicesBufferData.insert(indicesBufferData.end(), arrayOfByte.begin(), arrayOfByte.end());
      //}
      //k++;
    }
    if (indicesBufferData.size() % 8 != 0) {
      std::cout << "needs padding";
      //May be add empty like we did in Triangle test
      /*auto padding = indicesBufferData.size() % size;
      for (int i = 0; i < padding; i++) {
        indicesBufferData.push_back((std::byte)0);
      }*/
    }
    if (indicesBufferData.size() < 4) {
      indicesBufferData.push_back(0x00);
    } else {
    
    }
    _indicesBuffer.insert(_indicesBuffer.end(), indicesBufferData.begin(), indicesBufferData.end());

    //TODO:??
    //_indicesBuffer.push_back((std::byte)0);

    const auto thisIndex = _accessors.size();
    _accessors.push_back(indAccessor);
    return thisIndex;
  }

  int AddCoordinates(Point3dVector& allVertices, std::vector<unsigned char>& _coordinatesBuffer, std::vector<GLTF::Accessor>& _accessors) {
    /*std::vector<float> values;*/
    std::vector<float> values;
    float divider = 1;
    for (auto& loc : allVertices) {
      values.push_back((float)(loc.x() / divider));  //remove casting as float is not used
      values.push_back((float)(loc.y() / divider));
      values.push_back((float)(loc.z() / divider));
    }

    int startingBufferPosition = _coordinatesBuffer.size();
    //std::vector<double> min = {DBL_MAX, DBL_MAX, DBL_MAX};
    //std::vector<double> max = {-DBL_MIN, -DBL_MIN, -DBL_MIN};  //?? (-ve)
    std::vector<float> min = {FLT_MAX, FLT_MAX, FLT_MAX};
    std::vector<float> max = {-FLT_MIN, -FLT_MIN, -FLT_MIN};  //?? (-ve)
    int i = 0;
    int skipremove = 0;

    for (auto& value : values) {
      //if (skipremove < 9) {

      min[i] = std::min(value, min[i]);
      max[i] = std::max(value, max[i]);
      i++;
      if (i > 2) i = 0;
      //std::vector<std::byte> thisbuff;
      ////thisbuff = BitConverter::GetBytes(value); TODO
      //thisbuff = {(std::byte)value};
      //_coordinatesBuffer.insert(_coordinatesBuffer.end(), thisbuff.begin(), thisbuff.end());
      //cast it(double) to arrray of byte

      /*  std::vector<unsigned char> arrayOfByte;
      
      arrayOfByte = splitValueToBytes(value);
      for (auto v : arrayOfByte) {
        thisbuff.push_back((std::byte)v);
      }*/
      //std::vector<std::byte> thisbuff;
      std::vector<unsigned char> v;
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&value);
      for (size_t i = 0; i < sizeof(float); ++i)
        v.push_back(ptr[i]);
      /* for (auto cv : v) {
        thisbuff.push_back((std::byte)cv);
      }*/
      //_coordinatesBuffer.push_back((std::byte)value);
      _coordinatesBuffer.insert(_coordinatesBuffer.end(), v.begin(), v.end());
      //}
      //skipremove++;
    }
    /* bool istwelve;
    if (values.size() == 12) {
      istwelve = true;
    } else {
      istwelve = false;
    }*/

    //_coordinatesBuffer.push_back((std::byte)0);
    //_coordinatesBuffer.push_back((std::byte)0);
    //_coordinatesBuffer.push_back((std::byte)0);
    //to fix : offset 18 is not a multiple of Comonent Type lenght 4
    auto padding = _coordinatesBuffer.size() % 4;
    for (int i = 0; i < padding; i++) {
      _coordinatesBuffer.push_back((unsigned)0);
    }

    GLTF::Accessor coordAccessor;
    coordAccessor.bufferView = 1;
    coordAccessor.byteOffset = startingBufferPosition;
    coordAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    coordAccessor.normalized = false;
    coordAccessor.count = values.size() / 3;  //allVertices.size()
    coordAccessor.type = TINYGLTF_TYPE_VEC3;
    /*coordAccessor.minValues = min;
    coordAccessor.maxValues = max;*/

    auto ret = _accessors.size();
    _accessors.push_back(coordAccessor);
    return ret;
  }

  bool GltfForwardTranslator::modelToGLTF(const Model& model, bool triangulateSurfaces, const path& outputPath) {
    return modelToGLTF(
      model, triangulateSurfaces, [](double percentage) {}, outputPath);
  }

  bool GltfForwardTranslator::modelToGLTF(const Model& model, bool triangulateSurfaces, std::function<void(double)> updatePercentage,
                                          const path& outputPath) {
    m_logSink.setThreadId(std::this_thread::get_id());
    m_logSink.resetStringStream();

    bool BufferInBase64 = false;  //no *.bin file is involed

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
    //std::vector<std::byte> _indicesBuffer;
    std::vector<unsigned char> _indicesBuffer;
    //std::vector<std::byte> _coordinatesBuffer;
    std::vector<unsigned char> _coordinatesBuffer;

    GLTF::Asset _asset;

#pragma region INIT

#pragma region MATERIALS

    initMaterials(_materials);

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
    std::vector<PlanarSurface>::size_type N = planarSurfaces.size() + planarSurfaceGroups.size() + buildingStories.size() + buildingUnits.size()
                                              + thermalZones.size() + spaceTypes.size() + defaultConstructionSets.size() + airLoopHVACs.size() + 1;

    //new test end
    updatePercentage(0.0);

#pragma region CREATE MATERIALS
    // add model specific materials
    buildMaterials(model, _materials, materialList);

#pragma endregion CREATE MATERIALS

    //stack<Transformation> transformStack;
    TransformationVector transformStack;
    Transformation transformation;
    transformStack.push_back(transformation);

    int l = 0;
    //loop over all surfaces
    for (const auto& planarSurface : planarSurfaces) {
      //if (l == 2) {
      if (true) {

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
        //Transformation r = t.rotationMatrix();
        Transformation tInv = t.inverse();
        Point3dVector faceVertices = reverse(tInv * vertices);
        /*std::vector<Point3d> points;
        for (auto& v : faceVertices) {
          points.push_back(v);
        }
        auto azfdfd = points.size();*/

        std::vector<double> matrix;
        for (auto& p : buildingTransformation.vector()) {
          matrix.push_back(p);
        }
        node.matrix = matrix;
        //node.matrix = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
        node.mesh = _meshes.size();

        GLTF::Mesh targetMesh;
        targetMesh.name = elementName;
        boost::optional<Surface> surface = planarSurface.optionalCast<Surface>();
        if (surface) {
          std::string surfaceType = surface->surfaceType();
          std::string surfaceTypeGLTFMaterialName = getSurfaceTypeGLTFMaterialName(surfaceType);
          materialIndex = getGLTFMaterialIndex(surfaceTypeGLTFMaterialName, materialList, allMaterials, _materials);
          materialIndicesUsed[surfaceTypeGLTFMaterialName] = materialIndex;
        }

        // get vertices of all sub surfaces
        Point3dVectorVector faceSubVertices;
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
        for (const auto& finalFaceVerts : finalFaceVertices) {
          /*Point3dVector finalVerts = buildingTransformation * t * finalFaceVerts;*/
          Point3dVector finalVerts = t * finalFaceVerts;
          triangleVertices.insert(triangleVertices.end(), finalVerts.begin(), finalVerts.end());
          //normal = buildingTransformation.rotationMatrix*r*z

          if (triangulateSurfaces) {
            //faceIndices.push_back(0);
          } else {
            faceIndices.push_back(openstudioFaceFormatId());
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
        //Mesher mesher = new Mesher();
#pragma region Process One triangle at a time.
        //auto trianglesCount = triangleVertices.size() / 3;
        //for (int t = 0; t < triangleVertices.size(); t++) {
        //  std::vector<size_t> faceIndices_1;
        //  Point3dVector triangleVertices_1;
        //  faceIndices_1.push_back(faceIndices[t]);
        //  faceIndices_1.push_back(faceIndices[t + 1]);
        //  faceIndices_1.push_back(faceIndices[t + 2]);

        //  triangleVertices_1.push_back(triangleVertices[t]);
        //  triangleVertices_1.push_back(triangleVertices[t + 1]);
        //  triangleVertices_1.push_back(triangleVertices[t + 2]);
        //  t = t + 2;
        //  ShapeComponentIds shapeCompoentIds{
        //    AddIndices(faceIndices_1, _indicesBuffer, _accessors),  //IndicesAccessorId
        //    //AddCoordinates(allVertices, _coordinatesBuffer, _accessors),  //VerticesAccessorId
        //    AddCoordinates(triangleVertices_1, _coordinatesBuffer, _accessors),
        //    0  //NormalsAccessorId = AddCoordinates(normals)
        //  };

        //  GLTF::Primitive thisPrimitive;

        //  std::map<std::string, int> attrib;
        //  //attrib["NORMAL"] = shapeCompoentIds.NormalsAccessorId;  //TODO BPS
        //  attrib["POSITION"] = shapeCompoentIds.VerticesAccessorId;

        //  thisPrimitive.attributes = attrib;
        //  thisPrimitive.indices = shapeCompoentIds.IndicesAccessorId;
        //  thisPrimitive.material = materialIndex;
        //  //thisPrimitive.mode = TINYGLTF_MODE_LINE;
        //  thisPrimitive.mode = TINYGLTF_MODE_TRIANGLES;

        //  //int initSize = targetMesh.primitives.size();
        //  /* if (initSize == 0) {
        //  targetMesh.primitives = {thisPrimitive};
        //} else {
        //  targetMesh.primitives.push_back(thisPrimitive);
        //}*/
        //  targetMesh.primitives.push_back(thisPrimitive);
        //}
#pragma endregion Process One triangle at a time.
        ShapeComponentIds shapeCompoentIds{
          AddIndices(faceIndices, _indicesBuffer, _accessors),  //IndicesAccessorId
          //AddCoordinates(allVertices, _coordinatesBuffer, _accessors),  //VerticesAccessorId
          AddCoordinates(triangleVertices, _coordinatesBuffer, _accessors),
          0  //NormalsAccessorId = AddCoordinates(normals)
        };

        GLTF::Primitive thisPrimitive;

        std::map<std::string, int> attrib;
        //attrib["NORMAL"] = shapeCompoentIds.NormalsAccessorId;  //TODO BPS
        attrib["POSITION"] = shapeCompoentIds.VerticesAccessorId;

        thisPrimitive.attributes = attrib;
        thisPrimitive.indices = shapeCompoentIds.IndicesAccessorId;
        thisPrimitive.material = materialIndex;
        //thisPrimitive.mode = TINYGLTF_MODE_LINE;
        thisPrimitive.mode = TINYGLTF_MODE_TRIANGLES;

        //int initSize = targetMesh.primitives.size();
        /* if (initSize == 0) {
          targetMesh.primitives = {thisPrimitive};
        } else {
          targetMesh.primitives.push_back(thisPrimitive);
        }*/
        targetMesh.primitives.push_back(thisPrimitive);
        _nodes.push_back(node);
        _meshes.push_back(targetMesh);

#pragma endregion MAIN
      }
      l++;
    }

    //for (int mcount = 0; mcount < _materials.size(); mcount++) {
    //  std::map<std::string, int>::const_iterator it = materialIndicesUsed.find(_materials[mcount].name);
    //  if (it != materialIndicesUsed.end()) {
    //    //found
    //  }
    //  else {
    //      //Remove unused material
    //    std::vector<GLTF::Material>::iterator new_end;
    //    new_end = remove(_materials.begin(), _materials.end(), _materials[mcount]);
    //  }
    //}

#pragma region BUILD SCENE | ELEMENT

    /*std::string elementName = "TEST_SURFACE";

    int materialIndex = 0;
    GLTF::Node node;
    node.name = elementName;
    node.matrix = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 6096.0, 6096.0, 3962.4, 1.0};
    node.mesh = _meshes.size();

    GLTF::Mesh targetMesh;
    targetMesh.name = elementName;

    _nodes.push_back(node);
    _meshes.push_back(targetMesh);*/

    // Now the geometry

#pragma endregion BUILD SCENE | ELEMENT
    if (_coordinatesBuffer.size() == 0) return false;

    /*  GLTF::BufferView bv_indices;
    bv_indices.buffer = 0;
    bv_indices.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
    _indicesBv = bv_indices;

    GLTF::BufferView bv_coordinates;
    bv_coordinates.buffer = 0;
    bv_coordinates.target = TINYGLTF_TARGET_ARRAY_BUFFER;
    bv_coordinates.byteStride = 12;
    _coordinatesBv = bv_coordinates;*/

    // coordinates of bufferviews in buffer
    /*_coordinatesBv.byteLength = _coordinatesBuffer.size();
    _coordinatesBv.byteOffset = 0;
    _indicesBv.byteLength = _indicesBuffer.size();
    _indicesBv.byteOffset = _coordinatesBuffer.size();*/

    if (BufferInBase64) {
      //TODO
      std::string sb = "data:application/octet-stream;base64,";
      _coordinatesBuffer.insert(_coordinatesBuffer.end(), _indicesBuffer.begin(), _indicesBuffer.end());

      //sb = sb + "BASE64DATAAFTERCONVERSION";
      for (unsigned char cb : _coordinatesBuffer) {
        //char* charBytes = reinterpret_cast<char*>(cb.data());
        auto adfads = (unsigned char const*)cb;
        if (adfads != nullptr) {
          //sb = sb + base64_encode((unsigned char const*)cb, sizeof(std::byte));
        }
      }
      //auto afdsfas = base64_encode(_coordinatesBuffer, _indicesBuffer.size() + _coordinatesBuffer.size());
      _buffer.uri = sb;
      //const char* b64Str =
    } else {
      //_coordinatesBuffer.insert(_coordinatesBuffer.end(), _indicesBuffer.begin(), _indicesBuffer.end());
      auto padding = _indicesBuffer.size() % 4;
      for (int i = 0; i < padding; i++) {
        _indicesBuffer.push_back((unsigned char)0);
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
    // buffers TODO
    //_buffer.byteLength = _indicesBuffer.size() + _coordinatesBuffer.size();

    //_buffer.data = _coordinatesBuffer;

    int asize = _buffer.data.size();
    int dfsadf = sizeof(_buffer.data);
    gltf_Model.accessors = _accessors;
    gltf_Model.bufferViews = _bufferViews;
    gltf_Model.buffers = {_buffer};

    //  // Other tie ups
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
    /*gltf_Model.buffers.push_back(_buffer);
    for (const auto& bv : _bufferViews) {
      gltf_Model.bufferViews.push_back(bv);
    }*/
    /*gltf_Model.bufferViews.push_back(bufferView1);
    gltf_Model.bufferViews.push_back(bufferView2);
    gltf_Model.accessors.push_back(accessor1);
    gltf_Model.accessors.push_back(accessor2);*/
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

  bool GltfForwardTranslator::toJSON() {
    return false;
  }

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
    //unsigned char arr[42];

    //// Copy all elements of vector to array
    //std::copy(buffer.data.begin(), buffer.data.end(), arr);
    //int value;
    //std::memcpy(&value, arr, sizeof(int));

    //std::cout << std::hex << value << '\n';

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

    // This is the raw data buffer.
    //buffer.data = {// 6 bytes of indices and two bytes of padding
    //               0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00,
    //               // 36 bytes of floating point numbers
    //               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    //               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00};

    //// Create an array of size equivalent to vector
    //unsigned char arr[42];

    //// Copy all elements of vector to array
    //std::copy(buffer.data.begin(), buffer.data.end(), arr);
    //int value;
    //std::memcpy(&value, arr, sizeof(int));

    //std::cout << std::hex << value << '\n';

    // "The indices of the vertices (ELEMENT_ARRAY_BUFFER) take up 6 bytes in the
    // start of the buffer.
    bufferView1.buffer = 0;
    /*bufferView1.byteOffset = 0;
    bufferView1.byteLength = 6;*/
    bufferView1.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

    // The vertices take up 36 bytes (3 vertices * 3 floating points * 4 bytes)
    // at position 8 in the buffer and are of type ARRAY_BUFFER
    bufferView2.buffer = 0;
    //bufferView2.byteOffset = 8;
    ////bufferView2.byteStride = 12;
    //bufferView2.byteLength = 36;
    bufferView2.target = TINYGLTF_TARGET_ARRAY_BUFFER;

    std::vector<size_t> faceIndices{0, 1, 2};
    std::vector<unsigned char> _indicesBuffer;
    std::vector<unsigned char> _coordinatesBuffer;
    std::vector<GLTF::Accessor> _accessors;
    Point3dVector triangleVertices;
    Point3d p1 =  Point3d(0, 0, 0);
    Point3d p2 = Point3d(1, 0, 0);
    Point3d p3 = Point3d(0, 1, 0);
    std::vector<Point3d> pvec{p1, p2, p3};
    triangleVertices = pvec;


    ShapeComponentIds shapeCompoentIds{
      AddIndices(faceIndices, _indicesBuffer, _accessors),  //IndicesAccessorId
      //AddCoordinates(allVertices, _coordinatesBuffer, _accessors),  //VerticesAccessorId
      AddCoordinates(triangleVertices, _coordinatesBuffer, _accessors),
      0  //NormalsAccessorId = AddCoordinates(normals)
    };

    //// Describe the layout of bufferView1, the indices of the vertices
    //accessor1.bufferView = 0;
    //accessor1.byteOffset = 0;
    //accessor1.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
    //accessor1.count = 3;
    //accessor1.type = TINYGLTF_TYPE_SCALAR;
    //accessor1.maxValues.push_back(2);
    //accessor1.minValues.push_back(0);

    //// Describe the layout of bufferView2, the vertices themself
    //accessor2.bufferView = 1;
    //accessor2.byteOffset = 0;
    //accessor2.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    //accessor2.count = 3;
    //accessor2.type = TINYGLTF_TYPE_VEC3;
    //accessor2.maxValues = {1.0, 1.0, 0.0};
    //accessor2.minValues = {0.0, 0.0, 0.0};

    // Build the mesh primitive and add it to the mesh
    primitive.indices = 0;                 // The index of the accessor for the vertex indices
    primitive.attributes["POSITION"] = shapeCompoentIds.VerticesAccessorId;  // The index of the accessor for positions
    primitive.material = 0;
    //primitive.mode = TINYGLTF_MODE_POINTS;
    //primitive.mode = TINYGLTF_MODE_LINE;
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
   /* m.accessors.push_back(accessor1);
    m.accessors.push_back(accessor2);*/
    m.accessors = _accessors;
    m.asset = asset;

    // Create a simple material
    GLTF::Material mat;
    mat.pbrMetallicRoughness.baseColorFactor = {1.0f, 0.9f, 0.9f, 1.0f};
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

  bool GltfForwardTranslator::loadGLTF(const path& filePath) {
    GLTF::TinyGLTF loader;
    GLTF::Model gltf_Model;
    std::string err;
    std::string warning;
    std::string fileName = toString(filePath);
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
      openstudio::path ofile;
      ofile = "D:/issue-4387_glTF/OpenStudio/build/resources" / toPath("utilities/Geometry/input.gltf");
      std::string output_filename = toString(ofile);
      std::string embedded_filename = output_filename.substr(0, output_filename.size() - 5) + "-Embedded.gltf";
      bool a = loader.WriteGltfSceneToFile(&gltf_Model, output_filename);

      // Embedd buffers and images
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