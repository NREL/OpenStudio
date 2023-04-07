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
#ifndef GLTF_GLTFBOUNDINGBOX_HPP
#define GLTF_GLTFBOUNDINGBOX_HPP

#include "GltfAPI.hpp"

#include "../utilities/core/Logger.hpp"

#include <vector>

namespace tinygltf {
class Value;
}

namespace openstudio {
namespace model {
  class PlanarSurfaceGroup;
}  // namespace model

namespace gltf {

  class GltfMetaData;

  /** GltfBoundingBox is an Interface between the PlanarSurfaceGroups of a Model and a glTF Scene's extras to set the boundingbox **/
  class GLTF_API GltfBoundingBox
  {
   public:
    GltfBoundingBox();
    GltfBoundingBox(const std::vector<model::PlanarSurfaceGroup>& planarSurfaceGroups);

    double lookAtR() const;
    void setLookAtR(double lookAtR);

    double lookAtX() const;
    void setLookAtX(double lookAtX);

    double lookAtY() const;
    void setLookAtY(double lookAtY);

    double lookAtZ() const;
    void setLookAtZ(double lookAtZ);

    double maxX() const;
    void setMaxX(double maxX);

    double maxY() const;
    void setMaxY(double maxY);

    double maxZ() const;
    void setMaxZ(double maxZ);

    double minX() const;
    void setMinX(double minX);

    double minY() const;
    void setMinY(double minY);

    double minZ() const;
    void setMinZ(double minZ);

   protected:
    std::map<std::string, tinygltf::Value> toExtras() const;
    GltfBoundingBox(const tinygltf::Value& boundingBoxObj);
    friend class GltfMetaData;

   private:
    REGISTER_LOGGER("openstudio.gltf.GltfBoundingBox");

    double m_lookAtR = 0.0;
    double m_lookAtX = 0.0;
    double m_lookAtY = 0.0;
    double m_lookAtZ = 0.0;
    double m_maxX = 0.0;
    double m_maxY = 0.0;
    double m_maxZ = 0.0;
    double m_minX = 0.0;
    double m_minY = 0.0;
    double m_minZ = 0.0;
  };
}  // namespace gltf
}  // namespace openstudio

#endif  //GLTF_GLTFBOUNDINGBOX_HPP
