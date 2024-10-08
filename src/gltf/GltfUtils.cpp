/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GltfUtils.hpp"

#include "../model/ModelObject.hpp"

#include "../utilities/core/Compare.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Vector3d.hpp"

#include <tiny_gltf.h>

#include <limits>
#include <type_traits>
#include <vector>

namespace openstudio {

namespace gltf {

  namespace detail {
    // Get Bytes from a value
    // typeparam name :"T"
    // param : value
    // returns : collection of bytes
    template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    std::vector<uint8_t> splitValueToBytes(T const& value) {
      std::vector<uint8_t> bytes;
      for (size_t i = 0; i < sizeof(value); ++i) {
        auto byte = static_cast<uint8_t>(value >> (i * 8));
        bytes.insert(bytes.begin(), byte);
      }
      return bytes;
    }

    // Adds & Creates Face Indices buffers and Accessors
    // This method infers the indicesbuffer and adds them to the glTF model accessor
    // which expects two types of accessor input one from indicesbuffer and the second
    // one form coordinates buffer. So here after appending the buffer content it is sending
    // over the index so the containing node will be aware of which one to refer.
    // A better overview here at https://github.com/KhronosGroup/glTF/blob/main/specification/2.0/figures/gltfOverview-2.0.0b.png
    // returns : index of the Face Indices
    int addIndices(const std::vector<size_t>& faceIndices, std::vector<unsigned char>& indicesBuffer, std::vector<tinygltf::Accessor>& accessors) {
      auto [min, max] = std::minmax_element(std::cbegin(faceIndices), std::cend(faceIndices));

      // This but seems to be just figuring out the best way to back the indices in
      // depenbding on the range of numbers
      int ct = TINYGLTF_COMPONENT_TYPE_BYTE;
      auto size = 0;

      if (*max <= static_cast<size_t>(std::pow(2, 8))) {
        ct = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
        size = sizeof(std::byte);
      } else if (*max <= static_cast<size_t>(std::pow(2, 16))) {
        ct = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
        size = sizeof(short);
      } else {
        ct = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
        size = sizeof(int);
      }
      // the offset position needs to be a multiple of the size
      // (this is from a warning we received in beta testing)
      // so we inject some padding when needed
      auto padding = indicesBuffer.size() % size;
      for (size_t i = 0; i < padding; ++i) {
        indicesBuffer.push_back(0x00);
      }

      // To avoid Accessor offset Validation Issue
      auto _padding = indicesBuffer.size() % 4;
      for (size_t i = 0; i < _padding; ++i) {
        indicesBuffer.push_back(0x00);
      }

      tinygltf::Accessor indAccessor;
      indAccessor.bufferView = 0;
      indAccessor.componentType = ct;
      indAccessor.byteOffset = indicesBuffer.size();
      indAccessor.normalized = false;
      indAccessor.type = TINYGLTF_TYPE_SCALAR;
      indAccessor.count = faceIndices.size();
      indAccessor.minValues = {static_cast<double>(*min)};
      indAccessor.maxValues = {static_cast<double>(*max)};

      std::vector<unsigned char> indicesBufferData;
      for (const auto index : faceIndices) {
        std::vector<unsigned char> arrayOfByte;
        if (ct == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
          arrayOfByte.push_back((unsigned char)index);
          // } else if (ct == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
          //  arrayOfByte = splitValueToBytes(index);
        } else {
          arrayOfByte = splitValueToBytes(index);
        }
        indicesBufferData.insert(indicesBufferData.end(), arrayOfByte.begin(), arrayOfByte.end());
      }

      if (indicesBufferData.size() < 4) {
        indicesBufferData.push_back(0x00);
      }
      indicesBuffer.insert(indicesBuffer.end(), indicesBufferData.begin(), indicesBufferData.end());

      const auto thisIndex = accessors.size();
      accessors.push_back(indAccessor);
      return thisIndex;
    }

    // Creates Coordinates / Normal Buffers and Accessors.
    // returns : index
    int createBuffers(const std::vector<float>& values, std::vector<unsigned char>& coordinatesBuffer, std::vector<tinygltf::Accessor>& accessors) {
      // Fixes ACCESSOR_TOTAL_OFFSET_ALIGNMENT
      // Accessor's total byteOffset XXXX isn't a multiple of componentType length 4.
      auto _padding = coordinatesBuffer.size() % 4;
      for (size_t i = 0; i < _padding; ++i) {
        coordinatesBuffer.push_back((unsigned)0);
      }
      int startingBufferPosition = coordinatesBuffer.size();
      std::vector<float> min(3, std::numeric_limits<float>::max());
      std::vector<float> max(3, std::numeric_limits<float>::lowest());
      int i = 0;
      for (const auto& value : values) {
        min[i] = std::min(value, min[i]);
        max[i] = std::max(value, max[i]);
        ++i;
        if (i > 2) {
          i = 0;
        }
        std::vector<unsigned char> v;
        // TODO: ideally we should revisit this...
        // cppcheck-suppress invalidPointerCast
        const auto* ptr = reinterpret_cast<const unsigned char*>(&value);
        for (size_t j = 0; j < sizeof(float); ++j) {
          v.push_back(ptr[j]);
        }
        coordinatesBuffer.insert(coordinatesBuffer.end(), v.begin(), v.end());
      }
      // To Fix : offset 18 is not a multiple of Component Type length 4
      auto padding = coordinatesBuffer.size() % 4;
      for (size_t j = 0; j < padding; ++j) {
        coordinatesBuffer.push_back((unsigned)0);
      }
      // convert min and max to double
      std::vector<double> min_d;
      std::vector<double> max_d;
      for (const auto mn : min) {
        min_d.push_back((double)mn);
      }
      for (const auto mx : max) {
        max_d.push_back((double)mx);
      }
      tinygltf::Accessor coordAccessor;
      coordAccessor.bufferView = 1;
      coordAccessor.byteOffset = startingBufferPosition;
      coordAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
      coordAccessor.normalized = false;
      coordAccessor.count = values.size() / 3;
      coordAccessor.type = TINYGLTF_TYPE_VEC3;
      coordAccessor.minValues = min_d;
      coordAccessor.maxValues = max_d;

      auto ret = accessors.size();
      accessors.push_back(coordAccessor);
      return ret;
    }

    // Adds Coordinate Buffers for all vertices of the surface
    // returns : index for the Coordinates Buffer
    int addCoordinates(const Point3dVector& allVertices, std::vector<unsigned char>& coordinatesBuffer, std::vector<tinygltf::Accessor>& accessors) {
      std::vector<float> values(3 * allVertices.size());
      size_t i = 0;
      for (const auto& point : allVertices) {
        values[i++] = static_cast<float>(point.x());
        values[i++] = static_cast<float>(point.y());
        values[i++] = static_cast<float>(point.z());
      }
      return createBuffers(values, coordinatesBuffer, accessors);
    }

    // Adds Normal Buffers for all normal Vectors
    // returns : index for the Normals Buffer
    int addNormals(const Vector3dVector& normalVectors, std::vector<unsigned char>& coordinatesBuffer, std::vector<tinygltf::Accessor>& accessors) {
      std::vector<float> values(3 * normalVectors.size());
      size_t i = 0;
      for (const auto& vec : normalVectors) {
        values[i++] = static_cast<float>(vec.x());
        values[i++] = static_cast<float>(vec.y());
        values[i++] = static_cast<float>(vec.z());
      }
      return createBuffers(values, coordinatesBuffer, accessors);
    }

    ShapeComponentIds::ShapeComponentIds(const std::vector<size_t>& faceIndices, const Point3dVector& allVertices,
                                         const Vector3dVector& normalVectors, std::vector<unsigned char>& indicesBuffer,
                                         std::vector<unsigned char>& coordinatesBuffer, std::vector<tinygltf::Accessor>& accessors) {

      indicesAccessorId = addIndices(faceIndices, indicesBuffer, accessors);
      verticesAccessorId = addCoordinates(allVertices, coordinatesBuffer, accessors);
      normalsAccessorId = addNormals(normalVectors, coordinatesBuffer, accessors);
    }
  }  // namespace detail

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
  std::string getObjectGLTFMaterialName(const model::ModelObject& object) {
    return getObjectGLTFMaterialName(object.iddObjectType().valueDescription(), object.nameString());
  }

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

    detail::ShapeComponentIds shapeComponentIds(faceIndices, allVertices, {}, _indicesBuffer, _coordinatesBuffer, m.accessors);

    // Build the mesh primitive and add it to the mesh
    // The index of the accessor for the vertex indices
    tinygltf::Primitive& primitive = mesh.primitives.emplace_back();
    primitive.indices = 0;
    // The index of the accessor for positions
    primitive.attributes["POSITION"] = shapeComponentIds.verticesAccessorId;
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

}  // namespace gltf
}  // namespace openstudio
