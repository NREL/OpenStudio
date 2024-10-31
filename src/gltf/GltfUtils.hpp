/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef GLTF_GLTFUTILS_HPP
#define GLTF_GLTFUTILS_HPP

#include "GltfAPI.hpp"

#include "../utilities/core/Path.hpp"

#include <string>
#include <vector>

namespace tinygltf {
struct Accessor;
}

namespace openstudio {

class Point3d;
class Vector3d;

namespace model {
  class ModelObject;
}

namespace gltf {

  // Gets GLTF Material name on the basis of idd Object Type and Name
  GLTF_API std::string getObjectGLTFMaterialName(const std::string& iddObjectType, const std::string& name);

  // Gets GLTF Material Name on the basis of Model Object
  GLTF_API std::string getObjectGLTFMaterialName(const model::ModelObject& object);

  // Export a Minimal GLTF file (Triangle with 3 Points) using raw buffer data.
  GLTF_API bool createTriangleGLTF(const path& outputPath);

  // Export a Minimal GLTF file (Triangle with 3 Points) using generated raw buffer data from Point3DVector
  GLTF_API bool createTriangleGLTFFromPoint3DVector(const path& outputPath);

  namespace detail {
    // For Indices of Indices, Coordinates & Normal buffers against each Components
    struct ShapeComponentIds
    {
      explicit ShapeComponentIds(const std::vector<size_t>& faceIndices, const std::vector<Point3d>& allVertices,
                                 const std::vector<Vector3d>& normalVectors, std::vector<unsigned char>& indicesBuffer,
                                 std::vector<unsigned char>& coordinatesBuffer, std::vector<tinygltf::Accessor>& accessors);

      int indicesAccessorId;
      int verticesAccessorId;
      int normalsAccessorId;

     private:
      // int addIndices(const std::vector<size_t>& faceIndices, std::vector<unsigned char>& indicesBuffer, std::vector<tinygltf::Accessor>& accessors);
      // int addCoordinates(const std::vector<Point3d>& allVertices, std::vector<unsigned char>& coordinatesBuffer,
      //                    std::vector<tinygltf::Accessor>& accessors);
      // int addNormals(const std::vector<Vector3d>& normalVectors, std::vector<unsigned char>& coordinatesBuffer,
      //                std::vector<tinygltf::Accessor>& accessors);
      // int createBuffers(const std::vector<float>& values, std::vector<unsigned char>& coordinatesBuffer, std::vector<tinygltf::Accessor>& accessors);
    };
  }  // namespace detail

}  // namespace gltf
}  // namespace openstudio

#endif  // GLTF_GLTFUTILS_HPP
