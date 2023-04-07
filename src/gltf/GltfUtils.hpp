/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
      // int createBuffers(std::vector<float>& values, std::vector<unsigned char>& coordinatesBuffer, std::vector<tinygltf::Accessor>& accessors);
    };
  }  // namespace detail

}  // namespace gltf
}  // namespace openstudio

#endif  // GLTF_GLTFUTILS_HPP
