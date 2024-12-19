/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GltfForwardTranslator.hpp"
#include "GltfUtils.hpp"
#include "GltfMetaData.hpp"
#include "GltfUserData.hpp"
#include "GltfModelObjectMetaData.hpp"
#include "GltfBoundingBox.hpp"
#include "GltfMaterialData.hpp"

#include "../model/Model.hpp"

#include "../model/RenderingColor.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/ConstructionAirBoundary.hpp"
#include "../model/ConstructionAirBoundary_Impl.hpp"
#include "../model/AirLoopHVAC.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/BuildingUnit.hpp"
#include "../model/BuildingUnit_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
#include "../model/InteriorPartitionSurface.hpp"
#include "../model/InteriorPartitionSurface_Impl.hpp"
#include "../model/PlanarSurfaceGroup.hpp"
#include "../model/PlanarSurfaceGroup_Impl.hpp"
#include "../model/PlanarSurface.hpp"
#include "../model/PlanarSurface_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/InteriorPartitionSurfaceGroup.hpp"

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
#include <iterator>
#include <numeric>
#include <type_traits>

namespace openstudio {
namespace gltf {

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

  template <typename T>
  std::vector<T> getObjectsAndSort(const model::Model& model) {
    std::vector<T> objects;
    if constexpr (std::is_same_v<T, model::PlanarSurface> || std::is_same_v<T, model::PlanarSurfaceGroup>) {
      objects = model.getModelObjects<T>();
    } else {
      objects = model.getConcreteModelObjects<T>();
    }
    std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());
    return objects;
  }

  boost::optional<tinygltf::Model> GltfForwardTranslator::toGltfModel(const model::Model& model, std::function<void(double)> updatePercentage) {
    // MAIN PIPELINE TO TRANSLATE OPENSTUDIO MODEL -> GLTF MODEL

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
    // make it deterministic by sorting!
    auto planarSurfaces = getObjectsAndSort<model::PlanarSurface>(model);
    auto planarSurfaceGroups = getObjectsAndSort<model::PlanarSurfaceGroup>(model);
    auto buildingStories = getObjectsAndSort<model::BuildingStory>(model);
    auto buildingUnits = getObjectsAndSort<model::BuildingUnit>(model);
    auto thermalZones = getObjectsAndSort<model::ThermalZone>(model);
    auto airLoopHVACs = getObjectsAndSort<model::AirLoopHVAC>(model);
    auto spaceTypes = getObjectsAndSort<model::SpaceType>(model);
    auto defaultConstructionSets = getObjectsAndSort<model::DefaultConstructionSet>(model);
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
      if (boost::optional<model::PlanarSurfaceGroup> planarSurfaceGroup_ = planarSurface.planarSurfaceGroup()) {
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
      size_t materialIndex = 0;
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
      if (auto surface_ = planarSurface.optionalCast<model::Surface>()) {
        for (const auto& subSurface : surface_->subSurfaces()) {
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

      detail::ShapeComponentIds shapeComponentIds(faceIndices, allVertices, normalVectors, indicesBuffer, coordinatesBuffer, accessors);

      tinygltf::Primitive& thisPrimitive = targetMesh.primitives.emplace_back();
      thisPrimitive.attributes["NORMAL"] = shapeComponentIds.normalsAccessorId;
      thisPrimitive.attributes["POSITION"] = shapeComponentIds.verticesAccessorId;
      thisPrimitive.indices = shapeComponentIds.indicesAccessorId;
      thisPrimitive.material = static_cast<int>(materialIndex);
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
      return boost::none;
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

    return gltfModel;
  }

  std::string GltfForwardTranslator::modelToGLTFString(const model::Model& model) {
    boost::optional<tinygltf::Model> gltfModel_ = toGltfModel(model, [](double percentage) {});
    if (!gltfModel_) {
      LOG(Error, "Failed to prepare GLTF model");
      return "";
    }

    auto gltfModel = gltfModel_.get();

    // Save it to a file
    // glTF Parser/Serialier context
    tinygltf::TinyGLTF ctx;
    // Uncomment this to use extras_as_string [std::string] instead of extras [tinygltf::Value]
    ctx.SetStoreOriginalJSONForExtrasAndExtensions(true);

    std::stringstream ss;
    bool ret = ctx.WriteGltfSceneToStream(&gltfModel, ss,
                                          true,    // pretty print
                                          false);  // write binary
    if (!ret) {
      LOG(Error, "Writing GLTF failed");
    }
    return ss.str();
  }

  // Exports a gltf against a Model
  // returns : exports a GLTF file against a Model
  bool GltfForwardTranslator::modelToGLTF(const model::Model& model, const path& outputPath) {
    return modelToGLTF(model, [](double percentage) {}, outputPath);
  }

  bool GltfForwardTranslator::modelToGLTF(const model::Model& model, std::function<void(double)> updatePercentage, const path& outputPath) {

    boost::optional<tinygltf::Model> gltfModel_ = toGltfModel(model, [](double percentage) {});
    if (!gltfModel_) {
      LOG(Error, "Failed to prepare GLTF model");
      return "";
    }

    // Save it to a file
    auto gltfModel = gltfModel_.get();

    // glTF Parser/Serialier context
    tinygltf::TinyGLTF ctx;
    // Uncomment this to use extras_as_string [std::string] instead of extras [tinygltf::Value]
    ctx.SetStoreOriginalJSONForExtrasAndExtensions(true);
    bool ret = ctx.WriteGltfSceneToFile(&gltfModel, toString(outputPath),
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
      LOG(Error, "Error loading GLTF: " << err);
      //ret = false;
    }
    if (!warning.empty()) {
      LOG(Warn, "Warning loading GLTF: " << warning);
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
        m_userDataCollection.emplace_back(GltfUserData(node.extras));
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

}  // namespace gltf
}  // namespace openstudio
