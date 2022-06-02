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

#include "GltfUtils.hpp"
#include "ModelObject.hpp"

#include "../utilities/core/Compare.hpp"
#include "../utilities/geometry/Point3d.hpp"

#include <tiny_gltf.h>

namespace openstudio {
//class GltfMetaDta;
namespace model {

  // Gets GLTF Material name on the basis of idd Object Type and Name
  // param : iddObjectType
  // param : name
  // returns : name of GLTF Material Name
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
      // TODO
      // This shouldn't happen
      // LOG_FREE(Error, "getObjectGLTFMaterialName", "Didn't expect it would be called for '" << iddObjectType << "' (name = '" << name << "')");
      // result = "ConstructionAirBoundary_" + name;
      result = "AirWall";
    } else {
      LOG_FREE(Error, "getObjectGLTFMaterialName", "Unknown iddObjectType '" << iddObjectType << "'");
    }
    return result;
  }

  // Gets GLTF Material Name on the basis of Model Object
  // param : object
  // returns : name of GLTF material Name
  std::string getObjectGLTFMaterialName(const ModelObject& object) {
    return getObjectGLTFMaterialName(object.iddObjectType().valueDescription(), object.nameString());
  }

  // TODO: put it back once I avoid the circular dependency
  /*
  // To export a Minimal GLTF file (Triangle with 3 Points) using generated raw data from Point3DVector
  bool createTriangleGLTFFromPoint3DVector(const path& outputPath) {
    tinygltf::Model m;

    //BPS: Buffer Views will not give the desired result if used emplace_back()
    tinygltf::BufferView bufferView1;
    tinygltf::BufferView bufferView2;

    tinygltf::Scene& scene = m.scenes.emplace_back();
    tinygltf::Mesh& mesh = m.meshes.emplace_back();
    tinygltf::Node& node = m.nodes.emplace_back();
    tinygltf::Buffer& buffer = m.buffers.emplace_back();
    tinygltf::Asset& asset = m.asset;

    // Create an array of size equivalent to vector
    // unsigned char arr[42];
    // The indices of the vertices (ELEMENT_ARRAY_BUFFER) take up 6 bytes in the
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
    Point3dVector allVertices{
      {0, 0, 0},
      {1, 0, 0},
      {0, 1, 0},
    };

    ShapeComponentIds shapeComponentIds{
      addIndices(faceIndices, _indicesBuffer, m.accessors),          //IndicesAccessorId
      addCoordinates(allVertices, _coordinatesBuffer, m.accessors),  //VerticesAccessorId
      0                                                              //NormalsAccessorId
    };

    // Build the mesh primitive and add it to the mesh
    // The index of the accessor for the vertex indices
    tinygltf::Primitive& primitive = mesh.primitives.emplace_back();
    primitive.indices = 0;
    // The index of the accessor for positions
    primitive.attributes["POSITION"] = shapeComponentIds.VerticesAccessorId;
    primitive.material = 0;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;

    bufferView1.byteOffset = 0;
    bufferView1.byteLength = _indicesBuffer.size();
    bufferView2.byteOffset = _indicesBuffer.size();
    bufferView2.byteLength = _coordinatesBuffer.size();

    std::vector<unsigned char> _allBuffer = _indicesBuffer;  //std::move(_indicesBuffer);
    _allBuffer.insert(_allBuffer.end(), _coordinatesBuffer.begin(), _coordinatesBuffer.end());

    buffer.data = _allBuffer;

    // Other tie ups
    node.mesh = 0;
    scene.nodes.push_back(0);  // Default scene

    // Define the asset. The version is required
    asset.version = "2.0";
    asset.generator = "OpenStudio";

    // Now all that remains is to tie back all the loose objects into the
    // our single model.
    m.bufferViews.emplace_back(std::move(bufferView1));
    m.bufferViews.emplace_back(std::move(bufferView2));

    // Create a simple material
    tinygltf::Material mat;
    mat.pbrMetallicRoughness.baseColorFactor = {1.0f, 0.9f, 0.9f, 1.0f};
    mat.pbrMetallicRoughness.metallicFactor = 1.0;
    mat.pbrMetallicRoughness.roughnessFactor = 0.5;
    mat.doubleSided = true;
    m.materials.emplace_back(std::move(mat));

    // Save it to a file
    tinygltf::TinyGLTF gltf;
    bool ret = gltf.WriteGltfSceneToFile(&m, toString(outputPath),
                                         true,    // embedImages
                                         true,    // embedBuffers
                                         true,    // pretty print
                                         false);  // write binary
    return ret;
  }

  // To export a Minimal GLTF file (Triangle with 3 Points) using raw buffer data.
  bool createTriangleGLTF(const path& outputPath) {
    tinygltf::Model m;
    tinygltf::Scene scene;
    tinygltf::Mesh mesh;
    tinygltf::Primitive primitive;
    tinygltf::Node node;
    tinygltf::Buffer buffer;
    tinygltf::BufferView bufferView1;
    tinygltf::BufferView bufferView2;
    tinygltf::Accessor accessor1;
    tinygltf::Accessor accessor2;
    tinygltf::Asset asset;

    // This is the raw data buffer.
    buffer.data = {// 6 bytes of indices and two bytes of padding
                   // 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00,
                   0x00, 0x01, 0x02, 0x00,
                   // 36 bytes of floating point numbers
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00};

    // Create an array of size equivalent to vector
    // unsigned char arr[42]

    // The indices of the vertices (ELEMENT_ARRAY_BUFFER) take up 6 bytes in the
    // start of the buffer.
    bufferView1.buffer = 0;
    bufferView1.byteOffset = 0;
    bufferView1.byteLength = 3;
    bufferView1.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

    // The vertices take up 36 bytes (3 vertices * 3 floating points * 4 bytes)
    // at position 8 in the buffer and are of type ARRAY_BUFFER
    bufferView2.buffer = 0;
    bufferView2.byteOffset = 4;
    bufferView2.byteLength = 36;
    bufferView2.target = TINYGLTF_TARGET_ARRAY_BUFFER;

    // Describe the layout of bufferView1, the indices of the vertices
    accessor1.bufferView = 0;
    accessor1.byteOffset = 0;
    accessor1.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;  // TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
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
    // Other Modes in GLTF TINYGLTF_MODE_POINTS | TINYGLTF_MODE_LINE;
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
    tinygltf::Material& mat = m.materials.emplace_back();
    mat.pbrMetallicRoughness.baseColorFactor = {1.0f, 0.9f, 0.9f, 1.0f};
    mat.pbrMetallicRoughness.metallicFactor = 0.8;
    mat.pbrMetallicRoughness.roughnessFactor = 0.5;
    mat.doubleSided = true;

    // Save it to a file
    tinygltf::TinyGLTF gltf;
    bool ret = gltf.WriteGltfSceneToFile(&m, toString(outputPath),
                                         true,    // embedImages
                                         true,    // embedBuffers
                                         true,    // pretty print
                                         false);  // write binary
    return ret;
  }
*/

  /*

     std::string getBoundaryMaterialName(boost::optional<Surface> surface) {
    std::string result;
    if (surface->outsideBoundaryCondition() == "Outdoors") {
      if ((surface->sunExposure() == "SunExposed") && (surface->windExposure() == "WindExposed")) {
        result = "Boundary_Outdoors_SunWind";
      } else if (surface->sunExposure() == "SunExposed") {
        result = "Boundary_Outdoors_Sun";
      } else if (surface->windExposure() == "WindExposed") {
        result = "Boundary_Outdoors_Wind";
      } else {
        result = "Boundary_Outdoors";
      }
    } else {
      result = "Boundary_" + surface->outsideBoundaryCondition();
    }
    return result;
  }

  std::string getBoundaryMaterialName(const GltfUserData& glTFUserData) {
    std::string result;
    if (glTFUserData.getOutsideBoundaryCondition() == "Outdoors") {
      if ((glTFUserData.getSunExposure() == "SunExposed") && (glTFUserData.getWindExposure() == "WindExposed")) {
        result = "Boundary_Outdoors_SunWind";
      } else if (glTFUserData.getSunExposure() == "SunExposed") {
        result = "Boundary_Outdoors_Sun";
      } else if (glTFUserData.getWindExposure() == "WindExposed") {
        result = "Boundary_Outdoors_Wind";
      } else {
        result = "Boundary_Outdoors";
      }
    } else {
      result = "Boundary_" + glTFUserData.getOutsideBoundaryCondition();
    }
    return result;
  }

*/

}  // namespace model
}  // namespace openstudio
