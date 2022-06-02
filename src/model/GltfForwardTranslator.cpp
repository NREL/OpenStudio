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

#include "GltfForwardTranslator.hpp"
#include "GltfUtils.hpp"
#include "GltfMetaData.hpp"
#include "GltfUserData.hpp"
#include "GltfModelObjectMetaData.hpp"
#include "GltfBoundingBox.hpp"
#include "GltfMaterialData.hpp"

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

#include <tiny_gltf.h>

#include <algorithm>
#include <climits>
#include <cmath>
#include <iterator>
#include <numeric>
#include <stack>
#include <tuple>

namespace openstudio {
namespace model {

  GltfForwardTranslator::GltfForwardTranslator() {
    m_logSink.setLogLevel(Warn);
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  std::vector<LogMessage> GltfForwardTranslator::warnings() const {
    std::vector<LogMessage> result;
    for (const auto& logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() == Warn) {
        result.push_back(logMessage);
      }
    }
    return result;
  }

  std::vector<LogMessage> GltfForwardTranslator::errors() const {
    std::vector<LogMessage> result;
    for (const auto& logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() > Warn) {
        result.push_back(logMessage);
      }
    }
    return result;
  }

  // Initializes our two main Buffer Views
  // one for the indices & other for Coordinates and Normals
  void initBufferViews(tinygltf::BufferView& indicesBv, tinygltf::BufferView& coordinatesBv) {
    indicesBv.buffer = 0;
    // defining bytestride is not required in this case
    indicesBv.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

    // The vertices take up 36 bytes (3 vertices * 3 floating points * 4 bytes)
    // at position 8 in the buffer and are of type ARRAY_BUFFER
    coordinatesBv.buffer = 0;
    coordinatesBv.target = TINYGLTF_TARGET_ARRAY_BUFFER;
    coordinatesBv.byteStride = 12;
  }

  // Finds the vertex Index from all Vertices of a planar Surface
  // param : point3d
  // param : allPoints
  // param : tol
  // returns :  index of the Vertex
  size_t getOrCreateVertexIndexT(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol = 0.001) {
    size_t n = allPoints.size();
    for (size_t i = 0; i < n; ++i) {
      if (openstudio::getDistance(point3d, allPoints[i]) < tol) {
        return i;
      }
    }
    allPoints.push_back(point3d);
    return (allPoints.size() - 1);
  }

  // Get Bytes from a value
  // typeparam name :"T"
  // param : value
  // returns : collection of bytes
  template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
  std::vector<uint8_t> splitValueToBytes(T const& value) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < sizeof(value); ++i) {
      uint8_t byte = value >> (i * 8);
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
  // param : faceIndices
  // param : _indicesBuffer
  // param : _accessors
  // returns : index of the Face Indices
  int addIndices(std::vector<size_t>& faceIndices, std::vector<unsigned char>& indicesBuffer, std::vector<tinygltf::Accessor>& accessors) {
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
      } else if (ct == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
        arrayOfByte = splitValueToBytes(index);
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
  int createBuffers(std::vector<float>& values, std::vector<unsigned char>& coordinatesBuffer, std::vector<tinygltf::Accessor>& accessors) {
    // Fixes ACCESSOR_TOTAL_OFFSET_ALIGNMENT
    // Accessor's total byteOffset XXXX isn't a multiple of componentType length 4.
    auto _padding = coordinatesBuffer.size() % 4;
    for (size_t i = 0; i < _padding; ++i) {
      coordinatesBuffer.push_back((unsigned)0);
    }
    int startingBufferPosition = coordinatesBuffer.size();
    std::vector<float> min = {FLT_MAX, FLT_MAX, FLT_MAX};
    std::vector<float> max = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
    int i = 0;
    for (const auto& value : values) {
      min[i] = std::min(value, min[i]);
      max[i] = std::max(value, max[i]);
      ++i;
      if (i > 2) {
        i = 0;
      }
      std::vector<unsigned char> v;
      const auto* ptr = reinterpret_cast<const unsigned char*>(&value);
      for (size_t i = 0; i < sizeof(float); ++i) {
        v.push_back(ptr[i]);
      }
      coordinatesBuffer.insert(coordinatesBuffer.end(), v.begin(), v.end());
    }
    // To Fix : offset 18 is not a multiple of Comonent Type lenght 4
    auto padding = coordinatesBuffer.size() % 4;
    for (size_t i = 0; i < padding; ++i) {
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

  // Exports a gltf against a Model
  // param : model
  // param : outputPath
  // returns : exports a GLTF file against a Model
  bool GltfForwardTranslator::modelToGLTF(const Model& model, const path& outputPath) {
    return modelToGLTF(
      model, [](double percentage) {}, outputPath);
  }

  // MAIN PIPELINE TO TRANSLATE OPENSTUDIO MODEL -> GLTF MODEL
  bool GltfForwardTranslator::modelToGLTF(const Model& model, std::function<void(double)> updatePercentage, const path& outputPath) {
    m_logSink.setThreadId(std::this_thread::get_id());
    m_logSink.resetStringStream();

    bool triangulateSurfaces = true;  //we're always triangulating the surfaces to get the best possible output.

    // TODO: cleanup
    tinygltf::Model gltfModel;
    std::string err;
    std::string warning;
    path p;

    std::vector<tinygltf::Accessor>& accessors = gltfModel.accessors;
    std::vector<tinygltf::Material>& materials = gltfModel.materials;
    std::vector<tinygltf::Mesh>& meshes = gltfModel.meshes;
    std::vector<tinygltf::Node>& nodes = gltfModel.nodes;

    tinygltf::Scene& scene = gltfModel.scenes.emplace_back();
    tinygltf::Buffer& buffer = gltfModel.buffers.emplace_back();

    // Note: Can't emplace_back twice and store refs (before a reserve at least),
    // because the first ref would be invalidated by the second emplace_back, so just resize and get refs after that
    gltfModel.bufferViews.resize(2);
    tinygltf::BufferView& indicesBv = gltfModel.bufferViews.front();
    tinygltf::BufferView& coordinatesBv = gltfModel.bufferViews.back();
    std::vector<unsigned char> indicesBuffer;
    std::vector<unsigned char> coordinatesBuffer;

    // Start Region INIT

    // Define the asset. The version is required
    gltfModel.asset.version = "2.0";
    gltfModel.asset.generator = "OpenStudio";

    // Init the scene
    // aware of all other nodes, meshes, materials, BufferViews, Accessors tightly
    // tied up in a Hierarchy using indices pointing one to other.
    scene.nodes = {0};

    // I'm going to be emplacing back elements in nodes a bunch later, which is going to invalidate the references, so limit scope
    {
      tinygltf::Node& topNode = nodes.emplace_back();
      topNode.name = "Z_UP";
      topNode.matrix = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    }

    initBufferViews(indicesBv, coordinatesBv);

    // End Region INIT

    /*

    This comprises of the other nodes that GLTF provides
    for further addition of complex details, textures, lights, etc.

    std::vector<tinygltf::Scene> scenes;
    std::vector<tinygltf::Buffer> buffers;
    std::vector<tinygltf::Animation> animations;
    std::vector<tinygltf::Texture> textures;
    std::vector<tinygltf::Image> images;
    std::vector<tinygltf::Skin> skins;
    std::vector<tinygltf::Sampler> samplers;
    std::vector<tinygltf::Camera> cameras;
    std::vector<tinygltf::Light> lights;

    */

    // get number of things to translate
    auto planarSurfaces = model.getModelObjects<PlanarSurface>();
    auto planarSurfaceGroups = model.getModelObjects<PlanarSurfaceGroup>();
    auto buildingStories = model.getConcreteModelObjects<BuildingStory>();
    auto buildingUnits = model.getConcreteModelObjects<BuildingUnit>();
    auto thermalZones = model.getConcreteModelObjects<ThermalZone>();
    auto airLoopHVACs = model.getConcreteModelObjects<AirLoopHVAC>();
    auto spaceTypes = model.getConcreteModelObjects<SpaceType>();
    auto defaultConstructionSets = model.getConcreteModelObjects<DefaultConstructionSet>();
    double n = 0;

    size_t N = planarSurfaces.size() + planarSurfaceGroups.size() + buildingStories.size() + buildingUnits.size() + thermalZones.size()
               + airLoopHVACs.size() + spaceTypes.size() + defaultConstructionSets.size() + 1;

    updatePercentage(0.0);

    // Start Region CREATE MATERIALS
    // add model specific materials
    // End Region CREATE MATERIALS

    TransformationVector transformStack{Transformation{}};

    std::vector<double> matrixDefaultTransformation{1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};

    // We prepare a vector of Materials
    std::vector<GltfMaterialData> allMaterials = GltfMaterialData::buildMaterials(model);

    // TODO: make it deterministic by sorting!
    // std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());

    for (const auto& planarSurface : planarSurfaces) {
      // Start Region MAIN LOOP
      //
      // TODO: MOVE THAT ENTIRE LOGIC TO THE GltfUserData file? (and rename to GltfPlanarSurfaceData and make it export a Node directly)
      std::string planarSurfaceName = planarSurface.nameString();
      // Construct in place
      tinygltf::Node& node = nodes.emplace_back();
      node.name = planarSurfaceName;

      // Now the geometry
      Transformation buildingTransformation;
      if (boost::optional<PlanarSurfaceGroup> planarSurfaceGroup_ = planarSurface.planarSurfaceGroup()) {
        buildingTransformation = planarSurfaceGroup_->buildingTransformation();
      }
      // get the vertices
      Point3dVector vertices = planarSurface.vertices();
      Transformation& t = transformStack.emplace_back(Transformation::alignFace(vertices));
      Transformation tInv = t.inverse();
      Point3dVector faceVertices = reverse(tInv * vertices);
      std::vector<double> matrix = openstudio::toStandardVector(buildingTransformation.vector());

      // Adding a check to avoid warning "NODE_MATRIX_DEFAULT"  <Do not specify default transform matrix>.
      // This is the identity_matrix<4>
      if (matrixDefaultTransformation != matrix) {
        node.matrix = matrix;
      } else {
        node.matrix = {};
      }
      node.mesh = meshes.size();

      // EXTRAS

      // TODO: Based on flag add UserData to nodes..
      // Initializes all userdata attributes as per the planar Surface
      GltfUserData glTFUserData(planarSurface);

      tinygltf::Mesh& targetMesh = meshes.emplace_back();
      targetMesh.name = planarSurfaceName;

      auto matName = glTFUserData.surfaceTypeMaterialName();
      int materialIndex = 0;
      auto it = std::find_if(materials.cbegin(), materials.cend(), [&matName](const auto& mat) { return mat.name == matName; });
      if (it != materials.cend()) {
        materialIndex = std::distance(materials.cbegin(), it);
      } else {
        auto it2 = std::find_if(allMaterials.cbegin(), allMaterials.cend(), [&matName](const auto& mat) { return mat.materialName() == matName; });
        if (it2 == allMaterials.cend()) {
          it2 = allMaterials.cbegin();  // This is always the "Undefined" material
        }
        materialIndex = materials.size();
        // add to gltfModel
        materials.emplace_back(it2->toGltf());
      }

      // get vertices of all sub surfaces
      Point3dVectorVector faceSubVertices;
      if (boost::optional<Surface> surface = planarSurface.optionalCast<Surface>()) {
        for (const auto& subSurface : surface->subSurfaces()) {
          faceSubVertices.push_back(reverse(tInv * subSurface.vertices()));
        }
      }

      Point3dVectorVector finalFaceVertices;
      if (triangulateSurfaces) {
        finalFaceVertices = computeTriangulation(faceVertices, faceSubVertices);
        if (finalFaceVertices.empty()) {
          LOG_FREE(Error, "modelToGLTF",
                   "Failed to triangulate surface " << planarSurfaceName << " with " << faceSubVertices.size() << " sub surfaces");
        }
      } else {
        finalFaceVertices.push_back(faceVertices);
      }

      Point3dVector allVertices;
      Point3dVector triangleVertices;
      std::vector<size_t> faceIndices;
      for (const auto& finalFaceVerts : finalFaceVertices) {
        Point3dVector finalVerts = t * finalFaceVerts;
        triangleVertices.insert(triangleVertices.end(), finalVerts.begin(), finalVerts.end());
        auto it = finalVerts.rbegin();
        auto itend = finalVerts.rend();
        for (; it != itend; ++it) {
          faceIndices.push_back(getOrCreateVertexIndexT(*it, allVertices));
        }
      }

      Vector3d outwardNormal = planarSurface.outwardNormal();
      Vector3dVector normalVectors(allVertices.size(), outwardNormal);

      ShapeComponentIds shapeComponentIds{
        addIndices(faceIndices, indicesBuffer, accessors),          // IndicesAccessorId
        addCoordinates(allVertices, coordinatesBuffer, accessors),  // VerticesAccessorId
        addNormals(normalVectors, coordinatesBuffer, accessors)     // NormalsAccessorId
      };

      tinygltf::Primitive& thisPrimitive = targetMesh.primitives.emplace_back();
      thisPrimitive.attributes["NORMAL"] = shapeComponentIds.NormalsAccessorId;
      thisPrimitive.attributes["POSITION"] = shapeComponentIds.VerticesAccessorId;
      thisPrimitive.indices = shapeComponentIds.IndicesAccessorId;
      thisPrimitive.material = materialIndex;
      thisPrimitive.mode = TINYGLTF_MODE_TRIANGLES;

      // TODO: Based on a flag override UserData attribute
      // Addition of UserData as Extras to the node
      node.extras = tinygltf::Value(glTFUserData.toExtras());

      n += 1;
      updatePercentage(100.0 * n / N);
      // End Region MAIN
    }
    // Start Region BUILD SCENE | ELEMENT

    if (coordinatesBuffer.empty()) {
      return false;
    }

    // Write the buffer data
    // TODO: why use a single buffer for two buffersView instead of one buffer for each?
    // no *.bin file is involved | everything is integrated in the mail output gltf file only.
    // Having a separate input file for the GLTF is old now everything resides in the main GLTF file only... as a binary buffer data.
    auto padding = indicesBuffer.size() % 4;
    for (unsigned int i = 0; i < padding; i++) {
      indicesBuffer.push_back(0x00);  // padding bytes
    }

    std::vector<unsigned char> allBuffer = indicesBuffer;  //std::move(_indicesBuffer);
    allBuffer.insert(allBuffer.end(), coordinatesBuffer.begin(), coordinatesBuffer.end());

    buffer.data = allBuffer;

    indicesBv.byteLength = indicesBuffer.size();
    indicesBv.byteOffset = 0;

    coordinatesBv.byteLength = coordinatesBuffer.size();
    coordinatesBv.byteOffset = indicesBuffer.size();
    // End Region BUILD SCENE | ELEMENT

    // Other tie ups

    // Declare all nodes as a child of topNode
    auto& topNodeChildren = nodes.front().children;
    topNodeChildren.resize(nodes.size() - 1);
    std::iota(topNodeChildren.begin(), topNodeChildren.end(), 1);

    // SCENE EXTRAS | this will be having all the metadata
    scene.extras = tinygltf::Value(GltfMetaData(model).toExtras());

    // Save it to a file

    // glTF Parser/Serialier context
    tinygltf::TinyGLTF loader;
    // Uncomment this to use extras_as_string [std::string] instead of extras [tinygltf::Value]
    loader.SetStoreOriginalJSONForExtrasAndExtensions(true);
    bool ret = loader.WriteGltfSceneToFile(&gltfModel, toString(outputPath),
                                           true,    // embedImages
                                           true,    // embedBuffers
                                           true,    // pretty print
                                           false);  // write binary

    updatePercentage(100.0);

    return ret;
  }

  // TODO: either rename, or properly populate the model...
  // To populate a GLTF Model from an existing GLTF file.
  // also exports a gltf file with a .bin file (non embeded version).
  bool GltfForwardTranslator::loadGLTF(const path& inputPath, const path& inputNonEmbededPath) {
    tinygltf::TinyGLTF loader;
    tinygltf::Model gltf_Model;
    std::string err;
    std::string warning;
    std::string fileName = toString(inputPath);
    bool ret = loader.LoadASCIIFromFile(&gltf_Model, &err, &warning, fileName);
    if (!err.empty()) {
      LOG(Error, "Error loading GLTF " << err);
      //ret = false;
    }
    if (!warning.empty()) {
      LOG(Warn, "Error loading GLTF " << warning);
    }

    if (!ret) {
      LOG(Error, "Failed to parse glTF");
    } else {
      std::string output_filename = toString(inputNonEmbededPath);
      std::string embedded_filename = output_filename.substr(0, output_filename.size() - 5) + "-Embedded.gltf";
      bool a = loader.WriteGltfSceneToFile(&gltf_Model, output_filename);
      OS_ASSERT(a);
      // Embedd buffers and (images if present)
#ifndef TINYGLTF_NO_STB_IMAGE_WRITE
      bool b = loader.WriteGltfSceneToFile(&gltf_Model, embedded_filename, true, true);
      OS_ASSERT(b);
#endif
    }
    return ret;
  }

  bool GltfForwardTranslator::loadGLTF(const path& inputPath) {
    tinygltf::TinyGLTF loader;
    tinygltf::Model gltf_Model;
    std::string err;
    std::string warning;
    std::string fileName = toString(inputPath);
    bool ret = loader.LoadASCIIFromFile(&gltf_Model, &err, &warning, fileName);
    if (!err.empty()) {
      LOG(Error, "Error loading GLTF " << err);
      //ret = false;
    }
    if (!warning.empty()) {
      LOG(Warn, "Error loading GLTF " << warning);
    }

    if (!ret) {
      LOG(Error, "Failed to parse glTF");
    } else {
      // TODO: get the materials too?

      // USERDATA INITIALIZATION FROM EACH NODE's EXTRAS
      m_userDataCollection.clear();
      bool isTopNode = true;
      for (const auto& node : gltf_Model.nodes) {
        if (isTopNode) {
          isTopNode = false;
          continue;
        }
        m_userDataCollection.emplace_back(node.extras);
      }

      // METADATA INITIALIZATION FROM SCENE's EXTRAS
      if (gltf_Model.scenes.size() > 1) {
        LOG(Warn, "Found more than one scene object, using the first found, occurred in GLTF File '" << toString(inputPath) << "'");
      } else if (gltf_Model.scenes.empty()) {
        LOG_AND_THROW("Could not find a scene object in the GTLF file '" << toString(inputPath) << "'");
      }
      m_gltfMetaData = GltfMetaData(gltf_Model.scenes[0].extras);
    }
    return ret;
  }

  std::vector<GltfUserData> GltfForwardTranslator::getUserDataCollection() const {
    return m_userDataCollection;
  }

  boost::optional<GltfUserData> GltfForwardTranslator::getUserDataBySurfaceName(const std::string& surfaceName) const {
    boost::optional<GltfUserData> result;

    auto it =
      std::find_if(m_userDataCollection.begin(), m_userDataCollection.end(), [&surfaceName](const auto& obj) { return obj.name() == surfaceName; });
    if (it != m_userDataCollection.end()) {
      result = *it;
    }
    return result;
  }

  GltfMetaData GltfForwardTranslator::getMetaData() const {
    return m_gltfMetaData;
  }

}  // namespace model
}  // namespace openstudio
